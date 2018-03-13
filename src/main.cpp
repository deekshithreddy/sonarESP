#include <Arduino.h>
#include <ESP8266WiFi.h>

const char * SSID = "Deekshu:iPhone";
const char * PASSWORD = "123456789";
//http://localhost:9999/api/resources/index/?resource=TR_SERVICES&metrics=quality_gate_details
const char* HOST = "172.20.10.13";
const int PORT = 9009;
String STARTPATTERN = "quality_gate_details\",\"data\":\"{\\\"level\\\":\\\"";
String ENDPATTERN = "\\\",\\\"conditions";
void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(SSID);
  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void setup() {
  Serial.begin(9600);
  setup_wifi();
}

int services() {
  Serial.print("connecting to ");
  Serial.println(HOST);
  WiFiClient client2;
  if (!client2.connect(HOST, PORT)) {
    Serial.println("connection failed");
    return 0;
  }
  String url2 = "/api/resources/index/?resource=TR_SERVICES&metrics=quality_gate_details";
  Serial.print("Requesting URL: ");
  Serial.println(url2);
  client2.print(String("GET ") + url2 + " HTTP/1.1\r\n" +
               "Host: " + HOST + "\r\n" +
               "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client2.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client2 Timeout !");
      client2.stop();
    }
    return 0;
  }
  while (client2.available()) {
    String line = client2.readStringUntil('\r');
    int qg = line.indexOf(STARTPATTERN);
    int cd = line.indexOf(ENDPATTERN);
    Serial.println(line.substring(qg + STARTPATTERN.length(),cd));
    if(line.substring(qg + STARTPATTERN.length(),cd)=="OK")
    return 1;
    else
    return 0;
  }
}
int salesforce(){
  Serial.print("connecting to ");
  Serial.println(HOST);
  WiFiClient client1;
  if (!client1.connect(HOST, PORT)) {
    Serial.println("connection failed");
    return 0;
  }
  String url2 = "/api/resources/index/?resource=TR_SERVICES&metrics=quality_gate_details";
  Serial.print("Requesting URL: ");
  Serial.println(url2);
  client1.print(String("GET ") + url2 + " HTTP/1.1\r\n" +
               "Host: " + HOST + "\r\n" +
               "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client1.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client2 Timeout !");
      client1.stop();
    }
    return 0;
  }
  while (client1.available()) {
    String line = client1.readStringUntil('\r');
    int qg = line.indexOf(STARTPATTERN);
    int cd = line.indexOf(ENDPATTERN);
    Serial.println(line.substring(qg + STARTPATTERN.length(),cd));
    if(line.substring(qg + STARTPATTERN.length(),cd)=="OK")
    return 1;
    else
    return 0;
  }
}
int ledstate = 0;
void loop() {
  ledstate = services();
  Serial.println(ledstate);
  delay(5000);
  ledstate = salesforce();
  Serial.println(ledstate);
  delay(2000);
}
