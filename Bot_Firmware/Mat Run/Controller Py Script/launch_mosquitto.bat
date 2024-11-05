@echo off
set "userProfile=%USERPROFILE%"
title MQTT Server
cd "%userProfile%\Meritus-CVPRO-Windows\Meritus-CVPRO-main\Environment_Setup"
"C:\Program Files\mosquitto\mosquitto.exe" -v -c mqtt_conf.conf
