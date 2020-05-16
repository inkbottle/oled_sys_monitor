# OLED System Monitor

## introduction

This little is suite for waveshare 0.91inch OLED Module, details here: [link](http://www.waveshare.net/wiki/0.91inch_OLED_Module).

It's used to monitor the Raspberry Pi CPU load and temperature. 

registered as start up systemd service.

Tested on Raspberry Pi 4B which is running system `raspbian-buster`.

## system configure
due to using library wiringPi. change boot configure as below:

	vim /boot/config.txt

find `dtparam=i2c_arm=on`, change to `dtparam=i2c_arm=on,i2c_arm_baudrate=400000`

reboot

## installation

cp compiled exe files to `/sbin/`

cp `oled-monitor-serviced.service` to `/usr/lib/systemd/system`

config service:

    systemctl daemon-reload
    systemctl enable oled-monitor-serviced.service
    systemctl start oled-monitor-serviced.service

