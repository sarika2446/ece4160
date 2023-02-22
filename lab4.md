#Lab 4: IMU

## Set up the IMU

The IMU was wired to the Artemis with the QWIIC connector cable, according to the diagram below.

![IMG_1A08D919E607-1](https://user-images.githubusercontent.com/123786420/220541115-94a46004-95a7-4f70-8628-2af8e0baa2a1.jpeg)

I then ran the example program Example1_Basics, which came with the ICM 20948 library, on the Artemis successfully.

<img width="600" alt="Screen Shot 2023-02-21 at 2 29 06 PM" src="https://user-images.githubusercontent.com/123786420/220541605-12eeadbc-944f-43bc-9251-bc7dd8a4c6ac.png">

The AD0 pin's value is defined with the variable AD0_VAL, which represents the value of the last bit of its I2C address. I stuck with the default value, 1, which indicates that we are reading data from the IMU. When the ADR jumper is closed, this value becomes 0, indicatin that we are writing data to the IMU.

The accelerometer value changed when the IMU was rotated, flipped, or accelerated, with the changing component being dependent on the axis of the movement. The gyroscope value changed when the IMU was rotated or flipped, with the changing component being dependent on the axis of the movement.

I also added code that would make the Artemis blink its built-in LED three times on startup. 

<script src="https://gist.github.com/sarika2446/4c0a2638bd72bb3f08b7cf59a75a8567.js"></script>
