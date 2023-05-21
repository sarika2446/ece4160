# Lab 12: Localization: Path Planning and Execution

I collaborated with Mariya Sokolova (ms2663) for this lab.

## PID Control

For this lab, We initially attempted to use PID control to navigate the arena. The idea was to always be facing the nearest wall, adjust to the desired distance from the wall based on the waypoint, and use open loop control for making turns. 

We also only used right angle turns so we were always directly facing a wall as we found this made our TOF sensor readings more accurate. This meant there were additional, coded waypoints for my robot to hit in order to make it through the marked waypoints, since this required more maneuvering. Below is our code consisting of the commands for each of the eight movements we had to make.

<script src="https://gist.github.com/sarika2446/0dec5c37ca5bb31aaad5997ea414b692.js"></script>

The PID function was defined as follows:

<script src="https://gist.github.com/sarika2446/0e41571ecbc67a3d2baa5be24b9084ae.js"></script>

On the Python end, once a command finished executing, we would send the next one.

In general, the PID control worked very well for getting to the right distance from the wall. The most difficult part was getting the timing right for the open loop turns, as the timing necessary for turning a certain angle varied depending on the location within the arena, as well as between runs, due to uneven terrain and the arena being dirty at times. Another issue was that for some reason, my robot stopped being able to make right turns, so I had to turn to the left 270 degrees, which introduced more variability.

For our best trial, we were able to hit 3 waypoints, but unfortunately, this was not recorded on video and we were not able to replicate this run when recording. Below is an example of our PID in action when attempting to get to the second waypoint. The second turn was not as expected, but the sequence of PID control for the wall -> open loop turn -> PID control for the wall worked well.

[![PID](https://img.youtube.com/vi/zfY1S1t2PP8/0.jpg)](https://www.youtube.com/watch?v=zfY1S1t2PP8 "PID")

## Open Loop

In the interest of time, we then attempted open loop control of the robot, using Mariya's robot instead. Below is the code for this.

<img width="600" alt="olc_0" src="https://github.com/sarika2446/ece4160/assets/123786420/a00a59fc-4caa-4258-b2c9-fd119992d803">
<img width="600" alt="olc_1" src="https://github.com/sarika2446/ece4160/assets/123786420/a748b3db-28b1-4a27-8ec1-1b86ba69578a">
<img width="600" alt="olc_2" src="https://github.com/sarika2446/ece4160/assets/123786420/fed34f71-e8d3-47e3-8fdc-a6508c3228c0">
<img width="600" alt="olc_3" src="https://github.com/sarika2446/ece4160/assets/123786420/bad6b5c4-c794-4b85-9dfc-9c530c8a7fbf">

Although open loop control can be unreliable due to how much variability there can be between runs, we found this approach to be decently successful, hitting 6 out of the 9 waypoints.

Below is a video of one of our best runs.

[![OLC](https://img.youtube.com/vi/wp4BHi55qmI/0.jpg)](https://www.youtube.com/watch?v=wp4BHi55qmI "OLC")

## Conclusion

Overall, we found both PID and open loop control to work moderately well. If we had had more time to tune PWM values and delay times, I am sure we could have found even more success using these methods.
