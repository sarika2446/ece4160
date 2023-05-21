# Lab 12: Localization: Path Planning and Execution

## PID Control
For this lab, I initially attempted to use PID control to navigate the arena. The idea was to always be facing the nearest wall, adjust to the desired distance from the wall based on the waypoint, and use open loop control for making turns. 

I also only used right angle turns so I was always directly facing a wall as I found this made my TOF sensor readings more accurate. This meant there were additional, coded waypoints for my robot to hit in order to make it through the marked waypoints, since this required more maneuvering. Below is my code consisting of the commands for each of the eight movements I had to make.

<script src="https://gist.github.com/sarika2446/0dec5c37ca5bb31aaad5997ea414b692.js"></script>

My PID function was defined as follows:

<script src="https://gist.github.com/sarika2446/0e41571ecbc67a3d2baa5be24b9084ae.js"></script>

On the Python end, once a command finished executing, I would send the next one.

In general, the PID control worked very well for getting to the right distance from the wall. The most difficult part was getting the timing right for the open loop turns, as the timing necessary for turning a certain angle varied depending on the location within the arena, as well as between runs, due to uneven terrain and the arena being dirty at times. Another issue was that for some reason, my robot stopped being able to make right turns, so I had to turn to the left 270 degrees, which introduced more variability.

For my best trial, I was able to hit 3 waypoints, but unfortunately, this was not recorded on video and I was not able to replicate this run when recording. Below is an example of my PID in action when attempting to get to the second waypoint. The second turn was not as expected, but the sequence of PID control for the wall -> open loop turn -> PID control for the wall worked well.

[![PID](https://img.youtube.com/vi//zfY1S1t2PP8/0.jpg)](https://www.youtube.com/watch?v=/zfY1S1t2PP8 "PID")

