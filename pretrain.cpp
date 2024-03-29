#include <iostream>
#include <fstream>
#include <unordered_map>
#include <iomanip>
#include "csv.h"
#include "predict.h"

using std::vector, std::string, std::cout, std::endl;

class Prediction {
    private:
    string surface;
    string last_date;
    const int max_update = 500;
    const double max_score_update = max_update * 0.3;
    const double max_time_update = max_update * 0.2;
    const double max_rank_diff_update = max_update * 0.3;
    const double max_ace_update = max_update * 0.05;
    const double max_df_update = max_update * 0.05;
    const double max_bp_update = max_update * 0.05;

    void parse_last_time(int &year, int &month, int &day) {
        year = stoi(last_date.substr(0, 4));
        month = stoi(last_date.substr(4, 2));
        day = stoi(last_date.substr(6, 2));
    }

    void parse_time(const Player &p, int &year, int &month, int &day) {
        year = stoi(p.last_match_date.substr(0, 4));
        month = stoi(p.last_match_date.substr(4, 2));
        day = stoi(p.last_match_date.substr(6, 2));
    }

    int find_days_since_last(const Player &p) {
        int p_year, p_month, p_day, year, month, day;
        parse_time(p, p_year, p_month, p_day);
        parse_last_time(year, month, day);

        int result = 0;
        result += (year - p_year) * 365;
        result += (month - p_month) * 30;
        result += (day - p_day);
        return result;
    }

    int find_num_sets(const string &score) {
        int num = 0;
        for (auto c : score) {
            if (c == '-')
                ++num;
        }
        return num;
    }

    // Output score_update given the score, num sets, and max update
    double find_score_update(const string &score, int num_sets) {
        vector<int> sets;
        sets.reserve(static_cast<size_t>(num_sets));
        int counter = 0;
        sets.push_back(0);
        for (char c : score) {
            if (c == ' ')
                sets.push_back(counter +1);
            ++counter;
        }
        
        double result = 0;
        double max_per_set = max_score_update / num_sets;
        for (int i = 0; i < num_sets; ++i) {
            int set_ix = sets[i];
            int first = (score[set_ix]) - '0';
            int second = (score[set_ix +2]) - '0';

            // If winner won set
            if (first > second) {
                // If tiebreaker, assign max/10 points
                if (second == 6) {
                    result += max_per_set / 10;
                }
                else {
                    result += ((6 - second) / 6) * max_per_set;
                }
            }
            else {
                // If tiebreaker, assign max/10 points
                if (score[set_ix] == '6') {
                    result += max_per_set / 10;
                }
                else {
                    result += (first / 6) * max_per_set;
                }
            }
        }
        return result;
    }

    // Score, Rank_Diff, Time are metrics shared by both players
    double equal_updates(const Match &m, const Player &winner, const Player &loser) {
        double rank_diff_update = max_rank_diff_update, time_update = max_time_update;
        int num_sets = find_num_sets(m.score);
        int minutes = m.time;
        
        double score_update = find_score_update(m.score, num_sets);

        // update time according to Bo5
        if (m.best_of == 5) {
            // Lower cutoff: 100 mins and Upper cutoff: 270 mins
            double change = (0.1*max_update) / 170;
            if (minutes > 270) 
                time_update = 0;
            else {
                while (minutes > 100) {
                    --minutes;
                    time_update -= change;
                }
            }
        }
        // update time according to Bo3
        else {
            // Lower cutoff: 50 mins and Upper cutoff: 150 mins
            double change = (0.1*max_update) / 100;
            if (minutes > 150) 
                time_update = 0;
            else {
                while (minutes > 50) {
                    --minutes;
                    time_update -= change;
                }
            }
        }
        
        // Update rank_diff. The bigger the upset, the larger the value (30 is cutoff)
        // Beating someone 20 ranking below you is cutoff on other end
        int upset = winner.ranking - loser.ranking;
        if (upset > 0 && upset < 30)
            rank_diff_update = (max_rank_diff_update/4) + ((upset * max_rank_diff_update) / 40);
        else if (upset < 0 && upset >= -20)  
            rank_diff_update = (0.3 + (upset * 0.01)) * max_rank_diff_update;
        else if (upset < -20)
            rank_diff_update = 0.1;

        return score_update + time_update + rank_diff_update;
    }

    // Ace: Upper cutoff is 15 in Bo3, 20 in Bo5
    // Aces >= upper: full for winner, and 0 for loser
    double ace_update(const Match &m, const Player &p, bool winner) {
        if (m.best_of == 5) {
            if (winner) {
                if (m.w_ace < 20) 
                    return (m.w_ace * max_ace_update) / 20;
                
                return max_ace_update;
            }
            else {
                if (m.l_ace < 20) 
                    return max_ace_update - ((m.l_ace * max_ace_update) / 20);
                
                return 0;
            }
        }
        else {
            if (winner) {
                if (m.w_ace < 15) 
                    return (m.w_ace * max_ace_update) / 15;
                
                return max_ace_update;
            }
            else {
                if (m.l_ace < 15) 
                    return max_ace_update - ((m.l_ace * max_ace_update) / 15);
                
                return 0;
            }
        }
    }

