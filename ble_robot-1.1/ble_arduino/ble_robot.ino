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
float KP = 0.03;
float KI = 0;
float integral = 0;
float PWM = 0;
int index = 0;

float TOF_array[500];
float motor_array[500];
float time_array[500];

//////////// Global Variables ////////////

enum CommandTypes
{
    STORE_PID,
    SEND_PID,
};

void forward(int val){
  analogWrite(4,1.25*val);
  analogWrite(A5,0);
  analogWrite(A15,val);
  analogWrite(A16,0);
}

void backward(int val){
  analogWrite(4,0);
  analogWrite(A5,1.25*val);
  analogWrite(A15,0);
  analogWrite(A16,val);
}

void stop_motors(){
  analogWrite(4,0);
  analogWrite(A5,0);
  analogWrite(A15,0);
  analogWrite(A16,0);
}


void PID_control(){

  float current_distance = distanceSensor2.getDistance();
  float error = current_distance - target_distance; 

  float proportional = KP * error;

  PWM = proportional;

  if (PWM == 0){
    stop_motors();
  }
  else if (PWM > 0){
    forward(constrain(PWM,40,255));
  }
  else{
    backward(constrain(PWM,40,255));
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

    void STORE_PID:
    {
      index = 0;
      int starting = int(millis());
      while (int(millis()) - starting > 20000){
        void PID_control();
        if (index < 500){
          time_array[index] = int(millis());
          TOF_array[index] = distanceSensor2.getDistance();
          motor_array[index] = PWM;
          index += 1;
          }
      }
    }
    break;

      void SEND_PID:
      {
        while (index >= 0){
            tx_estring_value.append("T:");
            tx_estring_value.append(time_array[index]);
            tx_estring_value.append("|");
            tx_estring_value.append("TOF Sensor:");
            tx_estring_value.append(TOF_array[index]);
            tx_estring_value.append("|");
            tx_estring_value.append("Motor:");
            tx_estring_value.append(motor_array[index]);
            tx_estring_value.append("|");
            tx_characteristic_string.writeValue(tx_estring_value.c_str());
            index--;     
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

  
  if (distanceSensor2.begin() != 0) //Begin returns 0 on a good init
  {
    Serial.println("Sensor failed to begin. Please check wiring. Freezing...");
    while (1)
      ;
  }
  Serial.println("Sensor online!");
  
  distanceSensor2.setDistanceModeShort();

#ifdef USE_SPI
  SPI_PORT.begin();
#else
  WIRE_PORT.begin();
  WIRE_PORT.setClock(400000);
#endif



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



void
read_data()
{
    // Query if the characteristic value has been written by another BLE device
    if (rx_characteristic_string.written()) {
        handle_command();
    }
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