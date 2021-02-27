#include "BluetoothSerial.h"
#include <elapsedMillis.h>

#define IR1_PIN 2
#define IR2_PIN 3 

bool isConnected = false;
bool ballAbove = false;

long millisSinceDetected = 0;

int irStatus = HIGH;

BluetoothSerial SerialBT;

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32 Score Counter");
  SerialBT.register_callback(bt_callback);
  pinMode (IR1_PIN, INPUT);
  pinMode (IR2_PIN, INPUT);
  Serial.println("Waiting for client to connect...");
}

void bt_callback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param) {
  if (event == ESP_SPP_SRV_OPEN_EVT) {
    isConnected = true;
    Serial.println("Client Connected.");
  } 
  else if (event == ESP_SPP_CLOSE_EVT) {
    isConnected = false;
    ballAbove = false;
    millisSinceDetected = 0;
    Serial.println("Client Disconnected. Waiting for client to connect...");
  }
}

void loop() {
  if (isConnected) {
    if (!ballAbove) {
      irStatus = digitalRead(IR1_PIN);
      if (irStatus == LOW) {
        Serial.println("Ball detected above the hoop.");
        ballAbove = true;
        millisSinceDetected = 0;
      }
    }
    else {

      irStatus = digitalRead(IR2_PIN);
      if (irStatus == LOW) {
        Serial.println("Ball detected in the hoop. Score!.");
        SerialBT.write(int("scored"));
        ballAbove = false;
        delay(3000);
      } 
      else {
        millisSinceDetected+=10;
        if (millisSinceDetected>3000){
          ballAbove = false;
        }
      }

    }

    delay(10);
  }
}






