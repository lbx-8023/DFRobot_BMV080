
#include "DFRobot_BMV080.h"

SET_LOOP_TASK_STACK_SIZE(60 * 1024); // 60KB

DFRobot_BMV080_I2C sensor(&Wire);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  while(sensor.begin() != 0){
    Serial.println("初始化芯片失败，请确认芯片连接是否正确");
    delay(1000);
  }
  Serial.println("初始化芯片成功");
  while(sensor.openBmv080() != 0){
    Serial.println("初始化芯片失败，请确认芯片连接是否正确");
    delay(1000);
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}
