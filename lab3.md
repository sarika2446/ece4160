# Lab 3: Time of Flight Sensors

## Prelab

The expected I2C address for the TOF sensors was 0x52, according to their datasheet. Two sensors will be used, only one of which will have its XSHUT pin soldered to the Artemis because only one sensor needs to be shut down while the other one gets its address changed. This would be more efficient than utilizing the XSHUT pins of both sensors. 

Two sensors were used in order for the robot to be able to sense objects in more than one direction, allowing for better mspping of its path. In order to cover the widest possible area of the robot's motion, I would place one sensor on the front of the robot and one on its side. However, blindspots would still exist due to the angular sensitivity and limited range of the robot, which could impact situations such as performing turns. 

<img width="527" alt="Screen Shot 2023-02-20 at 5 46 57 AM" src="https://user-images.githubusercontent.com/123786420/220084082-89c02ecd-e573-4c02-9052-cbf26cb21901.png">



## Lab Tasks

<img width="480" alt="Screen Shot 2023-02-20 at 5 49 12 AM" src="https://user-images.githubusercontent.com/123786420/220084543-8351d81d-b2c2-405b-857e-ca06b3840925.png">

I first connected one TOF sensor and the IMU to the Artemis. The TOF sensor address outputted when running Example1_wire_I2C.ino was 0x29, not 0x52 as expected. However, this makes sense because the number 0x29 consists of the read/write bit as the most significant bit followed by the address bits, and 0x52 shifted right by one position equals 0x29.

<img width="457" alt="Screen Shot 2023-02-20 at 3 01 58 AM" src="https://user-images.githubusercontent.com/123786420/220078894-3f9ed09d-11ad-4087-9095-65d70ed0ad1e.png">

The TOF sensor has two modes, setDistanceModeShort(), which has a shorter range of 1.3m and setDistanceModeLong() which has a longer range of 4m. I chose to use .setDistanceModeShort() because the short range mode is less affected by ambient light than the long range mode, allowing for more accuracy in making quick decisions. For now, a range of 1.3m seems sufficient. 

I first hooked up one sensor and ran Example1_ReadDistance.ino on the Artemis, and was able to successfully receive an output.

<img width="423" alt="Screen Shot 2023-02-20 at 6 35 43 AM" src="https://user-images.githubusercontent.com/123786420/220097097-10126fa8-9e5d-4ad5-985c-dd436804c87a.png">


By moving an object as far away from the sensor as I could, I was able to determine that the maximum range of the TOF sensor was 2269 mm, which was higher than the expected range for the short range mode.

<img width="411" alt="Screen Shot 2023-02-15 at 4 03 18 PM" src="https://user-images.githubusercontent.com/123786420/220087856-1f16d23c-cfff-45eb-ae96-4abe10287bb5.png">

For my accuracy and repeatability measurements, I took 50 points at each distance specified and used them to find the mean and standard deviation. The sensor was very accurate for a distance of 100mm and got less accurate as the distance increased. Its repeatability was high, as the maximum standard deviation was 1.485. The standard deviation increased as the distance measured increased.



| Distance  (mm)   | Mean (mm) | Standard Deviation (mm)     |
|    :----:   |    :----:   |    :----:     |
| 100      | 101.9      | 0.9434  |
| 200   | 205.6        | 1.168      |
| 300      | 308.4      | 1.410 |
| 400   | 409.6        | 1.485    |

![Unknown-11](https://user-images.githubusercontent.com/123786420/220063997-f0a23a41-7325-4e3b-93a2-565bab145ac6.png)

I also found that the ranging time was 52 ms, using the code below and averaging the time measured across 10 cycles.

<img width="600" alt="Screen Shot 2023-02-20 at 4 22 26 AM" src="https://user-images.githubusercontent.com/123786420/220089785-941f18be-8606-4d30-b13b-befbd5faa10a.png">

I then hooked up the other sensor as well to the QWIIC breakout board and modified Example1_ReadDistance.ino, as shown below.

<img width="600" alt="Screen Shot 2023-02-20 at 6 18 11 AM" src="https://user-images.githubusercontent.com/123786420/220091148-852a3064-17fd-4633-80e7-a25f2200f0a7.png">
<img width="600" alt="Screen Shot 2023-02-20 at 6 20 48 AM" src="https://user-images.githubusercontent.com/123786420/220091664-e6564c60-963a-4283-9e5e-0b842d19cee6.png">

Both sensors were able to output data parallelly as Distance 1 and Distance 2.

<img width="423" alt="Screen Shot 2023-02-20 at 6 35 43 AM" src="https://user-images.githubusercontent.com/123786420/220095557-d37aad93-c682-47b4-9992-b6629c2b833a.png">


I found the speed of the loop to be 91 ms by printing out the timestamp, by calling millis() as seen in the code and serial monitor output above, at the beginning of each loop and averaging the time between timestamps across 10 loops. The current limiting factor is the time it takes to print all of the data to the serial monitor.

I then sent data collected over 5000 ms to the Python end via Bluetooth. I created a command in ble_arduino.ino called GET_TOF for this purpose.

<img width="597" alt="Screen Shot 2023-02-20 at 6 44 16 AM" src="https://user-images.githubusercontent.com/123786420/220097392-45bb2f70-7c82-4b8e-8ecb-83f7ab8175cf.png">


The data received is plotted below. The lines are nearly flat because the sensors were not being moved around while the data was collected.


![Unknown-12](https://user-images.githubusercontent.com/123786420/220094427-716347a4-ec52-4dd7-91c1-7f209098852c.png)



