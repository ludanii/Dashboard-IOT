const config = {
  mqtt: {
    host: 'broker.hivemq.com',
    port: 8884,
    topic: 'estacionamento/vaga1'
  },
  vagaTemplate: `
    <div class="card">
      <div class="card-header">
        <div class="vaga-icon">🚗</div>
        <div class="vaga-info">
          <h2>Vaga <span class="vaga-numero">1</span></h2>
          <p>Monitoramento em tempo real</p>
        </div>
      </div>
      <div class="status" id="status">
        <span class="status-text">AGUARDANDO DADOS...</span>
      </div>
      <div class="details">
        <div class="detail-item">
          <i class="material-icons">straighten</i>
          <div>Distância</div>
          <strong class="distancia">--</strong> cm
        </div>
        <div class="detail-item">
          <i class="material-icons">access_time</i>
          <div>Última atualização</div>
          <strong class="hora">--:--:--</strong>
        </div>
      </div>
    </div>
  `
};

const dashboard = document.getElementById('dashboard');
dashboard.innerHTML = config.vagaTemplate;

let client = null;

function connectMQTT() {
  if (client && client.connected) {
    console.log("Já conectado ao MQTT, ignorando nova conexão.");
    return;
  }

  client = mqtt.connect(`wss://${config.mqtt.host}:${config.mqtt.port}/mqtt`, {
    clientId: 'dashboard-' + Math.random().toString(16).substr(2, 8)
  });

  client.on("connect", () => {
    console.log("MQTT conectado ✅");
    client.subscribe(config.mqtt.topic, (err) => {
      if (err) {
        console.error("Erro ao se inscrever no tópico:", err);
        atualizarStatusConexao(false);
      } else {
        atualizarStatusConexao(true);
      }
    });
  });

  client.on("message", (topic, message) => {
    try {
      const data = JSON.parse(message.toString());
      updateDashboard(data);
      atualizarStatusConexao(true);
    } catch (e) {
      console.error("Erro ao analisar mensagem:", e);
    }
  });

  client.on("error", (err) => {
    console.error("Erro MQTT:", err);
    atualizarStatusConexao(false);
  });

  client.on("close", () => {
    console.warn("MQTT desconectado ❌");
    atualizarStatusConexao(false);
  });
}


function atualizarStatusConexao(ativo) {
  const dot = document.getElementById("connectionDot");
  const text = document.getElementById("connectionText");

  if (ativo) {
    dot.className = "connection-dot connected";
    text.textContent = "Conectado";
  } else {
    dot.className = "connection-dot disconnected";
    text.textContent = "Desconectado";
  }
}


function updateDashboard(data) {
  const card = dashboard.querySelector('.card');
  card.querySelector('.vaga-numero').textContent = data.vaga || '1';

  const statusElement = card.querySelector('.status');
  const statusText = statusElement.querySelector('.status-text');
  statusText.textContent = data.ocupada ? 'OCUPADA' : 'LIVRE';
  statusElement.className = `status ${data.ocupada ? 'ocupada' : 'livre'}`;

  card.querySelector('.distancia').textContent =
    data.distancia ? `${data.distancia.toFixed(2)} cm` : (data.ocupada ? '0-50 cm' : '>50 cm');

  card.querySelector('.hora').textContent = new Date().toLocaleTimeString();
}
window.addEventListener('DOMContentLoaded', () => {
  connectMQTT();
});
