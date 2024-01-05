## Building

The examples build as regular Raspberry Pi Pico SDK applications. You can build either from this root directory, or build from within the individual examples' directories.

See [Getting Started With Pico](https://datasheets.raspberrypi.org/pico/getting-started-with-pico.pdf) in the SDK documentation for setting up a Raspberry Pi Pico SDK build environment. If you are already set up, then just make sure `PICO_SDK_PATH` is set in your environment or pass it via `-DPICO_SDK_PATH=xxx` on the Cmake command line.

Set `FREERTOS_KERNEL_PATH` in your environment or on the CMake command line to point at a specific instance of the
FreeRTOS Kernel.

To build from the command line on unix, from this directory (or from the individual examples' directories), type:

```shell
mkdir build
cd build
cmake ..
make
```

This will generate `.uf2` files for each example under the relevant build directories.

## Flashing

1. Connect USB while **BOOTSEL** button is pressed on Pico. `RPI-RP2` should show up as USB Mass Storage device.
2. Copy the generated `.uf2` file to `RPI-RP2`. Once the file transfer is complete Pico would restart automatically and
   start running the program.
3. Additionally, use `screen` (or any other terminal) to monitor serial output over USB. `dmesg | grep tty` to find TTY
   device.

### Serial Monitor example

```bash
$ dmesg | grep tty
[    0.108413] printk: console [tty0] enabled
[    0.653298] 00:06: ttyS0 at I/O 0x3f8 (irq = 4, base_baud = 115200) is a 16550A
[    0.655126] 0000:00:16.3: ttyS4 at I/O 0xf0e0 (irq = 19, base_baud = 115200) is a 16550A
[ 5718.448836] cdc_acm 1-11:1.0: ttyACM0: USB ACM device

$ screen /dev/ttyACM0
```

