#include "tasks.h"

void tasksInit(void)
{
   #ifdef TASK1_INIT
      TASK1_INIT();
   #endif
   #ifdef TASK2_INIT
      TASK2_INIT();
   #endif
   #ifdef TASK3_INIT
      TASK3_INIT();
   #endif
   #ifdef TASK4_INIT
      TASK4_INIT();
   #endif
   #ifdef TASK5_INIT
      TASK5_INIT();
   #endif
   #ifdef TASK6_INIT
      TASK6_INIT();
   #endif
   #ifdef TASK7_INIT
      TASK7_INIT();
   #endif
   #ifdef TASK8_INIT
      TASK8_INIT();
   #endif
}


void tasksSchedule(void)
{
   #ifdef TASK1
      TASK1();
   #endif
   #ifdef TASK2
      TASK2();
   #endif
   #ifdef TASK3
      TASK3();
   #endif
   #ifdef TASK4
      TASK4();
   #endif
   #ifdef TASK5
      TASK5();
   #endif
   #ifdef TASK6
      TASK6();
   #endif
   #ifdef TASK7
      TASK7();
   #endif
   #ifdef TASK8
      TASK8();
   #endif
}

