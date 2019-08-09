#include <iostream>
#include <utility>
#include <stack>
#include <map>
#include <vector>

#define pii pair<int, int>
#define mp make_pair

using namespace std;

bool checkStateVisited(pii curState, map<pii, int> statesCheck) {
    return !!statesCheck[curState];
}

pii generateNewState(pii curState, map<pii, int> *statesCheck, stack<pii> *states, int j1, int j2) {
    // Fill jug 1.
    pii tempState = { j1, curState.second };
    if (!checkStateVisited(tempState, *statesCheck)) {
        return tempState;
    }

    // Fill jug 2.
    tempState = { curState.first, j2 };
    if (!checkStateVisited(tempState, *statesCheck)) {
        return tempState;
    }

    // Empty jug 1.
    tempState = { 0, curState.second };
    if (!checkStateVisited(tempState, *statesCheck)) {
        return tempState;
    }

    // Empty jug 2.
    tempState = { curState.first, 0 };
    if (!checkStateVisited(tempState, *statesCheck)) {
        return tempState;
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
            return tempState;
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
            return tempState;
        }
    }
}

void DFS(int j1, int j2, int t1, int t2) {
    map<pii, int> statesCheck;
    vector<pii> path;
    stack<pii> states;
    bool isSolvable = false;

    states.push({0, 0});
    while(!states.empty()) {
        pii curState = states.top();
        states.pop();

        if (statesCheck[curState]) {
            continue;
        }

        path.push_back(curState);
        statesCheck[curState] = 1;

        if (curState.first == t1) {
            isSolvable = true;

            if (curState.second != t2) {
                path.push_back({ curState.first, 0 });
            }

            for (auto item = path.begin(); item != path.end(); item++) {
                cout<< "(" << (*item).first << ", " << (*item).second << ")" <<endl;
            }

            break;
        }

        pii tempState = generateNewState(curState, &statesCheck, &states, j1, j2);

        states.push(tempState);
    }

    if (!isSolvable) {
        cout<<"The solution does not exist..!!";
    }
}

int main() {
    int jug1 = 4, jug2 = 3, target1 = 2, target2 = 0;
    DFS(jug1, jug2, target1, target2);
    return 0;
}
