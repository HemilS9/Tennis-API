# Tennis-API
This API currently supports 2 primary features:
1. It can return the predicted winner of an ATP tennis match when given the names of the 2 players (see `all_players.txt` for the list of all players) and the surface (Hard, Clay, Grass)
2. It can return the top N (1 to 20) ATP players by the given surface (Hard, Clay, Grass)

## Installation
- First, navigate to the fastapi directory: `cd fastapi`
- Create a new virtual environment with: `python3 -m venv myenv`
- Activate your virtual environmnt with: `source myenv/bin/activate`
- Install fastapi and uvicorn: `pip install fastapi` and `pip install uvicorn`

## Calling the API
- Enter the following commands in the terminal:
    - `cd fastapi`
    - `source myenv/bin/activate`
    - `uvicorn main:app --reload`
- Navigate to `localhost:8000` on your browser

## Predicting the Winner of a Match
- Change the URL to: `localhost:8000/predict/?player1=First%20Last&player2=First%20Last&surface=Surface`
- Replace Surface with the name of the surface (Hard/Clay/Grass)

## Viewing the Best Players by Surface (Hard, Clay, Grass)
- Change the URL to: `localhost:8000/current_best/surface/?n=N`
- Replace surface with the name of the surface (Hard/Clay/Grass)
- Replace N with a number between 1 and 20

## Credits:
- The CSV parser used in this project is by Ben Strasser: https://github.com/ben-strasser/fast-cpp-csv-parser
- The match data in CSV format is compiled by Jeff Sackmann: https://github.com/JeffSackmann/tennis_atp
- The command line version which I originally created: https://github.com/HemilS9/Tennis-Match-Prediction

Created by Hemil Shah: <heshah@umich.edu>