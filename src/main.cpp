#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager
#include <ESP8266WebServer.h>
#include <DNSServer.h>
//#include <ResetDetector.h>  //prefer this
#include <DoubleResetDetector.h>

#define PORT 80
ESP8266WebServer server(PORT); //listening server

#define DRD_TIMEOUT 2
// RTC Memory Address for the DoubleResetDetector to use
#define DRD_ADDRESS 0
DoubleResetDetector drd(DRD_TIMEOUT, DRD_ADDRESS); //this is not nearly as reliable as ResetDetector

const int PIN_LED = 2;

void handleRoot();              // function prototypes for HTTP handlers
void handleON();
void handleOFF();
void handle404();

//const char MAIN_page[] PROGMEM = R"=====(<html><body>Ciclk to turn <a href="\on">LED ON</a><br>Ciclk to turn <a href="\off">LED OFF</a><br></body></html>)=====";
//this stores in the flash as static
const char MAIN_page[] = "<html><body>Click to turn <a href=\"on\">LED ON</a><br>Ciclk to turn <a href=\"off\">LED OFF</a><br></body></html>";

void blinkLED (int delayMS, int count, const int pinLED) {
    pinMode(pinLED, OUTPUT);
    for (int i = 0; i < count; i++) {            
            digitalWrite(pinLED, LOW);
            delay(delayMS);
            digitalWrite(pinLED, HIGH);
    }
}

void handleRoot() {
    Serial.println("/");
    blinkLED(200, 5, PIN_LED);
    String s = MAIN_page;
    server.send(200, "text/html", s);
}


void handleON() { 
    Serial.println("LED on page");
    blinkLED(500, 2, PIN_LED);
    server.send(200, "text/html", "<b>ON action</b>");
}
 
void handleOFF() { 
    Serial.println("LED off page");
    blinkLED(10, 2, PIN_LED);
    server.send(200, "text/html", "<b>OFF action</b>");
}

void handle404(){
    blinkLED(100, 10, PIN_LED);
    server.send(404, "text/plain", "404: Not found");
}

void setup() {

    //int resetCounter = ResetDetector::execute(2000); //capture number of resets during 2 seconsd

    Serial.begin(115200);

    

    //WiFiManager
    //Local intialization. Once its business is done, there is no need to keep it around
    WiFiManager wifiManager;
    //default is true print a lot of debug info under *WM
    wifiManager.setDebugOutput(false);
    //WiFi.printDiag(Serial);

    //reset Wifi settings or initial setup
    if (drd.detectDoubleReset()) {
        wifiManager.resetSettings();
        //twice to indicate in setup mode
        blinkLED(500, 2, PIN_LED);
        Serial.println("wifi: entering setup mode");
        //set the AP's name for you to connect and configure
        wifiManager.autoConnect("ap");
        //wifiManager.startConfigPortal(); //I don't know what this actually do
        Serial.println("wifi: entering Config Portal");
    }
    else { //enter running mode
        Serial.println("wifi: press reset twice to setup Wifi");
    }

    if (WiFi.status()==WL_CONNECTED) {
        Serial.print("wifi: local ip-> ");
        Serial.println(WiFi.localIP());
        server.on("/", handleRoot);
        server.on("/on", handleON);
        server.on("/off", handleOFF);
        server.onNotFound(handle404);
        server.begin();
        Serial.println("http: HTTP server started");
    }
}

void loop() {
    server.handleClient();
}