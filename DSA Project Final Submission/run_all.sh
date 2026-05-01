#!/bin/bash
python3 testcase_generator.py $1

make build

make phase1
./phase1 $1 ./queries/phase1_queries.json phase1_outputs.json

make phase2
./phase2 $1 ./queries/phase2_queries.json phase2_outputs.json

make phase3
./phase3 $1 ./queries/phase3_queries.json phase3_outputs.json

mkdir -p outputs
mv phase[1-3]_outputs.json outputs/