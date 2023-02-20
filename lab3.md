# Lab 3: Time of Flight Sensors

## Prelab

The expected I2C address for the TOF sensors was 0x52, according to their datasheet. Two sensors will be used, only one of which will have its XSHUT pin soldered to the Artemis because only one sensor needs to be shut down while the other one gets its address changed. This would be more efficient than utilizing the XSHUT pins of both sensors. In order to cover the widest possible area of the robot's motion, I would place one sensor on the front of the robot and one on its side. However, blindspots would still exist due to the angular sensitivity and limited range of the robot, which could impact situations such as performing turns. 

<img width="527" alt="Screen Shot 2023-02-20 at 5 46 57 AM" src="https://user-images.githubusercontent.com/123786420/220084082-89c02ecd-e573-4c02-9052-cbf26cb21901.png">



<img width="457" alt="Screen Shot 2023-02-20 at 3 01 58 AM" src="https://user-images.githubusercontent.com/123786420/220078894-3f9ed09d-11ad-4087-9095-65d70ed0ad1e.png">




| Distance  (mm)   | Mean (mm) | Standard Deviation (mm)     |
|    :----:   |    :----:   |    :----:     |
| 100      | 101.9      | 0.9434  |
| 200   | 205.6        | 1.168      |
| 300      | 308.4      | 1.410 |
| 400   | 409.6        | 1.485    |

![Unknown-11](https://user-images.githubusercontent.com/123786420/220063997-f0a23a41-7325-4e3b-93a2-565bab145ac6.png)
