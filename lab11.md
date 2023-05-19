# Lab 11: Localization on the real robot
Using the provided code, I was able to successfully run the simulation.

<img width="752" alt="Screen Shot 2023-05-17 at 8 55 36 PM" src="https://github.com/sarika2446/ece4160/assets/123786420/8b992c59-ae27-4b39-a757-d409884ac207">

I then implemented the member function perform_observation_loop of class RealRobot. I ended up taking 20 measurements for each rotation instead of 18 because I found that to be the most accurate for my robot.

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
<img width="600" alt="Screen Shot 2023-05-18 at 8 49 48 PM" src="https://github.com/sarika2446/ece4160/assets/123786420/c43fb964-48f2-4f1b-ac88-644c1b899ad5">

### (0,3)
<img width="600" alt="Screen Shot 2023-05-18 at 8 44 17 PM" src="https://github.com/sarika2446/ece4160/assets/123786420/c2cbc26c-15d8-4189-87b1-4c29d2ef05c6">

### (5,3)

<img width="600" alt="Screen Shot 2023-05-18 at 9 07 00 PM" src="https://github.com/sarika2446/ece4160/assets/123786420/93c59f45-8369-427f-93ea-5f4df12d92e5">

### (5,-3)

<img width="600" alt="Screen Shot 2023-05-18 at 9 08 45 PM" src="https://github.com/sarika2446/ece4160/assets/123786420/0b6b186b-89dc-4069-8403-78aeb48808e9">

Generally, very inaccurate beliefs in comparison to ground truths. Did better for the first two.
