#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>

const char* ssid = "Redmi";
const char* password = "gnrn5080";
const char* mqtt_server = "160.75.154.100";

// Set your Static IP address
IPAddress local_IP(192, 168, 43, 184);
// Set your Gateway IP address
IPAddress gateway(192, 168, 43, 1);

IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(8, 8, 8, 8);   //optional
IPAddress secondaryDNS(8, 8, 4, 4); //optional

const char * udpAddress = "192.168.43.185";
const int udpPort = 44444;
//create UDP instance
WiFiUDP udp;

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;
String startSystem = "false";
String sysOut;
bool flag = 0;
#define DHTPIN 2     // Digital pin connected to the DHT sensor
#define DHTTYPE    DHT11

DHT dht(DHTPIN, DHTTYPE);

float temperature = 0;
float humidity = 0;

// LED Pin
const int ledPin = 4;

void setup() {
  Serial.begin(115200);
  dht.begin();
  //status = bme.begin();
//  if (!bme.begin(0x76)) {
//    Serial.println("Could not find a valid BME280 sensor, check wiring!");
//    while (1);
//  }
  setup_wifi();
  client.setServer(mqtt_server, 1884);
  client.setCallback(callback);

  pinMode(ledPin, OUTPUT);
  udp.begin(udpPort);
}

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* message, unsigned int length) {
  String messageTemp = "";
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();
  
  if (String(topic) == "group8/start") {
    startSystem = messageTemp;
  }
  if (String(topic) == "group8/SystemOut") {
    flag = 1;
    sysOut = messageTemp;
    
  }  
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP32Client","iturockwell","963258741")) {
      Serial.println("connected");
      client.subscribe("group8/start");
      client.subscribe("group8/SystemOut");
      
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  if(startSystem=="true"){
    long now = millis();
    if (now - lastMsg > 1000) {
      lastMsg = now;
      temperature = dht.readTemperature();
      if (isnan(temperature)) {
        Serial.println("Failed to read from DHT sensor!");
      }else {
        // Convert the value to a char array
        char tempString[8];
        dtostrf(temperature, 2, 2, tempString);
        Serial.print("Temperature: ");
        Serial.println(temperature);
        client.publish("group8/temperature", tempString);
        udp.beginPacket(udpAddress, udpPort);
        char buffer[16] = "DHTtemp:";
        strcat(buffer, tempString);       
        udp.write((uint8_t *)buffer,16);
        udp.endPacket();
      }
      
      humidity = dht.readHumidity();
      if (isnan(humidity)) {
        Serial.println("Failed to read from DHT sensor!");
      }else {
        // Convert the value to a char array
        char humString[8];
        dtostrf(humidity, 2, 2, humString);
        Serial.print("Humidity: ");
        Serial.println(humidity);
        client.publish("group8/humidity", humString);
      }
      if(flag == 1)
      {
        Serial.println("asdasdasd");
        flag = 0;
        udp.beginPacket(udpAddress, udpPort);
        char buffer[50] = "symsout:";
        strcat(buffer,sysOut.c_str());       
        udp.write((uint8_t *)buffer,16);
        udp.endPacket();

      }

    }
  }
}
