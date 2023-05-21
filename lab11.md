# Lab 11: Localization on the real robot
Using the provided code, I was able to successfully run the simulation.

<img width="752" alt="Screen Shot 2023-05-17 at 8 55 36 PM" src="https://github.com/sarika2446/ece4160/assets/123786420/8b992c59-ae27-4b39-a757-d409884ac207">

I then implemented the member function perform_observation_loop of class RealRobot. I ended up taking 20 measurements for each rotation instead of 18 because I found that to be the most accurate for my robot. I was able to reuse my code from Lab 9 for this.

    def perform_observation_loop(self, rot_vel=120):
        """Perform the observation loop behavior on the real robot, where the robot does  
        a 360 degree turn in place while collecting equidistant (in the angular space) sensor
        readings, with the first sensor reading taken at the robot's current heading. 
        The number of sensor readings depends on "observations_count"(=18) defined in world.yaml.
        
        Keyword arguments:
            rot_vel -- (Optional) Angular Velocity for loop (degrees/second)
                        Do not remove this parameter from the function definition, even if you don't use it.
        Returns:
            sensor_ranges   -- A column numpy array of the range values (meters)
            sensor_bearings -- A column numpy array of the bearings at which the sensor readings were taken (degrees)
                               The bearing values are not used in the Localization module, so you may return a empty numpy array
        """
        
        ble.send_command(CMD.SPIN, "")
        ble.stop_notify(ble.uuid['RX_STRING'])
        s = ble.receive_string(ble.uuid['RX_STRING'])
        
        sensor_ranges = []
        sensor_bearings = []
        
        for sample in data1:
          count = 1
          flag = 0
          string = ''
          for i in range(len(sample)):
            if (sample[i] == ':'):
              flag = 1;
            if (sample[i] == '|'):
              flag = 0;
              if (count == 2):
                sensor_ranges.append(float(string))
                string = ''
                count = count + 1
              elif (count == 4):
                sensor_bearings.append(float(string))
                string = ''
                count = count + 1
            elif (flag == 1):
              if (sample[i]!=':'):
                string = string + sample[i]
        
        return sensor_ranges, sensor_bearings

## Running Update Step on Marked Poses

### (-3,-2)
<img width="600" alt="Screen Shot 2023-05-21 at 9 38 46 AM" src="https://github.com/sarika2446/ece4160/assets/123786420/a367ff1a-676b-4c1c-be8e-81a562d84211">


### (0,3)
<img width="600" alt="Screen Shot 2023-05-21 at 9 36 44 AM" src="https://github.com/sarika2446/ece4160/assets/123786420/5217f89c-8883-477b-b9fd-e613580fbeb1">


### (5,3)
<img width="600" alt="Screen Shot 2023-05-21 at 9 35 16 AM" src="https://github.com/sarika2446/ece4160/assets/123786420/3676aa4e-a401-465f-987f-dffa52163972">


### (5,-3)
<img width="600" alt="Screen Shot 2023-05-21 at 9 29 52 AM" src="https://github.com/sarika2446/ece4160/assets/123786420/0d10aae9-85cb-4b76-a922-a5cb914c79de">




Generally, very inaccurate beliefs in comparison to ground truths. Did better for the first two.
