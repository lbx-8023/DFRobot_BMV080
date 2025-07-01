
#include "DFRobot_BMV080.h"

SET_LOOP_TASK_STACK_SIZE(60 * 1024); // 60KB

DFRobot_BMV080_I2C sensor;


void setup() {
  // put your setup code here, to run once:
  char id[13];
  Serial.begin(115200);
  delay(500);
//  test();
  while(sensor.begin() != 0){
    Serial.println("初始化芯片失败，请确认芯片连接是否正确");
    delay(1000);
  }
  
  //sensor.driverVersion(major, minor, patch);
  delay(500);
  Serial.println("初始化芯片成功");
  while(sensor.openBmv080())
  {
    Serial.println("open失败");
    delay(1000);
  }
  Serial.println("open成功");
  delay(500);
//  if(sensor.resetBmv080())Serial.println("复位失败");
  sensor.getBmv080ID(id);
  
}

void loop() {
  // put your main code here, to run repeatedly:

}
