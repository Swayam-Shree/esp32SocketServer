#include <WiFi.h>
#include <WebSocketsClient.h>

const char* ssid = "swayam";
const char* password = "87655678";
const char* websocket_host = "wss://esp32socketserver.onrender.com/";
const uint16_t websocket_port = 6969;

WebSocketsClient webSocket;

bool connectedToServer = false;

void webSocketEvent(WStype_t type, uint8_t* payload, size_t length) {
    char* message = (char*) payload;
    Serial.println(message);
    Serial.println(connectedToServer);

    if (strcmp(message, "espInitAck") == 0) {
        connectedToServer = true;
    } else if (strcmp(message, "on") == 0) {
        digitalWrite(2, HIGH);
    } else if (strcmp(message, "off") == 0) {
        digitalWrite(2, LOW);
    }
}

void setup() {
    Serial.begin(9600);
    delay(1000);

    pinMode(2, OUTPUT);

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    Serial.println("\nConnecting");

    while(WiFi.status() != WL_CONNECTED){
        Serial.print(".");
        delay(100);
    }

    Serial.println("\nConnected to the WiFi network");
    Serial.print("Local ESP32 IP: ");
    Serial.println(WiFi.localIP());

    webSocket.beginSSL("esp32socketserver.onrender.com", 443);
    webSocket.onEvent(webSocketEvent);
}

// int connectAttempts = 10;
void loop() {
    webSocket.loop();
    if (!connectedToServer) {
        webSocket.sendTXT("espInit");
    }
}