//con mqtt local

#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define RELEPIN 5
#define BME_SDA 21
#define BME_SCL 22

// Credenciales WiFi
#define WIFI_SSID "Prueba"
#define WIFI_PASSWORD "pollofrito"

// Servidor MQTT (Usa un broker público como test.mosquitto.org)
#define MQTT_SERVER "test.mosquitto.org"
#define MQTT_PORT 1883
#define MQTT_USER ""
#define MQTT_PASSWORD ""

// Crear instancias de WiFi y MQTT
WiFiClient espClient;
PubSubClient client(espClient);
Adafruit_BME280 bme;

// Función para conectar a WiFi
void setupWifi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Conectando a WiFi ");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" ¡Conectado!");
}

// Función para conectar a MQTT
void reconnectMQTT() {
  while (!client.connected()) {
    Serial.print("Conectando a MQTT...");
    if (client.connect("esp32_5c795f6e", MQTT_USER, MQTT_PASSWORD)) {
      Serial.println(" ¡Conectado!");
      client.subscribe("esp32/rele");  // Suscribirse para controlar el relé
    } else {
      Serial.print(" Error, rc=");
      Serial.print(client.state());
      Serial.println(" Reintentando en 5 segundos...");
      delay(5000);
    }
  }
}

// Función para recibir mensajes MQTT
void callback(char* topic, byte* message, unsigned int length) {
  String msg;
  for (int i = 0; i < length; i++) {
    msg += (char)message[i];
  }
  
  Serial.print("Mensaje recibido en ");
  Serial.print(topic);
  Serial.print(": ");
  Serial.println(msg);

  // Si el mensaje es "1", encender relé, si es "0", apagar
  if (String(topic) == "esp32/rele") {
    if (msg == "1") {
      digitalWrite(RELEPIN, HIGH);
    } else if (msg == "0") {
      digitalWrite(RELEPIN, LOW);
    }
  }
}

void sendSensorData() {
  float temp = bme.readTemperature();
  float humidity = bme.readHumidity();
  float pressure = bme.readPressure() / 100.0F;  // hPa

  // Publicar en MQTT
  client.publish("esp32/sensores/temperatura", String(temp).c_str());
  client.publish("esp32/sensores/humedad", String(humidity).c_str());
  client.publish("esp32/sensores/presion", String(pressure).c_str());

  // Enviar mensaje para controlar el relé automáticamente
  if (temp > 28) {
    client.publish("esp32/rele", "1");
  } else {
    client.publish("esp32/rele", "0");
  }

  Serial.println("Datos enviados a MQTT");
}

void setup() {
  Serial.begin(115200);
  setupWifi();
  client.setServer(MQTT_SERVER, MQTT_PORT);
  client.setCallback(callback);

  pinMode(RELEPIN, OUTPUT);
  digitalWrite(RELEPIN, LOW);
  
  Wire.begin(BME_SDA, BME_SCL);
  if (!bme.begin(0x76)) {
    Serial.println("No se encontró el BME280. ¡Revisa conexiones!");
    while (1);
  }
}

void loop() {
  if (!client.connected()) {
    reconnectMQTT();
  }
  client.loop();
  
  sendSensorData();
  delay(5000);
}