# Spot ESP32 Controller App
Controller app for the Spot Micro ESP32 model using Cordova development framework.

## Credits
Spot ESP32 Controller app is inspired by Maarteen Weyn's corodova app implementation to test the Kinematic implementation for the Spot Micro ESP32 model. Lot of code is in this app is derived from Maarteen's work.
https://github.com/maartenweyn/SpotMicro_ESP32

## TODO: app is still under development

## Features:
- [] Linkup to robot over Bluetooth BLE connection
- [] Virtual joystick based navigation
- [] Commands to robot
- [] Camera view connect
- [] Automated tasks
- [] Firmware Updates

## App
Make sure to turn on Bluetooth and Location, and allow required permissions.
Only been tested on Android
[![App]../assets/app_ss.jpeg)]

## Build Procedure
Cordova development framework setup guide
https://cordova.apache.org/docs/en/latest/guide/overview/index.html

Compile
(if you have installed Cordova using Node.js framework, run the following from Node.js command prompt)

```console
cordova platform add android
cordova platform add ios

brew install imagemagick
sudo npm install -g cordova-splash cordova-icon
cordova-icon
cordova-platform

cordova build ios
cordova build android
```

## Run on Android
```console
adb run android
```
## Debug log monitor on Android
```console
adb logcat -e CONSOLE
```
## iOS: TODO