@echo off
@REM g++ -o preproc preproc.cpp
preproc.exe < array.txt > graph.txt
@REM echo Preproc complete
@REM g++ -w -o graphcut main.cpp graph.h block.h graph.cpp maxflow.cpp
graphcut.exe < graph.txt > seamout.txt
@REM echo Graphcut complete
type seamout.txt >> rgbseams.txt
python get_seamtry.py
@REM echo Postproc complete