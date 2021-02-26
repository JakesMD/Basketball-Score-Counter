#include "BluetoothSerial.h"

#define IR_PIN 2

bool isConnected = false;
int irStatus = HIGH;

BluetoothSerial SerialBT;

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32 Score Counter");
  SerialBT.register_callback(bt_callback);
  pinMode (IR_PIN, INPUT);
  Serial.println("Waiting for client to connect...");
}

void bt_callback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param) {
  if (event == ESP_SPP_SRV_OPEN_EVT) {
    isConnected = true;
    Serial.println("Client Connected.");
  } else if (event == ESP_SPP_CLOSE_EVT) {
    isConnected = false;
    Serial.println("Client Disconnected. Waiting for client to connect...");
  }
}

void loop() {
  if (isConnected) {
    irStatus = digitalRead(IR_PIN);
    if (irStatus == LOW) {
      Serial.println("Ball detected.");
      SerialBT.write(int("ball detected"));
      delay(3000);
    }
    delay(50);
  }
}
