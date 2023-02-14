# Lab 2: Bluetooth

## Prelab

### Setup

For this lab, using Terminal, I installed the latest releases of Python and pip. I also activated the virtual environment using the command _source FastRobots_ble/bin/activate_ and installed the necessary Python packages using the command _pip install numpy pyyaml colorama nest_asyncio bleak jupyterlab_. Additionally, I started the Jupyter server. I then burned the ble_arduino.ino sketch onto the Artemis, after which the Artemis printed its MAC address to the serial monitor.

### Codebase

In the project directory I created, I added the unzipped codebase, as well as the _ble_python_ directory. Some important functions included in this codebase are uuid4(), writeValue(value), clear(), append(), c_str(), and handle_command().

## Lab Tasks

### Configurations

<img width="493" alt="Screen Shot 2023-02-14 at 1 37 01 AM" src="https://user-images.githubusercontent.com/123786420/218658420-61b187a6-2461-445f-9a06-b281ad76ecea.png">

### Demo.ipynb

<img width="776" alt="Screen Shot 2023-02-14 at 1 34 28 AM" src="https://user-images.githubusercontent.com/123786420/218657975-622e0a28-589b-4922-a2f4-4a66856d928c.png">


### Send Echo Command

<img width="531" alt="Screen Shot 2023-02-14 at 1 18 26 AM" src="https://user-images.githubusercontent.com/123786420/218655199-6198ac64-b05c-4bc1-a895-d641f76fcd39.png">

<img width="367" alt="Screen Shot 2023-02-14 at 1 15 47 AM" src="https://user-images.githubusercontent.com/123786420/218654749-e67275c2-ad16-4342-88d1-a101a21aa4a6.png">


### Get Time Command

<img width="474" alt="Screen Shot 2023-02-14 at 1 18 51 AM" src="https://user-images.githubusercontent.com/123786420/218655258-d217368c-9cae-40f8-95f6-1dae2eeb5c37.png">


<img width="368" alt="Screen Shot 2023-02-11 at 4 45 35 PM" src="https://user-images.githubusercontent.com/123786420/218657220-34a9653e-d59f-44ea-a8c3-b3fe00eec9ca.png">


### Notification Handler

<img width="425" alt="Screen Shot 2023-02-14 at 1 32 14 AM" src="https://user-images.githubusercontent.com/123786420/218657609-b3f596d8-9b9a-4068-98cd-9e6ba658ca27.png">

### Get Temperature Commands

<img width="467" alt="Screen Shot 2023-02-14 at 1 28 05 AM" src="https://user-images.githubusercontent.com/123786420/218656853-76bb3287-8d42-4582-b1ce-3bd01067f671.png">

<img width="725" alt="Screen Shot 2023-02-14 at 1 36 18 AM" src="https://user-images.githubusercontent.com/123786420/218658293-2ea18a39-fd79-4e6c-8d03-f16b76e1054b.png">

<img width="464" alt="Screen Shot 2023-02-14 at 1 28 28 AM" src="https://user-images.githubusercontent.com/123786420/218656905-4c4aa532-02c5-4852-96b2-4fb16e0955e0.png">


<img width="683" alt="Screen Shot 2023-02-14 at 1 27 21 AM" src="https://user-images.githubusercontent.com/123786420/218656741-483141eb-cd90-4cb7-b5da-ce35183f8859.png">


### Limitations



