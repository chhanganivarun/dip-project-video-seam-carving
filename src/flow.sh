g++ -o preproc preproc.cpp
./preproc < array.txt > graph.txt
# echo Preproc complete
g++ -w -o graphcut main.cpp graph.h block.h graph.cpp maxflow.cpp
./graphcut < graph.txt > seamout.txt
# echo Graphcut complete
python get_seamtry.py
# echo Postproc complete