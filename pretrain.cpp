#include <iostream>
#include <fstream>
#include <unordered_map>
#include <iomanip>
#include "csv.h"
// #include "predict.h"

using std::vector, std::string, std::cout, std::endl;


// ----------------------------------------- Function Declarations -----------------------------------------
void print_header(string csv_file);


// ------------------------------------------------- Main -------------------------------------------------

// Store info from Player class into "player_data.csv"
int main() {
    const string csv_file_name = "player_data.csv";
    print_header(csv_file_name);

    return 0;
}


// ---------------------------------------- Function Implementations ----------------------------------------

void print_header(string csv_file) {
    std::ofstream csv_out(csv_file);
    csv_out << "name,last_match_data,elo_hard,elo_clay,elo_grass,ranking,recent_wins" << std::endl;
    csv_out.close();
}