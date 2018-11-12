#pragma once

#include <functional>
#include <array>
#include <limits>

#include <FreeRTOS.h>
#include <task.h>

class Task {
public:
    virtual void start() = 0;
    virtual void stop() = 0;
};

template <unsigned stack_size>
class StaticTask : public Task {
public:
    StaticTask(const char *name, int32_t priority, std::function<void()> code)
        : name(name), priority(priority), code(code)
        {
            const UBaseType_t p = 
                (std::numeric_limits<UBaseType_t>::max() >> 1) + priority;
            static_assert(stack_size >= configMINIMAL_STACK_SIZE);
            handle = xTaskCreateStatic(&callcode, name, stack_size,
                                       this, p, stack.data(), &task);
        }

    void start() override {
        vTaskResume(handle);
    }

    void stop() override {
        vTaskSuspend(handle);
    }
    
private:
    static void callcode(void *t) {
        const StaticTask<stack_size> *task
            = static_cast<StaticTask<stack_size>*>(t);
        while (1) {
            task->code();
        }
    };

    const char *name;
    int32_t priority;
    std::function<void()> code;
    std::array<StackType_t, stack_size> stack;
    StaticTask_t task;
    TaskHandle_t handle;
};

void delay(uint32_t ms);
