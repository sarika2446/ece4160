# Lab 10: Grid Localization using Bayes Filter

In this lab, I implemented grid localization using a Bayes filter.

To perform grid localization for the sample trajectory, I had to write the necessary code for the functions compute_control, odom_motion_model, prediction_step, sensor_model, and update_step given in the Lab 10 notebook.

For the first function, compute control, I was able to calculate initial rotation, translation, and final rotation using the equations given in lecture, as seen below.

<img width="600" alt="Screen Shot 2023-04-26 at 3 37 26 AM" src="https://user-images.githubusercontent.com/123786420/234503713-47a9a0ec-d9ed-40c7-9578-dffb16f6e460.png">

I implemented this in Python using the code below.

<script src="https://gist.github.com/sarika2446/4c9110f574824759829811f2e69698ce.js"></script>
