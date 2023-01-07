#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <DHT.h>

#define DHTPIN  2     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11

IPAddress local_IP(192, 168, 137, 184);
IPAddress gateway(192, 168, 137, 1);

IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(8, 8, 8, 8);   //optional
IPAddress secondaryDNS(8, 8, 4, 4); //optional

const char* ssid = "Redmi";
const char* password = "gnrn5080";
const char* mqtt_server = "160.75.154.100";

const char * udpAddress = "192.168.137.185";
const int udpPort = 44444;

//create UDP instance
WiFiUDP udp;

DHT dht(DHTPIN, DHTTYPE);

WiFiClient espClient;
PubSubClient client(espClient);

long lastMsg = 0;

String startSystem = "false";
String sysOut;

bool sendSysTempFlag = 0;

float temperature = 0;
float humidity = 0;

// LED Pin
const int ledPin = 4;

void reconnect() ;
void setup_wifi();
void callback(char* topic, byte* message, unsigned int length);
void send_udp(String header,String msg,int port,char * sendAddress);

void setup() {
  Serial.begin(115200);
  
  dht.begin();
  
  setup_wifi();
  udp.begin(udpPort);
  
  client.setServer(mqtt_server, 1884);
  client.setCallback(callback);

  pinMode(ledPin, OUTPUT);
  
}

void loop() {
  if (!client.connected()) 
    reconnect();
    
  client.loop();
  if(startSystem=="true")
  {
    long now = millis();
    if (now - lastMsg > 1000) 
    {
      lastMsg = now;
      temperature = dht.readTemperature();
      if (isnan(temperature))
        Serial.println("Failed to read from DHT sensor!");
      else
      {
        char tempString[8];
        dtostrf(temperature, 2, 2, tempString);
//        Serial.print("Temperature: ");
//        Serial.println(temperature);
        client.publish("group8/temperature", tempString);
        String str(tempString);
        send_udp("DHTtemp:",str,udpPort,(char *)udpAddress);
      }
      
      humidity = dht.readHumidity();
      if (isnan(humidity))
        Serial.println("Failed to read from DHT sensor!");
      else
      {
        // Convert the value to a char array
        char humString[8];
        dtostrf(humidity, 2, 2, humString);
//        Serial.print("Humidity: ");
//        Serial.println(humidity);
        client.publish("group8/humidity", humString);
      }
      if(sendSysTempFlag == 1)
      {
        sendSysTempFlag = 0;
        send_udp("symsout:",sysOut,udpPort,(char *)udpAddress);
      }
    }
  }
}
void send_udp(String header,String msg,int port,char * sendAddress)
{
  udp.beginPacket(sendAddress, port);
  char buffer[50] = "";

  strcat(buffer,header.c_str()); 
  strcat(buffer,msg.c_str());
  
  Serial.println(buffer);
  
  udp.write((uint8_t *)buffer,16);
  udp.endPacket();
}

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);


    
  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
    Serial.println("STA Failed to configure");
  }

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
//  Serial.print("Message arrived on topic: ");
//  Serial.print(topic);
//  Serial.print(". Message: ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();
  
  if (String(topic) == "group8/start") {
    startSystem = messageTemp;
  }
  if (String(topic) == "group8/SystemOut") {
    sendSysTempFlag = 1;
    sysOut = messageTemp;
    
  }  
}

void reconnect() 
{
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
