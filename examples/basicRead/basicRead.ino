
#include "DFRobot_BMV080.h"
#include <string>

SET_LOOP_TASK_STACK_SIZE(60 * 1024); // 60KB

DFRobot_BMV080_I2C sensor(&Wire);


void setup() {
  // put your setup code here, to run once:
  char id[13];
  Serial.begin(115200);
  delay(500);

  while(sensor.begin() != 0){
    Serial.println("初始化芯片失败，请确认芯片连接是否正确");
    delay(1000);
  }
  
  delay(500);
  Serial.println("初始化芯片成功");
  while(sensor.openBmv080())
  {
    Serial.println("open失败");
    delay(1000);
  }
  Serial.println("open成功");
  delay(100);
  sensor.getBmv080ID(id);
  Serial.println("id is:" + String(id));
  delay(100);
  if(sensor.setBmv080Mode(0))
    Serial.println("模式设置成功");
}

float pm1,pm2_5,pm10;

void loop() {
  // put your main code here, to run repeatedly:
  if(sensor.getBmv080Data(&pm1,&pm2_5,&pm10))
  {
    Serial.println("读取成功");
    Serial.println("pm1:");
    Serial.println(pm1);
    Serial.println("pm2_5:");
    Serial.println(pm2_5);
    Serial.println("pm10:");
    Serial.println(pm10);
  }
  delay(1000);
  //Serial.println("读取失败");
}
