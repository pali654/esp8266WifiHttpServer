#include <ResetDetector.h>  //prefer this works more reliable than doubleResetDetector
//#include <DoubleResetDetector.h>

#include <wifi.h>  //refactored out WiFiManager
#include <utils.h> //utilities functions

// Dependency Graph
// |-- <ResetDetector> 1.0.3
// |   |-- <EEPROM> 1.0
// |-- <DNSServer> 1.1.1
// |   |-- <ESP8266WiFi> 1.0
// |-- <ESP8266WebServer> 1.0
// |   |-- <ESP8266WiFi> 1.0
// |-- <ESP8266WiFi> 1.0
// |-- <WifiManager> 0.15.0
// |   |-- <DNSServer> 1.1.1
// |   |   |-- <ESP8266WiFi> 1.0
// |   |-- <ESP8266WebServer> 1.0
// |   |   |-- <ESP8266WiFi> 1.0
// |   |-- <ESP8266WiFi> 1.0

#define PORT 80
ESP8266WebServer server(PORT);  //listening server

void handleRoot();              // function prototypes for HTTP handlers
void handleON();
void handleOFF();
void handle404();

//const char MAIN_page[] PROGMEM = R"=====(<html><body>Ciclk to turn <a href="\on">LED ON</a><br>Ciclk to turn <a href="\off">LED OFF</a><br></body></html>)=====";
//above does not work and causes esp8266 to crash
const char MAIN_page[] = "<html><body>Click to turn <a href=\"on\">LED ON</a><br>Ciclk to turn <a href=\"off\">LED OFF</a><br></body></html>";

const int PIN_LED = 2;

void handleRoot() {
    Serial.println("/");
    blinkLED(500, 10, PIN_LED);
    String s = MAIN_page;
    server.send(200, "text/html", s);
}


void handleON() { 
    Serial.println("LED on page");
    blinkLED(500, 5, PIN_LED);
    server.send(200, "text/html", "<b>ON action</b>");
}
 
void handleOFF() { 
    Serial.println("LED off page");
    blinkLED(10, 5, PIN_LED);
    server.send(200, "text/html", "<b>OFF action</b>");
}

void handle404(){
    blinkLED(100, 10, PIN_LED);
    server.send(404, "text/plain", "404: Not found");
}

void setup() {

    int resetCounter = ResetDetector::execute(2000); //capture number of resets during 2 seconds

    Serial.begin(115200);

    
    setupWifi(resetCounter);
    
    // //WiFiManager
    // //Local intialization. Once its business is done, there is no need to keep it around
    // WiFiManager wifiManager;
    // //default is true print a lot of debug info under *WM
    // wifiManager.setDebugOutput(false);
    // //WiFi.printDiag(Serial);

    // //reset Wifi settings or initial setup
    // //only when reset buttons pressed 2 times or more
    // if (resetCounter >= 2) {
    //     wifiManager.resetSettings();
    //     //twice to indicate in setup mode
    //     blinkLED(500, 2, PIN_LED);
    //     Serial.println("wifi: entering setup mode");
    //     //set the AP's name for you to connect and configure
    //     wifiManager.autoConnect("ap");
    //     //wifiManager.startConfigPortal(); //I don't know what this actually do
    //     Serial.println("wifi: entering Config Portal");
    // }
    // else { //enter running mode
    //     Serial.println("wifi: press reset twice to setup Wifi");
    // }

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