#include <TinyGPS++.h>
#include <HardwareSerial.h>
#include <HttpClient.h>
#include <WiFi.h>
#include <inttypes.h>
#include <stdio.h>
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs.h"
#include "nvs_flash.h"
#define LED_PIN 21
#define BUZZER_PIN 22
// Create TinyGPSPlus instance
TinyGPSPlus gps;

// Define the hardware serial port
HardwareSerial gpsSerial(1);

char sound;
char light;
int bodyLen;

char ssid[50] = "jenny (2)"; // network SSID (name)
char pass[50] = "jenny123"; // network password
const char kHostname[] = "18.212.215.254";
// Path to download (this is the bit after the hostname in the URL
// that you want to download
const char kPath[] = "5000";
// Number of milliseconds to wait without receiving any data before we give up
const int kNetworkTimeout = 30 * 1000;
// Number of milliseconds to wait if no data is available before trying again
const int kNetworkDelay = 1000;


void setup()
{
    Serial.begin(9600);             // Initialize the serial monitor
    gpsSerial.begin(9600, SERIAL_8N1, 26, 27); // GPS module baud rate, TX/RX pins
    Serial.println("GPS Module Connected!");
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.disconnect(true);
    WiFi.begin(ssid, pass);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    Serial.println("MAC address: ");
    Serial.println(WiFi.macAddress());
    pinMode(LED_PIN, OUTPUT);
    pinMode(BUZZER_PIN, OUTPUT);
} 

// function to send latitude and longitude to server
void sendToServer(float latitude, float longitude) {
  int err = 0;
  WiFiClient c;
  HttpClient http(c);
  char query[64];
  snprintf(query, sizeof(query), "/?latitude=%.2f&longitude=%.2f", latitude, longitude);

  err = http.get(kHostname, atoi(kPath), query, NULL);
  if (err == 0) {
    Serial.println("startedRequest ok");
    err = http.responseStatusCode();
    if (err >= 0) {
      Serial.print("Got status code: ");
      Serial.println(err);
      err = http.skipResponseHeaders();
      if (err >= 0) {
        bodyLen = http.contentLength();
        int test = http.contentLength();
        Serial.print("Content length is: ");
        Serial.println(bodyLen);
        Serial.println();
        Serial.println("Body returned follows:");
        char c;
        int place;
        unsigned long timeoutStart = millis();
        sound = 'n';
        light = 'n';
        while ((http.connected() || http.available()) &&
               ((millis() - timeoutStart) < kNetworkTimeout)) {
          if (http.available()) {
          c = http.read();
          Serial.print(c);
          if(test ==  5 && c == 'n')
          {
            sound = 's';
          }
          else if(test == 1 && c == 'n')
          {
            light = 'l';
          }
          timeoutStart = millis();
          test--;
          } else {
            delay(kNetworkDelay);
          }
        }
      } else {
        Serial.print("Failed to skip response headers: ");
        Serial.println(err);
      }
    } else {
      Serial.print("Getting response failed: ");
      Serial.println(err);
    }
  } else {
    Serial.print("Connect failed: ");
    Serial.println(err);
  }

  http.stop();
}

void loop()
{
    // Check if data is available from the GPS module
    while (gpsSerial.available() > 0)
    {
        gps.encode(gpsSerial.read());
    }

    // Print GPS data if available & send to server
    if (gps.location.isUpdated())
    {
        Serial.print("Latitude: ");
        Serial.print(gps.location.lat(), 6);
        Serial.print(", Longitude: ");
        Serial.println(gps.location.lng(), 6);
        float latitude = gps.location.lat();
        float longitude = gps.location.lng();
        sendToServer(latitude, longitude);
    }

  // Send sound and light signals to development board 
  if(sound == 's' || bodyLen == 5 )
  {
    tone(BUZZER_PIN, 261, 500);
    tone(BUZZER_PIN, 294, 500);
    tone(BUZZER_PIN, 330, 500);
    delay(500);
    noTone(BUZZER_PIN);
  }

  if(light == 'l' || bodyLen == 5)
  {
    digitalWrite(LED_PIN, HIGH);
    delay(500);
    digitalWrite(LED_PIN, LOW);
  }

}
