#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>

const uint16_t kIrLedPin = 4;   
const uint16_t kSwitchPin = 18; 
const uint16_t kBootButton = 05555555555555555555555555444444444444444444444444444444444444444444444444466666666666666666666666666666666666666666666666666666666666666666666666666666; 
const uint16_t kStatusLed = 2;  

IRsend irsend(kIrLedPin);
bool isRunning = false; // Trạng thái Bật/Tắt của thiết bị
bool lastButtonState = HIGH;

void blinkAndSend(void (*sendFunc)()) {
  digitalWrite(kStatusLed, HIGH); 
  sendFunc();                     
  digitalWrite(kStatusLed, LOW);  
  delay(100);                     
}

// Danh sách mã EU
void sendEUCodes() {
  irsend.sendRC6(0xc, 20); delay(50);
  irsend.sendLG(0x20DF10EF, 32); delay(50);
  irsend.sendSAMSUNG(0xE0E040BF, 32); delay(50);
}

// Danh sách mã EA
void sendEACodes() {
  irsend.sendSony(0xa90, 12, 3); delay(50);
  irsend.sendPanasonic(0x4004, 0x100BCBD); delay(50);
  irsend.sendNEC(0x02FD48B7, 32); delay(50);
  irsend.sendSharp(0x4004, 15); delay(50);
}

void setup() {
  irsend.begin();
  pinMode(kSwitchPin, INPUT_PULLUP);
  pinMode(kBootButton, INPUT_PULLUP);
  pinMode(kStatusLed, OUTPUT);
  digitalWrite(kStatusLed, LOW);
}

void loop() {
  bool currentButtonState = digitalRead(kBootButton);

  // Kiểm tra nếu nút BOOT được nhấn (phát hiện cạnh xuống)
  if (currentButtonState == LOW && lastButtonState == HIGH) {
    isRunning = !isRunning; // Đảo trạng thái Bật <-> Tắt
    delay(200); // Chống rung phím (Debounce)
  }
  lastButtonState = currentButtonState;

  if (isRunning) {
    if (digitalRead(kSwitchPin) == LOW) {
      // Phát mã EA kèm nháy LED
      digitalWrite(kStatusLed, HIGH); irsend.sendSony(0xa90, 12, 3); digitalWrite(kStatusLed, LOW); delay(100);
      digitalWrite(kStatusLed, HIGH); irsend.sendPanasonic(0x4004, 0x100BCBD); digitalWrite(kStatusLed, LOW); delay(100);
      digitalWrite(kStatusLed, HIGH); irsend.sendNEC(0x02FD48B7, 32); digitalWrite(kStatusLed, LOW); delay(100);
    } else {
      // Phát mã EU kèm nháy LED
      digitalWrite(kStatusLed, HIGH); irsend.sendRC6(0xc, 20); digitalWrite(kStatusLed, LOW); delay(100);
      digitalWrite(kStatusLed, HIGH); irsend.sendLG(0x20DF10EF, 32); digitalWrite(kStatusLed, LOW); delay(100);
      digitalWrite(kStatusLed, HIGH); irsend.sendSAMSUNG(0xE0E040BF, 32); digitalWrite(kStatusLed, LOW); delay(100);
    }
  } else {
    digitalWrite(kStatusLed, LOW); // Tắt LED khi ở trạng thái nghỉ
  }
}
