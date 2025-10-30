#include <WiFi.h>
#include <PubSubClient.h>
#include <SPI.h>
// #include <MFRC522.h> // Biblioteca RFID (para uso real futuramente)

// ====================== CONFIGURAÇÕES DE REDE ======================
const char* ssid = "Wokwi-GUEST";
const char* password = "";
const char* mqttServer = "maqiatto.com";
const int mqttPort = 1883;
const char* mqttUser = "ludanielle2005@hotmail.com";
const char* mqttPassword = "FIAP555292"; 
const char* mqttTopic = "ludanielle2005@hotmail.com/vagas";

// ====================== CONFIGURAÇÕES DO SENSOR ======================
const int trigPin = 2;
const int echoPin = 4;
const int ledVaga = 5;

// ====================== CONFIGURAÇÃO DO RFID (Futuro Real) ======================
// #define SS_PIN 5
// #define RST_PIN 22
// MFRC522 rfid(SS_PIN, RST_PIN);

// ====================== VARIÁVEIS GLOBAIS ======================
#define NUM_LEITURAS 5
float historico[NUM_LEITURAS];
int indice = 0;
bool vagaOcupada = false;
WiFiClient espClient;
PubSubClient client(espClient);

// ====================== FUNÇÕES ======================
void setupWiFi() {
  WiFi.begin(ssid, password);
  Serial.print("Conectando ao Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("✅ Conectado!");
}

void reconnectMQTT() {
  while (!client.connected()) {
    Serial.print("Conectando ao broker Maqiatto...");
    if (client.connect("ESP32Vaga1", mqttUser, mqttPassword)) {
      Serial.println("✅ Conectado ao Maqiatto!");
    } else {
      Serial.print("❌ Falha, rc=");
      Serial.print(client.state());
      delay(2000);
    }
  }
}

float medirDistancia() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH);
  return duration * 0.034 / 2;
}

float medirDistanciaFiltrada() {
  historico[indice] = medirDistancia();
  indice = (indice + 1) % NUM_LEITURAS;
  float soma = 0;
  for (int i = 0; i < NUM_LEITURAS; i++)
    soma += historico[i];
  return soma / NUM_LEITURAS;
}

long lerRFIDSerial() {
  if (Serial.available()) {
    delay(100);
    String idStr = Serial.readStringUntil('\n');
    idStr.trim();

    if (idStr.length() == 0) return -1;

    for (int i = 0; i < idStr.length(); i++) {
      if (!isDigit(idStr.charAt(i))) {
        Serial.println("❌ Entrada inválida. Digite apenas números.");
        return -1;
      }
    }

    return idStr.toInt();
  }
  return -1;
}

void atualizarLedStatus(bool ocupada) {
  digitalWrite(ledVaga, ocupada ? HIGH : LOW);
}

void enviarMensagem(String payload) {
  if (!client.connected()) reconnectMQTT();
  client.loop();
  client.publish(mqttTopic, payload.c_str());
  Serial.print("📤 Enviado: ");
  Serial.println(payload);
}

// ====================== SETUP ======================
void setup() {
  Serial.begin(115200);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledVaga, OUTPUT);

  setupWiFi();
  client.setServer(mqttServer, mqttPort);
  reconnectMQTT();

  Serial.println("Sistema iniciado. Aguardando moto...");
  Serial.println("Simulação RFID: Digite um ID numérico e pressione Enter.");
}

// ====================== LOOP PRINCIPAL ======================
void loop() {
  float distancia = medirDistanciaFiltrada();
  bool detectouMoto = distancia < 50;
  long idLido = lerRFIDSerial();

  // ===== Caso 1: Moto detectada e ID lido =====
  if (detectouMoto && idLido != -1) {
    vagaOcupada = true;
    atualizarLedStatus(true);
    String msg = "{\"id\":" + String(idLido) + ",\"vaga\":1}";
    enviarMensagem(msg);
    Serial.printf("✅ Moto estacionada com ID: %ld\n", idLido);
  }

  // ===== Caso 2: Moto saiu da vaga =====
  if (!detectouMoto && vagaOcupada) {
    Serial.println("❌ Moto saiu da vaga.");
    vagaOcupada = false;
    atualizarLedStatus(false);
  }

  delay(1500);
}