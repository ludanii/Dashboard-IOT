import dotenv from "dotenv";
import { client } from "./mqtt/broker.js";
import { listenVagas } from "./api/vagas.js";
import { listenMotos } from "./api/motos.js";

dotenv.config();

console.log("🟢 Iniciando serviço de integração MQTT com Java Backend...");

client.on("connect", () => {
  listenVagas();
  listenMotos();
  console.log("🚀 Sistema pronto e ouvindo tópicos...");
});

client.on("close", () => {
  console.warn("⚠️ Conexão MQTT encerrada.");
});

client.on("reconnect", () => {
  console.log("♻️ Tentando reconectar ao broker MQTT...");
});
