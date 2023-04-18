/* 
 * File:   tasks.h
 * Author: mschmidl
 *
 * Created on 22. September 2015, 11:33
 */

#ifndef TASKS_H
#define	TASKS_H

#ifdef	__cplusplus
extern "C" {
#endif
    
    #include "cmdline.h"
    #define TASK1      cmdlineWorker
    #define TASK1_INIT cmdlineInit

    #include "ledctrl.h"
    #define TASK2      ledctrlWorker
    #define TASK2_INIT ledctrlInit

    #include "neopixel.h"
    #define TASK3      neopixelWorker
    #define TASK3_INIT neopixelInit

    #undef TASK4      
    #undef TASK4_INIT 

    #undef TASK5      
    #undef TASK5_INIT 

    #undef TASK6      
    #undef TASK6_INIT 

    #undef TASK7      
    #undef TASK7_INIT 

    #undef TASK8      
    #undef TASK8_INIT 

    // if more are needed, change TASKS.C as well

    void tasksInit(void);
    void tasksSchedule(void);

#ifdef	__cplusplus
}
#endif

#endif	/* TASKS_H */

