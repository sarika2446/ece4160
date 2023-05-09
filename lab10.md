# Lab 10: Grid Localization using Bayes Filter

In this lab, I implemented grid localization using a Bayes filter.

To perform grid localization for the sample trajectory, I had to write the necessary code for the functions compute_control, odom_motion_model, prediction_step, sensor_model, and update_step given in the Lab 10 notebook.

For the first function, compute_control, I was able to calculate initial rotation, translation, and final rotation using the equations given in lecture, as seen below. This function extracts the control information based on the odometry motion model, given the current and previous odometry poses.

<img width="600" alt="Screen Shot 2023-04-26 at 3 37 26 AM" src="https://user-images.githubusercontent.com/123786420/234503713-47a9a0ec-d9ed-40c7-9578-dffb16f6e460.png">

I implemented this in Python using the code below.

<script src="https://gist.github.com/sarika2446/4c9110f574824759829811f2e69698ce.js"></script>

Next, I implemented the function for the odometry motion model, as seen below. This was based on the algorithm given in lecture.

<img width="830" alt="Screen Shot 2023-05-09 at 4 07 37 PM" src="https://github.com/sarika2446/ece4160/assets/123786420/d2f394c5-0a93-4ef8-9226-3d23d6e84f5f">

<script src="https://gist.github.com/sarika2446/1be1b96fb04b214b71f364c4cacbafbf.js"></script>

