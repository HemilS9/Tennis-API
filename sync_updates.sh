#!/bin/bash

make pretrain.exe
./predict.exe
make clean

python3 csv_to_json.py