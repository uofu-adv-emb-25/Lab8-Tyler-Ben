# Lab 8: CAN Bus

## Pins of the CAN
* Pin 1: Txd
* Pin 2: Ground
* Pin 3: Power
* Pin 4: Rxd
* Pin 5: Vio
* Pin 6: CANL
* Pin 7: CANH
* Pin 8: STBY


## Wiring

| CAN Pin | CAN Signal  | Wiring Connection Microcontroller/CAN |
|---------|-------------|---------------------------------------|
| Pin 1 | Txd | GPIO5 (PIN 7) |
| Pin 2 | Ground | Ground |
| Pin 3 | Power | 5V |
| Pin 4 | Rxd | GPIO4 (PIN 6) |
| Pin 5 | Vio | 3V3 (works at 5V too) |
| Pin 6 | CANL | CANL |
| Pin 7 | CANH | CANH |
| Pin 8 | STBY | Ground |


Also tie the ends of the bus (CANH and CANL) with 120-ohm resistors so that the bus impedence is 60-ohms


## Activity 1 Discussion
Observe the electrical behavior of the bus line and the microcontroller lines:

Bus Line:

We wired up the oscillioscope to the CANH and CANL bus lines and saw that when a message was sent, a pulse occurs with the CANH and CANL lines increasing/decreasing (respectively) at the same voltage level. We observed that the resting voltage of the bus line was +2.5V compared to micrcontroller ground. And then the 

(insert image here)

Microcontroller Line:

We wired up the oscillioscope to the Tx and Rx pins on the pico acting as the receiver. We observed a approx. 220us oscilating voltage from 0V to 5V (presumably PWM) on the Rx line. This output voltage happened every time a message was received over the CAN bus (which we were sending every 500ms). The Tx on the receiver board is static around 3.3V (which is to be expected).

(insert image here)
(insert image here)

We wired up the oscillioscope to the Tx and Rx pins on the pico acting as the transmitter. We observed the same functionality, except with the Tx and Rx functions flipped, as expected (with the Rx having no behavior and the Tx line sending out encoded information every 500ms).

(insert image here)


## Activity 2 Discussion
We combined the transmit and receive code into one single file (we called this main.c).
We configured the picos such that one acted as a "babbling node" (broadcasting messages with a high priority continuously), and the other sent data periodically (with a delay of 500ms or 2Hz).
We hooked up the CAN bus to the oscilloscope and observed that the babbling node was constantly starving the system.
This is to be expected because-due to the physical makeup of the bus itself-the CAN bus can only transmit one signal at a time.


Next, we introduced a short busy wait delay into the babbling node. We found that with a 1 MS delay, we were able to consistently receive both the periodic signal (at 2Hz) as well as the noise signal (See images below of the output being printed to the serial monitor).
We observed that the 'lower priority' message was able to be transmitted and received when the bus was idle.
So after adding in even a short delay, we were able to observe the correct (ideal) behavior where the important data is not lost (starved).

(insert image(s) here)
