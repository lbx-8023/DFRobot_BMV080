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
  
  typedef enum{
    DFRobot_BMV080_MODE_CONTINUOUS = 0,
    DFRobot_BMV080_MODE_DUTY_CYCLE = 1
  }eBmv080Mode_t;
public: 
  DFRobot_BMV080(void);

  int begin(void);

  static int8_t BMV080_write_16bit_cb(bmv080_sercom_handle_t, uint16_t, const uint16_t*, uint16_t);
  static int8_t BMV080_read_16bit_cb(bmv080_sercom_handle_t, uint16_t, uint16_t*, uint16_t);
  static void getBmv080Data_cb(bmv080_output_t bmv080_output, void *cb_parameters);
  static int8_t BMV080_delay_cb(uint32_t);
  static uint32_t BMV080_delay_cycling_cb(void);

  uint8_t openBmv080(void);
  bool closeBmv080(void);
  bool resetBmv080(void);
  bool getBmv080DV(uint16_t &major, uint16_t &minor, uint16_t &patch);
  bool getBmv080ID(char *id);
  bool getBmv080Data(float *PM1, float *PM2_5, float *PM10);
  bool get_bmv080Data(bmv080_output_t bmv080_output);
  bool setBmv080Mode(uint8_t mode);
  bool stopBmv080(void);

  bool setIntegrationTime(float integration_time);
  float getIntegrationTime(void);
  bool setDutyCyclingPeriod(uint16_t duty_cycling_period);
  uint16_t getDutyCyclingPeriod(void);
  bool setObstructionDetection(bool obstructed);
  bool getObstructionDetection(void);
  bool setDoVibrationFiltering(bool do_vibration_filtering);
  bool getDoVibrationFiltering(void);
  bool setMeasurementAlgorithm(uint8_t measurement_algorithm);
  uint8_t getMeasurementAlgorithm(void);

private:
  
  bmv080_handle_t _bmv080_handle_class = NULL;
  bmv080_output_t _bmv080Data;
  bool _bmv080DataOK = false;
};

class DFRobot_BMV080_I2C:public DFRobot_BMV080{
public:
  DFRobot_BMV080_I2C(TwoWire *Wire);
  int begin(void);
  
  uint8_t writeReg(uint16_t reg, const uint16_t* pBuf, size_t size);
  uint8_t readReg(uint16_t reg, uint16_t* pBuf, size_t size);

  TwoWire *_pWire; 
  uint8_t _deviceAddr;
};



#endif