# Lab 1: Artemis

## Introduction

In this lab, I gained familiarity with the Artemis Nano board by running four demo programs on it using the Arduino IDE.

## Materials

- 1 x SparkFun RedBoard Artemis Nano
- 1 x USB C-to-C cable

## Implementation

### Blink it Up

For this program, I used the Blink example code that came with the Arduino IDE. This program allowed me to blink the onboard LED. I slowed the baud rate down to 9600 to be able to see the blinking better.


[![Blink](https://img.youtube.com/vi/yjpnnuYoAPc/0.jpg)](https://www.youtube.com/watch?v=yjpnnuYoAPc "Blink")




### Serial

For this program, I used the Example04_Serial example code that came with the Artemis Nano installation. This program printed out text to the serial monitor and also "echoed" any text I provided as input, by printing it to the serial monitor. I had to adjust the serial monitor's baud rate to 115200, matching the rate defined in the code, in order to be able to view the output.


[![Serial](https://img.youtube.com/vi/E9h5a1GjAis/0.jpg)](https://www.youtube.com/watch?v=E9h5a1GjAis "Serial")



### Analog Read

For this program, I used the Example02_AnalogRead example code that came with the Artemis Nano installation. This program printed out the temperature measured, using the board's temperature sensor, and printed it to the serial monitor. In this video, I blew air on the temperature sensor, causing its measured temperature to slowly rise from around 32300 to 33000.


[![Analog Read](https://img.youtube.com/vi/ULwuDB3LZps/0.jpg)](https://www.youtube.com/watch?v=ULwuDB3LZps "Analog Read")



### Microphone Output

For this program, I used the Example1_MicrophoneOutput example code that came with the Artemis Nano installation. The program printed out the loudest frequency detected by the board's microphone to the serial monitor. In this video, I played a 600 Hz tone, and the value printed to the serial monitor was consistent and almost exactly 600 Hz.

[![Microphone Output](https://img.youtube.com/vi/lORvnI_mxfM/0.jpg)](https://www.youtube.com/watch?v=lORvnI_mxfM "Microphone Output")



## Conclusion

By running various demo programs on the Artemis Nano board, I was able to become comfortable with programming it and utilizing its onboard components. 
