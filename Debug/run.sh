#!/bin/bash

#libLocation="/path/to/libGlucoseLib.so"
libLocation="/home/simone/Workspaces/WorkspaceC++/IgraphSAT/MyUtil"
export LD_LIBRARY_PATH=$libLocation:$LD_LIBRARY_PATH

./IgraphSAT Prova.txt
