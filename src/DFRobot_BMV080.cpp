#include "DFRobot_BMV080.h"

DFRobot_BMV080::DFRobot_BMV080(void) {
  // Constructor implementation 
}

int8_t DFRobot_BMV080::BMV080_write_16bit_cb(bmv080_sercom_handle_t sercom_handle, uint16_t header, const uint16_t* payload, uint16_t payload_length)
{
  DFRobot_BMV080 *_pDev = (DFRobot_BMV080 *)sercom_handle;
  header = header << 1;
  if(_pDev->writeReg(header, payload, payload_length) != 0){
    DBG("writeReg failed");
    return E_BMV080_ERROR_HW_WRITE; 
  }

  return E_BMV080_OK;
}

int8_t DFRobot_BMV080::BMV080_read_16bit_cb(bmv080_sercom_handle_t sercom_handle, uint16_t header, uint16_t* payload, uint16_t payload_length)
{
  DFRobot_BMV080 *_pDev = (DFRobot_BMV080 *)sercom_handle;
  header = header << 1;

  return _pDev->readReg(header, payload, payload_length) == payload_length ? E_BMV080_OK : E_BMV080_ERROR_HW_READ;
}

int8_t DFRobot_BMV080::BMV080_delay_cb(uint32_t duration_in_ms)
{
  delay(duration_in_ms);

  return E_BMV080_OK;
}

uint32_t DFRobot_BMV080::BMV080_delay_cycling_cb(void)
{
  return millis();
}

void DFRobot_BMV080::getBmv080Data_cb(bmv080_output_t bmv080_output, void *cb_parameters)
{
  *(bmv080_output_t*)cb_parameters = bmv080_output;
  //((DFRobot_BMV080 *)cb_parameters)->get_bmv080Data(bmv080_output);
}

uint8_t DFRobot_BMV080::openBmv080(void)
{
  bmv080_status_code_t bmv080_status = bmv080_open(
        &_bmv080_handle_class, (bmv080_sercom_handle_t)this, (bmv080_callback_read_t)BMV080_read_16bit_cb,
        (bmv080_callback_write_t)BMV080_write_16bit_cb, (bmv080_callback_delay_t)BMV080_delay_cb);
  DBG("status is:" + String(bmv080_status));

  return bmv080_status;
}

bool DFRobot_BMV080::closeBmv080(void)
{
  bmv080_status_code_t bmv080_status = bmv080_close(&_bmv080_handle_class);

  return bmv080_status == E_BMV080_OK;  
}

bool DFRobot_BMV080::stopBmv080(void)
{
  bmv080_status_code_t bmv080_status = bmv080_stop_measurement(_bmv080_handle_class);

  return bmv080_status == E_BMV080_OK;
}

bool DFRobot_BMV080::resetBmv080(void)
{
  bmv080_status_code_t bmv080_status = bmv080_reset(&_bmv080_handle_class);

  if(bmv080_status != E_BMV080_OK) {
    DBG("bmv080_reset failed, status is:" + String(bmv080_status));
    return false;
  }
  return bmv080_status == E_BMV080_OK;
}

bool DFRobot_BMV080::getBmv080ID(char *id)
{
  bmv080_status_code_t bmv080_status = bmv080_get_sensor_id(_bmv080_handle_class, id);
  
  return (bmv080_status == E_BMV080_OK);
}

bool DFRobot_BMV080::getBmv080DV(uint16_t &major, uint16_t &minor, uint16_t &patch)
{
  char git_hash[12];
  int32_t num_commits_ahead = 0;

  bmv080_status_code_t bmv080_status = bmv080_get_driver_version(&major, &minor, &patch, git_hash, &num_commits_ahead);

  return (bmv080_status == E_BMV080_OK);
}

bool DFRobot_BMV080::get_bmv080Data(bmv080_output_t bmv080_output)
{
  _bmv080Data = bmv080_output;
  _bmv080DataOK = true;
  DBG("I am here get_bmv080Data");
  return E_BMV080_OK;
}

