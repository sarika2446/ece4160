# Lab 6: Closed-loop control (PID)

## Prelab

To send and receive debugging data over Bluetooth, I created a command called GET_TOF_PID in which PID control executes for 20 seconds, and timestamp, ToF, and motor input data is collected every half a second. At the end of the 20 seconds, the data is sent via Bluetooth to the Python end. On the Python end, the data is handled and displayed by a notification handler.

<script src="https://gist.github.com/sarika2446/1bbae2fb5ddcfcd791081a1c1613fc8c.js"></script>

<img width="400" alt="Screen Shot 2023-03-23 at 12 55 53 AM" src="https://user-images.githubusercontent.com/123786420/227107152-13d7535a-8717-48da-ab93-3e038ebfb5f6.png">
