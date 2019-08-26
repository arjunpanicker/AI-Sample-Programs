#include <iostream>
#include <utility>
#include <queue>
#include <stack>
#include <map>
#include <vector>

#define pii pair<int, int>
#define mp make_pair

using namespace std;

bool checkStateVisited(pii curState, map<pii, int> statesCheck) {
    return !!statesCheck[curState];
}

void generateNewState(pii curState, map<pii, int> *statesCheck, vector<pii> *openStatesBfs, int j1, int j2) {
    // Fill jug 1.
    pii tempState = { j1, curState.second };
    if (!checkStateVisited(tempState, *statesCheck)) {
        (*openStatesBfs).push_back(tempState);
    }

    // Fill jug 2.
    tempState = { curState.first, j2 };
    if (!checkStateVisited(tempState, *statesCheck)) {
        (*openStatesBfs).push_back(tempState);
    }

    // Empty jug 1.
    tempState = { 0, curState.second };
    if (!checkStateVisited(tempState, *statesCheck)) {
        (*openStatesBfs).push_back(tempState);
    }

    // Empty jug 2.
    tempState = { curState.first, 0 };
    if (!checkStateVisited(tempState, *statesCheck)) {
        (*openStatesBfs).push_back(tempState);
    }

    // Pour from jug 2 to jug 1.
    if ((curState.first < j1) && (curState.second != 0)) {
        int diff = j1 - curState.first;
        if (diff >= curState.second) {
            // Pour all content of jug 2 to jug 1.
            tempState = {curState.first + curState.second, 0};
        } else {
            // Pour some content of jug 2 to jug 1.
            tempState = { curState.first + diff, curState.second - diff };
        }

        if (!checkStateVisited(tempState, *statesCheck)) {
            (*openStatesBfs).push_back(tempState);
        }
    }

    // Pour from jug 1 to jug 2.
    if ((curState.second < j2) && (curState.first != 0)) {
        int diff = j2 - curState.second;
        if (diff >= curState.first) {
            // Pour all content of jug 1 to jug 2.
            tempState = { 0, curState.second + curState.first };
        } else {
            // Pour some content of jug 1 to jug 2.
            tempState = { curState.first - diff, curState.second + diff };
        }

        if (!checkStateVisited(tempState, *statesCheck)) {
            (*openStatesBfs).push_back(tempState);
        }
    }
}

// Recursive function for DFID search
bool RecDFIDSearch(pii curState, map<pii, int> *statesCheck,
                   vector<pii> *path, int j1, int j2, int target, int limit) {

    vector<pii> tempOpenStates;
    
    // Clear the variables if limit hits 0
    if (limit == 0) {
        (*statesCheck).clear();
        (*statesCheck)[{0, 0}] = 1;
        tempOpenStates.clear();
        return false;
    }

    if ((curState.first == target) && (curState.second == 0)) {
        return true;
    } else {
        (*statesCheck)[curState] = 1;
    }

    // Generate new states and add them to a temporary vector
    generateNewState(curState, statesCheck, &tempOpenStates, j1, j2);

    for(auto ele = tempOpenStates.begin(); ele != tempOpenStates.end(); ele++) {

        (*statesCheck)[(*ele)] = 1;

        if (RecDFIDSearch((*ele), statesCheck, path, j1, j2, target, limit - 1)) {
            (*path).push_back((*ele));
            return true;
        }
    }

    return false;
}

void DFIDSearch(int j1, int j2, int t1, int limit) {
    map<pii, int> statesCheck;
    stack<pii> openStatesDfs;
    vector<pii> openStatesBfs;
    vector<pii> path;
    bool isSolvable = false;

    openStatesDfs.push({0, 0});

    while(!openStatesDfs.empty()) {
        pii curState = openStatesDfs.top();

        statesCheck[curState] == 1;

        if (!(limit == 0 || (curState.first == t1))) {
            if (RecDFIDSearch(curState, &statesCheck, &path, j1, j2, t1, limit)) {
                path.push_back({0, 0});
                isSolvable = true;
                break;
            }
        }

    }

    if (!isSolvable) {
        cout<<"The solution does not exist..!!";
    } else {
        for(auto ele = path.rbegin(); ele != path.rend(); ele++) {
            cout<< "(" << ele->first << ", " << ele->second <<")"<<endl;
        }
    }
}

int main() {
    int jug1 = 4, jug2 = 3, target1 = 2, limit = 7;
    DFIDSearch(jug1, jug2, target1, limit);
    return 0;
}