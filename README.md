# ACO algorithm

Algorithm in C++ to solve discrete optimization problems like for example: TSP.

# Building 
 - clone repository
 `git clone https://github.com/ProtegerPW/ACO-algorithm-V-SEM`

 - create **bin** folder in repository folder

 - build app using command below
 `make`

 - copy example graph from **src/Graphs** to **bin**
 - run 
 `cd bin\
 ./ACO.o < fileWithGraph > < beginVertice > < endVertice >`
 
 or using shell script from **src** - copy it to **bin** folder
 (script runs program in loops and writes data to file **acoResults**
 `./run.sh < startVertice > < endVertice>`
