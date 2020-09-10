#include "ESP8266WiFi.h"
#include "ESPAsyncTCP.h"
#include "ESPAsyncWebServer.h"

const char *ssid = "Car";

AsyncWebServer server(80);

int SPEED = 1023;

int motor1a = D1;
int motor1b = D2;
int motor2a = D3;
int motor2b = D4;
int pwm1 = D5;
int pwm2 = D6;

const char index_html[] PROGMEM = R"rawliteral(
  <html>
      <head>
          <meta name="viewport" content="width=device-width, initial-scale=1, maximum-scale=1, user-scalable=no;" />
          <style>
              * { 
                  text-align: center; 
                  -webkit-user-select: none; 
                  -webkit-touch-callout: none; 
                  -moz-user-select: none; 
                  -ms-user-select: none; 
                  user-select: none; 
              }
              button { 
                  width: 120px; 
                  height: 120px; 
                  font-size: 16px; 
                  border-radius: 0px; 
                  border: 1px solid #e1e1e1; 
                  background-color: #a0fff2; 
              }
              button:active { 
                  background-color: red; 
              }
          </style>
          <script>
              function getsend(action) { 
                  var xhttp = new XMLHttpRequest(); 
                  xhttp.open("GET", action + "?" + new Date().getTime(), true); 
                  xhttp.send(); 
              }
          </script>
      </head>
      <body>
          <p>
              <button onmousedown="getsend('go')" onmouseup="getsend('stop')" ontouchstart="getsend('go')" ontouchend="getsend('stop')">Go</button>
          </p>
          <p>
              <button onmousedown="getsend('left')" onmouseup="getsend('stop')" ontouchstart="getsend('left')" ontouchend="getsend('stop')">Left</button>
              <button style="visibility: hidden;">Stop</button>
              <button onmousedown="getsend('right')" onmouseup="getsend('stop')" ontouchstart="getsend('right')" ontouchend="getsend('stop')">Right</button>
          </p>
          <p>
              <button onmousedown="getsend('back')" onmouseup="getsend('stop')" ontouchstart="getsend('back')" ontouchend="getsend('stop')">Back</button>
          </p>
      </body>
  </html>
)rawliteral";

void setup() {
    Serial.begin(9600);
    Serial.println();
    
    // MOTOR ตัวที่ 1
    pinMode(motor1a, OUTPUT);
    pinMode(motor1b, OUTPUT);
    pinMode(pwm1, OUTPUT);
    
    // MOTOR ตัวที่ 2
    pinMode(motor2a, OUTPUT);
    pinMode(motor2b, OUTPUT);
    pinMode(pwm2, OUTPUT);

    SetSpeed();
    
    WiFi.mode(WIFI_AP);
    WiFi.softAP(ssid);
    Serial.print("Connect to : ");
    Serial.println(ssid);
    IPAddress apip = WiFi.softAPIP();
    Serial.print("IP Address: ");
    Serial.println(apip);
    
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        Serial.println("root");
        request->send(200, "text/html", index_html);
    });
    server.on("/go", HTTP_GET, [](AsyncWebServerRequest *request) {
        Go();
        request->send(200, "text/plain", "go");
    });
    server.on("/right", HTTP_GET, [](AsyncWebServerRequest *request) {
        Right();
        request->send(200, "text/plain", "right");
    });
    server.on("/back", HTTP_GET, [](AsyncWebServerRequest *request) {
        Back();
        request->send(200, "text/plain", "back");
    });
    server.on("/left", HTTP_GET, [](AsyncWebServerRequest *request) {
        Left();
        request->send(200, "text/plain", "left");
    });
    server.on("/stop", HTTP_GET, [](AsyncWebServerRequest *request) {
        Stop();
        request->send(200, "text/plain", "stop");
    });
    server.on("/speed", HTTP_GET, [](AsyncWebServerRequest *request) {
        if (request->hasParam("val")) {
            String val = request->getParam("val")->value();
            SPEED = val.toInt();
            SetSpeed();
            Serial.println(SPEED);
            request->send(200, "text/plain", String(SPEED));
        } else {
            Serial.println(SPEED);
            request->send(200, "text/plain", String(SPEED));
        }
    });
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
    server.begin();
}

void loop(){
}
void SetSpeed() {
    analogWrite(pwm1, SPEED);
    analogWrite(pwm2, SPEED);  
}
void Go() {
    Serial.println("Go");
    digitalWrite(motor1a, HIGH);
    digitalWrite(motor1b, LOW);
    digitalWrite(motor2a, HIGH);
    digitalWrite(motor2b, LOW);
}

void Back() {
    Serial.println("Back");
    digitalWrite(motor1a, LOW);
    digitalWrite(motor1b, HIGH);
    digitalWrite(motor2a, LOW);
    digitalWrite(motor2b, HIGH);
}

void Left() {
    Serial.println("Left");
    digitalWrite(motor1a, HIGH);
    digitalWrite(motor1b, LOW);
    digitalWrite(motor2a, LOW);
    digitalWrite(motor2b, LOW);
}

void Right() {
    Serial.println("Right");
    digitalWrite(motor1a, LOW);
    digitalWrite(motor1b, LOW);
    digitalWrite(motor2a, HIGH);
    digitalWrite(motor2b, LOW);
}

void Stop() {
    Serial.println("Stop");
    digitalWrite(motor1a, LOW);
    digitalWrite(motor1b, LOW);
    digitalWrite(motor2a, LOW);
    digitalWrite(motor2b, LOW);
}
