#include <TridentTD_LineNotify.h>

// Fill-in information from your Blynk Template here
#define BLYNK_TEMPLATE_ID "TMPLn_dd3PDm"
#define BLYNK_DEVICE_NAME "EPS32"

#define BLYNK_FIRMWARE_VERSION        "0.1.0"

#define BLYNK_PRINT Serial
//#define BLYNK_DEBUG

#define APP_DEBUG

// Uncomment your board, or configure a custom board in Settings.h
//#define USE_WROVER_BOARD
//#define USE_TTGO_T7

#include "BlynkEdgent.h"

#define LINE_TOKEN "RBBNo7ST5AOgSRVaNE9rLgJeiqOzUU1I80GOjF3krbG"
#define RELAY_PIN 27
#define HUMIDITY_SENSOR 32

int state = 0; // สถานะของline notify
String isStatus = "disabled"; //สถานะของการทำงานอัตโนมัติ

WidgetTerminal terminal(V4);

BLYNK_WRITE(V0)
{
  int value = param.asInt();
  if (value == 0) {
    pumpOn();
    terminal.println("Pump on");

  } else {
    pumpOff();
    terminal.println("Pump off");
  }
}

BLYNK_WRITE(V1)
{
}

BLYNK_WRITE(V3)
{
  int value = param.asInt();
  if (value == 1) {
    isStatus = "enabled";
  } else {
    isStatus = "disabled";
  }
}

BLYNK_WRITE(V4) {
}

void setup()
{
  pinMode(15, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(32, INPUT);
  Serial.begin(115200);

  delay(100);
  digitalWrite(RELAY_PIN, 1);
  LINE.setToken(LINE_TOKEN);
  BlynkEdgent.begin();
  //  timer.setInterval(1000L, terminalWrite);
}

void loop() {
  runSystem();
  gaugeWrite();
  BlynkEdgent.run();
  //  timer.run();
}

void runSystem() {

  if (isStatus == "enabled") {
    //  Serial.println(getHumidity());
    if (state == 0) {
      if (getHumidity() > 2000.00) {
        pumpOn();
        lineMsg("ทำการเปิดปั้มน้ำ! เนื่องจากความชิ้นในดินมีค่าต่ำกว่าเกณฑ์");
        state = 1;
      }
    } else if (state == 1) {
      if (getHumidity() > 2000.00) {
        pumpOn();
      } else {
        pumpOff();
        state = 0;
      }
    }
  }
}
double getHumidity() {
  return analogRead(HUMIDITY_SENSOR);
}

void lineMsg(String msg) {
  //  String ctx = "11/05/23\n" + msg;
  LINE.notifySticker(11539, 52114141);
  LINE.notify(msg);
}

void pumpOn() {
  digitalWrite(RELAY_PIN, 0);
}

void pumpOff() {
  digitalWrite(RELAY_PIN, 1);
}

void getDatetime() {
  Serial.println(time(0));
}

void gaugeWrite() {
  Blynk.virtualWrite(V1, getHumidity());
}
