# DFRobot_BMV080
DFRobot_BMV080 is a arduino library designed to drive the new Bosch air quality measurement chip BMV080. <br>               BMV080 is currently the smallest PM2.5 chip in the world and it uses laser for measurement. <br>                            This library provides two communication methods: IIC and SPI, as well as some basic routines.Currently, it only supports the ESP32 platform.<br>

## Functionality description
This sensor can measure PM1/PM2.5/PM10 air indicators and can be used as an IIC and SPI slave device. This library can obtain these sensor data through just a few simple functions and it provides IIC and SPI interfaces.

### Note
Since Bosch currently only provides the pre-compiled library of BMV080, it can only support the platforms that are supported by the Bosch library. The SDK used for this update is from Bosch. You can download and update it from their official website. [Click here to jump to the next page](https://www.bosch-sensortec.com/products/environmental-sensors/particulate-matter-sensor/bmv080/#documents).<br>
This library has reserved the SDK of version Bosch bmv080-sdk-v11-1-0. You can directly use it or replace it with the latest SDK of your own choice.

## BMV080 Sodtware
You can refer to the wiki for detailed usage instructions.<br>

### BMV080 WIKI
* [点击此处查看中文wiki](https://github.com/lbx-8023/DFRobot_BMV080/wiki/%E4%B8%AD%E6%96%87-WIKI)<br>
* [Click here to view the English WIKI](https://github.com/lbx-8023/DFRobot_BMV080/wiki/English-WIKI)

