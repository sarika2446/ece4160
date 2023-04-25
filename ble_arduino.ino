
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
#define SERIAL_PORT Serial

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
float KP = 0.015;
float PWM = 0;


int t_forward = 1090;


float TOF_array[40];
float motor_array[40];
float time_array[40];
float TOF_array_ext[14];
float time_array_ext[14];
float motor_array_ext[14];

float stunt_TOF[15];
float stunt_motor[15];
float stunt_time[15];

float orientation_TOF[36];
float orientation_PWM[36];
float orientation_time[36];

int m = 0;

int k = 0;
int i = 0;
int pred_dist = 0;
int sample_count = 1;
int distance = 0;

float angle = 0;
float total_yaw = 0;

ICM_20948_I2C *sensor;
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
    STUNT,
    SPIN,
};

void backward(int val){
  analogWrite(4,0);
  analogWrite(A5,val);
  analogWrite(A15,val);
  analogWrite(A14,0);
}

void forward(int val){
  analogWrite(4,val);
  analogWrite(A5,0);
  analogWrite(A15,0);
  analogWrite(A14,val);
}

void stop_motors(){
  analogWrite(4,0);
  analogWrite(A5,0);
  analogWrite(A15,0);
  analogWrite(A14,0);
}

void left(int val){
  analogWrite(4,0);
  analogWrite(A5,0);
  analogWrite(A15,val);
  analogWrite(A16,0);
  delay(475);
}

void right(int val){
  analogWrite(4,val);
  analogWrite(A5,0);
  analogWrite(A15,0);
  analogWrite(A16,0);
  delay(475);
}


void PID_control(int dist){
  int current_distance = dist;
  float error = current_distance - target_distance;
  float proportional = KP * error;
  float range = KP * 4;


  
  PWM = proportional;
  

  if (PWM < range && PWM > -range){
    PWM = 0;
    stop_motors();
  }
  else if (PWM > 0){
    PWM = constrain(PWM,40,80);
    forward(PWM);
  }
  else{
    PWM = constrain(PWM,40,80);
    backward(PWM);
  }

  
}

