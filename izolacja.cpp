//
// Created by jacek on 5/31/20.
// Here will be implemented project in one file
//
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdio>
#include <algorithm>
#include <sstream>

using namespace std;

class Map{

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
public:
    Map(const string &fileName) {
        readFile(fileName);
    }

    Map(){
        readFromConsole();
    }

    char get(int x, int y) {
        return map[x * W + y];
    }

    void set(int x, int y, char val) {
        map[x * W + y] = val;
    }

    int getGraph(int x, int y) {
        return graphInt[y * numberRoad + x];
    }

    void setGraph(int x, int y, int val) {
        graphInt[y * numberRoad + x] = val;
    }

    bool isRoad(int x, int y) {
        return get(x, y) != '.';
    }

    bool isCross(int x, int y) {
        return get(x, y) == '+';
    }

    int getGraphPosition(int h, int w) {
        for (int i = 0; i < numberRoad; i++) {
            if (graph[i].h == h && graph[i].w == w) {
                return i;
            }
        }
        return -1;
    }

    void readFromConsole(){
        string s;
        getline(cin, s);

        std::stringstream in(s);
        in >> W;
        in >> H;
        in >> L;
        in >> K;

        map = new char[W * H];

        getline(cin, parkName);

        int a=0;
        for (int i = 0; i< H;i++){
            getline(cin, s);
            for (int i = 0; i < s.size(); i++) {
                set(a, i, s[i]);
            }
            a++;
        }
    }

    void readFile(const std::string &fileName) {
        std::ifstream infile("/home/jacek/studia/semestr_6/apto/project/izolacja/maps/" + fileName, std::ios_base::in);

        if (!infile.is_open()) {
            perror("Error open");
            exit(EXIT_FAILURE);
        }

        infile >> W >> H >> L >> K;
        infile >> parkName;

        std::string line;
        map = new char[W * H];

        int a = 0;
        while (getline(infile, line)) {
            if (!line.empty()) {
                for (int i = 0; i < line.size(); i++) {
                    set(a, i, line[i]);
                }
                a++;
            }
        }
    }

    void convertToGraph() {
        int s = 0;

//    calculate number of road
        for (int i = 0; i < H; i++) {
            for (int j = 0; j < W; j++) {
                if (isRoad(i, j)) {
                    s++;
                }
            }
        }

        numberRoad = s;

        graphInt = new int[numberRoad * numberRoad];
        graph = new Position[numberRoad];

        for (int i = 0; i < numberRoad * numberRoad; i++) {
            graphInt[i] = -1;
        }

        setUpGraphPosition();

        int i, j;
        for (s = 0; s < numberRoad; s++) {
            i = graph[s].h;
            j = graph[s].w;
            setGraph(s, s, 0);

//            down
            setDistanceIfConnection(i, j, i - 1, j, '|', s);
//            up
            setDistanceIfConnection(i, j, i + 1, j, '|', s);
//            right
            setDistanceIfConnection(i, j, i, j + 1, '-', s);
//            left
            setDistanceIfConnection(i, j, i, j - 1, '-', s);
        }
    }

    void setUpGraphPosition() {
        int s = 0;
        for (int i = 0; i < H; i++) {
            for (int j = 0; j < W; j++) {
                if (isRoad(i, j)) {
                    graph[s].h = i;
                    graph[s].w = j;
                    s++;
                }
            }
        }
    }

    void setDistanceIfConnection(int x1, int y1, int x2, int y2, char connection_mode, int s) {
        if (isConnection(x1, y1, x2, y2, connection_mode)) {
            int p = getGraphPosition(x2, y2);
            setGraph(p, s, 1);
            setGraph(s, p, 1);
        }
    }

    bool isConnection(int x1, int y1, int x2, int y2, char connection_mode) {
        return isRoad(x1, y1) && isRoad(x2, y2) && (
                isCross(x1, y1) && (isCross(x2, y2) || get(x2, y2) == connection_mode)
                || get(x1, y1) == connection_mode && (isCross(x2, y2) || get(x2, y2) == connection_mode)
        );
    }