bool DFRobot_BMV080::getBmv080Data(float *PM1, float *PM2_5, float *PM10)
{
  _bmv080DataOK = false;

  if(_bmv080_handle_class == NULL) {
    Serial.println("bmv080_handle_class is null 1");
    return false;
  }
  bmv080_status_code_t bmv080_status =
    bmv080_serve_interrupt(_bmv080_handle_class, (bmv080_callback_data_ready_t)getBmv080Data_cb, &_bmv080Data);

  if(bmv080_status == E_BMV080_OK) {
    _bmv080DataOK = true;
  } else {
    DBG("bmv080_serve_interrupt failed, status is:" + String(bmv080_status));
    return false;
  }

  return _bmv080DataOK;
}

bool DFRobot_BMV080::setBmv080Mode(uint8_t mode)
{
  bmv080_status_code_t bmv080_status = E_BMV080_ERROR_PARAM_INVALID_VALUE;

  if(mode == DFRobot_BMV080_MODE_CONTINUOUS) {
    bmv080_status = bmv080_start_continuous_measurement(_bmv080_handle_class);
  }else if(mode == DFRobot_BMV080_MODE_DUTY_CYCLE) {
    bmv080_duty_cycling_mode_t bmv080_duty_cycling_mode = E_BMV080_DUTY_CYCLING_MODE_0;
    bmv080_status = bmv080_start_duty_cycling_measurement(_bmv080_handle_class, (bmv080_callback_tick_t)BMV080_delay_cycling_cb, bmv080_duty_cycling_mode);
  } else {
    DBG("setBmv080Mode failed, mode is invalid");
    return false;
  }

  return (bmv080_status == E_BMV080_OK);
}

bool DFRobot_BMV080::setIntegrationTime(float integration_time)
{
  bmv080_status_code_t bmv080_status = bmv080_set_parameter(_bmv080_handle_class, "integration_time", (void *)&integration_time);

  if(bmv080_status != E_BMV080_OK) {
    DBG("setIntegrationTime failed, status is:" + String(bmv080_status));
    return false;
  }

  return true;
}

float DFRobot_BMV080::getIntegrationTime(void)
{
  float integration_time = 0.0f;

  bmv080_status_code_t bmv080_status = bmv080_set_parameter(_bmv080_handle_class, "integration_time", (void *)&integration_time);

  if(bmv080_status != E_BMV080_OK) {
    DBG("setIntegrationTime failed, status is:" + String(bmv080_status));
    return false;
  }

  return integration_time;
}

bool DFRobot_BMV080::setDutyCyclingPeriod(uint16_t duty_cycling_period)
{
  bmv080_status_code_t bmv080_status =
      bmv080_set_parameter(_bmv080_handle_class, "duty_cycling_period", (void *)&duty_cycling_period);

  return (bmv080_status == E_BMV080_OK);
}

bool DFRobot_BMV080::getDutyCyclingPeriod(uint16_t *duty_cycling_period)
{
  bmv080_status_code_t bmv080_status = bmv080_get_parameter(_bmv080_handle_class, "duty_cycling_period", (void *)&duty_cycling_period);

  return (bmv080_status == E_BMV080_OK ? duty_cycling_period : 0);
}

bool DFRobot_BMV080::setObstructionDetection(bool obstructed)
{
  bmv080_status_code_t bmv080_status = bmv080_set_parameter(_bmv080_handle_class, "do_obstruction_detection", (void *)&obstructed);

  return (bmv080_status == E_BMV080_OK);
}

bool DFRobot_BMV080::getObstructionDetection(void)
{
  bool obstructed = false;
  bmv080_status_code_t bmv080_status = bmv080_get_parameter(_bmv080_handle_class, "do_obstruction_detection", (void *)&obstructed);

  return bmv080_status == E_BMV080_OK ? obstructed : false;
}

bool DFRobot_BMV080::setDoVibrationFiltering(bool do_vibration_filtering)
{
  bmv080_status_code_t bmv080_status =
      bmv080_set_parameter(_bmv080_handle_class, "do_vibration_filtering", (void *)&do_vibration_filtering);

  return (bmv080_status == E_BMV080_OK);
}

bool DFRobot_BMV080::getDoVibrationFiltering(void)
{
  bool do_vibration_filtering = false;
  bmv080_status_code_t bmv080_status = bmv080_get_parameter(_bmv080_handle_class, "do_vibration_filtering", (void *)&do_vibration_filtering);

  return (bmv080_status == E_BMV080_OK ? do_vibration_filtering : false);
}

