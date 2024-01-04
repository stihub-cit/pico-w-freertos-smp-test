#include "FreeRTOS.h"
#include "task.h"

#include "IntQueue.h"
#include "IntQueueTimer.h"

#include "hardware/irq.h"
#include "pico/time.h"

#define tmrMAX_PRIORITY (0UL)
#define trmSECOND_HIGHEST_PRIORITY (tmrMAX_PRIORITY + 0x40)

#define FIRST_TIMER_PERIOD_US 500
#define SECOND_TIMER_PERIOD_US 487

void prvAlarm0Callback(uint timer) {
  UBaseType_t uxSavedInterruptState;
  BaseType_t xHigherPriorityTaskWoken;

  configASSERT(timer == 0);

  uxSavedInterruptState = taskENTER_CRITICAL_FROM_ISR();
  {
    xHigherPriorityTaskWoken = xFirstTimerHandler();
    hardware_alarm_set_target(0, make_timeout_time_us(FIRST_TIMER_PERIOD_US));
  }
  taskEXIT_CRITICAL_FROM_ISR(uxSavedInterruptState);

  portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
}

void prvAlarm1Callback(uint timer) {
  UBaseType_t uxSavedInterruptState;
  BaseType_t xHigherPriorityTaskWoken;

  configASSERT(timer == 1);

  uxSavedInterruptState = taskENTER_CRITICAL_FROM_ISR();
  {
    xHigherPriorityTaskWoken = xSecondTimerHandler();
    hardware_alarm_set_target(1, make_timeout_time_us(SECOND_TIMER_PERIOD_US));
  }
  taskEXIT_CRITICAL_FROM_ISR(uxSavedInterruptState);

  portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
}

void vInitialiseTimerForIntQueueTest(void) {

  hardware_alarm_claim(0);
  hardware_alarm_claim(1);

  taskDISABLE_INTERRUPTS();

  irq_set_priority(TIMER_IRQ_0, tmrMAX_PRIORITY);
  irq_set_priority(TIMER_IRQ_1, trmSECOND_HIGHEST_PRIORITY);
  hardware_alarm_set_callback(0, prvAlarm0Callback);
  hardware_alarm_set_callback(1, prvAlarm1Callback);
  hardware_alarm_set_target(0, make_timeout_time_us(FIRST_TIMER_PERIOD_US));
  hardware_alarm_set_target(1, make_timeout_time_us(SECOND_TIMER_PERIOD_US));
}
