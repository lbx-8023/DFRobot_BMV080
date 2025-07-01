#ifndef __DFROBOT_BMV080_H
#define __DFROBOT_BMV080_H

#include <Arduino.h>
#include "bmv080.h"
#include "bmv080_defs.h"
#include <Wire.h>
#include <SPI.h>

#define DBG(...) {Serial.print("[");Serial.print(__FUNCTION__); Serial.print("(): "); Serial.print(__LINE__); Serial.print(" ] "); Serial.println(__VA_ARGS__);}

#define DFRobot_BMV080_I2C_ADDR 0x57

class DFRobot_BMV080 {
public:
  #define ERR_OK             0      ///< 无错误
  #define ERR_DATA_BUS      -1      ///< 数据总线错误
  #define ERR_DATA_READ     -2      ///< 数据总线错误
  #define ERR_IC_VERSION    -3     ///< 芯片版本不匹配
public: 
  DFRobot_BMV080(void);

  int begin(void);

  static int8_t BMV080_write_16bit_cb(bmv080_sercom_handle_t, uint16_t, const uint16_t*, uint16_t);
  static int8_t BMV080_read_16bit_cb(bmv080_sercom_handle_t, uint16_t, uint16_t*, uint16_t);
  static int8_t BMV080_delay_cb(uint32_t);
  uint8_t openBmv080(void);
  bool getBmv080DV(uint16_t &major, uint16_t &minor, uint16_t &patch);
  bool getBmv080ID(char *id);

  bmv080_handle_t _bmv080_handle_class = NULL;
  
  
};

class DFRobot_BMV080_I2C:public DFRobot_BMV080{
public:
  DFRobot_BMV080_I2C();
  int begin(void);
  
  uint8_t writeReg(uint16_t reg, const uint16_t* pBuf, size_t size);
  uint8_t readReg(uint16_t reg, uint16_t* pBuf, size_t size);

  TwoWire *_pWire; 
  uint8_t _deviceAddr;
};



#endif