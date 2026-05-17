#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <WEMOS_SHT3X.h>
#include <Wire.h>

// ================= WIFI =================
const char* ssid = "WIFI_SSID";
const char* password = "WIFI_PASSWORD";

// ================= MQTT =================
const char* mqtt_server = "MQTT_SERVER";

WiFiClient espClient;
PubSubClient client(espClient);

// ================= SENSOR =================
SHT3X sht30(0x45);

// ================= WIFI SETUP =================
void setup_wifi() {
  Serial.print("Connexion WiFi...");

  IPAddress local_IP(ESP_LOCAL_IP);
  IPAddress gateway(GATEWAY_IP);
  IPAddress subnet(SUBNET_MASK);

  WiFi.config(local_IP, gateway, subnet);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connecté !");
  Serial.print("IP ESP: ");
  Serial.println(WiFi.localIP());
}

// ================= MQTT RECONNECT (AMÉLIORÉ) =================
void reconnect() {
  while (!client.connected()) {
    Serial.print("Connexion MQTT...");

    if (client.connect("ESP8266_Client")) {
      Serial.println("OK");

      // message de statut
      client.publish("iot/status", "ESP connecté");
    } else {
      Serial.print("FAIL, rc=");
      Serial.print(client.state());
      Serial.println(" retry dans 2s...");
      delay(2000);
    }
  }
}

// ================= SETUP =================
void setup() {
  Serial.begin(115200);
  Wire.begin(D2, D1);

  delay(1000);
  sht30.get();

  setup_wifi();

  client.setServer(mqtt_server, 1883);
}

// ================= LOOP =================
void loop() {

  if (!client.connected()) {
    reconnect();
  }

  client.loop();

  // ================= SENSOR READ =================
  if (sht30.get() == 0) {

    float temp = sht30.cTemp;
    float hum = sht30.humidity;

    char tempStr[10];
    char humStr[10];

    dtostrf(temp, 1, 2, tempStr);
    dtostrf(hum, 1, 2, humStr);

    // ================= MQTT PUBLISH =================
    client.publish("iot/weather/temp", tempStr);
    client.publish("iot/weather/hum", humStr);

    // ================= SERIAL DEBUG =================
    Serial.println("------ DATA ------");
    Serial.print("Temp: ");
    Serial.println(tempStr);

    Serial.print("Hum: ");
    Serial.println(humStr);

  } else {
    Serial.println("Erreur capteur ❌");
  }

  delay(2000);
}