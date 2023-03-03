import csv
import json

csvFile = 'player_data.csv'
jsonFile = 'player_data.json'

def csv_to_json(csvFile, jsonFile):
    my_json = {}
    with open(csvFile, 'r') as inputCSV:
        reader = csv.DictReader(inputCSV)
        for row in reader:
            key = row['name']
            my_json[key] = row 

    with open(jsonFile,'w') as jsonObj:
        jsonObj.write(json.dumps(my_json, indent=2))

csv_to_json(csvFile, jsonFile)