//UTS LISTY 

#include "WiFi.h"
#include "PubSubClient.h"
#include "DHT.h"

// Konfigurasi WiFi
const char* ssid = "Wokwi-GUEST";
const char* pass = "";

// Konfigurasi broker MQTT
const char* mqtt_server = "broker.hivemq.com"; // Ganti dengan alamat broker Anda
const int mqtt_port = 1883;
const char* mqtt_client_id = "ESP32_Client";
const char* topic_suhu = "suhu002";
const char* topic_kelembaban = "kelembaban002";
const char* topic_relay = "pompa002";

// Konfigurasi sensor DHT
#define DHTPIN 4         // Pin DHT22 ke pin 4 di ESP32
#define DHTTYPE DHT22    // Jenis sensor DHT22

// Konfigurasi pin
#define LED_RED_PIN 14    // LED Merah ke pin 14
#define LED_YELLOW_PIN 5  // LED Kuning ke pin 5
#define LED_GREEN_PIN 33  // LED Hijau ke pin 33
#define BUZZER_PIN 18     // Buzzer ke pin 18
#define RELAY_PIN 2       // Relay ke pin 2

DHT dht(DHTPIN, DHTTYPE); // Inisialisasi objek DHT
WiFiClient espClient;
PubSubClient client(espClient);

bool previousPumpStatus = LOW; // Status relay sebelumnya

void setup_wifi() {
  delay(10);
  Serial.println("Menghubungkan ke WiFi...");
  WiFi.begin(ssid, pass);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("\nTerhubung ke WiFi");
  Serial.print("Alamat IP: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Menghubungkan ke MQTT...");
    if (client.connect(mqtt_client_id)) {
      Serial.println("Terhubung ke MQTT");
    } else {
      Serial.print("Gagal, rc=");
      Serial.print(client.state());
      Serial.println(" coba lagi dalam 5 detik");
      delay(5000);
    }
  }
}

void setup() {
  // Setup Serial Monitor
  Serial.begin(115200);
  
  setup_wifi();  // Menghubungkan ke WiFi
  client.setServer(mqtt_server, mqtt_port);

  // Setup sensor DHT22
  dht.begin();

  // Setup pin
  pinMode(LED_RED_PIN, OUTPUT);
  pinMode(LED_YELLOW_PIN, OUTPUT);
  pinMode(LED_GREEN_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Membaca suhu dan kelembaban
  float suhu = dht.readTemperature();
  float kelembaban = dht.readHumidity();

  if (isnan(suhu) || isnan(kelembaban)) {
    Serial.println("Gagal membaca sensor!");
    return;
  }

  // Menampilkan suhu dan kelembaban di Serial Monitor
  Serial.print("Suhu: ");
  Serial.print(suhu);
  Serial.print(" C\t");
  Serial.print("Kelembaban: ");
  Serial.print(kelembaban);
  Serial.println(" %");

  // Mengirim data suhu dan kelembaban ke broker MQTT
  client.publish(topic_suhu, String(suhu).c_str());
  client.publish(topic_kelembaban, String(kelembaban).c_str());

  // Mengontrol LED dan Buzzer berdasarkan suhu
  if (suhu > 35) {
    digitalWrite(LED_RED_PIN, HIGH);    // Menyalakan LED Merah
    digitalWrite(LED_YELLOW_PIN, LOW);
    digitalWrite(LED_GREEN_PIN, LOW);
    digitalWrite(BUZZER_PIN, HIGH); // Menyalakan Buzzer
  } else if (suhu >= 30 && suhu <= 35) {
    digitalWrite(LED_YELLOW_PIN, HIGH); // Menyalakan LED Kuning
    digitalWrite(LED_RED_PIN, LOW);
    digitalWrite(LED_GREEN_PIN, LOW);
    digitalWrite(BUZZER_PIN, LOW);  // Mematikan Buzzer
  } else {
    digitalWrite(LED_GREEN_PIN, HIGH);  // Menyalakan LED Hijau
    digitalWrite(LED_RED_PIN, LOW);
    digitalWrite(LED_YELLOW_PIN, LOW);
    digitalWrite(BUZZER_PIN, LOW);  // Mematikan Buzzer
  }

  // Mengontrol pompa berdasarkan kelembaban
  bool pumpStatus = kelembaban < 40; // Relay ON jika kelembaban di bawah 40
  digitalWrite(RELAY_PIN, pumpStatus ? HIGH : LOW);

  // Mengirim status relay ke MQTT jika statusnya berubah
  if (pumpStatus != previousPumpStatus) {
    client.publish(topic_relay, pumpStatus ? "ON" : "OFF");
    previousPumpStatus = pumpStatus;
  }

  delay(2000);
}