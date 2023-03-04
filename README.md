# Tennis-API
An API that returns the predicted winner of a tennis match when given the 2 player names and the surface (Hard, Clay, Grass)

### Installation
First, navigate to the fastapi directory: `cd fastapi`
Create a new virtual environment with: `python3 -m venv myenv`
Activate your virtual environmnt with: `source myenv/bin/activate`
Install fastapi and uvicorn: `pip install fastapi` and `pip install uvicorn`

### Calling the API
`cd fastapi`
`source myenv/bin/activate`
`uvicorn main:app --reload`
Head to `localhost:8000` on your browser
Change the URL to: `localhost:8000/Player 1's Name/Player 2's Name/surface`
