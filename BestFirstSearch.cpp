#include <iostream>
#include <queue>
#include <map>
#include <utility>
#include <vector>
#include <functional>
#include <algorithm>

#define pii pair<int, int>
#define mp map<pii, int>
#define pb push_back

using namespace std;

int calcHeuristics(pii state, int target1) {
    return abs(state.first - target1) + abs(state.second - target1);
}

void evalAndAdd(pii newState, vector<pii> *openStates, mp *stateHeuristics, mp *states, int target) {
    if (!(*states)[newState]) {
        (*stateHeuristics)[newState] = calcHeuristics(newState, target);
        (*openStates).pb(newState);
    }
}

void generateNewStates(pii curState, vector<pii> *openStates, mp *stateHeuristics, mp *states, int j1, int j2, int target) {
    pii tempState = { j1, curState.second };
    evalAndAdd(tempState, openStates, stateHeuristics, states, target);

    tempState = {curState.first, j2};
    evalAndAdd(tempState, openStates, stateHeuristics, states, target);

    tempState = {0, curState.second};
    evalAndAdd(tempState, openStates, stateHeuristics, states, target);

    tempState = {curState.first, 0};
    evalAndAdd(tempState, openStates, stateHeuristics, states, target);

    if ((curState.second < j2) && (curState.first != 0)) {
        int diff = j2 - curState.second;
        if (diff >= curState.first) {
            tempState = {0, curState.second + curState.first};
        } else {
            tempState = {curState.first - diff, curState.second + diff};
        }

        evalAndAdd(tempState, openStates, stateHeuristics, states, target);
    }

    if ((curState.first < j1) && (curState.second != 0)) {
        int diff = j1 - curState.first;
        if (diff >= curState.second) {
            tempState = {curState.second + curState.first, 0};
        } else {
            tempState = {curState.first + diff, curState.second - diff};
        }

        evalAndAdd(tempState, openStates, stateHeuristics, states, target);
    }
}

bool bestFirstSearchRecursive(pii curState, map<pii, int> *statesCheck,
                   vector<pii> *path, mp *stateHeuristics, int j1, int j2, int target) {

    vector<pii> tempOpenStates;

    if ((curState.first == target) && (curState.second == 0)) {
        return true;
    }

    generateNewStates(curState, &tempOpenStates, stateHeuristics, statesCheck, j1, j2, target);

    pii checkMinState = (*min_element((*stateHeuristics).begin(), (*stateHeuristics).end(),
            [](const auto& l, const auto& r) { return l.second < r.second; })).first;

    (*stateHeuristics).erase(checkMinState);

    (*statesCheck)[checkMinState] = 1;

    if (bestFirstSearchRecursive(checkMinState, statesCheck, path, stateHeuristics, j1, j2, target)) {
        (*path).push_back(checkMinState);
        return true;
    }

    return false;
}

void BestFirstSearchDriver(int j1, int j2, int t) {
    mp states;
    vector<pii> path;
    mp stateHeuristics;
    bool isSolvable = false;

    pii initialState = {0, 0};
    stateHeuristics[initialState] = calcHeuristics(initialState, t);

    states[initialState] = 1;

    if (bestFirstSearchRecursive(initialState, &states, &path, &stateHeuristics, j1, j2, t)) {
        isSolvable = true;
        path.pb({0, 0});
    }

    if (!isSolvable) {
        cout<<"Solution not found..!!";
    } else {
        for(auto ele = path.rbegin(); ele != path.rend(); ele++) {
            cout<< "(" << ele->first << ", " << ele->second <<")"<<endl;
        }
    }
}

int main() {
    int jug1 = 4, jug2 = 3, target1 = 2;
    BestFirstSearchDriver(jug1, jug2, target1);
    return 0;
}