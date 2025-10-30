import { client } from "../mqtt/broker";
import { api } from "./api";
import dotenv from "dotenv";

dotenv.config();

const TOPIC_VAGAS = process.env.MQTT_TOPIC_VAGAS || "";

export function listenVagas() {
  client.subscribe(TOPIC_VAGAS, (err) => {
    if (err) return console.error("❌ Erro ao inscrever no tópico de vagas:", err);
    console.log(`📡 Inscrito no tópico: ${TOPIC_VAGAS}`);
  });

  client.on("message", async (topic, message) => {
    if (topic !== TOPIC_VAGAS) return;
    try {
      const data = JSON.parse(message.toString());
      console.log("📥 [VAGA] Mensagem recebida:", data);

      if (!data.id) {
        console.warn("⚠️ Ignorado — sem ID da vaga");
        return;
      }

      const response = await api.put(`/${data.id}`, data);
      console.log("✅ Vaga atualizada no backend:", response.status);
    } catch (err: any) {
      console.error("❌ Erro ao processar mensagem de vaga:", err.message);
    }
  });
}