
#include "DFRobot_BMV080.h"
#include <string>

SET_LOOP_TASK_STACK_SIZE(60 * 1024); // 60KB

DFRobot_BMV080_I2C sensor(&Wire);

#define DUTY_CYCLE_PERIOD 20 // Duty cycle period in seconds


void setup() {
  // put your setup code here, to run once:
  char id[13];
  Serial.begin(115200);
  while(!Serial) delay(100);

  while(sensor.begin() != 0){
    Serial.println("Initialization of the sensor failed! Please confirm if the sensor chip connection is correct.");
    delay(1000);
  }
  Serial.println("Initialization of the sensor was successful.");
  while(sensor.openBmv080()){
    Serial.println("open failed");
    delay(1000);
  }
  Serial.println("open successful");
  sensor.getBmv080ID(id);
  Serial.println("Chip ID is:" + String(id));
  
  sensor.setDutyCyclingPeriod(DUTY_CYCLE_PERIOD);
  if(sensor.setBmv080Mode(DFRobot_BMV080_MODE_DUTY_CYCLE))
    Serial.println("Mode setting successful");
}

float pm1,pm2_5,pm10;

void loop() {
  if(sensor.getBmv080Data(&pm1,&pm2_5,&pm10)){
    Serial.print("pm1:" + String(pm1) + "  " + "pm2.5:" + String(pm2_5) + "  " + "pm10:" + String(pm10));

    if(sensor.ifObstructed()){
      Serial.print("  Obstructed The data may be invalid.");
    }

    Serial.println();
  }
  delay(100);
}
