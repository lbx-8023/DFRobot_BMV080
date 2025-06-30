#include "DFRobot_BMV080.h"

DFRobot_BMV080::DFRobot_BMV080(void) {
  // Constructor implementation
}

int DFRobot_BMV080::begin(void) 
{
  // Initialization code
  // Assuming initialization is successful
  return ERR_OK;
}







DFRobot_BMV080_I2C::DFRobot_BMV080_I2C(TwoWire *pWire)
{
  _pWire = pWire;
  _deviceAddr = DFRobot_BMV080_I2C_ADDR;
}

int DFRobot_BMV080_I2C::begin(void) 
{
  // Initialize the I2C communication
  _pWire->begin();
  _pWire->beginTransmission(_deviceAddr);
  return _pWire->endTransmission() == 0 ? ERR_OK : ERR_DATA_BUS; 
}

void DFRobot_BMV080_I2C::writeReg(uint8_t reg, const uint16_t* pBuf, size_t size)
{
  if(pBuf == NULL){
	  DBG("pBuf ERROR!! : null pointer");
  }
  uint8_t * _pBuf = (uint8_t *)pBuf;
  _pWire->beginTransmission(_deviceAddr);
  _pWire->write(&reg, 1);

  for(uint16_t i = 0; i < size; i++){
    _pWire->write(_pBuf[i]);
  }
  _pWire->endTransmission();
}

uint8_t DFRobot_BMV080_I2C::readReg(uint8_t reg, void* pBuf, size_t size)
{
  size_t nRead = 0;
  if(pBuf == NULL){
    DBG("pBuf ERROR!! : null pointer");
  }
  uint8_t * _pBuf = (uint8_t *)pBuf;
  _pWire->beginTransmission(_deviceAddr);
  _pWire->write(&reg, 1);
  if( _pWire->endTransmission() != 0){
      return 0;
  }
 nRead = _pWire->requestFrom(_deviceAddr, (uint8_t) size);
  for(uint16_t i = 0; i < size; i++){
    _pBuf[i] = _pWire->read();
  }
  return nRead;
}

void DFRobot_BMV080_I2C::BMV080_write_16bit_cb(bmv080_sercom_handle_t sercom_handle, uint16_t header, const uint16_t* payload, uint16_t payload_length)
{
  DFRobot_BMV080_I2C *_pWire = (DFRobot_BMV080_I2C *)sercom_handle;
  _pWire->writeReg(header, payload, payload_length); 
}


uint8_t DFRobot_BMV080_I2C::BMV080_read_16bit_cb(bmv080_sercom_handle_t sercom_handle, uint16_t header, uint16_t* payload, uint16_t payload_length)
{
  size_t nRead = 0;
  DFRobot_BMV080_I2C *_pWire = (DFRobot_BMV080_I2C *)sercom_handle;
  nRead = _pWire->readReg(header, payload, payload_length);
  if(nRead == payload_length)
    return E_BMV080_OK;
  else
    return E_BMV080_ERROR_HW_READ;
}
uint8_t DFRobot_BMV080_I2C::BMV080_delay_cb(uint32_t duration_in_ms)
{
  delay(duration_in_ms);

  return E_BMV080_OK;
}

uint8_t DFRobot_BMV080_I2C::openBmv080(void)
{
  bmv080_status_code_t status = bmv080_open(
        &_bmv080_handle_class, (bmv080_sercom_handle_t)_pWire, (bmv080_callback_read_t)BMV080_read_16bit_cb,
        (bmv080_callback_write_t)BMV080_write_16bit_cb, (bmv080_callback_delay_t)BMV080_delay_cb);
        
  return status;
}