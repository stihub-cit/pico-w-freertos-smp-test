#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"

#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"
#include <stdio.h>

#define mainQUEUE_RECEIVE_TASK_PRIORITY (tskIDLE_PRIORITY + 2)
#define mainQUEUE_SEND_TASK_PRIORITY (tskIDLE_PRIORITY + 1)

#define mainQUEUE_SEND_FREQUENCY_MS (1000 / portTICK_PERIOD_MS)

#define mainQUEUE_LENGTH (1)

#define mainTASK_LED (PICO_DEFAULT_LED_PIN)

void main_blinky(void);

static void prvQueueReceiveTask(void *pvParameters);

static void prvQueueSendTask(void *pvParameters);

static QueueHandle_t xQueue = NULL;

void main_blinky(void) {
  printf(" Starting main_blinky.\n");

  xQueue = xQueueCreate(mainQUEUE_LENGTH, sizeof(uint32_t));

  if (xQueue != NULL) {

    xTaskCreate(prvQueueReceiveTask, "Rx", configMINIMAL_STACK_SIZE, NULL,
                mainQUEUE_RECEIVE_TASK_PRIORITY, NULL);

    xTaskCreate(prvQueueSendTask, "TX", configMINIMAL_STACK_SIZE, NULL,
                mainQUEUE_SEND_TASK_PRIORITY, NULL);

    vTaskStartScheduler();
  }

  for (;;)
    ;
}

static void prvQueueSendTask(void *pvParameters) {
  TickType_t xNextWakeTime;
  const unsigned long ulValueToSend = 100UL;

  (void)pvParameters;

  xNextWakeTime = xTaskGetTickCount();

  for (;;) {

    vTaskDelayUntil(&xNextWakeTime, mainQUEUE_SEND_FREQUENCY_MS);

    xQueueSend(xQueue, &ulValueToSend, 0U);
  }
}

static void prvQueueReceiveTask(void *pvParameters) {
  unsigned long ulReceivedValue;

  (void)pvParameters;

  for (;;) {
    const unsigned long ulExpectedValue = 100UL;

    xQueueReceive(xQueue, &ulReceivedValue, portMAX_DELAY);

    if (ulReceivedValue == ulExpectedValue) {
      cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN,
                          !cyw43_arch_gpio_get(CYW43_WL_GPIO_LED_PIN));
      ulReceivedValue = 0U;
    }
  }
}
