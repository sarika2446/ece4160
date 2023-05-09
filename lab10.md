# Lab 10: Grid Localization using Bayes Filter

In this lab, I implemented grid localization using a Bayes Filter.

The Bayes Filter is an algorithm that uses measurements taken and control data to generate the belief distribution.

<img width="600" alt="Screen Shot 2023-05-09 at 5 07 10 PM" src="https://github.com/sarika2446/ece4160/assets/123786420/9b4e7041-7d46-4bac-988e-ee1130bf11b6">

To perform grid localization for the sample trajectory, I had to write the necessary code for the functions _compute_control_, _odom_motion_model_, _prediction_step_, _sensor_model_, and _update_step_ given in the Lab 10 notebook.

For the first function, _compute_control_, I was able to calculate initial rotation, translation, and final rotation using the equations given in lecture, as seen below. This function extracts the control information based on the odometry motion model, given the current and previous odometry poses.

<img width="600" alt="Screen Shot 2023-04-26 at 3 37 26 AM" src="https://user-images.githubusercontent.com/123786420/234503713-47a9a0ec-d9ed-40c7-9578-dffb16f6e460.png">

I implemented this in Python using the code below.

<script src="https://gist.github.com/sarika2446/4c9110f574824759829811f2e69698ce.js"></script>

Next, I implemented the function for the odometry motion model, _odom_motion_model_, as seen below. This was based on the algorithm given in lecture.

<img width="600" alt="Screen Shot 2023-05-09 at 4 23 10 PM" src="https://github.com/sarika2446/ece4160/assets/123786420/5a3468be-22eb-4cc3-9bed-f89476e6e8db">

<script src="https://gist.github.com/sarika2446/1be1b96fb04b214b71f364c4cacbafbf.js"></script>

I then implemented the function for the prediction step of the Bayes filter, _prediction_step_. This function took the current and previous poses as arguments and updated the probabilities in loc.bel_bar based on loc.bel from the previous time step and the odometry motion model. 

<script src="https://gist.github.com/sarika2446/62773d1d3b4fc5654d4390f6aa9dd9f8.js"></script>

Next, I implemented the function _sensor_model_, which calculated p(z\|x). This function took a 1D array consisting of the true observations for a specific robot pose in the map as an argument and returned a 1D array, with a size equal to the number of observations per cell, with the likelihoods of each individual sensor measurement.

<img width="308" alt="Screen Shot 2023-05-09 at 5 02 04 PM" src="https://github.com/sarika2446/ece4160/assets/123786420/82eaacac-3a9a-4991-8b65-d714fdf44cd0">

<script src="https://gist.github.com/sarika2446/35fc5ed0657528ab4a3a83e66b8062dc.js"></script>

Finally, I implemented the function _update_step_, which performed the update step of the Bayes Filter by updating the probabilities in loc.bel based on loc.bel_bar and the sensor model.

<script src="https://gist.github.com/sarika2446/82553d97453a68809f4306557c593912.js"></script>

I then used these functions to perform grid localization for the given sample trajectory, as seen in the video below, where red is the odometry model, blue is the belief of the robot, and green is the truth pose.

[![Trajectory](https://img.youtube.com/vi/BOzCaoiIVz0/0.jpg)](https://www.youtube.com/watch?v=BOzCaoiIVz0 "Trajectory")
