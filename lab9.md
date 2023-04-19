# Lab 9: Mapping

## Control

For this lab, I chose to do implement open loop control. In order to get open loop control to work, I had to ensure that my robot could spin on its axis slowly while taking TOF sensor measurements. 

I used the following code to accomplish collect this data and send it to the Python end via Bluetooth.

<script src="https://gist.github.com/sarika2446/9bc6df13f540c4c641ba04e589819775.js"></script>

Below is a video of my robot turning.

https://user-images.githubusercontent.com/123786420/232996587-10f94f04-a77d-4d11-a67e-50f908a23207.mov

## Read out Distances

I chose to take my measurements in the hallway outside of the lab, starting at the orientation seen below seen below.

![car](https://user-images.githubusercontent.com/123786420/233005775-a35eb08a-4880-48e4-9ccf-6790e11ffbb2.jpg)

Below is a plot of the readings taken from one complete rotation in the hallway.

![Unknown-32](https://user-images.githubusercontent.com/123786420/233005662-68351ac0-64b8-4495-bacf-20835f9abf9e.png)


