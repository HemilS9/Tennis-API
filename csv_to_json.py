import csv
import json

csvFile = 'player_data.csv'
jsonFile = 'fastapi/player_data.json'
allPlayersFile = "fastapi/all_players.txt"

my_json = {}

def csv_to_json(csvFile, jsonFile):
    with open(csvFile, 'r') as inputCSV:
        reader = csv.DictReader(inputCSV)
        for row in reader:
            if (row['ranking'] != '0'):
                key = row['name']
                my_json[key] = row 

    with open(jsonFile,'w') as jsonObj:
        jsonObj.write(json.dumps(my_json, indent=2))

def store_player_names(playerFile):
    with open(playerFile, 'w') as fileObj:
        fileObj.writelines([name + "\n" for name in my_json.keys()])

csv_to_json(csvFile, jsonFile)
store_player_names(allPlayersFile)