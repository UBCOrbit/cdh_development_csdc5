One higher priority watchdog task reads the other tasks' bits, and updates if they finished correctly.
Problem: 
- use vTaskDelayUntil to make the watchdog task wait (I used this successfully before, but for this project I couldn't manage to make it work)
