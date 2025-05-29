require('dotenv').config({ path: __dirname + '/../.env' });
const express = require("express");
const rutas = require("./routes/Rutas");

const app = express();

app.set("appName", "Pasteleria");
app.set("host", process.env.APP_HOST);
app.set("port", process.env.APP_PORT);

async function startServer() {
  try {

    // Usa las rutas
    app.use("/", rutas);

    // Middleware para 404
    app.use((req, res) => {
      res.status(404).send(`<h1>Esta página no existe master</h1>`);
    });

    app.listen(app.get("port"), () => {
      console.log(
        `Servidor corriendo en: http://${app.get("host")}:${app.get("port")}`
      );
    });
  } catch (error) {
    console.error("Error al conectar con la base de datos:", error);
  }
}

startServer();
