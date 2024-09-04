#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

// Wifi network station credentials
#define WIFI_SSID 
#define WIFI_PASSWORD 
// Telegram BOT Token (Get from Botfather)
#define BOT_TOKEN 
#define chat_id

// Define the pin connected to the OUT pin of the IR sensor
const int irSensorPin = D5; // Change to the correct pin you connected the sensor to

String pesanTerbuka = "pintu terbuka";
String pesanTertutup = "pintu tertutup";
bool isOpen = true; // Flag to alternate messages

X509List cert(TELEGRAM_CERTIFICATE_ROOT);
WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);

void setup()
{
  Serial.begin(115200);
  Serial.println();

  // Set the sensor pin as input
  pinMode(irSensorPin, INPUT);

  // attempt to connect to Wifi network:
  configTime(0, 0, "pool.ntp.org");      // get UTC time via NTP
  secured_client.setTrustAnchors(&cert); // Add root certificate for api.telegram.org
  Serial.print("Connecting to Wifi SSID ");
  Serial.print(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.print("\nWiFi connected. IP address: ");
  Serial.println(WiFi.localIP());

  // Check NTP/Time, usually it is instantaneous and you can delete the code below.
  Serial.print("Retrieving time: ");
  time_t now = time(nullptr);
  while (now < 24 * 3600)
  {
    Serial.print(".");
    delay(100);
    now = time(nullptr);
  }
  Serial.println(now);
}

void loop()
{
  int sensorValue = digitalRead(irSensorPin);
  // If the sensor detects an obstacle (usually LOW state), send the message
  if (sensorValue == LOW)
  {
    if (isOpen)
    {
      bot.sendMessage(chat_id, pesanTerbuka);
    }
    else
    {
      bot.sendMessage(chat_id, pesanTertutup);
    }
    isOpen = !isOpen; // Toggle the flag
    delay(1000); // Delay to prevent bouncing issues
  }

  // Small delay to avoid bouncing issues
  delay(100);
}
