Independent WatchDog Timer as a FreeRTOS task
Design Ideas: 
1) Have one highest priority task that refreshes the WD after doing a health check of all the remaining lower priority tasks.
This task will consequently be blocked for a while just a bit shorter than the max timeout value of the WD timer, which will set via the prescaler and the MCU clock frequency, depending on how long each of our tasks will take. 
The WD task must be blocked with vTaskDelayUntil(), rather than vTaskDelay(), because we want to return to this task precisely x seconds later, independent of whatever unexpected delay might occur inside of other lower priority tasks.
2) Have a struct to pass to the tasks other than the WD task; for the flag bit and whatever other parameters they need. However, creating AND initializing a struct for each task is really long to do in main, have a setup() function for this.
see https://betterembsw.blogspot.com/2014/05/proper-watchdog-timer-use.html
see https://www.embedded.com/design/debug-and-optimization/4402288/Watchdog-Timers
Challenges: How to record which task(s) caused the software problem so that we can "maybe" record this and take action about it before/after the reset??
To Ask: Why is the SDIO option highlighted in red in CubeMX?
To Do: Connection with ISRs, and also the regular tasks and the WD task should mask and then unmask interrupts while doing bit setting and bit checking. Use a mutex to protect the flag_bit variable!! (Even though the tasks will not be 'meant to' write to the same bit locations, we can't ever know something might just go wrong)
