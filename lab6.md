# Lab 6: Closed-loop control (PID)

## Prelab

To send and receive debugging data over Bluetooth, I created a command called GET_TOF_PID in which PID control executes for 20 seconds, and timestamp, ToF, and motor input data is collected every half a second. At the end of the 20 seconds, the data is sent via Bluetooth to the Python end. On the Python end, the data is handled and displayed by a notification handler.

<script src="https://gist.github.com/sarika2446/1bbae2fb5ddcfcd791081a1c1613fc8c.js"></script>

<img width="400" alt="Screen Shot 2023-03-23 at 12 55 53 AM" src="https://user-images.githubusercontent.com/123786420/227107152-13d7535a-8717-48da-ab93-3e038ebfb5f6.png">

## Lab Tasks

I chose to do task A.

Through extensive testing, I found that a controller with only the proportional term worked best for my system and caused minimal overshooting and oscillation. I started out by increasing the proportional gain from 0 to 0.03, where I saw a lot of oscillation. I then reduced it by a factor of 2 to 0.015, and found that this caused the robot to stop at a distance very close to 304 mm from the obstacle. I found that I needed to implement a small range around 304 mm in which the target would be considered reached because the ToF sensor would not always detect a value of 304 mm even if it passed that point due to the relatively low sampling frequency.

I used the short distance mode for higher accuracy, but this meant I could not start the car from a very far distance from the obstacle and still have it operate efficiently. I also tried to decrease sampling time by only sampling the distance once each loop and passing that distance as a parameter to the PID control function. I also tried to increase the sampling rate by decreasing sampling time by sending the timestamped data via Bluetooth only after the PID control loop had finished executing and removing any delays that were in the loop.

<script src="https://gist.github.com/sarika2446/cc1df2c13a3bf3afa6c2e5523a372ccf.js"></script>

I ran three trials using the code above, starting from a distance of either 2 meters or approximately 1.6 meters, and the robot stopped at 305 mm, 305 mm, and 304 mm from the obstacle with minimal oscillation, as seen in the videos below. In the following graphs of the ToF and motor input data, the first, second, and third trial are represented by the blue, green, and red lines, respectively.

I also ensured that the PWM value would never be in the deadband zone by constraining it to be between 40 and 255 when in motion. However, the low proportional gain term caused the PWM to only either be 0 or 40.

![Unknown-21](https://user-images.githubusercontent.com/123786420/227113278-8958d861-d863-4f9c-9633-d65d477f4684.png)

![Unknown-22](https://user-images.githubusercontent.com/123786420/227113296-c9e50ff3-8dcf-4f9d-a5c7-9d21c8d71b00.png)

[![Trial 1](https://img.youtube.com/vi/HWcBgK-kpQY/0.jpg)](https://www.youtube.com/watch?v=HWcBgK-kpQY "Trial 1")

[![Trial 2](https://img.youtube.com/vi/twxQ9wwRzno/0.jpg)](https://www.youtube.com/watch?v=twxQ9wwRzno "Trial 2")

[![Trial 3](https://img.youtube.com/vi/K7b_ZMfY2p4/0.jpg)](https://www.youtube.com/watch?v=K7b_ZMfY2p4 "Trial 3")
