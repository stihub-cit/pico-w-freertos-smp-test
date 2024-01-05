#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"

#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/i2c.h"
#include <stdio.h>

#define mainQUEUE_RECEIVE_TASK_PRIORITY (tskIDLE_PRIORITY + 2)
#define mainQUEUE_SEND_TASK_PRIORITY (tskIDLE_PRIORITY + 1)

#define mainQUEUE_SEND_FREQUENCY_MS (1000 / portTICK_PERIOD_MS)

#define mainQUEUE_LENGTH (1)

bool reserved_addr(uint8_t addr) {
  return (addr & 0x78) == 0 || (addr & 0x78) == 0x78;
}

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

  for (;;);
}

static void prvQueueSendTask(void *pvParameters) {
  TickType_t xNextWakeTime;
  const unsigned long ulValueToSend = 100UL;

  (void) pvParameters;

  xNextWakeTime = xTaskGetTickCount();

  for (;;) {
    vTaskDelayUntil(&xNextWakeTime, mainQUEUE_SEND_FREQUENCY_MS);

    xQueueSend(xQueue, &ulValueToSend, 0U);
  }
}

static void prvQueueReceiveTask(void *pvParameters) {
  unsigned long ulReceivedValue;

  (void) pvParameters;

  for (;;) {
    const unsigned long ulExpectedValue = 100UL;

    xQueueReceive(xQueue, &ulReceivedValue, portMAX_DELAY);

    if (ulReceivedValue == ulExpectedValue) {
      printf("\nI2C Bus Scan\n");
      printf("   0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F\n");

      for (int addr = 0; addr < (1 << 7); ++addr) {
        if (addr % 16 == 0) {
          printf("%02x ", addr);
        }

        // Perform a 1-byte dummy read from the probe address. If a slave
        // acknowledges this address, the function returns the number of bytes
        // transferred. If the address byte is ignored, the function returns
        // -1.

        // Skip over any reserved addresses.
        int ret = 0;
        uint8_t rxdata = 0;
        if (reserved_addr(addr)) {
          ret = PICO_ERROR_GENERIC;
        } else {
          ret = i2c_read_blocking(((&i2c1_inst)), addr, &rxdata, 1, false);
        }

        printf(ret < 0 ? "." : "@");
        printf(addr % 16 == 15 ? "\n" : "  ");
      }
      printf("Done.\n");
      cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN,
                          !cyw43_arch_gpio_get(CYW43_WL_GPIO_LED_PIN));
      ulReceivedValue = 0U;
    }
  }
}
