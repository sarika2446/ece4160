# Lab 4: IMU

## Set up the IMU

The IMU was wired to the Artemis with the QWIIC connector cable, according to the diagram below.

<img width="527" alt="Screen Shot 2023-02-20 at 5 46 57 AM" src="https://user-images.githubusercontent.com/123786420/220084082-89c02ecd-e573-4c02-9052-cbf26cb21901.png">

I then ran the example program Example1_Basics, which came with the ICM 20948 library, on the Artemis successfully.

<img width="600" alt="Screen Shot 2023-02-22 at 1 59 31 AM" src="https://user-images.githubusercontent.com/123786420/220546379-f7473907-9442-4b20-9d9c-f331e98eb588.png">

The AD0 pin's value is defined with the variable AD0_VAL, which represents the value of the last bit of its I2C address. I stuck with the default value, 1, which indicates that we are reading data from the IMU. When the ADR jumper is closed, this value becomes 0, indicating that we are writing data to the IMU.

The accelerometer value changed when the IMU was rotated, flipped, or accelerated, with the changing component being dependent on the axis of the movement. The gyroscope value changed when the IMU was rotated or flipped, with the changing component being dependent on the axis of the movement.

I also added code that would make the Artemis blink its built-in LED three times on startup. 

<script src="https://gist.github.com/sarika2446/4c0a2638bd72bb3f08b7cf59a75a8567.js"></script>



## Accelerometer

The accelerometer's pitch and roll were calculated using the equations below, taken from the lecture slides, where θ is roll and ϕ is pitch. I converted these values to degrees in my code.

<img width="237" alt="Screen Shot 2023-02-22 at 2 03 41 AM" src="https://user-images.githubusercontent.com/123786420/220547342-bb5f7988-d98e-4e67-8aee-ef1b05add992.png">

<script src="https://gist.github.com/sarika2446/972fa6949348000c2f274847d52a26d9.js"></script>

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

I then implemented complementary filters, with α = 0.5, for both the pitch and the roll, combining the gyroscope and accelerometer measurements as shown below. I chose an α of 0.5 after testing different values to see the accuracy they provided.

<script src="https://gist.github.com/sarika2446/a27ece4b262f1ad0f054340b966c5a77.js"></script>

The accuracy and working range are demonstrated in the video below. The accuracy seems to be higher than the accelerometer measurements alone and much higher than the gyroscope measurements alone.

[![Filter](https://img.youtube.com/vi/hazu8Rz-Gck/0.jpg)](https://www.youtube.com/watch?v=hazu8Rz-Gck "Filter")

They were also not very susceptible to drift and quick vibrations, shown by the first and second image below, respectively.

<img width="600" alt="nodrift" src="https://user-images.githubusercontent.com/123786420/220559073-dfd87ab1-b6f1-4e7e-85d2-0784be7b88b0.png">

<img width="600" alt="Screen Shot 2023-02-21 at 6 38 10 PM" src="https://user-images.githubusercontent.com/123786420/220559250-01160945-3157-499e-b6ed-6f501dc5cdd4.png">



## Sample Data

After removing extra delays and Serial.print statements, and checking if the data is ready rather than waiting for it to be ready, I was able to sample new data every 133 ms.

I was also able to collect and store timestamped IMU data in arrays of size 500 using the code below.

<script src="https://gist.github.com/sarika2446/dfec36c15124502d10d7f4943ecc9701.js"></script>

<img width="345" alt="Screen Shot 2023-02-21 at 7 30 10 PM" src="https://user-images.githubusercontent.com/123786420/220560496-bfbafc72-a193-45e5-9608-d18c0722f02b.png">

By integrating my IMU code into the existing code to transmit 5 seconds of TOF data via Bluetooth from the Artemis to the Python end, and creating a new command called GET_TOF_IMU, I was able to transmit 5 seconds of both TOF and IMU data.

<script src="https://gist.github.com/sarika2446/8bfcf60db0d8f817fe32505702322fd0.js"></script>

On the Python end, I received the data using a notification handler.


<img width="600" alt="Screen Shot 2023-02-22 at 3 20 05 AM" src="https://user-images.githubusercontent.com/123786420/220562798-af663f25-5d56-4f43-ab60-161f7913f6bb.png">

<img width="620" alt="Screen Shot 2023-02-22 at 3 18 23 AM" src="https://user-images.githubusercontent.com/123786420/220562839-650efa32-b05b-4b4f-9a8a-5fc49885d06a.png">

<img width="598" alt="Screen Shot 2023-02-22 at 3 19 50 AM" src="https://user-images.githubusercontent.com/123786420/220562860-f9ed61bd-a846-4667-b3ce-f2becfb87737.png">

## Cut the Coord

The digital electronics were powered by a 3.7V 650mAh battery, while the motors were powered by a 3.7V 850mAh battery. The battery with more capacity is used to drive the motors so the motors can run faster.

<img width="427" alt="Screen Shot 2023-02-22 at 3 28 26 AM" src="https://user-images.githubusercontent.com/123786420/220564505-b5f32c6f-c518-4f0b-9a17-dd29917c2661.png">

## Record a Stunt!

When playing around with the car, before mounting the Artemis onto it, I noticed that the car moved at a high speed, accelerated quickly, and made fairly sharp turns.

[![Car](https://img.youtube.com/vi/keNKlf3qw3o/0.jpg)](https://www.youtube.com/watch?v=keNKlf3qw3o "Car")





