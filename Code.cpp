#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>

#include <Servo.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// WiFi config
#define WIFI_SSID "xxx"
#define WIFI_PASSWORD "xxx"

// Firebase Realtime Database URL (อย่าลืมใส่ /servo/angle.json)
#define SERVO_URL "xxx"
#define LED_URL "xxx"
// webhook discord
const char* webhook_url = "xxx";

WiFiClientSecure client;
Servo myServo;

// Rain sersor
#define RAIN_PIN D5
bool lastRainStatus = false;


int lastAngle = -1;
int lastLedStatus = -1;
//Led
#define LED_PIN D7

void setup() {
  Serial.begin(115200);
  //pinMode(TRIG_PIN, OUTPUT);
  //pinMode(ECHO_PIN, INPUT);
  pinMode(RAIN_PIN, INPUT); 
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  myServo.attach(D3); // หรือเปลี่ยนเป็น D4 ถ้า D3 มีปัญหา

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");

  client.setInsecure(); // ข้ามการตรวจสอบ SSL certificate
  delay(2000);
}

enum ControlMode {
  MODE_AUTO,    
  MODE_MANUAL   // ควบคุมด้วย Discord/Firebase
};

ControlMode currentMode = MODE_AUTO;
unsigned long lastManualCommandTime = 0;
const unsigned long manualModeTimeout = 5000; // 5 วินาที โหมด manual ค้างไว้นานสุด


void loop() {

  // อ่านค่ามุมจาก Firebase
  int angle = readAngleFromFirebase();
  if (angle >= 0 && angle <= 180) {
    if (angle != lastAngle) {
      Serial.printf("New angle from Firebase: %d\n", angle);
      myServo.write(angle);
      lastAngle = angle;

      // ตั้งโหมดเป็น manual เมื่อมีคำสั่งใหม่จาก Discord/Firebase
      currentMode = MODE_MANUAL;
      lastManualCommandTime = millis();
    }
  } else {
    Serial.println("Invalid angle or error reading from Firebase");
  }
  
  
  int ledStatus = readLedStatusFromFirebase();
  if (ledStatus == 0 || ledStatus == 1) {
    if (ledStatus != lastLedStatus) {
      digitalWrite(LED_PIN, ledStatus == 1 ? HIGH : LOW);
      Serial.printf("LED %s\n", ledStatus == 1 ? "ON" : "OFF");
      lastLedStatus = ledStatus;
    }
  } else {
    Serial.println("Error reading LED status from Firebase");
  }

  int rainVal = digitalRead(RAIN_PIN); // HIGH=ไม่มีฝน, LOW=ฝนตก
  bool isRaining = (rainVal == LOW);

  if (isRaining != lastRainStatus) {
    lastRainStatus = isRaining;

    if (isRaining) {
      Serial.println("ฝนตก!");
      sendToDiscord("ฝนตกแล้ว! เก็บผ้าด้วยนะ");
      updateRainToFirebase(true);
    } else {
      Serial.println("ฝนหยุดแล้ว");
      sendToDiscord("ฝนหยุดแล้ว");
      updateRainToFirebase(false);
    }


  }
  

  // ถ้าโหมด manual อยู่เกิน timeout ให้กลับไป auto
  if (currentMode == MODE_MANUAL && (millis() - lastManualCommandTime > manualModeTimeout)) {
    currentMode = MODE_AUTO;
    Serial.println("Switching back to AUTO mode");
  }

  if (currentMode == MODE_AUTO) {
    
  }

  delay(100);
}
