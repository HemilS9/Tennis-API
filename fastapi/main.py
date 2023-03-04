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

def determine_winner(name1, name2, surface):
    # Load player data from the JSON
    player_data = read_player_json()

    # Determine the winner
    if (surface.lower() == "hard"):
        # print(name1, ": ", player_data[name1]["elo_hard"])
        # print(name2, ": ", player_data[name2]["elo_hard"])
        if (float(player_data[name1]["elo_hard"]) > float(player_data[name2]["elo_hard"])):
            return {"Winner" : name1}
        else:
            return {"Winner" : name2}
        
    elif (surface.lower() == "clay"):
        if (float(player_data[name1]["elo_clay"]) > float(player_data[name2]["elo_clay"])):
            return {"Winner" : name1}
        else:
            return {"Winner" : name2}
        
    elif (surface.lower() == "grass"):
        if (float(player_data[name1]["elo_grass"]) > float(player_data[name2]["elo_grass"])):
            return {"Winner" : name1}
        else:
            return {"Winner" : name2}
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