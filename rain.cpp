void updateRainToFirebase(bool isRaining) {
  HTTPClient http;
  String url = "xxx";  

  http.begin(client, url);  
  http.addHeader("Content-Type", "application/json");

  String payload = isRaining ? "true" : "false";
  int httpCode = http.PUT(payload);

  if (httpCode > 0) {
    Serial.printf("Firebase updated rain: %s\n", payload.c_str());
  } else {
    Serial.printf("Firebase update failed: %s\n", http.errorToString(httpCode).c_str());
  }

  http.end();
}
