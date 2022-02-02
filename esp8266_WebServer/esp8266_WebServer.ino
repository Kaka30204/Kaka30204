#include <ESP8266WiFi.h>

// เปลี่ยนตรงนี้เป็นของเครือข่ายตัวเอง
const char* ssid = "Suruay";
const char* password = "0871398136";

// ตั้งค่าพอร์ตเป็นพอร์ต 80
WiFiServer server(80);

// ประกาศตัวแปรสำหรับเก็บหน้า HTTP
String header;

// กำหนดสถานะ LED ที่แสดงบนหน้าเว็บ
String output2State = "Unlocked";
String output3State = "Unlocked";

// กำหนด Pin สำหรับ LED
const int MotorPin2 = D2;
const int MotorPin3 = D3;

void setup() {

Serial.begin(115200);

// กำหนด Pin และตั้งค่าสถานะ LOW (ไฟดับ)
pinMode(MotorPin2, OUTPUT);
pinMode(MotorPin3, OUTPUT);

digitalWrite(MotorPin2, LOW);
digitalWrite(MotorPin3, LOW);

// เชื่อมต่อกับเครือข่าย WIFI
Serial.print("Connecting to ");
Serial.println(ssid);
WiFi.begin(ssid, password);
while (WiFi.status() != WL_CONNECTED) {
delay(500);
Serial.print(".");
}

// ถ้าเชื่อมต่อสำเร็จให้แสดง IP Address
Serial.println("");
Serial.println("WiFi connected.");
Serial.println("IP address: ");
Serial.println(WiFi.localIP());

// เริ่มการทำงานของ Server
server.begin();
}


void loop(){

// รอ Client มาเชื่อมต่อ
WiFiClient client = server.available(); 

// ถ้ามี Client ใหม่มาเชื่อมต่อ
if (client) {
Serial.println("New Client."); 
String currentLine = ""; 

// เช็คสถานะว่า Cient ยังเชื่อมต่ออยู่หรือไม่
while (client.connected()) { 
if (client.available()) { 
char c = client.read(); 
Serial.write(c); // 
header += c;

if (c == '\n') {

// ถ้าไม่มีข้อมูลเข้ามาแสดงว่า Client ตัดการเชื่อมต่อไปแล้ว 
if (currentLine.length() == 0) {

client.println("HTTP/1.1 200 OK");
client.println("Content-type:text/html");
client.println("Connection: close");
client.println();

// ชุดคำสั่งในการเปิด-ปิด 
if (header.indexOf("GET /D2/locked") >= 0) {
Serial.println("GPIO 2 locked");
output2State = "locked";
digitalWrite(MotorPin2, LOW);
digitalWrite(MotorPin3, HIGH);
delay(5000)
} else if (header.indexOf("GET /D2/Unlocked") >= 0) {
Serial.println("GPIO 2 Unlocked");
output2State = "Unlocked";
digitalWrite(MotorPin2, HIGH);
digitalWrite(MotorPin3, LOW);
delay(5000)
} else if (header.indexOf("GET /D3/locked") >= 0) {
Serial.println("GPIO 3 locked");
output3State = "locked";
digitalWrite(MotorPin2, HIGH);
digitalWrite(MotorPin3, LOW);
delay(5000)
} else if (header.indexOf("GET /D3/Unlocked") >= 0) {
Serial.println("GPIO 3 Unlocked");
output3State = "Unlocked";
digitalWrite(MotorPin2, HIGH);
digitalWrite(MotorPin3, LOW);
delay(5000)..................
}

// ส่วนโค้ดแสดงหน้าเว็บที่ส่งไปให้ Client แสดง
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");

            // CSS style ของปุ่มกด
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #77878A;}</style></head>");
            
            // ส่วนหัวของหน้า
            client.println("<body><h1>Door lock IOT </h1>");
            
            // แสดงสถานะและปุ่มของ GPIO 2  
            client.println("<p>Door Status : " + output2State + "</p>");
                  
            if (output2State=="Unlocked") {
              client.println("<p><a href=\"/D2/locked\"><button class=\"button\">LOCKED</button></a></p>");
            } else {
              client.println("<p><a href=\"/D2/Unlocked\"><button class=\"button button2\">UNLOCKED</button></a></p>");
            } 
               
            // แสดงสถานะและปุ่มของ GPIO 3  
            //client.println("<p>Door Status : " + output3State + "</p>");
            
            //if (output3State=="Unlocked") {
              //client.println("<p><a href=\"/D3/locked\"><button class=\"button\">LOCKED</button></a></p>");
            //} else {
              //client.println("<p><a href=\"/D3/Unlocked\"><button class=\"button button2\">UNLOCKED</button></a></p>");
            //}
            //client.println("</body></html>");

     // ส่วนตอนปิดการทำงาน
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
    // เคลียร์ส่วน Header
    header = "";

    // ตัดการเชื่อมต่อ
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
