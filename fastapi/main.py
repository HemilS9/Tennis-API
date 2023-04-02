from fastapi import FastAPI, HTTPException
import json
import csv

app = FastAPI()

def read_player_json():
    with open('player_data.json', 'r') as player_json:
        data = json.load(player_json)
    return data

player_data = read_player_json()


# HELPER FUNCTIONS

def valid_surface(surface: str):
    if (surface.lower() == "hard" or surface.lower() == "clay" or surface.lower() == "grass"):
        return True
    return False

def valid_names(name1: str, name2: str):
    all_names = []
    with open('all_players.txt', 'r') as all_players:
        for line in all_players:
            all_names.append(line.strip())

    if (name1 not in all_names) or (name2 not in all_names):
        print(name1, ", ", name2)
        return False
    
    return True

def time_since_last_match(name: str):
    last_match_date = ""
    with open("../other/past_year_data.csv", "r") as match_data:
        reader = csv.DictReader(match_data)
        for row in reader:
            last_match_date = row['tourney_date']   
    
    print("Last match date: ", last_match_date)
    player_last_match = player_data[name]["last_match_date"]

    last_year, last_month, last_day = int(last_match_date[:4]), int(last_match_date[4:6]), int(last_match_date[6:])
    last_p_year, last_p_month, last_p_day = int(player_last_match[:4]), int(player_last_match[4:6]), int(player_last_match[6:])

    result = 0
    result += (last_year - last_p_year) * 365
    result += (last_month - last_p_month) * 30
    result += (last_day - last_p_day)
    return result

def multiplier(player_data, name: str):
    mult = 1.0
    mult += (0.025 * (int(player_data[name]["recent_wins"]) / 10))

    days_since_last = time_since_last_match(name)
    if (days_since_last > 60):
        mult = 0.7
    elif (days_since_last > 25):
        mult -= (0.01 * (days_since_last - 25))

    return mult

def determine_winner(name1: str, name2: str, surface: str):
    if (surface.lower() == "hard"):
        player1_elo: float = float(player_data[name1]["elo_hard"]) * multiplier(player_data, name1)
        player2_elo: float = float(player_data[name2]["elo_hard"]) * multiplier(player_data, name2)
        return {"Winner" : name1} if (player1_elo > player2_elo) else {"Winner" : name2}
        
    elif (surface.lower() == "clay"):
        player1_elo: float = float(player_data[name1]["elo_clay"]) * multiplier(player_data, name1)
        player2_elo: float = float(player_data[name2]["elo_clay"]) * multiplier(player_data, name2)
        return {"Winner" : name1} if (player1_elo > player2_elo) else {"Winner" : name2}
        
    elif (surface.lower() == "grass"):
        player1_elo: float = float(player_data[name1]["elo_grass"]) * multiplier(player_data, name1)
        player2_elo: float = float(player_data[name2]["elo_grass"]) * multiplier(player_data, name2)
        return {"Winner" : name1} if (player1_elo > player2_elo) else {"Winner" : name2}
        
    else:
        return {"Error": "Invalid surface"}


def bestN(surface: str, n: int):
    if (surface.lower() == "hard"):
        best_players = sorted(player_data.items(), key=lambda x: float(x[1]["elo_hard"]), reverse=True)
        return best_players[:n]
        
    elif (surface.lower() == "clay"):
        best_players = sorted(player_data.items(), key=lambda x: float(x[1]["elo_clay"]), reverse=True)
        return best_players[:n]
        
    elif (surface.lower() == "grass"):
        best_players = sorted(player_data.items(), key=lambda x: float(x[1]["elo_grass"]), reverse=True)
        return best_players[:n]
        
    else:
        return {"Error": "Invalid surface"}


# ROUTES

@app.get("/")
def read_root():
    return {"Hello": "World"}


@app.get("/predict/")
def return_winner(player1: str, player2: str, surface: str):
    if (not valid_names(player1, player2)):
        raise HTTPException(status_code=404, detail="Invalid player name")
    elif (not valid_surface(surface)):
        raise HTTPException(status_code=404, detail="Invalid surface given")

    return determine_winner(player1, player2, surface)


@app.get("/current_best/{surface}")
def return_bestN(surface: str, n: int):
    if (n < 1 or n > 20):
        raise HTTPException(status_code=404, detail="n must be between 1 and 20")
    elif (not valid_surface(surface)):
        raise HTTPException(status_code=404, detail="Invalid surface given")

    # Return the best N players for the given surface
    best_players =  bestN(surface, n)
    best_players = [{"Name": player[0], "Elo": player[1]["elo_" + surface.lower()]} for player in best_players]
    return best_players