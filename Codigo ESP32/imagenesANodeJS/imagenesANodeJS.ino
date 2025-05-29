#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "";
const char* password = "";

// Dirección del servidor
const char* serverName = "http://unaip:8083/subirImagen";

unsigned long lastTime = 0;
unsigned long timerDelay = 5000;

int contador = 0;

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  Serial.println("Timer set to 5 seconds (timerDelay variable).");
}

void loop() {
  if ((millis() - lastTime) > timerDelay) {
    if (WiFi.status() == WL_CONNECTED) {
      contador++;

      WiFiClient client;
      HTTPClient http;

      http.begin(client, serverName);

      String boundary = "--------------------------123456789012345678901234";
      String contentType = "multipart/form-data; boundary=" + boundary;
      http.addHeader("Content-Type", contentType);

      // Contenido del archivo simulado
      String contenido = "Este es el contenido del archivo número " + String(contador);

      // Construcción del cuerpo de la petición multipart
      String body = "--" + boundary + "\r\n";
      body += "Content-Disposition: form-data; name=\"imagen\"; filename=\"" + String(contador) + ".txt\"\r\n";
      body += "Content-Type: text/plain\r\n\r\n";
      body += contenido + "\r\n";
      body += "--" + boundary + "--\r\n";

      int httpResponseCode = http.POST((uint8_t*)body.c_str(), body.length());

      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);

      String response = http.getString();
      Serial.println("Server response: " + response);

      http.end();
    } else {
      Serial.println("WiFi Disconnected");
    }

    lastTime = millis();
  }
}
