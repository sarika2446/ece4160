
#include "BLECStringCharacteristic.h"
#include "EString.h"
#include "RobotCommand.h"
#include <ArduinoBLE.h>
#include <Wire.h>
#include "SparkFun_VL53L1X.h" //Click here to get the library: http://librarymanager/All#SparkFun_VL53L1X
#include "ICM_20948.h" // Click here to get the library: http://librarymanager/All#SparkFun_ICM_20948_IMU
#include "math.h"

//////////// BLE UUIDs ////////////
#define BLE_UUID_TEST_SERVICE "d589a328-d84e-4d45-b462-95d49ccccddc"

#define BLE_UUID_RX_STRING "9750f60b-9c9c-4158-b620-02ec9521cd99"

#define BLE_UUID_TX_FLOAT "27616294-3063-4ecc-b60b-3470ddef2938"
#define BLE_UUID_TX_STRING "f235a225-6735-4d73-94cb-ee5dfce9ba83"
#define SHUTDOWN_PIN 8
#define Serial Serial

#define SPI_PORT SPI // Your desired SPI port.       Used only when "USE_SPI" is defined
#define CS_PIN 2     // Which pin you connect CS to. Used only when "USE_SPI" is defined

#define WIRE_PORT Wire // Your desired Wire port.      Used when "USE_SPI" is not defined
// The value of the last bit of the I2C address.
// On the SparkFun 9DoF IMU breakout the default is 1, and when the ADR jumper is closed the value becomes 0
#define AD0_VAL 1

#ifdef USE_SPI
ICM_20948_SPI myICM; // If using SPI create an ICM_20948_SPI object
#else
ICM_20948_I2C myICM; // Otherwise create an ICM_20948_I2C object
#endif
//////////// BLE UUIDs ////////////

//////////// Global Variables ////////////
BLEService testService(BLE_UUID_TEST_SERVICE);

BLECStringCharacteristic rx_characteristic_string(BLE_UUID_RX_STRING, BLEWrite, MAX_MSG_SIZE);

BLEFloatCharacteristic tx_characteristic_float(BLE_UUID_TX_FLOAT, BLERead | BLENotify);
BLECStringCharacteristic tx_characteristic_string(BLE_UUID_TX_STRING, BLERead | BLENotify, MAX_MSG_SIZE);

SFEVL53L1X distanceSensor1;
SFEVL53L1X distanceSensor2;

// RX
RobotCommand robot_cmd(":|");

// TX
EString tx_estring_value;
float tx_float_value = 0.0;

long interval = 500;
static long previousMillis = 0;
unsigned long currentMillis = 0;

int target_distance = 304;
float KP = 0.1;
float KI = 0;
float integral = 0;
float PWM = 0;
//////////// Global Variables ////////////

enum CommandTypes
{
    PING,
    SEND_TWO_INTS,
    SEND_THREE_FLOATS,
    ECHO,
    DANCE,
    SET_VEL,
    GET_TIME_MILLIS,
    GET_TEMP_5s,
    GET_TEMP_5s_RAPID,
    GET_TOF_IMU,
    GET_TOF_PID,
};

void PID_control() {

  float current_distance = distanceSensor1.getDistance();
  float error = current_distance - target_distance; 

  float proportional = KP * error;

  PWM = proportional;

  if (PWM < 40){
    PWM = 40;
  }

  
}


