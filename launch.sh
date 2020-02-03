#/bin/bash
gcc main.c -o main 
./main

python ./visualisation/vis.py smithOrder.txt
python ./visualisation/vis.py desc_smithOrder.txt
python ./visualisation/vis.py KOrder.txt
python ./visualisation/vis.py ROrder.txt
