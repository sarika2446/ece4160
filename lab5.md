# Lab 5: Motors and Open Loop Control

## Prelab

I connected the motor drivers, Artemis, and battery as detailed in the diagram below.

![image](https://user-images.githubusercontent.com/123786420/225211341-1390349b-e296-4f0a-a171-21553b9be0fb.jpeg)

The motors and the Artemis are powered by separate batteries, with the motors' battery having a higher capacity (850mAh) than the Artemis' battery (650mAh), because the motors require more power than the Artemis does.

## Lab Tasks

I connected the power supply and oscilloscope to one motor driver, as seen below.

![IMG_1117](https://user-images.githubusercontent.com/123786420/225213305-c44fce19-49dd-4a6f-ad62-e77cf0b1167a.jpg)

I then ran the following code on the Artemis send the motor driver PWM signals.

<script src="https://gist.github.com/sarika2446/a72bdd571865aa66e8aca7913dc52ca7.js"></script>

The oscilloscope waveform confirmed that I could regulate the power on the motor driver output.

![IMG_1102](https://user-images.githubusercontent.com/123786420/225213323-e2925b30-831d-44de-ac73-d6dd69c5ebfd.jpg)

I repeated this process for the second motor driver and got the same results.
