
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"

#include "main.h"

#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/i2c.h"
#include <stdio.h>
#if (mainRUN_ON_CORE == 1)
#include "pico/multicore.h"
#endif

#define I2C_SDA_PIN 26
#define I2C_SCL_PIN 27

static void prvSetupHardware(void);

extern void main_blinky(void);


void vApplicationMallocFailedHook(void);

void vApplicationIdleHook(void);

void vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName);

void vApplicationTickHook(void);

void vLaunch(void) { main_blinky(); }

int main(void) {
  prvSetupHardware();
  const char *rtos_name;
#if (portSUPPORT_SMP == 1)
  rtos_name = "FreeRTOS SMP";
#else
  rtos_name = "FreeRTOS";
#endif

#if (portSUPPORT_SMP == 1) && (configNUMBER_OF_CORES == 2)
  printf("%s on both cores:\n", rtos_name);
  vLaunch();
#endif

#if (mainRUN_ON_CORE == 1)
  printf("%s on core 1:\n", rtos_name);
  multicore_launch_core1(vLaunch);
  while (true)
    ;
#else
  printf("%s on core 0:\n", rtos_name);
  vLaunch();
#endif

  return 0;
}

static void prvSetupHardware(void) {
  stdio_init_all();

  cyw43_arch_init_with_country(CYW43_COUNTRY_INDIA);

  // This example will use I2C0 on the default SDA and SCL pins (GP4, GP5 on a Pico)
  i2c_init(((&i2c1_inst)), 100 * 1000);
  gpio_set_function(I2C_SDA_PIN, GPIO_FUNC_I2C);
  gpio_set_function(I2C_SCL_PIN, GPIO_FUNC_I2C);
  gpio_pull_up(I2C_SDA_PIN);
  gpio_pull_up(I2C_SCL_PIN);
  // Make the I2C pins available to picotool
  bi_decl(bi_2pins_with_func(I2C_SDA_PIN, I2C_SCL_PIN, GPIO_FUNC_I2C));
}

void vApplicationMallocFailedHook(void) { configASSERT((volatile void *)NULL); }

void vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName) {
  (void) pcTaskName;
  (void) pxTask;

  configASSERT((volatile void *)NULL);
}

void vApplicationIdleHook(void) {
  volatile size_t xFreeHeapSpace = xPortGetFreeHeapSize();

  (void) xFreeHeapSpace;
}
