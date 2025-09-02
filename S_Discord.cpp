void sendToDiscord(String message) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient https;
    https.begin(client, webhook_url);  // ใช้ webhook discord ของคุณ
    https.addHeader("Content-Type", "application/json");

    String payload = "{\"content\":\"" + message + "\"}";  
    int httpCode = https.POST(payload);

    if (httpCode > 0) {
      Serial.printf("Discord response: %d\n", httpCode);
    } else {
      Serial.printf("Discord error: %s\n", https.errorToString(httpCode).c_str());
    }
    https.end();
  } else {
    Serial.println("WiFi not connected, cannot send to Discord");
  }
}