void PID_orientation(int angle)
{
  int i = 0;
  int kp = 1;
  int target_angle = 25;
  int current_angle = abs(angle);
  float error = current_angle - target_angle;
  float proportional = kp * error;
  float range = kp * 2;

  PWM = proportional;

  if (PWM < range && PWM > -range)
  {
    distanceSensor2.startRanging();
    if (distanceSensor2.checkForDataReady())
    {
      orientation_TOF[i] = distanceSensor2.getDistance();
      distanceSensor2.clearInterrupt();
      distanceSensor2.stopRanging();
      orientation_time[i] = int(millis());
      orientation_PWM[i] = PWM;
      i = i + 1;
    }
    total_yaw = 0;
  }
  else if (PWM > 0){
    PWM = constrain(PWM,40,80);
    left(PWM);
  }
  else{
    PWM = constrain(PWM,40,80);
    right(PWM);
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



void extrapolate (){
  int velocity = (TOF_array[i-1] - TOF_array [i-2])/(time_array[i-1] - time_array [i-2]);
  pred_dist = TOF_array[i-1] + velocity * (int(millis())-time_array[i-1]);

  PID_control(pred_dist); 
} 

#ifdef USE_SPI
void printScaledAGMT(ICM_20948_SPI *sensor)
{
#else
void printScaledAGMT(ICM_20948_I2C *sensor)
{
#endif
 
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
          if (distanceSensor1.checkForDataReady())
          {
            
            int distance1 = distanceSensor1.getDistance(); //Get the result of the measurement from the sensor
            distanceSensor1.clearInterrupt();
            distanceSensor1.stopRanging();
  
            distanceSensor2.startRanging(); //Write configuration bytes to initiate measurement
            if (distanceSensor2.checkForDataReady())
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
                  //printScaledAGMT(&myICM); // This function takes into account the scale settings from when the measurement was made to calculate the values with units
                  
            }}
          }
        }
       
      }
        break;
    case GET_TOF_PID:
      {
         int sample_count = 1;
         int last_sample = 0;
         int i = 0;
         int starting = int(millis());
         
         while ((int(millis()) - starting)<20000)
         {
           if (sample_count%3 != 0)
           {
             sample_count += 1;
             distanceSensor2.startRanging(); //Write configuration bytes to initiate measurement
             if (distanceSensor2.checkForDataReady())
             {
              int distance = distanceSensor2.getDistance();
              distanceSensor2.clearInterrupt();
              distanceSensor2.stopRanging();
              PID_control(distance);
              if ((int(millis())-last_sample) > 500)
              {
                if (i < 40)
                {
                  time_array[i] = int(millis());
                  TOF_array[i] = distance;
                  motor_array[i] = PWM;
                  i++;
                  if (PWM == 0.0)
                  {
                    stop_motors();
                    break;
                  }
                  
                  last_sample = int(millis());
                }
              }
            } 
          }
          else{
            int velocity = (TOF_array[i-1] - TOF_array [i-2])/(time_array[i-1] - time_array [i-2]);
            pred_dist = TOF_array[i-1] + velocity * (int(millis())-time_array[i-1]);

            PID_control(pred_dist);
            sample_count += 1; 

            if ((int(millis())-last_sample) > 500)
              {
                if (k < 14)
                {
                  time_array_ext[k] = int(millis());
                  TOF_array_ext[k] = distance;
                  motor_array_ext[k] = PWM;
                  k++;
                  if (PWM == 0.0)
                  {
                    stop_motors();
                    break;
                  }
                  
                  last_sample = int(millis());
                }
              }
          }
        }

        int j = 0;
        while (j < i)
        {
          tx_estring_value.clear();
          tx_estring_value.append("T:");
          tx_estring_value.append(time_array[j]);
          tx_estring_value.append("|");
          tx_estring_value.append("TOF Sensor:");
          tx_estring_value.append(TOF_array[j]);
          tx_estring_value.append("|");
          tx_estring_value.append("Motor:");
          tx_estring_value.append(motor_array[j]);
          tx_estring_value.append("|");
          tx_characteristic_string.writeValue(tx_estring_value.c_str());
          j++;     
        }

        int s = 0;
        while (s < k)
        {
          tx_estring_value.clear();
          tx_estring_value.append("T:");
          tx_estring_value.append(time_array[s]);
          tx_estring_value.append("|");
          tx_estring_value.append("TOF Sensor extrapolated:");
          tx_estring_value.append(TOF_array[s]);
          tx_estring_value.append("|");
          tx_estring_value.append("Motor:");
          tx_estring_value.append(motor_array[s]);
          tx_estring_value.append("|");
          tx_characteristic_string.writeValue(tx_estring_value.c_str());
          s++;     
        }
      }
        break;
        
      case STUNT:
      {
        int starting_time = int(millis());
        int i = 0;
        int last_sample = 0;
      
        while((int(millis()) - starting_time) < t_forward)
        {
          forward(255);
          if (int(millis()) - last_sample > 250)
          {
            if (i < 15)
            {
              distanceSensor2.startRanging();
              if (distanceSensor2.checkForDataReady())
              {
                stunt_TOF[i] = distanceSensor2.getDistance();
              }
              distanceSensor2.clearInterrupt();
              distanceSensor2.stopRanging();
              stunt_motor[i] = PWM;
              stunt_time[i] = int(millis());
              last_sample = int(millis());
              i = i + 1;
            }
          }
           
        }
      
      
        starting_time = int(millis());
      
        while((int(millis()) - starting_time) < 1250)
        {
          backward(255);
          
          if (int(millis()) - last_sample > 250)
          {
            if (i < 15)
            {
              distanceSensor2.startRanging();
              if (distanceSensor2.checkForDataReady())
              {
                stunt_TOF[i] = distanceSensor2.getDistance();
              }
              distanceSensor2.clearInterrupt();
              distanceSensor2.stopRanging();
              stunt_motor[i] = PWM;
              stunt_time[i] = int(millis());
              last_sample = int(millis());
              i = i + 1;
            }
          }
          
        }

        
      
        stop_motors();

        int j = 0;
        while (j < i)
        {
          tx_estring_value.clear();
          tx_estring_value.append("T:");
          tx_estring_value.append(stunt_time[j]);
          tx_estring_value.append("|");
          tx_estring_value.append("TOF Sensor:");
          tx_estring_value.append(stunt_TOF[j]);
          tx_estring_value.append("|");
          tx_estring_value.append("Motor:");
          tx_estring_value.append(stunt_motor[j]);
          tx_estring_value.append("|");
          tx_characteristic_string.writeValue(tx_estring_value.c_str());
          j++;     
        }
        
      }
      break;
      case SPIN:
      {
         int starting = int(millis());
         while ((int(millis()) - starting)<5000)
         {
          
          Serial.print("enter while");
          if (myICM.dataReady())
          {
            
            Serial.print("enter if myICM.dataReady()");
            myICM.getAGMT();
            Serial.print("myICM.getAGMT()");
            angle = GyroYaw(sensor);
            Serial.print("got angle");
          }
          PID_orientation(angle);
          Serial.print("passed PID");
         }

        int j = 0;
        while (j < 36)
        {
          tx_estring_value.clear();
          tx_estring_value.append("T:");
          tx_estring_value.append(orientation_time[j]);
          tx_estring_value.append("|");
          tx_estring_value.append("TOF Sensor:");
          tx_estring_value.append(orientation_TOF[j]);
          tx_estring_value.append("|");
          tx_estring_value.append("Motor:");
          tx_estring_value.append(orientation_PWM[j]);
          tx_estring_value.append("|");
          tx_characteristic_string.writeValue(tx_estring_value.c_str());
          j++;     
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

  pinMode(A5,OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(A15,OUTPUT);
  pinMode(A14,OUTPUT);

  SERIAL_PORT.begin(115200);
  while (!SERIAL_PORT)
  {
  };

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

    SERIAL_PORT.print(F("Initialization of the sensor returned: "));
    SERIAL_PORT.println(myICM.statusString());
    if (myICM.status != ICM_20948_Stat_Ok)
    {
      SERIAL_PORT.println("Trying again...");
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
