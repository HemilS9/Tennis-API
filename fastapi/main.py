# from typing import Union
from fastapi import FastAPI
import json

app = FastAPI()


# HELPER FUNCTIONS

def valid_names(name1, name2):
    all_names = []
    with open('all_players.txt', 'r') as all_players:
        for line in all_players:
            all_names.append(line.strip())

    if (name1 not in all_names) or (name2 not in all_names):
        print(name1, ", ", name2)
        return False
    
    return True

def read_player_json():
    with open('player_data.json', 'r') as player_json:
        data = json.load(player_json)
    return data

def multiplier(player_data, name: str):
    mult = 1.0
    mult += (0.025 * (int(player_data[name]["recent_wins"]) / 10))
    return mult

def determine_winner(name1, name2, surface):
    # Load player data from the JSON
    player_data = read_player_json()

    # Determine the winner
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

# ROUTES

@app.get("/")
def read_root():
    return {"Hello": "World"}


@app.get("/{name1}/{name2}/{surface}")
def return_winner(name1, name2, surface):
    # Error check player names
    if (not valid_names(name1, name2)):
        return {"Error": "Invalid player name"}

    # Determine the winner
    return determine_winner(name1, name2, surface)