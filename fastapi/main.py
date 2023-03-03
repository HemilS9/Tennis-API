# from typing import Union
from fastapi import FastAPI
import json

app = FastAPI()


# HELPER FUNCTIONS

def valid_names(name1, name2):
    all_names = []
    with open('all_players.txt', 'r') as all_players:
        for line in all_players:
            all_names.append(line)

    print(all_names[0])
    if (name1 not in all_names) or (name2 not in all_names):
        return False
    
    return True

def valid_surface(surface):
    if surface.lower() not in ['hard', 'clay', 'grass']:
        return False

def read_player_json():
    with open('player_data.json', 'r') as player_json:
        data = json.load(player_json)
    return data


# ROUTES

@app.get("/")
def read_root():
    return {"Hello": "World"}


@app.get("/{name1}/{name2}/{surface}")
async def return_winner(name1, name2, surface):
    # Error check the arguments
    if (not valid_names(name1, name2)):
        return {"Error": "Invalid player name"}

    # Load player data from the JSON

    # Determine the winner

    return {"Winner": name1}