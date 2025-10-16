#include <WiFiS3.h>
#include <WiFiUdp.h>
#include <Coap.h>

// ===== ใส่ WiFi ของคุณ =====
const char* WIFI_SSID = "YOUR_SSID";
const char* WIFI_PASS = "YOUR_PASSWORD";

// กำหนด IP ของ CoAP Server (จากตัวอย่างที่ 1)
IPAddress serverIP(192, 168, 1, 123);   // <-- แก้ให้ตรงเครือข่ายคุณ
const uint16_t COAP_PORT = 5683;

WiFiUDP udp;
Coap coap(udp);

unsigned long lastPoll = 0;
const unsigned long POLL_MS = 5000;

void onClientResponse(CoapPacket &packet, IPAddress ip, int port) {
  Serial.print("[resp] from "); Serial.print(ip);
  Serial.print(" code=");       Serial.print(packet.code);
  Serial.print(" len=");        Serial.println(packet.payloadlen);

  String payload;
  payload.reserve(packet.payloadlen);
  for (int i = 0; i < packet.payloadlen; i++) payload += (char)packet.payload[i];
  Serial.print("payload: "); Serial.println(payload);
}

void connectWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(400);
  }
}

void setup() {
  Serial.begin(115200);
  connectWiFi();

  // สำหรับ client ไม่จำเป็นต้องผูกพอร์ตเฉพาะ ใช้ 0 ให้ระบบเลือก
  udp.begin(0);
  coap.response(onClientResponse);
  coap.start();

  Serial.print("WiFi IP: "); Serial.println(WiFi.localIP());
  Serial.println("CoAP client ready.");
}

void loop() {
  coap.loop(); // รับ response

  if (millis() - lastPoll >= POLL_MS) {
    lastPoll = millis();

    Serial.println("GET /ping");
    coap.get(serverIP, COAP_PORT, "ping");

    delay(500);

    Serial.println("GET /status");
    coap.get(serverIP, COAP_PORT, "status");

    // สาธิตสั่ง LED=on/off สลับทุกครั้ง
    static bool on = false;
    on = !on;
    const char* body = on ? "on" : "off";
    Serial.print("PUT /led "); Serial.println(body);
    coap.put(serverIP, COAP_PORT, "led", (uint8_t*)body, strlen(body));
  }
}
