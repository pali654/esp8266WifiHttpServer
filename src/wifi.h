//this sets up Wifi Manager
#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager
#include <ESP8266WebServer.h>
#include <DNSServer.h>

void setupWifi(int resetCounter) {
    //WiFiManager
    //Local intialization. Once its business is done, there is no need to keep it around
    WiFiManager wifiManager;
    //default is true print a lot of debug info under *WM
    wifiManager.setDebugOutput(false);
    //WiFi.printDiag(Serial);

    //reset Wifi settings or initial setup
    //only when reset buttons pressed 2 times or more
    if (resetCounter >= 2) {
        wifiManager.resetSettings();
        pinMode(2, OUTPUT);
        digitalWrite(2, LOW); //can't get util function to show just turn the LED on
        Serial.println("wifi: entering setup mode");
        //set the AP's name for you to connect and configure
        wifiManager.autoConnect("ap");
        //wifiManager.startConfigPortal(); //I don't know what this actually do
        Serial.println("wifi: entering Config Portal");
    }
    else { //enter running mode
        Serial.println("wifi: press reset twice to setup Wifi");
    }
}