void
handle_command()
{   
    // Set the command string from the characteristic value
    robot_cmd.set_cmd_string(rx_characteristic_string.value(),
                             rx_characteristic_string.valueLength());

    bool success;
    int cmd_type = -1;

    // Get robot command type (an integer)
    /* NOTE: THIS SHOULD ALWAYS BE CALLED BEFORE get_next_value()
     * since it uses strtok internally (refer RobotCommand.h and 
     * https://www.cplusplus.com/reference/cstring/strtok/)
     */
    success = robot_cmd.get_command_type(cmd_type);

    // Check if the last tokenization was successful and return if failed
    if (!success) {
        return;
    }

    // Handle the command type accordingly
    switch (cmd_type) {
        /*
         * Write "PONG" on the GATT characteristic BLE_UUID_TX_STRING
         */
        case PING:
            tx_estring_value.clear();
            tx_estring_value.append("PONG");
            tx_characteristic_string.writeValue(tx_estring_value.c_str());

            Serial.print("Sent back: ");
            Serial.println(tx_estring_value.c_str());

            break;
        /*
         * Extract two integers from the command string
         */
        case SEND_TWO_INTS:
            int int_a, int_b;

            // Extract the next value from the command string as an integer
            success = robot_cmd.get_next_value(int_a);
            if (!success)
                return;

            // Extract the next value from the command string as an integer
            success = robot_cmd.get_next_value(int_b);
            if (!success)
                return;

            Serial.print("Two Integers: ");
            Serial.print(int_a);
            Serial.print(", ");
            Serial.println(int_b);
            
            break;
        /*
         * Extract three floats from the command string
         */
        case SEND_THREE_FLOATS:
            /*
             * Your code goes here.
             */

            break;
        /*
         * Add a prefix and postfix to the string value extracted from the command string
         */
        case ECHO:
        {
            char char_arr[MAX_MSG_SIZE];

            // Extract the next value from the command string as a character array
            success = robot_cmd.get_next_value(char_arr);
            if (!success)
                return;

            tx_estring_value.clear();
            tx_estring_value.append(char_arr);
            tx_estring_value.append(":)");
            tx_characteristic_string.writeValue(tx_estring_value.c_str());
            
            break;
        }
        /*
         * DANCE
         */
        case DANCE:
            Serial.println("Look Ma, I'm Dancin'!");

            break;
        
        /*
         * SET_VEL
         */
        case SET_VEL:

            break;

       /*
        * GET_TIME_MILLIS
        */
       case GET_TIME_MILLIS:

            tx_estring_value.clear();
            tx_estring_value.append("T:");
            tx_estring_value.append(int(millis()));
            tx_characteristic_string.writeValue(tx_estring_value.c_str());

            break;

      case GET_TEMP_5s:
          //String temps[5];
          tx_estring_value.clear();
          for (int i = 0; i < 5; i++){
            tx_estring_value.append("T:");
            tx_estring_value.append(int(millis()));
            tx_estring_value.append("|C:");
            tx_estring_value.append(getTempDegC());
            tx_estring_value.append("|");
            delay(1000);
          }
          tx_characteristic_string.writeValue(tx_estring_value.c_str());

          break;

      case GET_TEMP_5s_RAPID:
      {
        int start_time = int(millis());
          
        
          while ((int(millis()) - start_time) < 5000){
  
            tx_estring_value.clear();
            tx_estring_value.append("T:");
            tx_estring_value.append(int(millis()));
            tx_estring_value.append("|C:");
            tx_estring_value.append(getTempDegC());
            tx_estring_value.append("|");
            myICM.getAGMT();
            
          }
        }
        
        break;
      case GET_TOF_IMU:
      {

        int start_time = int(millis());

        while ((int(millis()) - start_time) < 5000){
          
          distanceSensor1.startRanging(); //Write configuration bytes to initiate measurement
          if (!distanceSensor1.checkForDataReady())
          {
            
            int distance1 = distanceSensor1.getDistance(); //Get the result of the measurement from the sensor
            distanceSensor1.clearInterrupt();
            distanceSensor1.stopRanging();
  
            distanceSensor2.startRanging(); //Write configuration bytes to initiate measurement
            if (!distanceSensor2.checkForDataReady())
            {
              int distance2 = distanceSensor2.getDistance(); //Get the result of the measurement from the sensor
              distanceSensor2.clearInterrupt();
              distanceSensor2.stopRanging();

                if (myICM.dataReady())
                {
                  tx_estring_value.clear();
                  tx_estring_value.append("T:");
                  tx_estring_value.append(int(millis()));
                  tx_estring_value.append("|");
                  tx_estring_value.append("Sensor 1:");
                  tx_estring_value.append(distance1);
                  tx_estring_value.append("|");
                  tx_estring_value.append("Sensor 2:");
                  tx_estring_value.append(distance2);
                  tx_estring_value.append("|");
                  tx_characteristic_string.writeValue(tx_estring_value.c_str());
                  delay(10);
                  myICM.getAGMT();         // The values are only updated when you call 'getAGMT'
                  printScaledAGMT(&myICM); // This function takes into account the scale settings from when the measurement was made to calculate the values with units
                  
            }}
          }
        }
       
      }
        break;
      case GET_TOF_PID:
      {
        int start_time = int(millis());
        while (int(millis()) - start_time < 30000){
          distanceSensor2.startRanging(); //Write configuration bytes to initiate measurement
          if (distanceSensor2.checkForDataReady())
          {
            int distance1 = distanceSensor2.getDistance(); //Get the result of the measurement from the sensor
            distanceSensor1.clearInterrupt();
            distanceSensor1.stopRanging();
            tx_estring_value.clear();
            tx_estring_value.append("T:");
            tx_estring_value.append(int(millis()));
            tx_estring_value.append("|");
            tx_estring_value.append("TOF Sensor:");
            tx_estring_value.append(distance1);
            tx_estring_value.append("|");
            tx_estring_value.append("Motor:");
            tx_estring_value.append(PWM);
            tx_estring_value.append("|");
            tx_characteristic_string.writeValue(tx_estring_value.c_str());
            delay(500);
                    
            }
      }
      }

        break;
        
        
        /* 
         * The default case may not capture all types of invalid commands.
         * It is safer to validate the command string on the central device (in python)
         * before writing to the characteristic.
         */
        default:
            Serial.print("Invalid Command Type: ");
            Serial.println(cmd_type);
            break;
    }
    }


