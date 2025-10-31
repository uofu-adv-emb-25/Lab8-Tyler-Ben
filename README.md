# Lab 8: CAN Bus


# Pins of the CAN
Pin 1: Txd
Pin 2: Ground
Pin 3: Power
Pin 4: Rxd
Pin 5: Vio
Pin 6: CANL
Pin 7: CANH
Pin 8: STBY

# Wiring:
    CAN       >  MICROCONTROLLER
Pin 1: Txd    >  GPIO5 (PIN 7)
Pin 2: Ground >  ground
Pin 3: Power  >  5V
Pin 4: Rxd    >  GPIO4 (PIN 6)
Pin 5: Vio    >  3V3
Pin 6: CANL   >  CANL
Pin 7: CANH   >  CANH
Pin 8: STBY   >  ground

# Activity 1 Discussion
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


