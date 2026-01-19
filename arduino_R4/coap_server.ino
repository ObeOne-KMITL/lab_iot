#include <WiFiS3.h>
#include <WiFiUdp.h>
#include <coap-simple.h>

// ===== ใส่ WiFi SSID, Password ของตัวเอง =====
const char WIFI_SSID[] = "panwit_2.4G";     // CHANGE TO YOUR WIFI SSID
const char WIFI_PASSWORD[] = "0618265454";  // CHANGE TO YOUR WIFI PASSWORD

const uint16_t COAP_PORT = 5683;
const int LED_PIN = LED_BUILTIN;

WiFiUDP udp;
Coap coap(udp);

// ---------- Utilities ----------
void connectWiFi() {
 
  int status = WL_IDLE_STATUS;
  while (status != WL_CONNECTED) {
    Serial.print("Arduino UNO R4 - Attempting to connect to SSID: ");
    Serial.println(WIFI_SSID);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    // wait 10 seconds for connection:
    delay(10000);
  }
  // print your board's IP address:
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

}

// ---------- Handlers ----------

void handlePing(CoapPacket &packet, IPAddress ip, int port) {
  const char* msg = "pong";
  coap.sendResponse(ip, port, packet.messageid, msg);
}

void handleLed(CoapPacket &packet, IPAddress ip, int port) {
  // อ่าน payload
  String payload;
  payload.reserve(packet.payloadlen);
  for (int i = 0; i < packet.payloadlen; i++) payload += (char)packet.payload[i];
  payload.trim();

  if (payload.equalsIgnoreCase("on")) {
    digitalWrite(LED_PIN, HIGH);
  } else if (payload.equalsIgnoreCase("off")) {
    digitalWrite(LED_PIN, LOW);
  } else {
    const char* bad = "use: on|off";
    coap.sendResponse(ip, port, packet.messageid, bad);
    return;
  }

  String resp = String("LED=") + (digitalRead(LED_PIN) ? "on" : "off");
  char messageBuffers[100];
  resp.toCharArray(messageBuffers, 100);
  coap.sendResponse(ip, port, packet.messageid, messageBuffers);

}

void handleStatus(CoapPacket &packet, IPAddress ip, int port) {
  String s = "ip=" + WiFi.localIP().toString() + ", rssi=" + String(WiFi.RSSI());
  char messageBuffer[100];
  s.toCharArray(messageBuffer, 100);
  coap.sendResponse(ip, port, packet.messageid, messageBuffer);
  
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  Serial.begin(115200);
  connectWiFi();

  udp.begin(COAP_PORT);

  // ลงทะเบียน resource
  coap.server(handlePing,  "ping");    // GET  coap://<ip>/ping
  coap.server(handleLed,   "led");     // PUT  coap://<ip>/led
  coap.server(handleStatus,"status");  // GET  coap://<ip>/status
  coap.start();

  Serial.print("WiFi IP: ");   Serial.println(WiFi.localIP());
  Serial.println("CoAP server ready @ udp/5683   resources: /ping /led /status");
}

void loop() {
  coap.loop();  // ประมวลผลคำขอที่เข้ามา
}
