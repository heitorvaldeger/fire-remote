#include <Arduino.h>
#include <IRremoteESP8266.h>
// #include <IRsend.h>
// #include <IRrecv.h>
#include <IRutils.h>
#include <ir_Gree.h>
#include <FirebaseESP32.h>

#define FIREBASE_HOST ""
#define FIREBASE_AUTH ""
#define WIFI_SSID ""
#define WIFI_PASSWORD ""
#define PATH_TEMP ""
#define PATH_STATUS ""

// const int kIrLedR = 13; // ESP32 GPIO pin (D13) (RECEIVER)
const int kIrLedE = 12; // ESP32 GPIO pin (D12) (SEND)
bool stateBase = false;
int tempBase = 16;

FirebaseData firebaseData; //Define FirebaseESP32 data object

// IRrecv IrRecv(kIrLedR);
// IRsend IrSend(kIrLedE);  // Set the GPIO to be used to sending the message.
IRGreeAC ac(kIrLedE);

decode_results results;

void fetchData() {

  // Get Status
  if(Firebase.getBool(firebaseData, PATH_STATUS)) {
    bool state = firebaseData.boolData();
    if(state != stateBase) {
      if(state) {
        ac.on();
      }
      else {
        ac.off();
      } 
      ac.send();
      stateBase = state;
    }
  }

  // Get Temp
  if(Firebase.getInt(firebaseData, PATH_TEMP)) {
    int temp = firebaseData.intData();

    if (temp >= 16 && temp <= 30 && temp != tempBase)
    {
      ac.setTemp(temp);
      ac.send(); 
      tempBase = temp;
    }
  }
}

void setup() {
  // IrSend.begin();
  ac.begin();
  Serial.begin(115200);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
    delay(300);

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
  
  // IrRecv.enableIRIn();  // Start the receiver

  ac.off();
  ac.setMode(kGreeCool);
  ac.setTemp(16);
  ac.setXFan(false);
  ac.setLight(true);
  ac.setSleep(false);
  ac.setTurbo(false);
}

void loop() {
  fetchData();
  delay(5000);
//    switch(results.value) {
//      case 0xBE749278:
//        // IrSend.sendNEC(0x2FD48B7);
//        ac.on();
//        ac.send();
//        Serial.println("Ligado");
//        break;
//      case 0xA51162BC:
//        ac.off();
//        ac.send();
//        Serial.println("Desligado");
//        break;
//    }
}
