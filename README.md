# IdeaSpark ESP8266OLED Open Weather Map Implementation


A brief project on how to turn your ESP8266 OLED into a weather monitor




## Requirements
In order to host the server on your ESP8266 you need to have the following:
- Arduino IDE installed
- ESP8266 board support package installed
- An ESP8266 OLED board
- A USB-to-serial adapter or a development board with built-in USB support to connect the ESP8266 to your computer.

## Setup
- Paste my Code included in the repo into your Arduino IDE

- Replace the "INSERT_YOUR_WIFI_SSID_HERE" and "INSERT_YOUR_WIFI_PASSWD_HERE" placeholders in the code with your ACTUAL network credentials.
- Replace the "INSERT YOUR OPEN WEATHER MAP API KEY HERE" placeholder in the code with your ACTUAL open weather map api key.
    You can create your own API key here : https://openweathermap.org/guide
- Replace the "Kifissia" and "Greece" placeholders in the code with the city and country you want to monitor the weather.

    DISCLAIMER: The ESP8266 board can only connect to 2.4GHz WiFi networks so make sure your WiFi has a 2.4GHz band!

- Upload the code: Select the appropriate board and port in the Arduino IDE, then upload the code to your ESP8266.

You can further customize the server's functionality by adding additional routes and handling different types of requests.

🚀🚀🚀That's all I guess!?🚀🚀🚀

