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
const char* mqttTopic = "ludanielle2005@hotmail.com/motos";

// ====================== CONFIGURAÇÕES DE HARDWARE ======================
const int PIR_SENSOR_PIN = 21;
int PIR_STATUS = 0;
int valorAnterior = -1;

// ====================== CONFIGURAÇÃO DO RFID (Futuro Real) ======================
// #define SS_PIN 5
// #define RST_PIN 22
// MFRC522 rfid(SS_PIN, RST_PIN);

// ====================== VARIÁVEIS GLOBAIS ======================
WiFiClient espClient;
PubSubClient client(espClient);

int idMoto = 0;
String tagManutencao = "999999";
bool emManutencao = false;

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
    Serial.print("Conectando ao broker MQTT...");
    if (client.connect("MotoDevice", mqttUser, mqttPassword)) {
      Serial.println("✅ Conectado ao broker!");
    } else {
      Serial.print("❌ Falha, rc=");
      Serial.print(client.state());
      delay(2000);
    }
  }
}

void enviarMensagem(String status) {
  if (!client.connected()) reconnectMQTT();
  client.loop();

  String payload = "{\"id\":" + String(idMoto) + ",\"status\":\"" + status + "\"}";
  client.publish(mqttTopic, payload.c_str());
  Serial.println("📤 Enviado para broker: " + payload);
}

String lerRFIDSerial() {
  if (Serial.available()) {
    delay(100);
    String id = Serial.readStringUntil('\n');
    id.trim();

    for (int i = 0; i < id.length(); i++) {
      if (!isDigit(id.charAt(i))) {
        Serial.println("❌ Tag inválida. Use apenas números.");
        return "";
      }
    }
    return id;
  }
  return "";
}

void verificarMovimento() {
  PIR_STATUS = digitalRead(PIR_SENSOR_PIN);

  if (PIR_STATUS != valorAnterior) {
    valorAnterior = PIR_STATUS;

    if (emManutencao) {
      Serial.println("⚙️ Modo manutenção — não envia status de movimento.");
      return;
    }

    if (PIR_STATUS == HIGH) {
      Serial.println("🏍️ Movimento detectado → EM_USO");
      enviarMensagem("EM_USO");
    } else {
      Serial.println("🛑 Movimento não detectado → PARADA");
      enviarMensagem("PARADA");
    }
  }
}

// ====================== SETUP ======================
void setup() {
  Serial.begin(115200);
  pinMode(PIR_SENSOR_PIN, INPUT);

  Serial.println("===== Sistema IoT - Moto Inteligente =====");
  Serial.println("Digite o ID da moto para iniciar:");

  setupWiFi();
  client.setServer(mqttServer, mqttPort);
  reconnectMQTT();
}

// ====================== LOOP PRINCIPAL ======================
void loop() {
  if (idMoto == 0) {
    String entrada = lerRFIDSerial();
    if (entrada != "") {
      idMoto = entrada.toInt();
      Serial.print("✅ ID da moto configurado: ");
      Serial.println(idMoto);
      Serial.println("Digite '999999' para simular tag de manutenção.");
    }
    delay(500);
    return;
  }

  String tag = lerRFIDSerial();
  if (tag != "") {
    if (tag == tagManutencao) {
      emManutencao = true;
      Serial.println("🔧 Tag de manutenção detectada!");
      enviarMensagem("MANUTENCAO");
      delay(3000);
      return;
    } else {
      Serial.println("ℹ️ Tag desconhecida ignorada.");
    }
  }

  verificarMovimento();

  delay(500);
}
