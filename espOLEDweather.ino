#include <U8g2lib.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>

const char* ssid     = "INSERT_YOUR_WIFI_SSID_HERE";
const char* password = "INSERT_YOUR_WIFI_PASSWD_HERE";
const char* apiKey   = "INSERT_YOUR_OPENWEATHERMAP_API_KEY_HERE";
const char* city     = "Kifissia";
const char* country  = "GR";  // ISO 3166-1 alpha-2 country code

const unsigned long UPDATE_INTERVAL = 1800000UL;  // 30 minutes

U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

void showStatus(const char* line1, const char* line2 = nullptr) {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.setCursor(0, 16);
  u8g2.print(line1);
  if (line2) {
    u8g2.setCursor(0, 34);
    u8g2.print(line2);
  }
  u8g2.sendBuffer();
}

void connectWiFi() {
  showStatus("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected");
  showStatus("WiFi connected!");
  delay(800);
}

void updateWeather() {
  WiFiClient client;
  HTTPClient http;

  http.begin(client, String("http://api.openweathermap.org/data/2.5/weather?q=")
                     + city + "," + country
                     + "&units=metric&appid=" + apiKey);

  int httpCode = http.GET();

  if (httpCode != HTTP_CODE_OK) {
    char msg[22];
    snprintf(msg, sizeof(msg), "HTTP error: %d", httpCode);
    showStatus("API failed", msg);
    Serial.println(msg);
    http.end();
    return;
  }

  JsonDocument doc;
  DeserializationError err = deserializeJson(doc, http.getStream());
  http.end();

  if (err) {
    showStatus("JSON error", err.c_str());
    Serial.print("JSON error: ");
    Serial.println(err.c_str());
    return;
  }

  const char* description = doc["weather"][0]["description"] | "unknown";
  float temp      = doc["main"]["temp"]       | 0.0f;
  float feelsLike = doc["main"]["feels_like"] | 0.0f;
  int   humidity  = doc["main"]["humidity"]   | 0;

  char tempStr[20], feelsStr[20], humStr[20];
  snprintf(tempStr,  sizeof(tempStr),  "Temp:  %.1f C", temp);
  snprintf(feelsStr, sizeof(feelsStr), "Feels: %.1f C", feelsLike);
  snprintf(humStr,   sizeof(humStr),   "Humid: %d%%", humidity);

  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.setCursor(0, 10);
  u8g2.print(city);
  u8g2.print(", ");
  u8g2.print(country);
  u8g2.drawHLine(0, 13, 128);
  u8g2.setCursor(0, 24);  u8g2.print(description);
  u8g2.setCursor(0, 36);  u8g2.print(tempStr);
  u8g2.setCursor(0, 48);  u8g2.print(feelsStr);
  u8g2.setCursor(0, 60);  u8g2.print(humStr);
  u8g2.sendBuffer();

  Serial.println("Weather updated");
}

void setup() {
  Serial.begin(115200);
  u8g2.begin();
  connectWiFi();
  updateWeather();
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    connectWiFi();
  }

  static unsigned long lastUpdate = 0;
  if (millis() - lastUpdate >= UPDATE_INTERVAL) {
    updateWeather();
    lastUpdate = millis();
  }
}
