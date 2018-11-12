#pragma once

#include "taskmgr.hpp"
#include "span.hpp"

class State {
public:
    State(span<Task*> tasks);

    void enter();
    void leave();
private:
    span<Task*> tasks;
};

class StateMachine {
public:
    StateMachine(State *state);

    void transition(State*);
private:
    State *state;
};
