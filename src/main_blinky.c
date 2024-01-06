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
            uint8_t buf[2];
            uint8_t reg = 0x09 * 2;
            uint16_t data = 0;

            i2c_write_blocking(&i2c1_inst, 0x22, &reg, 1, false);
            i2c_read_blocking(&i2c1_inst, 0x22, buf, 2, false);
            data = buf[0] << 8 | buf[1];
            float float_data = data;
            float_data = float_data * (1.0023F + float_data * (
                                           8.1488e-5F + float_data * (-9.3924e-9F + float_data * 6.0135e-13F)));


            printf("%f, ", float_data);

            reg = 0x0A * 2;
            i2c_write_blocking(&i2c1_inst, 0x22, &reg, 1, false);
            i2c_read_blocking(&i2c1_inst, 0x22, buf, 2, false);
            data = buf[0] << 8 | buf[1];
            float_data = (float) (-45) + ((data * 175.00) / 1024.00 / 64.00);
            printf("%f, ", float_data);

            reg = 0x0B * 2;
            i2c_write_blocking(&i2c1_inst, 0x22, &reg, 1, false);
            i2c_read_blocking(&i2c1_inst, 0x22, buf, 2, false);
            data = buf[0] << 8 | buf[1];
            float_data = (float) data * 100 / 65536;
            printf("%f\n", float_data);

            cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN,
                                !cyw43_arch_gpio_get(CYW43_WL_GPIO_LED_PIN));
            ulReceivedValue = 0U;
        }
    }
}