    // DF: Upper cutoff is 12 in Bo3, 16 in Bo5
    // Df >= upper: 0 for winner, and full value for loser 
    double df_update(const Match &m, const Player &p, bool winner) {
        if (m.best_of == 5) {
            if (winner) {
                if (m.w_df < 16) 
                    return max_df_update - ((m.w_df * max_df_update) / 16);
                
                return 0;
            }
            else {
                if (m.l_df < 16) 
                    return (m.l_df * max_df_update) / 16;
                
                return max_df_update;
            }
        }
        else {
            if (winner) {
                if (m.w_df < 12) 
                    return max_df_update - ((m.w_df * max_df_update) / 12);
                
                return 0;
            }
            else {
                if (m.l_df < 12) 
                    return (m.l_df * max_df_update) / 12;
                
                return max_df_update;
            }
        }
    }

    void update_recent_form_metrics(const Match &m, Player &winner, Player &loser) {
        // recent wins 
        if (winner.recent_wins < 10)
            winner.recent_wins++;
        if (loser.recent_wins > 0)
            loser.recent_wins--;

        // last match date
        winner.last_match_date = m.date;
        loser.last_match_date = m.date;
    }


    public:

    std::unordered_map<string, Player> players; // Key: player name | Value: Player info

    void update_player_ELO(const Match &m, Player &winner, Player &loser) {
        winner.ranking = m.w_rank;
        loser.ranking = m.l_rank;
        double first_update = equal_updates(m, winner, loser);

        double winner_update = first_update;
        double loser_update = first_update;

        winner_update = first_update + ace_update(m, winner, true) + df_update(m, winner, true) 
                        + (m.w_bp_conversion * max_bp_update);
        loser_update = first_update + ace_update(m, loser, false) + df_update(m, loser, false) 
                        + (max_bp_update - (m.l_bp_conversion * max_bp_update));

        switch(m.surface[0]) {
            case 'H':
                winner.elo_hard += winner_update;
                loser.elo_hard -= loser_update;
                break; 
            case 'C':
                winner.elo_clay += winner_update;
                loser.elo_clay -= loser_update;
                break;
            case 'G':
                winner.elo_grass += winner_update;
                loser.elo_grass -= loser_update;
                break;
        }
    }

    void train(string &train_file) {
        string surface, score, date;
        string winner_name, loser_name;  
        int time;
        int w_rank, l_rank;
        int w_ace, l_ace;
        int w_df, l_df;
        double l_bp_faced, w_bp_faced;
        int l_bp_saved, w_bp_saved;
        uint8_t best_of;

        io::CSVReader<17> csvin(train_file);
        csvin.read_header(io::ignore_extra_column, "surface", "tourney_date", "winner_name",
        "loser_name", "score", "best_of", "minutes", "w_ace", "w_df", "w_bpSaved", 
        "w_bpFaced", "l_ace", "l_df", "l_bpSaved", "l_bpFaced", "winner_rank", "loser_rank");

        while (csvin.read_row(surface, date, winner_name, loser_name, score, best_of, time, 
        w_ace, w_df, w_bp_saved, w_bp_faced, l_ace, l_df, l_bp_saved, l_bp_faced, w_rank, l_rank)) {
            // If somebody retired mid-match, then skip this match data
            if (score.find("RET") != std::string::npos) {
                continue;
            }
            if (!w_rank || !l_rank) {
                continue;
            }
            
            // instantiate match data from csv data
            Match *m = new Match(surface, winner_name, loser_name, score, date, time, w_ace, w_df,
            l_ace, l_df, w_rank, l_rank, best_of, w_bp_saved, w_bp_faced, l_bp_saved, l_bp_faced);

            // find players, if not found then create new Players
            auto winner = players.find(winner_name);
            auto loser = players.find(loser_name);
            if (winner == players.end()) {
                Player p1(winner_name, w_rank);
                players.emplace(winner_name, p1);
                winner = players.find(winner_name);
            }
            if (loser == players.end()) {
                Player p2(loser_name, l_rank);
                players.emplace(loser_name, p2);
                loser = players.find(loser_name);
            }
            
            last_date = m->date;
            update_player_ELO(*m, winner->second, loser->second);
            update_recent_form_metrics(*m, winner->second, loser->second);
            
            delete m;
        }
    }

    // Export Player Data to CSV
    void export_to_csv(const string &csv_file) {
        std::ofstream csv_out(csv_file);

        // Header
        csv_out << "name,last_match_date,elo_hard,elo_clay,elo_grass,ranking,recent_wins" << std::endl;

        for (const auto &p : players) {
            csv_out << p.second.name << "," << p.second.last_match_date << "," << p.second.elo_hard << "," 
                    << p.second.elo_clay << "," << p.second.elo_grass << "," << p.second.ranking << "," 
                    << p.second.recent_wins << std::endl;
        }
        csv_out.close();
    }
};


// ------------------------------------------------- Main -------------------------------------------------

// Store info from Player class into "player_data.csv"
int main() {
    std::ios::sync_with_stdio(false);

    string train_file = "other/past_year_data.csv";
    string csv_file_out = "player_data.csv";

    Prediction *p = new Prediction;
    p->train(train_file);
    p->export_to_csv(csv_file_out);

    return 0;
}
