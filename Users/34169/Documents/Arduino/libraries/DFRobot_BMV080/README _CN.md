  /**
   * @fn get_bmv080Data
   * @brief 将 BMV080 的数据赋值给变量 bmv080_output_t
   * @param bmv080_output: BMV080传感器数据的输出结构体
   * @note 此功能是在回调函数“getBmv080Data_cb”中被调用的
   * @return 1 成功
   * @return 0 失败
   */
  bool get_bmv080Data(bmv080_output_t bmv080_output);

  /**
   * @fn setBmv080Mode
   * @brief 设置 BMV080 传感器的模式
   * @param mode: 设置的模式可为：DFRobot_BMV080_MODE_CONTINUOUS 或 DFRobot_BMV080_MODE_DUTY_CYCLE
   *              DFRobot_BMV080_MODE_CONTINUOUS: 传感器持续进行测量
   *              DFRobot_BMV080_MODE_DUTY_CYCLE: 传感器会按照指定的时间间隔进行测量
   * @return 1 成功
   * @return 0 失败
   */
  bool setBmv080Mode(uint8_t mode);

  /**
   * @fn stopBmv080
   * @brief 停止颗粒物测量。
   * @pre 必须在数据采集周期结束时调用以确保传感器单元准备好下一个测量周期。
   * @return 1 成功
   * @return 0 错误 
   */
  bool stopBmv080(void);

  /**
   * @fn setIntegrationTime
   * @brief 测量窗口时间。
   * @note 在占空循环模式下，该测量窗口也是传感器开启时间。
   * @param integration_time 测量积分时间，单位为毫秒（ms）。
   * @return 1 成功
   * @return 0 错误
   */
  bool setIntegrationTime(float integration_time);

  /**
   * @fn getIntegrationTime
   * @brief 获取当前积分时间。
   * @return 当前积分时间，单位为毫秒（ms）。
   */
  float getIntegrationTime(void);

  /**
   * @fn setDutyCyclingPeriod
   * @brief 设置占空循环周期。
   * @n 占空循环周期（积分时间和传感器关闭/休眠时间之和）。
   * @note 此值必须比积分时间至少大2秒。
   * @param duty_cycling_period 占空循环周期，单位为毫秒（ms）。
   * @return 1 成功
   * @return 0 错误
   */
  bool setDutyCyclingPeriod(uint16_t duty_cycling_period);

  /**
   * @fn getDutyCyclingPeriod
   * @brief 获取当前占空循环周期。
   * @param duty_cycling_period 当前占空循环周期，单位为毫秒（ms）。
   * @return 1 成功
   * @return 0 错误
   */
  bool getDutyCyclingPeriod(uint16_t *duty_cycling_period);

  /**
   * @fn setObstructionDetection
   * @brief 设置是否启用阻塞检测功能。
   * @param obstructed true 启用阻塞检测，false 禁用。
   * @return 1 成功
   * @return 0 错误
   */
  bool setObstructionDetection(bool obstructed);

  /**
   * @fn getObstructionDetection
   * @brief 获取阻塞检测功能是否启用状态。
   * @return 1 阻塞检测启用。
   * @return 0 阻塞检测禁用。
   */
  bool getObstructionDetection(void);

  /**
   * @fn setDoVibrationFiltering
   * @brief 启用或禁用振动过滤功能。
   * @param do_vibration_filtering 1 启用，0 禁用。
   * @return 1 成功
   * @return 0 错误
   */
  bool setDoVibrationFiltering(bool do_vibration_filtering);

  /**
   * @fn getDoVibrationFiltering
   * @brief 获取振动过滤功能的状态。
   * @return 1 振动过滤启用。
   * @return 0 振动过滤禁用。
   */
  bool getDoVibrationFiltering(void);

  /**
   * @fn setMeasurementAlgorithm
   * @brief 设置测量算法。
   * @param measurement_algorithm 使用的测量算法。
   *                              E_BMV080_MEASUREMENT_ALGORITHM_FAST_RESPONSE
   *                              E_BMV080_MEASUREMENT_ALGORITHM_BALANCED
   *                              E_BMV080_MEASUREMENT_ALGORITHM_HIGH_PRECISION
   * @return 1 成功
   * @return 0 错误
   */
  bool setMeasurementAlgorithm(uint8_t measurement_algorithm);

  /**
   * @fn getMeasurementAlgorithm
   * @brief 获取当前使用的测量算法。
   * @return 当前使用的测量算法。
   *         E_BMV080_MEASUREMENT_ALGORITHM_FAST_RESPONSE
   *         E_BMV080_MEASUREMENT_ALGORITHM_BALANCED
   *         E_BMV080_MEASUREMENT_ALGORITHM_HIGH_PRECISION
   */
  uint8_t getMeasurementAlgorithm(void);