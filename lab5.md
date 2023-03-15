# Lab 5: Motors and Open Loop Control

## Prelab

I connected the motor drivers, Artemis, and battery as detailed in the diagram below.

![Picture4](https://user-images.githubusercontent.com/123786420/225228567-e37be39f-6001-4ccf-b747-a9e77213d7b9.jpg)

The motors and the Artemis are powered by separate batteries, with the motors' battery having a higher capacity (850mAh) than the Artemis' battery (650mAh), because the motors require more power than the Artemis does.

## Lab Tasks

I connected the power supply and oscilloscope to one motor driver, as seen below. I chose to set the power supply to 3.7V because that is the voltage the battery is rated at.

![Picture1](https://user-images.githubusercontent.com/123786420/225213523-91ba58c8-c71a-489d-9725-d6d19a9a9b59.jpg)

I then ran the following code on the Artemis send the motor driver PWM signals.

<script src="https://gist.github.com/sarika2446/a72bdd571865aa66e8aca7913dc52ca7.js"></script>

The oscilloscope waveform confirmed that I could regulate the power on the motor driver output.

![Picture2](https://user-images.githubusercontent.com/123786420/225213640-a725cc5a-cff2-4815-9396-5a0e9533cd28.jpg)

After replacing the control PCB with my motor driver, I was able to run the motor in both directions, as seen in the following video, using the code below.

<script src="https://gist.github.com/sarika2446/3059fe69c1c9e9a47f7973b2f211a27a.js"></script>

[![Wheels](https://img.youtube.com/vi/P4ZfheMGi4o/0.jpg)](https://www.youtube.com/watch?v=P4ZfheMGi4o "Wheels")

I repeated this process for the second motor driver and got the same results.

Next, I powered both motor drivers using the 850mAh battery and was able to control both wheels as expected.

[![Wheels](https://img.youtube.com/vi/UsECsax5K7U/0.jpg)](https://www.youtube.com/watch?v=UsECsax5K7U "Wheels")

This was achieved using the following code.

<script src="https://gist.github.com/sarika2446/6e64bd3ed085b860a3fe03fb5b53f0b9.js"></script>

The picture below shows the car chassis with all of the components installed. I later secured the wires down further.

![Picture3](https://user-images.githubusercontent.com/123786420/225226531-426426af-f8f0-47a1-8008-fc9e0cd00271.jpg)

By testing out increasingly lower PWM values, I found that the lower limit in PWM value for which the robot moved forward and made turns on the ground was 40. While trying to make the car drive in a straight line, I found that I needed to implement a calibration factor of x1.25 to the PWM value of the left motor. 

In the video below, the car moves in a fairly straight line, following the line in the tiles, for about 7 feet.

[![Straight](https://img.youtube.com/vi/vh_hB88Teu0/0.jpg)](https://www.youtube.com/watch?v=vh_hB88Teu0 "Straight")

This was achieved using the following code.

<script src="https://gist.github.com/sarika2446/730a59f91d294e6aa5233dab61e59d3a.js"></script>

Finally I was able to demonstrate open loop control, as seen below, using the following code.

<script src="https://gist.github.com/sarika2446/4495c6edca9f789f570377556c4ef80c.js"></script>

[![Open Loop](https://img.youtube.com/vi/pZCUjEwJZMk/0.jpg)](https://www.youtube.com/watch?v=pZCUjEwJZMk "Open Loop")

