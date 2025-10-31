#include <can2040.h>
#include <hardware/regs/intctrl.h>
#include <stdio.h>
#include <pico/stdlib.h>
#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>

// Transmit and Recieve Code

static struct can2040 cbus;
QueueHandle_t msg_queue;

// ID 0 is data transmitter
// ID 1 is noise transmitter
uint8_t PICO_ID = 0;

static void can2040_cb(struct can2040 *cd, uint32_t notify, struct can2040_msg *msg)
{
    printf("HERE, notify: %d, CAN2040_NOTIFY_TX: %d\n", notify, CAN2040_NOTIFY_TX);
    if (notify == CAN2040_NOTIFY_RX) {
        xQueueSendToBackFromISR(msg_queue, msg, NULL);
    }
    else if (notify == CAN2040_NOTIFY_TX) {
        printf("sent a message\n");
    }
}

static void PIOx_IRQHandler(void)
{
    can2040_pio_irq_handler(&cbus);
}

void transmit_noise_task(__unused void *args)
{
    struct can2040_msg msg;
    msg.data32[0] = 1;
    msg.dlc = 8;
    msg.id = 1;
    while(1)
    {
        msg.data32[1] = msg.data32[1] * 67;
        if (msg.data32[1] == 0) msg.data32[1] = 1;  
        int Tstatus = can2040_transmit(&cbus, &msg);
        if(Tstatus < 0)
        {
<<<<<<< HEAD
            //printf("Failed to add message to buffer from noise, with data %d\n", msg.data32[1]);
=======
            // printf("Failed to add message to buffer from noise, with data %d", msg.data32[1]);
>>>>>>> e0eccd9753cc5dcac8a9eaa50baeffb04fa20a00
        }
    }
}

void transmit_data_task(__unused void *args)
{
    struct can2040_msg msg;
    msg.data32[0] = 1;
    msg.dlc = 8;
    msg.id = 1;
    while(1)
    {
        msg.data32[1] = 12345;
        int Tstatus = can2040_transmit(&cbus, &msg);
        if(Tstatus < 0)
        {
<<<<<<< HEAD
            printf("Failed to add message to buffer from noise, with data %d\n", msg.data32[1]);
=======
            // printf("Failed to add message to buffer from noise, with data %d", msg.data32[1]);
>>>>>>> e0eccd9753cc5dcac8a9eaa50baeffb04fa20a00
        }
        vTaskDelay(pdMS_TO_TICKS(500));
    }
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

void receive_task(__unused void *args)
{
    while(1) {
        struct can2040_msg data;
        xQueueReceive(msg_queue, &data, portMAX_DELAY);
        printf("Got message: %d\n", data.data32[1]);
        if(data.data32[1] == 12345)printf("Got the real data***************************************\n");
    }
}

int main(void)
{
    stdio_init_all();
    msg_queue = xQueueCreate(100, sizeof(struct can2040_msg));
    canbus_setup();
    sleep_ms(5000);
    TaskHandle_t trans_noise_task_h;
    TaskHandle_t trans_data_task_h;
    TaskHandle_t receive_task_h;
    if(PICO_ID == 1)
        xTaskCreate(transmit_noise_task, "NoiseThread",
                configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 5UL, &trans_noise_task_h);
    if(PICO_ID == 0)
        xTaskCreate(transmit_data_task, "DataThread",
                configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 0UL, &trans_data_task_h);
    xTaskCreate(receive_task, "ReceiveThread",
                configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 0UL, &receive_task_h);
    vTaskStartScheduler();
    return 0;
}