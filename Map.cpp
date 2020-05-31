//
// Created by jacek on 14.05.2020.
//

#include "Map.h"

Map::Map(const string &fileName) {
    readFile(fileName);
}

char Map::get(int x, int y) {
    return this->map[x * this->W + y];
}

void Map::set(int x, int y, char val) {
    this->map[x * this->W + y] = val;
}

int Map::getGraph(int x, int y) {
    return this->graphInt[y * this->numberRoad + x];
}

void Map::setGraph(int x, int y, int val) {
    this->graphInt[y * this->numberRoad + x] = val;
}

bool Map::isRoad(int x, int y) {
    return get(x, y) != '.';
}

bool Map::isCross(int x, int y) {
    return get(x, y) == '+';
}

int Map::getGraphPosition(int h, int w) {
    for (int i = 0; i < this->numberRoad; i++) {
        if (graph[i].h == h && graph[i].w == w) {
            return i;
        }
    }
    return -1;
}

void Map::readFile(const std::string &fileName) {
    std::ifstream infile("/home/jacek/studia/semestr_6/apto/project/izolacja/maps/" + fileName, std::ios_base::in);

    if (!infile.is_open()) {
        perror("Error open");
        exit(EXIT_FAILURE);
    }

    infile >> this->W >> this->H >> this->L >> this->K;
    infile >> parkName;

    std::string line;
    this->map = new char[this->W * this->H];

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

void Map::convertToGraph() {
    int s = 0;

//    calculate number of road
    for (int i = 0; i < this->H; i++) {
        for (int j = 0; j < this->W; j++) {
            if (isRoad(i, j)) {
                s++;
            }
        }
    }

    numberRoad = s;

    this->graphInt = new int[numberRoad * numberRoad];
    this->graph = new Position[numberRoad];

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

void Map::setUpGraphPosition() {
    int s = 0;
    for (int i = 0; i < this->H; i++) {
        for (int j = 0; j < this->W; j++) {
            if (isRoad(i, j)) {
                this->graph[s].h = i;
                this->graph[s].w = j;
                s++;
            }
        }
    }
}

void Map::setDistanceIfConnection(int x1, int y1, int x2, int y2, char connection_mode, int s) {
    if (isConnection(x1, y1, x2, y2, connection_mode)) {
        int p = getGraphPosition(x2, y2);
        setGraph(p, s, 1);
        setGraph(s, p, 1);
    }
}

bool Map::isConnection(int x1, int y1, int x2, int y2, char connection_mode) {
    return isRoad(x1, y1) && isRoad(x2, y2) && (
            isCross(x1, y1) && (isCross(x2, y2) || get(x2, y2) == connection_mode)
            || get(x1, y1) == connection_mode && (isCross(x2, y2) || get(x2, y2) == connection_mode)
    );
}

void Map::floydWarshall() {
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


void Map::zerosIfDistanceTooShort() {
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

bool Map::bronKerbosch(const vector<int> &SR, vector<int> SP, vector<int> SX) {
    if (SP.empty() && SX.empty() && SR.size() >= K) {
        cout << "Found solution" << endl;

        for (int i : SR) {
            cout << "(" << graph[i].h << ", " << graph[i].w << "), ";
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


void Map::printGraph() {
    printf("Graph \n");
    for (int i = 0; i < this->numberRoad; i++) {
        for (int j = 0; j < this->numberRoad; j++) {
            printf("%d ", getGraph(i, j));
        }
        printf("\n");
    }
}

void Map::printGraphPosition() {
    printf("Graph Position %d \n", this->numberRoad);
    for (int i = 0; i < this->numberRoad; i++) {
        printf("%d: %d %d \n", i, this->graph[i].h, this->graph[i].w);
    }
}

void Map::printLabirynt() {
    printf("Height: %d \n", this->H);
    printf("Width: %d \n", this->W);
    printf("Odstep: %d \n", this->L);
    printf("People: %d \n", this->K);

    for (int i = 0; i < this->H; i++) {
        printf("%d: ", i);
        for (int j = 0; j < this->W; j++) {
            printf("%c", get(i, j));
        }
        printf("\n");
    }
}

void Map::solve() {
    std::vector<int> SR;
    std::vector<int> SX;
    std::vector<int> SP;

    SP.reserve(numberRoad);

    for (int i = 0; i < numberRoad; i++) {
        SP.push_back(i);
    }

    bronKerbosch(SR, SP, SX);
}



