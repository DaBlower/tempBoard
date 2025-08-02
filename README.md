# TempBoard
![](https://hackatime-badge.hackclub.com/U092DB4LGMP/tempBoard) <br/>
A PCB that shows temperature and relative humidity on a 128x64px OLED screen using the SHTC3 sensor

## Features
- An SHTC3 temperature and relative humidity sensor gets data about temperature and relative humidity
- OLED display that shows the data
- Indicator LEDs that light up when the temperature or humidity gets extreme
- A XIAO RP2040 microcontroller

## Files
To program the XIAO RP2040, follow [this guide](https://wiki.seeedstudio.com/XIAO-RP2040-with-Arduino/) and open [this file](/firmware/tempBoard/tempBoard.ino) in the Arduino IDE.

## Schematic
<img src="/img/schema.png" alt="Schematic Image" width="600"/>

## PCB
<img src="/img/frontPCB.png" alt="Front PCB Image" width="600"/>
<img src="/img/backPCB.png" alt="Back PCB Image" width="600"/>
<img src="/img/3d_back.png" alt="Front 3D PCB Image" width="300"/>
<img src="/img/3d_front.png" alt="Back 3D PCB Image" width="600"/>

## BOM
* 1 100nf capacitor
* 2 4.7 k ohm resistors
* 2 220 ohm resistors
* 1 Blue LED
* 1 Red LED
* 1 SHTC3 Temperature and humidity sensor
* 1 128x64 px OLED display
* 1 XIAO RP2040

made by ```@obob``` on the Hack Club Slack for the Hermes YSWS! (hermes.hackclub.com)