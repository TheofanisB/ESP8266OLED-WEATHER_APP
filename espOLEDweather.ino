#include <U8g2lib.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>

const char* ssid = "INSERT_YOUR_WIFI_SSID_HERE";
const char* password = "INSERT_YOUR_WIFI_PASSWD_HERE";


const char* apiKey = "INSERT YOUR OPEN WEATHER MAP API KEY HERE";

// DEFINE YOUR LOCATION HERE
const char* city = "Kifissia";
const char* country = "Greece";


U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE); // OLED SCREEN INIT

void setup() {
  
  Serial.begin(115200); //SERIAL INIT

  
  WiFi.begin(ssid, password); //WIFI INIT
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

 
  u8g2.begin(); // OLED INIT
  u8g2.clearBuffer();


  updateWeather(); //API DATA PULL
}

void loop() {
 
  static unsigned long lastUpdate = 0;
  if (millis() - lastUpdate >= 1800000) { //UPDATED THE WEATHER DISPLAYED EVERY 30 MINS 
    updateWeather();
    lastUpdate = millis();
  }
}

void updateWeather() {
  WiFiClient client;
  HTTPClient http;
  String url = "http://api.openweathermap.org/data/2.5/weather?q=" + String(city) + "," + String(country) + "&appid=" + String(apiKey);
  http.begin(client, url);

 
  int httpResponseCode = http.GET();

  if (httpResponseCode > 0) {
    String payload = http.getString();
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, payload);
    JsonObject weather = doc["weather"][0];
    JsonObject main = doc["main"];
    const char* description = weather["description"];
    float temperature = main["temp"].as<float>() - 273.15; // Convert Kelvin to Celsius

    // INFORMATION THAT WILL BE DISPLAYED
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_ncenB14_tr);
    u8g2.setCursor(0, 20);
    u8g2.print("Weather in ");
    u8g2.print(city);
    u8g2.print(", ");
    u8g2.println(country);
    u8g2.setCursor(0, 40);
    u8g2.print("Description: ");
    u8g2.println(description);
    u8g2.print("Temperature: ");
    u8g2.print(temperature, 1);
    u8g2.println(" °C");
    u8g2.sendBuffer();
  } else {
    Serial.println("Error in HTTP request");
  }

  // CLOSE THE CONNECTION
  http.end();
}
