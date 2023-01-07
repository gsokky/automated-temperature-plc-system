#include <WiFi.h>
const char* ssid     = "Redmi";
const char* password = "gnrn5080";

char * DHTtemp;
char * sysout;

double DHTtempNumb;
double sysoutNumb;

uint8_t buffer[50];

// LED Pin
const int ledPin = 4;


IPAddress local_IP(192, 168, 137, 185);
IPAddress gateway(192, 168, 137, 1);

IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(8, 8, 8, 8);   //optional
IPAddress secondaryDNS(8, 8, 4, 4); //optional

const char * udpAddress = "192.168.137.184";
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
  
  udp.parsePacket();
  if(udp.read(buffer, 16) > 0)
  {
    char * header;
    char * msg;
    
    header = strtok((char*)buffer, ":");
    msg = strtok(NULL, ":");
    
//    Serial.print((char *)header);
//    Serial.print(" ");
//    Serial.println((char *)msg);
    
    if(!strcmp("DHTtemp",header))
    {
      DHTtemp = strdup(msg);
      DHTtempNumb = strtod(DHTtemp,NULL);
    }
    else if(!strcmp("symsout",header))
    {
      sysout = strdup(msg);
      sysoutNumb = strtod(sysout,NULL);
    } 

    
  }
  //Wait for 1 second

}
