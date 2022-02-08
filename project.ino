#include <ESP8266WiFi.h>

const char* ssid = "Kaka";
const char* password = "Khunnaphat2547";

WiFiServer server(80);

String header;

String output2State = "Unlocked";
String output3State = "Unlocked";

const int MotorPin2 = D2;
const int MotorPin3 = D3;

unsigned long previousMillis = 0;

const long interval = 1000;

void setup() {

Serial.begin(115200);

pinMode(MotorPin2, OUTPUT);
pinMode(MotorPin3, OUTPUT);

digitalWrite(MotorPin2, LOW);
digitalWrite(MotorPin3, LOW);

Serial.print("Connecting to ");
Serial.println(ssid);
WiFi.begin(ssid, password);
while (WiFi.status() != WL_CONNECTED) {
delay(500);
Serial.print(".");
}

Serial.println("");
Serial.println("WiFi connected.");
Serial.println("IP address: ");
Serial.println(WiFi.localIP());

server.begin();
}

void loop(){

WiFiClient client = server.available(); 

if (client) {
Serial.println("New Client."); 
String currentLine = ""; 

while (client.connected()) { 
if (client.available()) { 
char c = client.read(); 
Serial.write(c); // 
header += c;

if (c == '\n') {

if (currentLine.length() == 0) {

client.println("HTTP/1.1 200 OK");
client.println("Content-type:text/html");
client.println("Connection: close");
client.println();

if (header.indexOf("GET /D2/locked") >= 0) {
Serial.println("GPIO 2 locked");
output2State = "locked";
digitalWrite(MotorPin2, LOW);
digitalWrite(MotorPin3, HIGH);
delay(7000);
digitalWrite(MotorPin2, LOW);
digitalWrite(MotorPin3, LOW);
} else if (header.indexOf("GET /D2/Unlocked") >= 0) {
Serial.println("GPIO 2 Unlocked");
output2State = "Unlocked";
digitalWrite(MotorPin2, HIGH);
digitalWrite(MotorPin3, LOW);
delay(7000);
digitalWrite(MotorPin2, LOW);
digitalWrite(MotorPin3, LOW);
}

            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");

            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #77878A;}</style></head>");
            
            client.println("<body><h1>Door lock IOT </h1>");
            
            client.println("<p>Door Status : " + output2State + "</p>");
                  
            if (output2State=="Unlocked") {
              client.println("<p><a href=\"/D2/locked\"><button class=\"button\">LOCKED</button></a></p>");
            } else {
              client.println("<p><a href=\"/D2/Unlocked\"><button class=\"button button2\">UNLOCKED</button></a></p>");
            } 
               
            //client.println("<p>Door Status : " + output3State + "</p>");
            
            //if (output3State=="Unlocked") {
              //client.println("<p><a href=\"/D3/locked\"><button class=\"button\">LOCKED</button></a></p>");
            //} else {
              //client.println("<p><a href=\"/D3/Unlocked\"><button class=\"button button2\">UNLOCKED</button></a></p>");
            //}
            //client.println("</body></html>");

            client.println();
            
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {  
          currentLine += c;      
        }
      }
    }
    header = "";

    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
