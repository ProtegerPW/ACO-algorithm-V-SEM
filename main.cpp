#include <iostream>
#include <fstream>
#include <string>
#include "ACO.h"

using namespace std;

int main(int argc, char const *argv[])
{
    if (argc < 4)
    {
        cout << "Not enough arguments" << endl;
        return 0;
    }

    AntsColony ACO(argv[1], atoi(argv[2]), atoi(argv[3]));

    return 0;
}