void
setup()
{
    Serial.begin(115200);

    BLE.begin();

    // Set advertised local name and service
    BLE.setDeviceName("Artemis BLE");
    BLE.setLocalName("Artemis BLE");
    BLE.setAdvertisedService(testService);

    // Add BLE characteristics
    testService.addCharacteristic(tx_characteristic_float);
    testService.addCharacteristic(tx_characteristic_string);
    testService.addCharacteristic(rx_characteristic_string);

    // Add BLE service
    BLE.addService(testService);

    // Initial values for characteristics
    // Set initial values to prevent errors when reading for the first time on central devices
    tx_characteristic_float.writeValue(0.0);

    /*
     * An example using the EString
     */
    // Clear the contents of the EString before using it
    tx_estring_value.clear();

    // Append the string literal "[->"
    tx_estring_value.append("[->");

    // Append the float value
    tx_estring_value.append(9.0);

    // Append the string literal "<-]"
    tx_estring_value.append("<-]");

    // Write the value to the characteristic
    tx_characteristic_string.writeValue(tx_estring_value.c_str());

    // Output MAC Address
    Serial.print("Advertising BLE with MAC: ");
    Serial.println(BLE.address());

    BLE.advertise();

    Wire.begin();

  Serial.begin(115200);
  Serial.println("VL53L1X Qwiic Test");

  pinMode(SHUTDOWN_PIN,OUTPUT);
  digitalWrite(SHUTDOWN_PIN,LOW); 

  distanceSensor1.setI2CAddress(0x39);

  digitalWrite(SHUTDOWN_PIN, HIGH);

  if (distanceSensor2.begin() != 0) //Begin returns 0 on a good init
  {
    Serial.println("Sensor failed to begin. Please check wiring. Freezing...");
    while (1)
      ;
  }
  Serial.println("Sensor online!");
  
  if (distanceSensor1.begin() != 0) //Begin returns 0 on a good init
  {
    Serial.println("Sensor failed to begin. Please check wiring. Freezing...");
    while (1)
      ;
  }
  Serial.println("Sensor online!");

  
  distanceSensor1.setDistanceModeShort();
  distanceSensor2.setDistanceModeShort();

#ifdef USE_SPI
  SPI_PORT.begin();
#else
  WIRE_PORT.begin();
  WIRE_PORT.setClock(400000);
#endif

  //myICM.enableDebugging(); // Uncomment this line to enable helpful debug messages on Serial

  bool initialized = false;
  while (!initialized)
  {

#ifdef USE_SPI
    myICM.begin(CS_PIN, SPI_PORT);
#else
    myICM.begin(WIRE_PORT, AD0_VAL);
#endif

    Serial.print(F("Initialization of the sensor returned: "));
    Serial.println(myICM.statusString());
    if (myICM.status != ICM_20948_Stat_Ok)
    {
      Serial.println("Trying again...");
      delay(500);
    }
    else
    {
      initialized = true;
    }
  }

  pinMode(LED_BUILTIN, OUTPUT);
  for (int i=0;i<3;i++){
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(1000);                       // wait for a second
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    delay(1000);
  } 
}

