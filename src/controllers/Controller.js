const inicio = async (req, res) => {
  res.send("<h1>Ola pa</h1>");
};

const guardarImagen = async (req, res) => {
  try {
    const archivo = req.file;

    if (!archivo) {
      return res.status(400).json({ mensaje: "No se recibió ninguna imagen" });
    }

    return res.status(200).json({
      mensaje: "Imagen guardada correctamente",
      nombreArchivo: archivo.filename,
      ruta: archivo.path
    });
  } catch (error) {
    console.error("❌ Error al guardar la imagen:", error);
    res.status(500).json({ mensaje: "Error interno del servidor" });
  }
};


module.exports = {
  inicio,
  guardarImagen
};
