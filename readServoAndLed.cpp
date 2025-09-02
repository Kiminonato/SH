int readAngleFromFirebase() {
  HTTPClient http;
  http.begin(client, SERVO_URL);
  int httpCode = http.GET();

  if (httpCode == 200) {
    String payload = http.getString();
    Serial.println("Read from Firebase:");
    Serial.println(payload);

    // ตัวอย่าง payload: 120 หรือ "120"
    // Firebase อาจคืนค่าเป็นตัวเลขหรือตัวอักษร
    payload.trim();
    if (payload.startsWith("\"") && payload.endsWith("\"")) {
      payload = payload.substring(1, payload.length() - 1); // ตัด " ออก
    }
    int angle = payload.toInt();
    http.end();
    return angle;
  } else {
    Serial.printf("Failed to GET from Firebase, code: %d\n", httpCode);
  }
  http.end();
  return -1;
}

int readLedStatusFromFirebase() {
  HTTPClient http;
  http.begin(client, LED_URL);
  int httpCode = http.GET();

  if (httpCode == HTTP_CODE_OK) {
    String payload = http.getString();
    http.end();
    return payload.toInt(); // 0 หรือ 1
  }
  http.end();
  return -1;
}
