#include <can2040.h>
#include <hardware/regs/intctrl.h>
#include <stdio.h>
#include <pico/stdlib.h>
#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>

// Transmitter Code

static struct can2040 cbus;
uint16_t flipflop = 0;

static void can2040_cb(struct can2040 *cd, uint32_t notify, struct can2040_msg *msg)
{
    printf("sent a message\n");
}

static void PIOx_IRQHandler(void)
{
    can2040_pio_irq_handler(&cbus);
}

void canbus_setup(void)
{
    uint32_t pio_num = 0;
    uint32_t sys_clock = 125000000, bitrate = 500000;
    uint32_t gpio_rx = 4, gpio_tx = 5;

    // Setup canbus
    can2040_setup(&cbus, pio_num);
    can2040_callback_config(&cbus, can2040_cb);

    // Enable irqs
    irq_set_exclusive_handler(PIO0_IRQ_0, PIOx_IRQHandler);
    irq_set_priority(PIO0_IRQ_0, PICO_DEFAULT_IRQ_PRIORITY - 1);
    irq_set_enabled(PIO0_IRQ_0, 1);

    // Start canbus
    can2040_start(&cbus, sys_clock, bitrate, gpio_rx, gpio_tx);
}

void transmit_task(__unused void *args)
{
    struct can2040_msg msg;
        msg.data32[0] = 0;
        msg.dlc = 8;
        msg.id = 1;
    while(1)
    {
        if(!flipflop) flipflop = (1<<15);
        else flipflop = 0;

        printf("%d\n", flipflop);
        msg.data32[0] = flipflop;
        msg.data32[1] = flipflop;
        can2040_transmit(&cbus, &msg);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

int main(void)
{
    stdio_init_all();
    canbus_setup();
    TaskHandle_t task;
    xTaskCreate(transmit_task, "MainThread",
                configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1UL, &task);
    vTaskStartScheduler();
    return 0;
}