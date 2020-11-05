#include <iostream>
#include <fstream>
#include <string>
#include "ACO.h"

using namespace std;

int main(int argc, char const *argv[])
{
    if (argc < 4)
        return 0;

    antsColony ACO(argv[1], (int)argv[2], (int)argv[3]);

    return 0;
}