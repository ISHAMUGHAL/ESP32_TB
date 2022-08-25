#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ThingsBoard.h>
#include <iostream>
using namespace std;



// Declaring Network Essentials
const char* wifiSSID = "Telenor 4G-5B7A";
const char* wifiPassword = "45181313";

// Thingsboard Credentials
String tbHost = "demo.thingsboard.io";
String tbToken = "c6CTqVJzsGWlmqN0I3iw";

WiFiClient client;
ThingsBoard tb(client);

void connectWifi();
String randTemp();
String randHum();
void tbReconnect();


void connectWifi()
{  WiFi.mode(WIFI_STA);
  WiFi.begin(wifiSSID, wifiPassword);
    Serial.println("Connecting To Wifi");

    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(500);
    }
    Serial.println("Wifi Connected");
    Serial.println(WiFi.macAddress());
    Serial.println(WiFi.localIP());
 
}
void setup()
{
    Serial.begin(9600);
    connectWifi();
    }
    String randTemp()
{
    float randDec = random(0, 99);
    randDec /= 100;
    int randVal = random(25, 30);
    return String(randVal + randDec);
}

String randHum()
{
    float randDec = random(0, 99);
    randDec /= 100;
    int randVal = random(40, 60);
    return String(randVal + randDec);
}
void tbReconnect()
{
    while (!tb.connected())
    {
        if (WiFi.status()!= WL_CONNECTED)
        {
            connectWifi();
        }

        Serial.println("connecting to thingsboard");
        if (tb.connect(tbHost.c_str(), tbToken.c_str()))
        {
        
            Serial.println("Thingsboard Connected!");
        }
        else
        {
            Serial.println("Thingsboard connection failed");
            Serial.println("Retrying in 5 seconds");
            delay(5000);
        }
    }
}

void loop()
{
    if (!tb.connected())
    {
        tbReconnect();
    }

    float temp = randTemp().toDouble();
    float hum = randHum().toDouble();

    Serial.println("Sending data to Thingsboard");
    tb.sendTelemetryFloat("temperature", temp);
    tb.sendTelemetryFloat("humidity", hum);
    tb.loop();
    delay(1000);
}