void
write_data()
{
    currentMillis = millis();
    if (currentMillis - previousMillis > interval) {

        tx_float_value = tx_float_value + 0.5;
        tx_characteristic_float.writeValue(tx_float_value);

        if (tx_float_value > 10000) {
            tx_float_value = 0;
            
        }

        previousMillis = currentMillis;
    }
}

float total_pitch = 0;
float pitch_start_time = 0;
float GyroPitch(ICM_20948_I2C *sensor){
  float dt = (int(millis()) - pitch_start_time)/1000;
  pitch_start_time = int(millis());
  total_pitch = total_pitch - (sensor->gyrY() * dt);
  return total_pitch; 
}
  
float total_roll = 0;
float roll_start_time = 0;
float GyroRoll(ICM_20948_I2C *sensor){
  float dt = (int(millis()) - roll_start_time)/1000;
  roll_start_time = int(millis());
  total_roll = total_roll - (sensor->gyrX() * dt);
  return total_roll; 
}
  
float total_yaw = 0;
float yaw_start_time = 0;
float GyroYaw(ICM_20948_I2C *sensor){
  float dt = (int(millis()) - yaw_start_time)/1000;
  yaw_start_time = int(millis());
  total_yaw = total_yaw - (sensor->gyrZ() * dt);
  return total_yaw; 
}

float total_pitch_c = 0;
float pitch_start_time_c = 0;
float CompPitch(ICM_20948_I2C *sensor, float alpha){
  float dt_c = (int(millis()) - pitch_start_time)/1000;
  pitch_start_time_c = int(millis());
  float acc_pitch = atan2(sensor->accX(),sensor->accZ())*(180/M_PI);
  total_pitch_c = (total_pitch_c + (GyroRoll(sensor) * dt_c)) * (1-alpha) + acc_pitch*alpha;
  return total_pitch_c;
}

float total_roll_c = 0;
float roll_start_time_c = 0;
float CompRoll(ICM_20948_I2C *sensor, float alpha){
  float dt_c = (int(millis()) - roll_start_time)/1000;
  roll_start_time_c = int(millis());
  float acc_roll = atan2(sensor->accY(),sensor->accZ())*(180/M_PI);
  total_roll_c = (total_roll_c + (GyroPitch(sensor) * dt_c)) * (1-alpha) + acc_roll*alpha;
  return total_roll_c;
}


void
read_data()
{
    // Query if the characteristic value has been written by another BLE device
    if (rx_characteristic_string.written()) {
        handle_command();
    }
}

#ifdef USE_SPI
void printScaledAGMT(ICM_20948_SPI *sensor)
{
#else
void printScaledAGMT(ICM_20948_I2C *sensor)
{
#endif

  tx_estring_value.append("Pitch:");
  tx_estring_value.append(CompPitch(sensor, 0.5));
  tx_estring_value.append("|");
  tx_estring_value.append("Roll:");
  tx_estring_value.append(CompRoll(sensor,0.5));
  tx_estring_value.append("|");
  
  tx_characteristic_string.writeValue(tx_estring_value.c_str());
  
}

void
loop()
{
    // Listen for connections
    BLEDevice central = BLE.central();

    // If a central is connected to the peripheral
    if (central) {
        Serial.print("Connected to: ");
        Serial.println(central.address());

        // While central is connected
        while (central.connected()) {
            // Send data
            write_data();

            // Read data
            read_data();
        }

        Serial.println("Disconnected");
    }

    
}
