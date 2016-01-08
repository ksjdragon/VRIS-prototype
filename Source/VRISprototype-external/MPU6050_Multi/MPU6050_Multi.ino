#include "Wire.h"
/* I2Cdev and MPU6050 must be installed as libraries, or else the .cpp/.h files
 * for both classes must be in the include path of your project
 */
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"

#define NUMBER_OF_SENSORS 3 /// YOU MAY NEED TO CHANGE THIS

// Default I2C address is 0x68
// AD0 LOW(0) = 0x68 (Default for SparkFun breakout and InvenSense evaluation board)
// AD0 HIGH(1) = 0x69

// MPU Control Variables
MPU6050 mpu;
bool dmpReady; // Set true if DMP init was successful.
uint8_t devStatus;   // Return status after each device operation. (0 = success, !0 = error)
uint8_t mpuIntStatus; // Holds interrupt status byte from MPU.
uint16_t packetSize; // Expected DMP packet size. (Default is 42 bytes)
uint16_t fifoCount;  // Count of all bytes currently in FIFO.
uint8_t fifoBuffer[64]; // FIFO storage buffer.

// Orientation and Motion Variables
Quaternion q;        // [W, X, Y, Z] Quaternion container.
VectorFloat gravity;  // [X, Y, Z] Gravity vector
float ypr[3];         // [Yaw, Pitch, Roll] array container.

//Digital Pins Reference Variables
/* Displace array beginning with 0.
 * The numbers after the 0th index correspond to the pin connected to the ADO line of each sensor.
 */
int sensorPins[NUMBER_OF_SENSORS + 1] = {0, 5, 6, 7}; // {none, Sensor 1 ADO pin, Sensor 2 ADO pin,...}

// Other Variables
String finalParts[NUMBER_OF_SENSORS]; // Final output.
String final = ""; // Part of final output.

//==============================================================

void switchSensor(int sensorNumber) {
  digitalWrite(sensorPins[sensorNumber], LOW); // Change one ADO line to low. (The one we are recieving from! 0x68)
}

// ================================================================
// ===                  MAIN PROGRAM SETUP                      ===
// ================================================================

void setup() {

  // Setup Variables
  int i = 0; // For loop counter.
  int dmpReadyCounter = 0; // Counts number of sensors ready.
  
  // Set all ADO lines to default high, and configure pins.
  for(i = 1; i <= NUMBER_OF_SENSORS; i++) {
    pinMode(sensorPins[i], OUTPUT);
    digitalWrite(sensorPins[i], HIGH);
  }
  
  Serial.begin(115200); // Initialize serial communication with baud rate.  
 
  for (i = 1; i <= NUMBER_OF_SENSORS; i++) {
    /*  We read data from all sensors by switching addresses one by one, only reading from the first address (0x68).
     *  Therefore, we select sensors by making an ADO line LOW.
     */
    switchSensor(i);
    
    Wire.begin(); // Joins I2C bus. (I2C libary doesn't do this on it's own.)
    TWBR = 24; // Sets SCL higher.
    
    mpu.initialize();  // Intialize device.

    devStatus = mpu.dmpInitialize(); // Load and configure the DMP. (Digital Motion Processor)
    
    // Gyroscope offsets. (Change if necessary)
    mpu.setXGyroOffset(220);
    mpu.setYGyroOffset(76);
    mpu.setZGyroOffset(-85);
    mpu.setZAccelOffset(1788);

    // Check success of DMP.
    if (devStatus == 0) {
      mpu.setDMPEnabled(true);

      dmpReadyCounter += 1; // Add one to count number of ready sensors.
      
      packetSize = mpu.dmpGetFIFOPacketSize(); // Get expected DMP packet size for later comparison
      
    } else {
      // Error!
      Serial.println("Error on sensor " + String(i));
    }
    digitalWrite(sensorPins[i], HIGH); // Reset current ADO pin back to high. (0x69)
    Wire.endTransmission(); // End transmission for sensor.
  }  
  if (dmpReadyCounter == NUMBER_OF_SENSORS) {
    dmpReady = true; // Set DMP Ready flag. (Allows main loop to use the DMP.)
  }
}

// ================================================================
// ===                  MAIN PROGRAM LOOP                       ===
// ================================================================

void loop() {
  // If DMP isn't ready...
  if (!(dmpReady)) {
    return;
  }
  
  final = ""; // Reset final to nothing.

  for (int i = 1; i <= NUMBER_OF_SENSORS; i++) {

    switchSensor(i);
    
    // Check for overflow.
    if (fifoCount == 1024) {
      mpu.resetFIFO(); // Reset so we can continue cleanly.
    } else {
      fifoCount = mpu.getFIFOCount(); // Get current FIFO count.

      // Wait for correct avaliable data length.
      while (fifoCount < packetSize) {
        fifoCount = mpu.getFIFOCount();
      }
      
      mpu.getFIFOBytes(fifoBuffer, packetSize); // Read a packet from FIFO
      /* Track FIFO count in case there is more than 1 packet avaliable.
       *  (Read more without waiting for an interrupt.)
       */
      fifoCount -= packetSize;

      // Get values to process.
      mpu.dmpGetQuaternion(&q, fifoBuffer);
      mpu.dmpGetGravity(&gravity, &q);
      mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);

      // Concatenate for outputting. (Displays in Euler Angles in degrees.)
      finalParts[i - 1] = String(ypr[0] * 180 / M_PI) + "," + String(ypr[1] * 180 / M_PI) + "," + String(ypr[2] * 180 / M_PI) + ":";
      final += finalParts[i - 1];
      digitalWrite(sensorPins[i], HIGH); // Resets current ADO pin back to high. (0x69)
    }
  }
  Serial.println(final.substring(0, final.length() - 1)); // Prints and removes extra colon.
}