    void floydWarshall() {
        int k, i, j, weight;

        for (k = 0; k < numberRoad; k++) {
            for (i = 0; i < numberRoad; i++) {
                for (j = 0; j < numberRoad; j++) {
                    if (getGraph(i, k) == -1 || getGraph(k, j) == -1) {
                        continue;
                    }

                    weight = getGraph(i, k) + getGraph(k, j);

                    if (getGraph(i, j) > weight || getGraph(i, j) == -1) {
                        setGraph(i, j, weight);
                    }
                }
            }
        }
    }


    void zerosIfDistanceTooShort() {
        for (int i = 0; i < numberRoad; i++) {
            for (int j = 0; j < numberRoad; j++) {
                if (getGraph(j, i) == -1) {
                    continue;
                }

                if (getGraph(j, i) <= L && getGraph(j, i) != 0) {
                    setGraph(j, i, -2);
                    setGraph(i, j, -2);
                }
            }
        }
    }

    bool bronKerbosch(const vector<int> &SR, vector<int> SP, vector<int> SX) {
        if (SP.empty() && SX.empty() && SR.size() >= K) {
//            cout << "Found solution" << endl;

//            for (int i : SR) {
            for (int i = 0; i<K; i++){
//                cout << "(" << graph[i].h << ", " << graph[i].w << "), ";
                cout <<  graph[SR[i]].w << " " << graph[SR[i]].h << endl;
            }

            return true;
        }

        std::vector<int> RP;
        std::vector<int> PP;
        std::vector<int> XP;
        std::vector<int> SN;
        std::vector<int> temp;

// for v wierzcholki PPP
        for (int v: SP) {
            SN.clear();
            RP.clear();
            XP.clear();
            PP.clear();

            for (int p = 0; p < numberRoad; p++) {
                if (getGraph(p, v) != -2 && getGraph(p, v) != 0) {
                    SN.push_back(p);
                }
            }

            for (int i : SR) {
                RP.push_back(i);
            }

            RP.push_back(v);


            for (int &i : SN) {
                if (std::find(SP.begin(), SP.end(), i) != SP.end()) {
                    PP.push_back(i);
                }
            }

            for (int &i : SN) {
                if (std::find(SX.begin(), SX.end(), i) != SX.end()) {
                    XP.push_back(i);
                }
            }

            if (bronKerbosch(RP, PP, XP)) {
                return true;
            }

            temp.clear();
            for (int &i : SP) {
                if (i != v) {
                    temp.push_back(i);
                }
            }

            SP.clear();

            for (int &i : temp) {
                SP.push_back(i);
            }

            SX.push_back(v);

        }

        return false;
    }


    void printGraph() {
        printf("Graph \n");
        for (int i = 0; i < numberRoad; i++) {
            for (int j = 0; j < numberRoad; j++) {
                printf("%d ", getGraph(i, j));
            }
            printf("\n");
        }
    }

    void printGraphPosition() {
        printf("Graph Position %d \n", numberRoad);
        for (int i = 0; i < numberRoad; i++) {
            printf("%d: %d %d \n", i, graph[i].h, graph[i].w);
        }
    }

    void printLabirynt() {
        printf("Height: %d \n", H);
        printf("Width: %d \n", W);
        printf("Odstep: %d \n", L);
        printf("People: %d \n", K);

        for (int i = 0; i < H; i++) {
            printf("%d: ", i);
            for (int j = 0; j < W; j++) {
                printf("%c", get(i, j));
            }
            printf("\n");
        }
    }

    void solve() {
        std::vector<int> SR;
        std::vector<int> SX;
        std::vector<int> SP;

        SP.reserve(numberRoad);

        for (int i = 0; i < numberRoad; i++) {
            SP.push_back(i);
        }

        bronKerbosch(SR, SP, SX);
    }
};

int main(int argc, char *argv[]) {
    Map map;

    map.convertToGraph();
    map.floydWarshall();
    map.zerosIfDistanceTooShort();
    map.solve();
}

