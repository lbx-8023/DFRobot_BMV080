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
  //virtual void BMV080_write_16bit_cb(bmv080_sercom_handle_t sercom_handle, uint16_t header, const uint16_t* payload, uint16_t payload_length) = 0;
  //virtual uint8_t BMV080_read_16bit_cb(bmv080_sercom_handle_t sercom_handle, uint16_t header, uint16_t* payload, uint16_t payload_length) = 0;
  //virtual uint8_t BMV080_delay_cb(uint32_t duration_in_ms) = 0;
  //virtual uint8_t openBmv080(void);
  //bmv080_handle_t _bmv080_handle_class = NULL;
  //bmv080_output_t _bmv080Value;
  //bool openBmv080(void);
  //bool closeBmv080(void);
  //char *bmv080ID[13];
  //bool getBmv080ID(char *id);
  bmv080_handle_t _bmv080_handle_class = NULL;
  TwoWire *_pWire; ///< I2C interface pointer
  uint8_t _deviceAddr;
//protected:

/**
 * @fn writeReg
 * @brief 写寄存器函数，设计为虚函数，由派生类实现函数体
 * @param reg  寄存器地址 8bits
 * @param pBuf 要写入数据的存放缓存
 * @param size 要写入数据的长度
 * @return None
 */
  //virtual void writeReg(uint8_t reg, void* pBuf, size_t size);

/**
 * @fn readReg
 * @brief 读取寄存器函数，设计为虚函数，由派生类实现函数体
 * @param reg  寄存器地址 8bits
 * @param pBuf 要写入数据的存放缓存
 * @param size 要写入数据的长度
 * @return uint8_t类型,表示读寄存器的返回
 * @retval xx xxxxx
 */
  //virtual uint8_t readReg(uint8_t reg, void* pBuf, size_t size);
};

class DFRobot_BMV080_I2C:public DFRobot_BMV080{
public:
  DFRobot_BMV080_I2C(TwoWire *pWire=&Wire);
  int begin(void);
  uint8_t openBmv080(void);
protected:
  void writeReg(uint8_t reg, const uint16_t* pBuf, size_t size);
  uint8_t readReg(uint8_t reg, void* pBuf, size_t size);

  static void BMV080_write_16bit_cb(bmv080_sercom_handle_t, uint16_t, const uint16_t*, uint16_t);
  static uint8_t BMV080_read_16bit_cb(bmv080_sercom_handle_t, uint16_t, uint16_t*, uint16_t);
  static uint8_t BMV080_delay_cb(uint32_t);
  
};



#endif