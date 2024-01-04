#ifndef MAIN_H
#define MAIN_H

#define mainRUN_ON_CORE 0

#define mainENABLE_COUNTING_SEMAPHORE 1
#define mainENABLE_DEATH 1
#define mainENABLE_INTERRUPT_QUEUE 1
#define mainENABLE_MATH 1
#define mainENABLE_QUEUE_OVERWRITE 1
#define mainENABLE_REG_TEST 1
#define mainENABLE_SEMAPHORE 1
#define mainENABLE_TASK_NOTIFY 1

#if (configNUMBER_OF_CORES == 1) || (configRUN_MULTIPLE_PRIORITIES == 0)

#define mainENABLE_BLOCK_TIME 1
#define mainENABLE_BLOCKING_QUEUE 1
#define mainENABLE_GENERIC_QUEUE 1
#define mainENABLE_INTERRUPT_SEMAPHORE 1
#define mainENABLE_EVENT_GROUP 1
#define mainENABLE_RECURSIVE_MUTEX 1
#define mainENABLE_TIMER_DEMO 1
#endif

#if (configNUMBER_OF_CORES == 1)

#define mainENABLE_DYNAMIC_PRIORITY 1
#endif

#endif