#include <HX711.h>
#include <Wire.h>

#define calibrationFactor -3050.0 // This value is obtained using the SparkFun_HX711_Calibration sketch
#define NUMBER_OF_WEIGHTS 4

#define I2C_ADDRESS       0x10

#define weightSensorPinsStart 3
#define weightSensorClock  2

#define weightThreshold   30

HX711 weightSensor[NUMBER_OF_WEIGHTS];

float values[NUMBER_OF_WEIGHTS];

void setup() {
  Wire.begin(I2C_ADDRESS);

  Wire.onRequest(requestEvent);

  Serial.begin(9600);
  Serial.println("Cyber!123");

  initWeight();
}

void initWeight() {
  for (int i = 0; i < NUMBER_OF_WEIGHTS; i++) {
    weightSensor[i].begin(weightSensorPinsStart + i, weightSensorClock);
    weightSensor[i].set_scale(calibrationFactor); //This value is obtained by using the SparkFun_HX711_Calibration sketch
    weightSensor[i].tare(); //Assuming there is no weight on the scale at start up, reset the scale to 0
  }
}

void requestEvent() {
  Wire.write((uint8_t*)values, NUMBER_OF_WEIGHTS * sizeof(values[0]));
}

void loop() {
  for (int i = 0; i < NUMBER_OF_WEIGHTS; i++) {
    values[i] = (weightSensor[i].get_units(3));
  }
  delay(8);
}
