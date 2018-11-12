#include "statemachine.hpp"
#include "stm32f4xx_hal.h"

State::State(span<Task*> tasks)
    : tasks(tasks)
{}

void State::enter()
{
    for (auto t : tasks) {
        t->start();
    }
}

void State::leave()
{
    for (auto t : tasks) {
        t->stop();
    }
}

StateMachine::StateMachine(State *state)
    : state(state)
{
    taskENTER_CRITICAL();
    state->enter();
    taskEXIT_CRITICAL();
}

void StateMachine::transition(State *s)
{
    taskENTER_CRITICAL();
    state->leave();
    state = s;
    state->enter();
    taskEXIT_CRITICAL();
}
