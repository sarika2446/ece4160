# Lab 7: Kalman Filter

## Estimating Drag and Momentum

I used a step size of 50, which was slightly higher than the PWM value of 40 I used in lab 6. 

I found that the steady state speed was roughly 1318 mm/s and that the 90% rise time was 1.353 seconds.

![Unknown-28](https://user-images.githubusercontent.com/123786420/229270392-686165b6-7483-4c1d-8493-259a85333e0e.png)

![Unknown-27](https://user-images.githubusercontent.com/123786420/229270396-6ec94149-d9d4-49ec-ba10-8871f51319d6.png)

![Unknown-26](https://user-images.githubusercontent.com/123786420/229270399-5bad97cd-49f4-4eb1-8439-79cf6cf3d66a.png)

Using the equations given in lecture, I found that d = 0.00077 and m = 0.00045.

## Initializing KF

In Jupyter Lab, I initialized my A, B, C, process noise, and sensor noise covariance matrices, as well as my state vector.

<script src="https://gist.github.com/sarika2446/25da492d43e9cf04b448f69cb5847ed6.js"></script>

## Implementing and Testing Kalman Filter in Jupyter Lab

I implemented  my Kalman Filter in Jupyter lab and tested it on data from a straight run towards a wall.

<script src="https://gist.github.com/sarika2446/9a6066f4dc25589f6e06404ff090f3ed.js"></script>

![Unknown-29](https://user-images.githubusercontent.com/123786420/229270707-07cbeec6-c827-46d2-a4d8-a17ff9f291b5.png)

The Kalman filter was pretty close to the actual data after the first couple of seconds.


## 4A: Extrapolation

I used the following function to extrapolate every third distance data point based on the previous two TOF sensor readings.

<script src="https://gist.github.com/sarika2446/f200e49a58a3d139c3a8bd3b120d0811.js"></script>
