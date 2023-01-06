#include <WiFi.h>

const char* ssid     = "Redmi";
const char* password = "gnrn5080";

uint8_t buffer[50];

// Set your Static IP address
IPAddress local_IP(192, 168, 43, 185);
// Set your Gateway IP address
IPAddress gateway(192, 168, 43, 1);

IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(8, 8, 8, 8);   //optional
IPAddress secondaryDNS(8, 8, 4, 4); //optional

const char * udpAddress = "192.168.43.184";
const int udpPort = 44444;
//create UDP instance
WiFiUDP udp;

void setup() {
  Serial.begin(115200);

  // Configures static IP address
  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
    Serial.println("STA Failed to configure");
  }
  
  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  udp.begin(udpPort);

}

void loop(){
  memset(buffer, 0, 50);
  //processing incoming packet, must be called before reading the buffer
  udp.parsePacket();
  //receive response from server, it will be HELLO WORLD
  if(udp.read(buffer, 50) > 0){
    Serial.println((char *)buffer);
  }
  //Wait for 1 second

}
