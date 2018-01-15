Demonstration example of the Bluetooth Low Energy communication capability of the ESP32
This demonstration includes a example android application and example firmware 

NOTE:  The continuation for development of this project is now here: https://github.com/CalPlug/Espressif_ESP32-Scale

Coding Development by: Yanjie (Richard) Xu

Managed by: Dr. Michael Klopfer and Prof. G.P. Li

California Plug Load Research Center (CalPlug), California Institute of Telecommunications and Information Technology (Calit2), University of California, Irvine

Copyright (c) Regents of the University of California, 2017

***************************************************************************
***************************************************************************

To compile and run the code, you need to download an ESP32 arduino core and add it to your Arduino IDE.

You may go to the link below to download and install the arduino core

https://github.com/espressif/arduino-esp32
The arduino program also uses the dhp 11 library, you should download it before you compile.

The ADC port of esp32 are GPIO 32-39.
Since the ESP32 has an inaccurate ADC, you may use lower resistance pull-up resistor. In my demo code, I'm using 1 KΩ resister for both DHP11 and the photocell.

The Android BLE app is API23 (Android 6.0).

