#include "DFRobot_BMV080.h"

DFRobot_BMV080::DFRobot_BMV080(void) {
  // Constructor implementation 
}

//int DFRobot_BMV080::begin(void) 
//{
//  // Initialization code
//  // Assuming initialization is successful
//  return ERR_OK;
//}

int8_t DFRobot_BMV080::BMV080_write_16bit_cb(bmv080_sercom_handle_t sercom_handle, uint16_t header, const uint16_t* payload, uint16_t payload_length)
{
  DFRobot_BMV080_I2C *_pIIC = (DFRobot_BMV080_I2C *)sercom_handle;
  header = header << 1;
  if(_pIIC->writeReg(header, payload, payload_length) != 0){
    //Serial.println("writeReg failed");
    return E_BMV080_ERROR_HW_WRITE; 
  }

  return E_BMV080_OK;
}
int8_t DFRobot_BMV080::BMV080_read_16bit_cb(bmv080_sercom_handle_t sercom_handle, uint16_t header, uint16_t* payload, uint16_t payload_length)
{
  DFRobot_BMV080_I2C *_pIIC = (DFRobot_BMV080_I2C *)sercom_handle;
  header = header << 1;

  return _pIIC->readReg(header, payload, payload_length) == payload_length ? E_BMV080_OK : E_BMV080_ERROR_HW_READ;
}

int8_t DFRobot_BMV080::BMV080_delay_cb(uint32_t duration_in_ms)
{
  delay(duration_in_ms);

  return E_BMV080_OK;
}

uint8_t DFRobot_BMV080::openBmv080(void)
{
  bmv080_status_code_t status = bmv080_open(
        &_bmv080_handle_class, (bmv080_sercom_handle_t)this, (bmv080_callback_read_t)BMV080_read_16bit_cb,
        (bmv080_callback_write_t)BMV080_write_16bit_cb, (bmv080_callback_delay_t)BMV080_delay_cb);

  //Serial.println("status is:" + String(status));
  return status;
}

bool DFRobot_BMV080::getBmv080DV(uint16_t &major, uint16_t &minor, uint16_t &patch)
{
  char git_hash[12];
  int32_t num_commits_ahead = 0;

  bmv080_status_code_t bmv080_status = bmv080_get_driver_version(&major, &minor, &patch, git_hash, &num_commits_ahead);

  return (bmv080_status == E_BMV080_OK);
}

bool DFRobot_BMV080::getBmv080ID(char *id)
{
  bmv080_status_code_t bmv080_status = bmv080_get_sensor_id(_bmv080_handle_class, id);
  
  return (bmv080_status == E_BMV080_OK);
}

DFRobot_BMV080_I2C::DFRobot_BMV080_I2C()
{
  _pWire = &Wire;
  _deviceAddr = DFRobot_BMV080_I2C_ADDR;
}

int DFRobot_BMV080_I2C::begin(void) 
{
  _pWire->begin();
  _pWire->beginTransmission(_deviceAddr);
  if(_pWire == NULL)
  {
    Serial.println("_pWire == NULL 1");
  }
  return _pWire->endTransmission() == 0 ? ERR_OK : ERR_DATA_BUS; 
}

uint16_t sftk_byte_swap(uint16_t i)
{
  #if defined(__clang__) || defined(__GNUC__)
      return __builtin_bswap16(i);
  #else
      return (i << 8) | (i >> 8);
  #endif
}

uint8_t DFRobot_BMV080_I2C::writeReg(uint16_t reg, const uint16_t* pBuf, size_t size)
{ 
  uint16_t data16[size];
  for (size_t i = 0; i < size; i++)
      data16[i] = sftk_byte_swap(pBuf[i]);
  reg = sftk_byte_swap(reg);
  if(pBuf == NULL){
      DBG("pBuf ERROR!! : null pointer");
      return 1;
  }

  _pWire->beginTransmission(_deviceAddr);
  _pWire->write((uint8_t*)&reg, sizeof(reg));
  _pWire->write((const uint8_t*)data16, size * sizeof(uint16_t));
  uint8_t status = _pWire->endTransmission();

  return status == 0 ? 0 : 1;
}

uint8_t DFRobot_BMV080_I2C::readReg(uint16_t reg, uint16_t* pBuf, size_t size)
{
  if (_pWire == NULL) {
      DBG("_pWire ERROR!! : null pointer");
      return 0;
  }

  reg = sftk_byte_swap(reg);
  _pWire->beginTransmission(_deviceAddr);
  _pWire->write((uint8_t*)&reg, sizeof(reg));  // 写入寄存器地址
  if (_pWire->endTransmission() != 0) {
      return 0; // 传输失败
  }

  size_t totalBytes = size * sizeof(uint16_t); // 需要读取的总字节数
  _pWire->requestFrom(_deviceAddr, (uint8_t)totalBytes); // 请求数据
  uint8_t* pData = reinterpret_cast<uint8_t*>(pBuf);
  for (size_t i = 0; i < totalBytes; ++i) {
      pData[i] = _pWire->read();
  }
  for (size_t i = 0; i < size; ++i) {
      pBuf[i] = sftk_byte_swap(pBuf[i]);
  }
  
return totalBytes/2;
}


