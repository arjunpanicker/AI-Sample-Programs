#include <iostream>
#include <vector>
#include <utility>
#include <map>
#include <queue>
#include <cstdlib>
#include <cmath>

#define pii pair<int, int>
#define mp map<pii, int>
using namespace std;

// Function declarations
void simulatedAnnealing(int jug1, int jug2, int target1);
void performInitializations(queue<pii> *openStates, pii *bestSoFar, int *T, mp *stateHeuristicVals, mp *delEVals, int target1);
int calcHeuristics(pii state, int target1);
int delE(pii curState, pii prevStateVal, mp *stateHeuristicVals);
pii generateNewStates(pii curState, queue<pii> *openStates, mp *stateHeuristicVals, mp *states, mp *delEVals, int j1, int j2, int target, bool *goalFlag);
bool evalAndAdd(pii newState, pii curState, queue<pii> *openStates, mp *stateHeuristicVals, mp *states, mp *delEVals, int target, bool *goalFlag);
bool checkGoalCondition(pii state, int target);

int main() {
    int jug1 = 4, jug2 = 3, target1 = 2;
    simulatedAnnealing(jug1, jug2, target1);
    return 0;
}

void simulatedAnnealing(int jug1, int jug2, int target1) {
    mp states;
    vector<pii> path;
    queue<pii> openStates;
    mp stateHeuristicVals;
    mp delEVals;
    pii bestSoFar;
    bool goalFlag = false;
    bool firstTime = true;
    pii prevState;
    int T;
    pii tempState;

    performInitializations(&openStates, &bestSoFar, &T, &stateHeuristicVals, &delEVals,target1);

    while ( !openStates.empty() ) {
        pii curState;
        if (firstTime) {
            curState = openStates.front();
            firstTime = false;
        } else {
            if (delEVals[tempState] > delEVals[prevState]) {
                bestSoFar = tempState;
                curState = tempState;
                openStates.pop();
                T -= 100;  // reduce annealing schedule by 100 each time.
            } else {
                float prob = (rand() % 10) / 10;
                float condProb = exp( (-delEVals[curState])/T );

                if (!(prob < condProb)) {
                    continue;
                }
                bestSoFar = tempState;
                curState = tempState;
                openStates.pop();
                T -= 100;  // reduce annealing schedule by 100 each time.
            }
        }


        if (!states[curState]) {
            path.push_back(curState);
            states[curState] = 1;
        }

        tempState = generateNewStates(curState, &openStates, &stateHeuristicVals, &states, &delEVals, jug1, jug2, target1, &goalFlag);
        prevState = curState;

        if (goalFlag || T == 0) {
            bestSoFar = tempState;
            path.push_back(tempState);
            break;
        }
    }

    cout<<"Best so far: (" << bestSoFar.first << ", " << bestSoFar.second << ")" << endl;

    for (auto i = path.begin(); i != path.end(); i++) {
        cout<<"(" << (*i).first << ", " << (*i).second << ")" << endl;
    }
}

pii generateNewStates(pii curState, queue<pii> *openStates, mp *stateHeuristicVals, mp *states, mp *delEVals, int j1, int j2, int target, bool *goalFlag) {
    pii tempState = { j1, curState.second };
    if (evalAndAdd(tempState, curState, openStates, stateHeuristicVals, states, delEVals, target, goalFlag)) {
        return tempState;
    }

    tempState = make_pair( curState.first, j2 );
    if (evalAndAdd(tempState, curState, openStates, stateHeuristicVals, states, delEVals, target, goalFlag)) {
        return tempState;
    }

    tempState = make_pair( 0, curState.second );
    if (evalAndAdd(tempState, curState, openStates, stateHeuristicVals, states, delEVals, target, goalFlag)) {
        return tempState;
    }

    tempState = make_pair( curState.first, 0 );
    if (evalAndAdd(tempState, curState, openStates, stateHeuristicVals, states, delEVals, target, goalFlag)) {
        return tempState;
    }

    if ((curState.second < j2) && (curState.first != 0)) {
        int diff = j2 - curState.second;
        if (diff >= curState.first) {
            tempState = make_pair(0, curState.second + curState.first);
        } else {
            tempState = make_pair(curState.first - diff, curState.second + diff);
        }

        if (evalAndAdd(tempState, curState, openStates, stateHeuristicVals, states, delEVals, target, goalFlag)) {
            return tempState;
        }
    }

    if ((curState.first < j1) && (curState.second != 0)) {
        int diff = j1 - curState.first;
        if (diff >= curState.second) {
            tempState = make_pair(curState.second + curState.first, 0);
        } else {
            tempState = make_pair(curState.first + diff, curState.second - diff);
        }

        if (evalAndAdd(tempState, curState, openStates, stateHeuristicVals, states, delEVals, target, goalFlag)) {
            return tempState;
        }
    }
}

void performInitializations(queue<pii> *openStates, pii *bestSoFar, int *T, mp *stateHeuristicVals, mp *delEVals, int target1) {
    pii initState = {0, 0};
    (*openStates).push(initState);    // Inserting start state into the open states list
    (*T) = 1000;    // Setting annealing schedule
    (*stateHeuristicVals)[initState] = calcHeuristics(initState, target1);
    (*delEVals)[initState] = 0;
    (*bestSoFar) = initState;
}

int calcHeuristics(pii state, int target1) {
    return abs(state.first - target1) + abs(state.second - target1);
}

int delE(pii newState, pii curState, mp *stateHeuristicVals) {
    return (*stateHeuristicVals)[curState] - (*stateHeuristicVals)[newState];
}

bool evalAndAdd(pii newState, pii curState, queue<pii> *openStates, mp *stateHeuristicVals, mp *states, mp *delEVals, int target, bool *goalFlag) {
    if (!(*states)[newState]) {
        if (checkGoalCondition(newState, target)) {
            (*goalFlag) = true;
            return true;
        }
        (*stateHeuristicVals)[newState] = calcHeuristics(newState, target);
        (*delEVals)[newState] = delE(newState, curState, stateHeuristicVals);
        (*openStates).push(newState);

        return true;
    }

    return false;
}

bool checkGoalCondition(pii state, int target) {
    return ((state.first == target) && (state.second == 0));
}
