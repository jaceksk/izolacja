//
// Created by jacek on 05.05.2020.
//

#include <string>
#include "Map.h"

using namespace std;

int main(int argc, char *argv[]) {
    string fileName;

    if (argc == 1) {
        fileName = "trivial.in";
    } else {
        fileName = argv[1];
    }

    Map map("trivial.in");

    map.convertToGraph();
    map.floydWarshall();
    map.zerosIfDistanceTooShort();
    map.solve();
}
