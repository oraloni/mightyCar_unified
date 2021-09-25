#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESP8266HTTPClient.h>
//#include <WiFiClient.h>
//#include <AsyncElegantOTA.h>
#include <Servo.h>

#include "htmls.h"
#include "CarDefs.h"

#ifndef STASSID
#define STASSID "aloni"
#define STAPSK  "0524663051"
#endif


String cmd_in;
char command = 'S';

String steer_cmd_in;
char steer_command;

String speed_in;

int speed_val = 0; 
int angle_val = 90;

String special_cmd_in;
char special_cmd;

Servo myservo;

AsyncWebServer server(80);

void connect_to_wifi();

void setup() {
  Serial.begin(115200);
  
    
    pinMode(motor1_EN, OUTPUT);
    pinMode(motor1_A, OUTPUT);
    pinMode(motor1_B, OUTPUT);
    pinMode(motor2_EN, OUTPUT);
    pinMode(motor2_A, OUTPUT);
    pinMode(motor2_B, OUTPUT);

  myservo.attach(servoPin);
  myservo.write(angle_val);

  connect_to_wifi();
  
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send_P(200, "text/html", car_html);
  });

  server.on("/car", HTTP_GET, [](AsyncWebServerRequest *request){
        if (request->hasParam("cmd")) {
            cmd_in = request->getParam("cmd")->value();
        }
        request->send_P(200, "text/html", car_html);
        char cmd_in_array[2];
        cmd_in.toCharArray(cmd_in_array, 2);
        command = cmd_in_array[0];
        //Serial.print("command: "); Serial.println(command);
  });

    server.on("/car2", HTTP_GET, [](AsyncWebServerRequest *request){
        if (request->hasParam("cmd")) {
            steer_cmd_in = request->getParam("cmd")->value();
        }
        request->send_P(200, "text/html", car_html);
        char cmd_in_array[2];
        steer_cmd_in.toCharArray(cmd_in_array, 2);
        steer_command = cmd_in_array[0];
        //Serial.print("steer command: "); Serial.println(steer_command);
  });
  
server.on("/car1", HTTP_GET, [](AsyncWebServerRequest *request){
        if (request->hasParam("speed")) {
            speed_in = request->getParam("speed")->value();
            speed_val = speed_in.toInt() * 2.5;
        }
        request->send_P(200, "text/html", car_html);
        //Serial.println(speed_in);
  });

server.on("/special", HTTP_GET, [](AsyncWebServerRequest *request){
        if (request->hasParam("cmd")) {
            special_cmd_in = request->getParam("cmd")->value();
        }
        request->send_P(200, "text/html", car_html);
        char cmd_in_array[2];
        special_cmd_in.toCharArray(cmd_in_array, 2);
        special_cmd = cmd_in_array[0];
        //Serial.print("special command: "); Serial.println(special_cmd);
  });

  Serial.println("Starting HTTP server");
  server.begin();
}

void loop() {
    controlCar();
    controlAngle();
    specialControl();
}

void connect_to_wifi()
{
      const char *ssid = STASSID;
      const char *password = STAPSK;

// For static IP address
      IPAddress local_IP(10, 0, 0, 222);
      IPAddress gateway(10, 0, 0, 138);
      IPAddress subnet(255, 255, 0, 0);
        
      WiFi.mode(WIFI_STA);
      WiFi.begin(ssid, password);
      
      Serial.println("");
       // set wifi configuration
      if (!WiFi.config(local_IP, gateway, subnet)) {
         Serial.println("STA Failed to configure");
      }
      // Wait for connection
      Serial.println("Connecting to server!");
      while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
      }
      Serial.println("");
      Serial.print("Connected to ");
      Serial.println(ssid); 
      Serial.print("IP address: ");
      Serial.println(WiFi.localIP());

      WiFi.setAutoReconnect(true);
      WiFi.persistent(true);
}

void controlCar()
{
    Serial.print("onBreakRight: "); Serial.println(onBreakRight);
    Serial.print("onBreakLeft: "); Serial.println(onBreakLeft);
    if (command == 'F')
    {
   //      Serial.println("moving foward");
        if(onBreakRight == true)
        {
            onBreakRight = false;
            onBreakLeft = false;
            Serial.println("out of right break "); 
            angle_val = 90;
            //myservo.write(angle_val);
        }
        else if(onBreakLeft == true)
        {
            onBreakRight = false;
            onBreakLeft = false;
            Serial.println("out of left break "); 
            angle_val = 90;
            //myservo.write(angle_val);
        }
        move_forward(speed_val);
    }
    else if (command == 'B')
    {
   //     Serial.println("moving backward");
        onBreakRight = false;
        onBreakLeft = false;
        move_backward(speed_val);
    }
    else if (command == 'S')
    {
   //     Serial.println("stop");
        stop_car();
    }
    command = '0';
}

void controlAngle()
{
    // servo to left
    if(steer_command == 'L')
    { 
      if(angle_val >= 30)
      {     
        angle_val -= 30;
      }
      steer_command = '0';
    } 

    // servo to left 
    else if (steer_command == 'R')
    {
      if(angle_val <= 150)
      {     
        angle_val += 30;
      }   
      steer_command = '0';     
    } 
    
    Serial.printf("angle: %d\n", angle_val);
    myservo.write(angle_val);
}

void specialControl()
{
    if(special_cmd == '<')
    {
        command = '0';
        angle_val = 0;
        myservo.write(angle_val);
        break_left(speed_val);
        special_cmd = '0';
    }
    else if(special_cmd == '>')
    {
        command = '0';
        angle_val = 180;
        myservo.write(angle_val);
        break_right(speed_val);
        special_cmd = '0';
    }
}
