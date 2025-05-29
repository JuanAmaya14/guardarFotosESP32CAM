require('dotenv').config({ path: __dirname + '/../.env' });
const express = require("express");
const rutas = require("./routes/Rutas");
const fs = require('fs');
const path = require('path');

const app = express();

app.set("appName", "ImagenesESP32CAM");
app.set("host", process.env.APP_HOST);
app.set("port", process.env.APP_PORT);

// Crear carpeta uploads si no existe
const uploadsDir = path.join(__dirname, 'uploads');
if (!fs.existsSync(uploadsDir)) {
  fs.mkdirSync(uploadsDir);
}

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
    console.error("Error:", error);
  }
}

startServer();
