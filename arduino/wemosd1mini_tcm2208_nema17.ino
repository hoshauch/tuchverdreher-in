/*Example sketch to control a stepper motor with A4988/DRV8825 stepper motor driver and Arduino without a library. More info: https://www.makerguides.com */

// Define stepper motor connections and steps per revolution:

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiClient.h>
#include <AccelStepper.h>

const char *ssid = "...";
const char *password = "...";
ESP8266WebServer server(80);

//const int dirPin = 5; //This pin corresponds to GPIO5 (D1)
//const int stepPin = 4; //This pin corresponds to GPIO4 (D2)
#define DIR_PIN 13
#define STEP_PIN 15

#define stepsPerRevolution 6000

int shouldRun = 0;
int steps = 0;
int speed = 0;
int accel = 200;
int rotations = 200;
bool dir = HIGH; //Rotation direction. HIGH is clockwise.

AccelStepper stepper = AccelStepper(1, STEP_PIN, DIR_PIN);

void setup()
{
    //pinMode(DIR_PIN, OUTPUT);  // Pins are outputs
    //pinMode(STEP_PIN, OUTPUT);

    delay(1000);
    Serial.begin(115200); //I can debbug through the serial port

    // Configure NODEMCU as Access Point
    Serial.println("Configuring access point...");
    WiFi.hostname("tuchverdreher:in1");
    WiFi.begin(ssid, password);
    // WiFi.softAP(ssid); //Password is not necessary
    // IPAddress myIP = WiFi.softAPIP();
    // Serial.print("AP IP address: ");
    // Serial.println(myIP);

    server.on("/", HTTP_GET, handleRootPath); //I specify which code to execute when an HTTP request is performed to each path.
    server.on("/", HTTP_OPTIONS, sendCrossOriginHeader);
    server.on("/Init", HTTP_GET, handleInit); //Associate the handler function to the path "/Init".
    server.on("/Init", HTTP_OPTIONS, sendCrossOriginHeader);
    server.on("/Stop", HTTP_GET, stopStepper);
    server.on("/Stop", HTTP_OPTIONS, sendCrossOriginHeader);
    server.begin(); //Let's call the begin method on the server object to start the server.
    Serial.println("HTTP server started");
}

void sendCrossOriginHeader()
{
    server.send(204);
}

void stopStepper()
{
    Serial.println("stopStepper");
    setCrossOrigin();
    shouldRun = 0;

    server.send(200, "text/plain", "Tuchverdreher:in angehalten nach " + String(stepper.currentPosition()));
    stepper.stop();
    stepper.setCurrentPosition(0);
}

void loop()
{
    server.handleClient();

    if (shouldRun == 1)
    {
        if (stepper.distanceToGo() == 0)
            stepper.moveTo(-stepper.currentPosition());

        stepper.run();
    }
}

void handleRootPath()
{
    Serial.println("handleRootPath");
    setCrossOrigin();
    server.send(200, "text/plain", "Tuchverdreher:in bereit Tuecher zu verdrehen");
}

void setCrossOrigin()
{
    server.sendHeader(F("Access-Control-Allow-Origin"), F("*"));
    server.sendHeader(F("Access-Control-Max-Age"), F("600"));
    server.sendHeader(F("Access-Control-Allow-Methods"), F("PUT,POST,GET,OPTIONS"));
    server.sendHeader(F("Access-Control-Allow-Headers"), F("*"));
};

void handleInit()
    Serial.println("handleInit");
shouldRun = 0;

setCrossOrigin();

steps = 0; //Motor stopped if the arguments are wrong.
speed = 0;
accel = 200;
rotations = 0;
String message = "";

if (server.hasArg("Accel"))
{
    Serial.println("Accel");
    accel = (server.arg("Accel")).toInt();
}
if (server.hasArg("Speed"))
{
    Serial.println("Speed");
    speed = (server.arg("Speed")).toInt(); //Converts the string to integer.
}
if (server.hasArg("Steps"))
{
    Serial.println("Steps");
    steps = (server.arg("Steps")).toInt();
}
if (server.hasArg("Rotations"))
{
    Serial.println("Rotations");
    rotations = (server.arg("Rotations")).toInt();
}

message += "Starte mit Speed: " + String(speed) + ", Accel: " + String(accel) + ", Steps: " + String(steps) + " Rotations: " + String(rotations);

if (accel > 0)
{
    stepper.setMaxSpeed(speed);
    stepper.setAcceleration(accel);
}
else
{
    stepper.setSpeed(speed);
}

if (steps > 0)
{
    stepper.moveTo(steps);
}
else if (rotations > 0)
{
    stepper.moveTo(rotations * stepsPerRevolution);
}
else
{
    server.send(400, "text/plain", "Tuchverdreher:in versteht nicht, was du willst :("); //It's better to return something so the browser don't get frustrated+
    return;
}
shouldRun = true;
server.send(200, "text/plain", message); //It's better to return something so the browser don't get frustrated+
}