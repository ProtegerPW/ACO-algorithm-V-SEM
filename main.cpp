#include <iostream>
#include <fstream>
#include <string>
#include "ACO.h"

using namespace std;

int main(int argc, char const *argv[])
{
    if (argc < 2)
        return 0;

    antsColony ACO(argv[1]);

    return 0;
}