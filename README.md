# 🏍️ Sistema de Monitoramento de Vagas para Motos com IoT

Este projeto demonstra uma solução baseada em **Internet das Coisas (IoT)** para monitorar vagas de motos em tempo real. Utilizando sensores, ESP32, protocolo **MQTT** e um backend com **FastAPI**, o sistema oferece uma visão clara sobre a ocupação do pátio de motos.

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

Automatizar o monitoramento das vagas de estacionamento utilizando tecnologias de IoT, oferecendo um **painel de visualização em tempo real**, reduzindo erros humanos e otimizando a operação.

---

## 🧰 Tecnologias Utilizadas

| Tecnologia           | Função                                                                 |
|----------------------|------------------------------------------------------------------------|
| ESP32 (Wokwi)        | Simulação de microcontrolador para leitura de sensores                 |
| Sensor Ultrassônico  | Detecta a presença de motos em vagas                                   |
| MQTT                 | Protocolo leve de comunicação entre dispositivos IoT                   |
| Mosquitto (Broker)   | Gerencia a troca de mensagens MQTT                                     |
| FastAPI (Python)     | Backend para processar dados e servir o dashboard                      |
| HTML/CSS (Dashboard) | Exibição dos dados de forma visual                                     |

---

## 🔄 Arquitetura

```text
Sensor Ultrassônico
        │
        ▼
     ESP32 (Wokwi)
        │
        ▼
Publica via MQTT (tópico ex: /vaga1)
        │
        ▼
   Broker MQTT (Mosquitto)
        │
        ▼
 FastAPI (Subscriber MQTT + API)
        │
        ▼
    Dashboard Web (Status das Vagas)
```

---

## 🚀 Funcionalidades

- Leitura de distância para detectar presença de motos.
- Envio de dados via MQTT.
- Backend em FastAPI que escuta os tópicos e atualiza os dados.
- Dashboard simples com status "LIVRE" ou "OCUPADA" em tempo real.

---

## ✅ Como Executar (Modo Desenvolvimento)

### 1. Clonar o repositório
```bash
git clone https://github.com/seu-usuario/seu-repo.git
cd seu-repo
```

### 2. Criar e ativar o ambiente virtual
```bash
python -m venv venv
source venv/bin/activate  # Linux/Mac
venv\Scripts\activate     # Windows
```

### 3. Instalar as dependências
```bash
pip install fastapi uvicorn paho-mqtt
```

### 4. Rodar o servidor
```bash
uvicorn main:app --reload
```

---