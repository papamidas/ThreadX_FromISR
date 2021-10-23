# ThreadX_FromISR
Trying out AZURE RTOS ThreadX 1.1.0 on a NUCLEO-H723-Board: Waking up a thread from an ISR

Before Compiling with STM32CubeIDE 1.7.0 either

#define USE_EVENTFLAGS_FROM_ISR

or

#define USE_SEMAPHORE_FROM_ISR


in order to compare execution times for both notification methods.