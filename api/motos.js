import { client } from "../mqtt/broker.js";
import { api } from "./api.js";
import dotenv from "dotenv";

dotenv.config();

const TOPIC_MOTOS = process.env.MQTT_TOPIC_MOTOS || "";

export function listenMotos() {
  client.subscribe(TOPIC_MOTOS, (err) => {
    if (err) return console.error("❌ Erro ao inscrever no tópico de motos:", err);
    console.log(`📡 Inscrito no tópico: ${TOPIC_MOTOS}`);
  });

  client.on("message", async (topic, message) => {
    if (topic !== TOPIC_MOTOS) return;
    try {
      const data = JSON.parse(message.toString());
      console.log("📥 [MOTO] Mensagem recebida:", data);

      if (!data.id) {
        console.warn("⚠️ Ignorado — sem ID da moto");
        return;
      }

      const response = await api.put(`/${data.id}`, data);
      console.log("✅ Moto atualizada no backend:", response.status);
    } catch (err) {
      console.error("❌ Erro ao processar mensagem de moto:", err.message);
    }
  });
}
