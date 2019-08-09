#include <iostream>
#include <vector>
#include <utility>
#include <map>
#include <queue>
#define pii pair<int, int>

using namespace std;

void checkVisitedStateAndPushToQueue(pii state, map<pii, int> m, queue<pii> **q) {
    if (m[state] != 1) {
        (**q).push(state);
    }
}

void generateNewStates(pii curState, map<pii, int> *m, queue<pii> *q, int j1, int j2) {    
    pii tempState = { j1, curState.second };
    checkVisitedStateAndPushToQueue(tempState, *m, &q);

    tempState = { curState.first, j2 };
    checkVisitedStateAndPushToQueue(tempState, *m, &q);

    tempState = { 0, curState.second };
    checkVisitedStateAndPushToQueue(tempState, *m, &q);

    tempState = { curState.first, 0 };
    checkVisitedStateAndPushToQueue(tempState, *m, &q);

    // Pour from jug 1 to jug 2.
    if ((curState.second < j2) && (curState.first != 0)) {
        int diff = j2 - curState.second;
        if (diff >= curState.first) {
            tempState = { 0, curState.second + curState.first };
        } else {
            tempState = { curState.first - diff, curState.second + diff };
        }
        checkVisitedStateAndPushToQueue(tempState, *m, &q);
    }

    // Pour from jug 2 to jug 1.
    if ((curState.first < j1) && (curState.second != 0)) {
        int diff = j1 - curState.first;
        if (diff >= curState.second) {
            tempState = { curState.first + curState.second , 0};
        } else {
            tempState = { curState.first + diff, curState.second - diff };
        }

        checkVisitedStateAndPushToQueue(tempState, *m, &q);
    }
}

void BFS(int j1, int j2, int target) {
    map<pii, int> states;
    bool isSolvable = false;
    vector<pii> path;

    // This queue is used to maintain the states. 
    queue<pii> q;
    q.push({0, 0});

    while (!q.empty()) {
        pii curState = q.front();
        q.pop();

        if (states[curState] == 1) {
            continue;
        }

        if ((curState.first > j1 || curState.second > j2) || (curState.first < 0 || curState.second < 0)) {
            continue;
        }

        path.push_back(curState);
        states[curState] = 1;

        if (curState.first == target) {
            isSolvable = true;
            if (curState.second != 0) {
                path.push_back({ curState.first, 0 });
            }

            for(auto i = path.begin(); i != path.end(); i++) {
                cout<<"("<<(*i).first <<", "<<(*i).second <<")"<<endl;
            }
            break;
        }

        generateNewStates(curState, &states, &q, j1, j2);
    }
    
    if (!isSolvable) {
        cout<<"No solution..!!"<<endl;
    }
}

int main() {
    int Jug1 = 4, Jug2 = 3, target = 2;
    cout<<"Path from initial state to solution state:"<<endl;
    BFS(Jug1, Jug2, target);
    return 0;
}