bool DFRobot_BMV080::setMeasurementAlgorithm(uint8_t measurement_algorithm)
{
  bmv080_measurement_algorithm_t bmv080_measurement_algorithm = (bmv080_measurement_algorithm_t)measurement_algorithm;
  bmv080_status_code_t bmv080_status = bmv080_set_parameter(_bmv080_handle_class, "measurement_algorithm", (void *)&bmv080_measurement_algorithm);

  return (bmv080_status == E_BMV080_OK);
}
uint8_t DFRobot_BMV080::getMeasurementAlgorithm(void)
{
  bmv080_measurement_algorithm_t measurement_algorithm;
  bmv080_status_code_t bmv080_status = bmv080_get_parameter(_bmv080_handle_class, "measurement_algorithm", (void *)&measurement_algorithm);

  return (bmv080_status == E_BMV080_OK ? (uint8_t)measurement_algorithm : 0);
}

DFRobot_BMV080_I2C::DFRobot_BMV080_I2C(TwoWire * Wire)
{
  _pWire = Wire;
  _deviceAddr = DFRobot_BMV080_I2C_ADDR;
}

int DFRobot_BMV080_I2C::begin(void) 
{
  _pWire->begin();
  _pWire->beginTransmission(_deviceAddr);
  if(_pWire == NULL)
  {
    Serial.println("_pWire == NULL");
    return ERR_DATA_BUS;
  }
  return _pWire->endTransmission() == 0 ? ERR_OK : ERR_DATA_BUS; 
}

uint16_t DFRobot_swap16(uint16_t i)
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
      data16[i] = DFRobot_swap16(pBuf[i]);
  reg = DFRobot_swap16(reg);

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

  reg = DFRobot_swap16(reg); // Ensure reg is in big-endian format
  _pWire->beginTransmission(_deviceAddr);
  _pWire->write((uint8_t*)&reg, sizeof(reg));  
  if (_pWire->endTransmission() != 0) {
      return 0; 
  }

  size_t totalBytes = size * sizeof(uint16_t); 
  _pWire->requestFrom(_deviceAddr, (uint8_t)totalBytes); 
  uint8_t* pData = reinterpret_cast<uint8_t*>(pBuf);
  for (size_t i = 0; i < totalBytes; ++i) {
      pData[i] = _pWire->read();
  }
  for (size_t i = 0; i < size; ++i) {
      pBuf[i] = DFRobot_swap16(pBuf[i]); // Convert back to host byte order
  }

  return totalBytes/2;
}

DFRobot_BMV080_SPI::DFRobot_BMV080_SPI(SPIClass *spi, uint8_t csPin)
{
  _pSpi = spi;
  _csPin = csPin;
}

int DFRobot_BMV080_SPI::begin(void) 
{
  if (_pSpi == NULL) {
    DBG("_pSpi ERROR!! : null pointer");
    return ERR_DATA_BUS;
  }
  pinMode(_csPin, OUTPUT);
  _pSpi->begin();

  return ERR_OK; 
}

uint8_t DFRobot_BMV080_SPI::writeReg(uint16_t reg, const uint16_t* pBuf, size_t size)
{
  if (_pSpi == NULL) {
    DBG("pBuf ERROR!! : null pointer");
    return 1;
  }
  Serial.println("DFRobot_BMV080_SPI::writeReg");
  uint16_t * _pBuf = (uint16_t *)pBuf;
  _pSpi->beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
  digitalWrite(_csPin, LOW);
  _pSpi->transfer16(reg);
  
  while(size--) {
    _pSpi->transfer16(*_pBuf);
    _pBuf ++;
  }
  
  digitalWrite(_csPin, HIGH);
  _pSpi->endTransaction();

  return 0;
}

uint8_t DFRobot_BMV080_SPI::readReg(uint16_t reg, uint16_t* pBuf, size_t size) 
{
  if (_pSpi == NULL) {
    DBG("_pSpi ERROR!! : null pointer");
    return 0;
  }

  if (pBuf == NULL) {
    DBG("pBuf ERROR!! : null pointer");
    return 0;
  }
  uint16_t * _pBuf = (uint16_t *)pBuf;
  size_t count = 0;

  _pSpi->beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
  digitalWrite(_csPin, LOW);

  _pSpi->transfer16(reg);

  while(size--) {
    *_pBuf = _pSpi->transfer16(0x00);
    _pBuf++;
    count++;
  }

   digitalWrite(_csPin, HIGH);
  _pSpi->endTransaction();
 
  return count;
}