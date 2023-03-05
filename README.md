# Tennis-API
This API returns the predicted winner of an ATP tennis match when given the names of the 2 players (see `all_players.txt` for the list of all players) and the surface (Hard, Clay, Grass)

### Installation
- First, navigate to the fastapi directory: `cd fastapi`
- Create a new virtual environment with: `python3 -m venv myenv`
- Activate your virtual environmnt with: `source myenv/bin/activate`
- Install fastapi and uvicorn: `pip install fastapi` and `pip install uvicorn`

### Calling the API
- Enter the following commands in the terminal:
    - `cd fastapi`
    - `source myenv/bin/activate`
    - `uvicorn main:app --reload`
- Navigate to `localhost:8000` on your browser
- Change the URL to: `localhost:8000/Player 1's Name/Player 2's Name/surface`
- When you're finished, deactivate your virtual environment with `deactivate`