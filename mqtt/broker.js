import mqtt from "mqtt";
import dotenv from "dotenv";

dotenv.config();

const {
  MQTT_HOST,
  MQTT_PORT,
  MQTT_USER,
  MQTT_PASSWORD,
} = process.env;

export const client = mqtt.connect(`mqtt://${MQTT_HOST}:${MQTT_PORT}`, {
  username: MQTT_USER || undefined,
  password: MQTT_PASSWORD || undefined,
  clientId: "NodeListener-" + Math.random().toString(16).slice(2, 8),
});

client.on("connect", () => {
  console.log(`✅ Conectado ao broker MQTT: ${MQTT_HOST}:${MQTT_PORT}`);
});

client.on("error", (err) => {
  console.error("❌ Erro de conexão MQTT:", err.message);
});
