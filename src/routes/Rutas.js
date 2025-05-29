const multer = require("multer");
const path = require("path");
const express = require("express");
const router = express.Router();
const Controller = require("../controllers/Controller");

// Configuración de multer
const storage = multer.diskStorage({
  destination: (req, file, cb) => {
    cb(null, path.join(__dirname, "../uploads")); // Guarda en /uploads
  },
  filename: (req, file, cb) => {
    cb(null, Date.now() + "-" + file.originalname); // Evita duplicados
  }
});

const upload = multer({ storage });

// Ruta GET de ejemplo
router.get("/", Controller.inicio);

// Ruta POST para subir imagen
router.post("/subirImagen", upload.single("imagen"), Controller.guardarImagen);

module.exports = router;
