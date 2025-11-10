#include <Arduino.h>
#include <WiFi.h>

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n\n==========================================");
  Serial.println("  ESP32-S3 - Clasificador de Residuos");
  Serial.println("  UTEC - Prueba Inicial");
  Serial.println("==========================================\n");
  
  Serial.println("✅ ESP32-S3 inicializado correctamente!");
  Serial.printf("💾 PSRAM disponible: %d bytes\n", ESP.getPsramSize());
  Serial.printf("🧠 Frecuencia CPU: %d MHz\n", ESP.getCpuFreqMHz());
  Serial.printf("📦 Flash: %d bytes\n", ESP.getFlashChipSize());
  Serial.printf("🆔 Chip Model: %s\n", ESP.getChipModel());
  Serial.printf("📍 MAC Address: %s\n", WiFi.macAddress().c_str());
  Serial.println("\n🚀 Sistema listo!\n");
}

void loop() {
  Serial.println("⏱️  Sistema funcionando... (cada 2 segundos)");
  delay(2000);
}
