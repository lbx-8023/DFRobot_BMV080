
#include "DFRobot_BMV080.h"
#include <string>

SET_LOOP_TASK_STACK_SIZE(80 * 1024); // 60KB

DFRobot_BMV080_I2C sensor(&Wire);


void setup() {
  // put your setup code here, to run once:
  char id[13];
  Serial.begin(115200);
  delay(500);

  while(sensor.begin() != 0){
    Serial.println("初始化芯片失败，请确认芯片连接是否正确");
    delay(500);
  }
  Serial.println("初始化芯片成功");
  while(sensor.openBmv080()){
    Serial.println("open失败");
    delay(500);
  }
  Serial.println("open成功");
  sensor.getBmv080ID(id);
  Serial.println("id is:" + String(id));
  Serial.println("ObstructionDetection" + String(sensor.getObstructionDetection()));
  if(sensor.setBmv080Mode(0))
    Serial.println("模式设置成功");
}

float pm1,pm2_5,pm10;

void loop() {
  if(sensor.getBmv080Data(&pm1,&pm2_5,&pm10)){
    Serial.print("pm1:");
    Serial.print(pm1);
    Serial.print("\t");
    Serial.print("pm2_5:");
    Serial.print(pm2_5);
    Serial.print("\t");
    Serial.print("pm10:");
    Serial.print(pm10);

    // if(sensor.ifObstructed()){
    //   Serial.print("Obstructed");
    // }

    Serial.println();
  }
  delay(100);
}
