# Lab 4: IMU

## Set up the IMU

The IMU was wired to the Artemis with the QWIIC connector cable, according to the diagram below.

<img width="527" alt="Screen Shot 2023-02-20 at 5 46 57 AM" src="https://user-images.githubusercontent.com/123786420/220084082-89c02ecd-e573-4c02-9052-cbf26cb21901.png">

I then ran the example program Example1_Basics, which came with the ICM 20948 library, on the Artemis successfully.

<img width="600" alt="Screen Shot 2023-02-22 at 1 59 31 AM" src="https://user-images.githubusercontent.com/123786420/220546379-f7473907-9442-4b20-9d9c-f331e98eb588.png">

The AD0 pin's value is defined with the variable AD0_VAL, which represents the value of the last bit of its I2C address. I stuck with the default value, 1, which indicates that we are reading data from the IMU. When the ADR jumper is closed, this value becomes 0, indicatin that we are writing data to the IMU.

The accelerometer value changed when the IMU was rotated, flipped, or accelerated, with the changing component being dependent on the axis of the movement. The gyroscope value changed when the IMU was rotated or flipped, with the changing component being dependent on the axis of the movement.

I also added code that would make the Artemis blink its built-in LED three times on startup. 

<script src="https://gist.github.com/sarika2446/4c0a2638bd72bb3f08b7cf59a75a8567.js"></script>

## Accelerometer

The accelerometer's pitch and roll were calculated using the equations below, taken from the lecture slides, where θ is roll and ϕ is pitch. I converted these values to degrees in my code.

<img width="237" alt="Screen Shot 2023-02-22 at 2 03 41 AM" src="https://user-images.githubusercontent.com/123786420/220547342-bb5f7988-d98e-4e67-8aee-ef1b05add992.png">

<script src="https://gist.github.com/sarika2446/972fa6949348000c2f274847d52a26d9.js"></script>

<img width="345" alt="Screen Shot 2023-02-21 at 7 30 10 PM" src="https://user-images.githubusercontent.com/123786420/220548512-c188cf15-0a9c-4106-918d-36d1d1625892.png">


I plotted the output at {-90,0,90} degrees using the Serial Plotter, where pink is pitch and gray is roll.

<img width="600" alt="Screen Shot 2023-02-21 at 1 56 58 PM" src="https://user-images.githubusercontent.com/123786420/220547584-f35f4d6c-0297-4dce-b9f3-363143d6ed35.png">

I found, using a protractor, that the output was off by about 5 degrees from the measured value, on average, which is pretty accurate.

I then tapped the IMU to generate noise, recorded the pitch and roll data, and performed an FFT on them using Python.

![Unknown-15](https://user-images.githubusercontent.com/123786420/220551371-8e603e1a-3005-499b-b816-4769fabcd6af.png)

![Unknown-13](https://user-images.githubusercontent.com/123786420/220550643-e0949a01-2d97-4f32-8deb-d68e73143063.png)

![Unknown-16](https://user-images.githubusercontent.com/123786420/220551609-ce3889ed-2bc4-4a52-a1c3-3a7faba03f6c.png)

![Unknown-14](https://user-images.githubusercontent.com/123786420/220550686-8e2187ac-b432-4544-9878-c7cb40e11a1c.png)

There do not seem to be any spikes in the FFT outputs due to the noise from tapping on the IMU. This is because the IMU already has an internal low-pass filter that filters out noise. There does not seem to be a need to implement an addition low-pass filter.

## Gyroscope

Using the equations given in class, I created functions to calculate the pitch, roll, and yaw, as seen below.

<script src="https://gist.github.com/sarika2446/3ae8f9ff6f38e95b9238d945944777c7.js"></script>

The following images show the Serial Plotter output for the pitch (red), roll (pink), and yaw (blue) for zero degrees, 90 degrees, and -90 degrees, with respect to the x-axis, respectively.

<img width="600" alt="0" src="https://user-images.githubusercontent.com/123786420/220554465-e2fafe59-f644-4677-8734-b0767a822cbd.png">

<img width="600" alt="90plus" src="https://user-images.githubusercontent.com/123786420/220554513-0d1dede1-1dc9-4d0b-b749-80127ebeb906.png">

<img width="600" alt="90minus" src="https://user-images.githubusercontent.com/123786420/220554529-371fb11d-c954-4592-8804-6e0520077907.png">

When looking at these plots, it is clear that these measurements are susceptible to drift. I also tried decreasing the sampling rate by increasing the delay in the loop, which decreased the accuracy of the estimated angles.

<img width="600" alt="lowersamplingrate" src="https://user-images.githubusercontent.com/123786420/220555273-f4c35ebb-a990-4bcd-bd0d-03fbfbae3005.png">

I then implemented complementary filters, with α = 0.5, for both the pitch and the roll, combining the gyroscope and accelerometer measurements as shown below.

<script src="https://gist.github.com/sarika2446/a27ece4b262f1ad0f054340b966c5a77.js"></script>

The accuracy and working range are demonstrated in the video below. The accuracy seems to be higher than the accelerometer measurements alone and much higher than the gyroscope measurements alone.







