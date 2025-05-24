//Código usado na simulação do protótipo de sensor de distância ultrassônico HC-SR04
#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "Wokwi-GUEST";
const char* password = "";
const char* mqttServer = "broker.hivemq.com";
const int mqttPort = 1883;
const char* mqttTopic = "estacionamento/vaga1";

WiFiClient espClient;
PubSubClient client(espClient);

const int trigPin = 2;
const int echoPin = 4;
const int ledVaga = 5;

#define NUM_LEITURAS 5
float historico[NUM_LEITURAS];
int indice = 0;

void setupWiFi() {
  WiFi.begin(ssid, password);
  Serial.print("Conectando ao Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" Conectado!");
}

void reconnectMQTT() {
  while (!client.connected()) {
    Serial.print("Conectando ao broker MQTT...");
    if (client.connect("ESP32Client")) {
      Serial.println("Conectado!");
    } else {
      Serial.print("Erro: ");
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
  for (int i = 0; i < NUM_LEITURAS; i++) soma += historico[i];
  return soma / NUM_LEITURAS;
}

void atualizarLedStatus(bool ocupada) {
  static unsigned long ultimoToggle = 0;
  if (!client.connected()) {
    if (millis() - ultimoToggle > 200) {
      digitalWrite(ledVaga, !digitalRead(ledVaga));
      ultimoToggle = millis();
    }
  } else {
    digitalWrite(ledVaga, ocupada ? HIGH : LOW);
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledVaga, OUTPUT);

  setupWiFi();
  client.setServer(mqttServer, mqttPort);
}

void loop() {
  if (!client.connected()) reconnectMQTT();
  client.loop();

  float distancia = medirDistanciaFiltrada();
  bool ocupada = distancia < 50;
  atualizarLedStatus(ocupada);

  String mensagem = "{\"vaga\":1,\"ocupada\":" + String(ocupada ? "true" : "false") +
                    ",\"timestamp\":" + String(millis() / 1000) + "}";
  Serial.println("Enviando: " + mensagem);
  client.publish(mqttTopic, mensagem.c_str());

  delay(2000);
}