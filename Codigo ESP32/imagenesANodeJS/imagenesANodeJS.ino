#include "esp_camera.h"
#include <WiFi.h>

const char* ssid = "";
const char* password = "";
const char* serverName = "";
const int serverPort = 8083;
const char* serverPath = "/subirImagen";

void startCamera();

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.print("Conectando al WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConectado al WiFi");

  startCamera();
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    camera_fb_t* fb = esp_camera_fb_get();
    if (!fb) {
      Serial.println("Error al capturar imagen");
      return;
    }

    WiFiClient client;
    if (!client.connect(serverName, serverPort)) {
      Serial.println("Error al conectar con servidor");
      esp_camera_fb_return(fb);
      return;
    }

    const char* boundary = "----123456789ESP32";

    // Armamos la cabecera del multipart
    client.printf("POST %s HTTP/1.1\r\n", serverPath);
    client.printf("Host: %s:%d\r\n", serverName, serverPort);
    client.printf("Content-Type: multipart/form-data; boundary=%s\r\n", boundary);

    // Calcular content-length manualmente
    int part1 = strlen(boundary) + 4;  // "--BOUNDARY\r\n"
    int part2 = strlen("Content-Disposition: form-data; name=\"imagen\"; filename=\"captura.jpg\"\r\n");
    int part3 = strlen("Content-Type: image/jpeg\r\n\r\n");
    int part4 = fb->len;
    int part5 = 2;                     // \r\n antes de cierre
    int part6 = strlen(boundary) + 6;  // "--BOUNDARY--\r\n"

    int totalLength = part1 + part2 + part3 + part4 + part5 + part6;
    client.printf("Content-Length: %d\r\n", totalLength);
    client.print("Connection: close\r\n\r\n");

    // Parte 1: encabezado del archivo
    client.printf("--%s\r\n", boundary);
    client.print("Content-Disposition: form-data; name=\"imagen\"; filename=\"captura.jpg\"\r\n");
    client.print("Content-Type: image/jpeg\r\n\r\n");

    // Parte 2: contenido (imagen)
    client.write(fb->buf, fb->len);

    // Parte 3: cierre del multipart
    client.printf("\r\n--%s--\r\n", boundary);

    // Leer respuesta
    while (client.connected()) {
      String line = client.readStringUntil('\n');
      if (line == "\r") break;
    }

    String response = client.readString();
    Serial.println("Respuesta del servidor:\n" + response);

    esp_camera_fb_return(fb);
    client.stop();
  } else {
    Serial.println("WiFi desconectado");
  }

  delay(5000);  // Espera 10 segundos
}

void startCamera() {
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = 5;
  config.pin_d1 = 18;
  config.pin_d2 = 19;
  config.pin_d3 = 21;
  config.pin_d4 = 36;
  config.pin_d5 = 39;
  config.pin_d6 = 34;
  config.pin_d7 = 35;
  config.pin_xclk = 0;
  config.pin_pclk = 22;
  config.pin_vsync = 25;
  config.pin_href = 23;
  config.pin_sscb_sda = 26;
  config.pin_sscb_scl = 27;
  config.pin_pwdn = 32;
  config.pin_reset = -1;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;

  config.frame_size = FRAMESIZE_QVGA;
  config.jpeg_quality = 10;
  config.fb_count = 1;

  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Error al iniciar cámara: 0x%x\n", err);
    return;
  }

  sensor_t* s = esp_camera_sensor_get();
  if (s != nullptr) {
    s->set_gain_ctrl(s, 0);      // Desactiva control automático de ganancia
    s->set_exposure_ctrl(s, 0);  // Desactiva control automático de exposición
    s->set_whitebal(s, 1);            // Desactiva balance automático de blancos
    s->set_agc_gain(s, 10);      // Establece ganancia fija (entre 0-30)
    s->set_aec_value(s, 300);   // Valor de exposición manual (cuanto más alto, más luz)
    s->set_brightness(s, 0);     // Opcional: sube brillo (de -2 a 2)
  }


  Serial.println("Cámara iniciada correctamente");
}
