#include <WiFiS3.h>
#include <WiFiUdp.h>
#include <Coap.h>

// ===== ใส่ WiFi ของคุณ =====
const char* WIFI_SSID = "YOUR_SSID";
const char* WIFI_PASS = "YOUR_PASSWORD";

const uint16_t COAP_PORT = 5683;
const int LED_PIN = LED_BUILTIN;

WiFiUDP udp;
Coap coap(udp);

// ---------- Utilities ----------
void connectWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(400);
  }
  // ตัวเลือก: กำหนด hostname (ถ้ารองรับ)
  // WiFi.setHostname("uno-r4-coap");
}

// ---------- Handlers ----------
void handlePing(CoapPacket &packet, IPAddress ip, int port) {
  const char* msg = "pong";
  coap.sendResponse(packet, (uint8_t*)msg, strlen(msg), COAP_CONTENT, COAP_TEXT_PLAIN);
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
    coap.sendResponse(packet, (uint8_t*)bad, strlen(bad), COAP_BAD_REQUEST, COAP_TEXT_PLAIN);
    return;
  }

  String resp = String("LED=") + (digitalRead(LED_PIN) ? "on" : "off");
  coap.sendResponse(packet, (uint8_t*)resp.c_str(), resp.length(), COAP_CHANGED, COAP_TEXT_PLAIN);
}

void handleStatus(CoapPacket &packet, IPAddress ip, int port) {
  String s = "ip=" + WiFi.localIP().toString() + ", rssi=" + String(WiFi.RSSI());
  coap.sendResponse(packet, (uint8_t*)s.c_str(), s.length(), COAP_CONTENT, COAP_TEXT_PLAIN);
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
