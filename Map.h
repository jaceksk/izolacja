//
// Created by jacek on 14.05.2020.
//

#ifndef IZOLACJA_MAP_H
#define IZOLACJA_MAP_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Map.h"
#include <cstdio>
#include <algorithm>

using namespace std;

class Map {
    int W;
    int H;
    int L;  // odległośc pomiedzy osobami
    int K; // ilosc osob do umieszczenia w parku
    int numberRoad;

    string parkName;

    struct Position
    {
        int h;
        int w;
    };

    char *map;
    Position *graph;
    int *graphInt;

    char get(int x, int y);
    void set(int x, int y, char val);
    void setGraph(int x, int y, int val);
    int getGraph(int x, int y);
    int getGraphPosition(int x, int y);
    bool isRoad(int x, int y);
    bool isCross(int x, int y);

    bool isConnection(int x1, int y1, int x2, int y2, char connection_mode);
    void setDistanceIfConnection(int x1, int y1, int x2, int y2, char connection_mode, int s);
    void setUpGraphPosition();


public:

    Map(const string& fileName);
    void readFile(const std::string &fileName);

    void convertToGraph();
    void zerosIfDistanceTooShort();


    void floydWarshall();
    bool bronKerbosch(const std::vector<int>& SP, std::vector<int> SR, std::vector<int> SX);
    void solve();

    void printGraph();
    void printLabirynt();
    void printGraphPosition();

};


#endif //IZOLACJA_MAP_H
