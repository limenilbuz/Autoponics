## AUTOPONICS
Automated Hydroponics

![](https://github.com/limenilbuz/Autoponics/blob/main/images/autoponics_logo_crop.png)

![](https://github.com/limenilbuz/Autoponics/blob/main/images/autoponics_gif.gif)

## Overview
Autoponics is an automated hydroponics system. It provides a modular device that can be attached to an already existing hydroponics system. The device will automatically adjust the pH and EC levels of the hydroponic’s solution

The following parts were used
1. ESP32 WROOM Devkit v1
1. WT32-SC01
1. Gravity: Analog Electrical Conductivity Sensor PRO (K=1)
1. Gravity: Analog Industrial pH Sensor / Meter Pro Kit V2
1. Gravity: Non-contact Digital Liquid Level Sensor for Arduino
1. 5V One Channel Relay Module 

Motor Assembly
![](https://github.com/limenilbuz/Autoponics/blob/main/images/motor_assembly.jpg)

Every part is encased in a 3d printed case that was created through Autodesk Fusion 360

CAD
![](https://github.com/limenilbuz/Autoponics/blob/main/images/CAD_model.png)

CAD Render
![](https://github.com/limenilbuz/Autoponics/blob/main/images/CAD_model_render.png)

## Dependencies

FreeRTOS was used for the main code to create multithreading tasks. 

For the GUI code, the following libraries are used
1. https://github.com/Bodmer/TFT_eSPI
1. https://github.com/lvgl/lvgl

A separate github repo for the GUI can be found here
https://github.com/wjswogur56/Autoponics-GUI

## How To Use

The code can be used an ran by uploading it to a ESP32 through PlatformIO. Note that framework-espidf version 4.4.2 was used in testing.
The GUI code is separately uploaded to a WT32-SC01, and is connected to the main ESP32 through I2C using pin 18 (SDA) and 19 (SCL)

A simple schematic for wiring is as follows (needs improvement for clarity)
![](https://github.com/limenilbuz/Autoponics/blob/main/images/wiring.png)

## Future

The system is currenty tested on a single 5gallon bucket. In the future, we wish to scale the hydroponics system, to test Autoponics on a larger scale.

![](https://github.com/limenilbuz/Autoponics/blob/main/images/scale.png)
