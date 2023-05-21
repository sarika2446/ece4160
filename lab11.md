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
        
        for sample in data_list:
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

<img width="463" alt="Screen Shot 2023-05-21 at 9 54 19 AM" src="https://github.com/sarika2446/ece4160/assets/123786420/328c8b7c-4c1c-4644-ac3b-7477401dcf3e">


### (0,3)
<img width="600" alt="Screen Shot 2023-05-21 at 9 36 44 AM" src="https://github.com/sarika2446/ece4160/assets/123786420/5217f89c-8883-477b-b9fd-e613580fbeb1">

<img width="457" alt="Screen Shot 2023-05-21 at 10 01 35 AM" src="https://github.com/sarika2446/ece4160/assets/123786420/3104e017-3567-4e78-8f16-856acde61c48">


### (5,3)

<img width="600" alt="Screen Shot 2023-05-21 at 10 11 15 AM" src="https://github.com/sarika2446/ece4160/assets/123786420/4963bedc-ceb6-44c7-9d71-45a218c15002">

<img width="459" alt="Screen Shot 2023-05-21 at 10 06 48 AM" src="https://github.com/sarika2446/ece4160/assets/123786420/f7c9295a-8f1e-42be-a28e-dba7e08a32d4">


### (5,-3)
<img width="600" alt="Screen Shot 2023-05-21 at 9 29 52 AM" src="https://github.com/sarika2446/ece4160/assets/123786420/0d10aae9-85cb-4b76-a922-a5cb914c79de">

<img width="459" alt="Screen Shot 2023-05-21 at 9 55 38 AM" src="https://github.com/sarika2446/ece4160/assets/123786420/952110a6-7362-4b2e-b655-d80ea411fd9f">


It was very accurate for the points (-3,-2) and (5,-3), with the ground truth and belief points overlapping exactly such that the ground truth point was not even visible. For the point (0,3), the y-coordinate was accurate, but the x-coordinate was off by 1.219 m. For the point (5,3), the y-coordinate was accurate, but the x-coordinate was also off by 1.219 m. Considering that both of these points with the y-coordinate 3 are off by the exact same amount in the x-direction, some adjustments may need to be made. It is unclear to me why this might be happening, since it is unlikely to be due to random noise as the error was the exact same.
