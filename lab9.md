# Lab 9: Mapping

## Control

The goal of the lab was to create a map of a specific room, which would be used for future navigation and localization tasks. To construct the map, the robot needed to be placed at different points in the room and rotate while collecting ToF readings. The room being mapped, in this case the hallway, was static.

I chose to implement orientation control, where I was able to get my robot to do on-axis turns in small increments using a PID controller.

I used the following code to accomplish collect this data and send it to the Python end via Bluetooth.

<script src="https://gist.github.com/sarika2446/b55a06cb10055132facb7352c82f1776.js"></script>

I used the following function to implement PID control for the orientation, with a target angle of 25 degrees for each increment.

<script src="https://gist.github.com/sarika2446/21f59e39d031b0e0c2350a95f5159c00.js"></script>

Below is a video of my robot turning.

[![Spin](https://img.youtube.com/vi/udNTjOFebKc/0.jpg)](https://www.youtube.com/watch?v=udNTjOFebKc "Spin")

## Read out Distances

I chose to take my measurements in the hallway outside of the lab, starting at the orientation seen below seen below.

![IMG_3202](https://user-images.githubusercontent.com/123786420/233005926-9dc48a66-6be7-4337-a1f5-036f1d9a3cdd.jpg)

Below is a plot of the readings taken from one complete rotation in the hallway.

![Unknown-32](https://user-images.githubusercontent.com/123786420/233005662-68351ac0-64b8-4495-bacf-20835f9abf9e.png)


