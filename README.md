# 🏍️ Sistema de Monitoramento de Vagas para Motos com IoT

Este projeto demonstra uma solução baseada em **Internet das Coisas (IoT)** para monitorar vagas e status de motos em tempo real. Utilizando sensores, ESP32, protocolo **MQTT** e um backend com **Node**, o sistema oferece uma visão clara sobre a ocupação do pátio e status de motos.

---
## 👩‍💻 Integrantes

**Luisa Danielle**  RM 555292

**Ana Carolina de Castro**  RM 554669

**Michelle Ponteza**  RM 557702

## 📌 Problema Real

Pátios de motos frequentemente enfrentam problemas como:

- ❌ Falta de controle sobre onde as motos estão estacionadas e seu estado atual.
- ❌ Dificuldade em manter as informações de disponibilidade atualizadas.
- ❌ Risco de desorganização, retrabalho, perda de tempo e confusão entre funcionários e clientes.

---

## 🎯 Objetivo da Solução

Automatizar o monitoramento das vagas de estacionamento e status das motos utilizando tecnologias de IoT, oferecendo um **painel de visualização em tempo real**, reduzindo erros humanos e otimizando a operação.

---

## 🧰 Tecnologias Utilizadas

| Tecnologia           | Função                                                                 |
|----------------------|------------------------------------------------------------------------|
| ESP32 (Wokwi)        | Simulação de microcontrolador para leitura de sensores                 |
| Sensor Ultrassônico  | Detecta a presença de motos em vagas                                   |
| Sensor de Movimento  | Detecta a movimentação das motos 
| RFID ⚠️   | Recebimento e envio de tags ( atualmente somente simulação - Wokwi não oferece o componente)                                  |
| MQTT                 | Protocolo leve de comunicação entre dispositivos IoT                   |
| Maqiatto (Broker)   | Gerencia a troca de mensagens MQTT                                     |
| [app.js](./app.js) (Node)     | Arquivo servir de subscrição do broker e chamar a API JAVA                      |
| BACKEND JPA | Tratamento de dados e atualização de banco de dados
| FRONTEND MOBILE | Visualização de dados integrado com a API de JAVA |

---

## 🔄 Arquitetura

```text
     Sensor
        │
        ▼
     ESP32 (Wokwi)
        │
        ▼
 Publica via MQTT
        │
        ▼
 Broker MQTT (Maqiatto)
        │
        ▼
 NODE (Subscrição MQTT + API JAVA)
        │
        ▼
   BACKEND JPA 
        │
        ▼
 FRONTEND MOBILE
```

---

## 🚀 Funcionalidades

- Leitura de distância para detectar presença de motos.
- Envio de dados via MQTT.
- Envio de requisições para a API de JAVA via node.
- Tratamento de dados e atualização de banco de dados. (JAVA)
- Visualização de dados em frontend integrado com a API de JAVA. (MOBILE)

---

## ✅ Como Executar (Modo Desenvolvimento)

### 1. Instalar as dependências
```bash
npm i
```
### 2. Rodar backend java
[Aplicação java](https://dashboard.render.com/web/srv-d42f9bripnbc73bpha90)

### 3. Rodar o servidor
```bash
node app.js
```
### 4. Rodar simulador
- [Dispositivo Moto](https://wokwi.com/projects/446267595641899009)
- [Dispositivo Vaga](https://wokwi.com/projects/443549619825989633)

---