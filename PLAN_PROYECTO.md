# Plan Detallado: Clasificador de Residuos Automático
## ESP32-S3 + OV5640 + Machine Learning

---

## 📊 ESTADO DEL PROYECTO - Actualizado: Noviembre 9, 2025

### ✅ **Completado:**
- ✅ FASE 0: Entorno de desarrollo configurado
  - Python 3.11.4 instalado
  - PlatformIO instalado y funcionando
  - Proyecto inicializado correctamente
  - ESP32-S3 programable en COM3
  - Código de prueba básico funciona correctamente

### 🔄 **En Progreso:**
- ⏳ FASE 1: Pruebas de Hardware
  - Cámara OV5640 identificada (modelo VVS-OV5640OCSP)
  - Conector FPC identificado (sin pestaña de seguridad)
  - **PRÓXIMO:** Asegurar cable flex con cinta Kapton y probar detección

### 📋 **Pendiente:**
- ⏸️ FASE 1: Completar pruebas de cámara (captura, web server)
- ⏸️ FASE 2: Desarrollo del modelo ML
- ⏸️ FASE 3: Integración del modelo en ESP32-S3
- ⏸️ FASE 4: Integración al sistema de tacho
- ⏸️ FASE 5: Pruebas finales

---

## 📋 Resumen del Proyecto

**Hardware:**
- ESP32-S3 WROOM N16R8 (WiFi + Bluetooth)
- Cámara OV5640 (5MP, soporta hasta 2592x1944)
- Memoria: 16MB Flash, 8MB PSRAM

**Objetivo:**
Clasificar residuos automáticamente en 3 categorías:
1. 🔵 Plástico
2. 📄 Papel
3. ⚫ General (todo lo demás)

**Enfoque:**
1. Verificar funcionamiento del hardware
2. Desarrollar modelo ML optimizado
3. Integrar en el ESP32-S3 para inferencia local

---

## 🎯 FASE 0: PREPARACIÓN DEL ENTORNO DE DESARROLLO ✅ **COMPLETADA**

### Paso 0.1: Instalación de herramientas base ✅
**Estado:** ✅ COMPLETADO
**Objetivo:** Configurar el entorno de desarrollo para ESP32-S3

**Acciones:**
1. Instalar Python 3.8+ (verificar con `python --version`)
2. Instalar PlatformIO o Arduino IDE
3. Instalar ESP-IDF (Espressif IoT Development Framework)

**Comandos (Windows PowerShell):**
```powershell
# Verificar Python
python --version

# Instalar PlatformIO CLI (recomendado)
pip install platformio

# Verificar instalación
pio --version
```

**Verificación:**
- [x] Python instalado correctamente (Python 3.11.4)
- [x] PlatformIO extensión instalada en VSCode
- [x] Puerto COM del ESP32 visible en Device Manager (COM3)

**Herramientas recomendadas:**
- **VSCode** con extensión PlatformIO
- **ESP-IDF v5.0+** para soporte completo de ESP32-S3
- **esptool.py** para flash y diagnóstico

---

### Paso 0.2: Configurar proyecto PlatformIO ✅
**Estado:** ✅ COMPLETADO
**Objetivo:** Crear estructura de proyecto para ESP32-S3

**Acciones:**
```powershell
# Crear proyecto nuevo
pio project init --board esp32-s3-devkitc-1

# O si ya tienes el proyecto, crear platformio.ini
```

**Archivo `platformio.ini` inicial:**
```ini
[env:esp32-s3-devkitc-1]
platform = espressif32
board = esp32-s3-devkitc-1
framework = arduino
monitor_speed = 115200

; Configuración para PSRAM
board_build.arduino.memory_type = qio_opi
board_build.f_flash = 80000000L
board_build.flash_mode = qio

; Librerías necesarias
lib_deps = 
    espressif/esp32-camera @ ^2.0.4
```

**Verificación:**
- [x] Proyecto inicializado correctamente
- [x] `platformio.ini` configurado
- [x] Estructura de carpetas creada (src/, include/, lib/)
- [x] Código de prueba funciona correctamente
- [x] ESP32-S3 programable en COM3

---

## 🔌 FASE 1: PRUEBAS DE HARDWARE Y CÁMARA ⏳ **EN PROGRESO**

### Paso 1.1: Conexión física ESP32-S3 + OV5640 ⏳ **EN PROGRESO**
**Estado:** 🔄 Parcialmente completado - Falta asegurar cable flex con cinta Kapton
**Objetivo:** Conectar correctamente la cámara al ESP32-S3

**Diagrama de pines OV5640 → ESP32-S3:**

| OV5640 Pin | ESP32-S3 Pin | Función |
|------------|--------------|---------|
| SIOD (SDA) | GPIO 4       | I2C Data |
| SIOC (SCL) | GPIO 5       | I2C Clock |
| VSYNC      | GPIO 6       | Vertical Sync |
| HREF       | GPIO 7       | Horizontal Reference |
| PCLK       | GPIO 13      | Pixel Clock |
| XCLK       | GPIO 15      | System Clock |
| D0-D7      | GPIO 11,9,8,10,12,18,17,16 | Data Bus |
| PWDN       | GPIO 42      | Power Down |
| RESET      | GPIO 41      | Reset |
| 3.3V       | 3.3V         | Alimentación |
| GND        | GND          | Tierra |

**Estado actual de hardware:**
- [x] ESP32-S3 N16R8 con conector FPC identificado
- [x] Cámara OV5640 (modelo VVS-OV5640OCSP) disponible
- [ ] **PENDIENTE:** Fijar cable flex con cinta Kapton (el conector FPC no tiene pestaña de seguridad)
- [ ] **PENDIENTE:** Probar conexión y detección de cámara

**Verificación física:**
- [ ] Cable flex conectado y asegurado con cinta
- [x] Alimentación correcta (3.3V automático vía conector FPC)
- [ ] LED de la cámara enciende (si tiene)
- [x] ESP32-S3 detectado en puerto COM3

**⚠️ Precauciones:**
- ✅ Alimentación correcta (3.3V automático vía conector FPC)
- ⚠️ **IMPORTANTE:** El conector FPC no tiene pestaña de seguridad - usar cinta Kapton para fijar el cable flex
- Asegurar que el cable flex esté completamente insertado antes de colocar la cinta

---

### Paso 1.2: Código de prueba básica - Detección de cámara ⏳ **PRÓXIMO PASO**
**Estado:** 🔜 Pendiente - Ejecutar después de asegurar cable flex
**Objetivo:** Verificar que el ESP32-S3 puede comunicarse con la OV5640

**Crear archivo:** `src/test_camera_detect.cpp`

```cpp
#include <Arduino.h>
#include "esp_camera.h"

// Configuración de pines para OV5640
#define PWDN_GPIO_NUM     42
#define RESET_GPIO_NUM    41
#define XCLK_GPIO_NUM     15
#define SIOD_GPIO_NUM     4
#define SIOC_GPIO_NUM     5

#define Y9_GPIO_NUM       16
#define Y8_GPIO_NUM       17
#define Y7_GPIO_NUM       18
#define Y6_GPIO_NUM       12
#define Y5_GPIO_NUM       10
#define Y4_GPIO_NUM       8
#define Y3_GPIO_NUM       9
#define Y2_GPIO_NUM       11

#define VSYNC_GPIO_NUM    6
#define HREF_GPIO_NUM     7
#define PCLK_GPIO_NUM     13

void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println("\n\n=== Test de Detección de Cámara OV5640 ===");

  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  
  // Configuración inicial con baja resolución para prueba
  config.frame_size = FRAMESIZE_QVGA; // 320x240
  config.jpeg_quality = 12;
  config.fb_count = 1;
  config.fb_location = CAMERA_FB_IN_PSRAM;
  config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;

  // Inicializar cámara
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("❌ Error al inicializar cámara: 0x%x\n", err);
    return;
  }

  Serial.println("✅ Cámara inicializada correctamente!");
  
  // Obtener información del sensor
  sensor_t * s = esp_camera_sensor_get();
  if (s != NULL) {
    Serial.printf("✅ Sensor detectado: PID=0x%02X VER=0x%02X\n", 
                  s->id.PID, s->id.VER);
  }
}

void loop() {
  delay(1000);
  Serial.println("🔄 Sistema listo. Cámara funcionando.");
}
```

**Compilar y subir:**
```powershell
pio run --target upload
pio device monitor
```

**Resultado esperado:**
```
=== Test de Detección de Cámara OV5640 ===
✅ Cámara inicializada correctamente!
✅ Sensor detectado: PID=0x56 VER=0x40
🔄 Sistema listo. Cámara funcionando.
```

**Verificación:**
- [ ] Código compila sin errores
- [ ] ESP32-S3 no se reinicia en bucle
- [ ] Mensaje "Cámara inicializada correctamente" aparece
- [ ] PID del sensor es correcto (0x56 para OV5640)

**Si falla:**
- Verificar conexiones físicas
- Revisar que los pines coincidan con tu módulo
- Probar con `config.xclk_freq_hz = 10000000` (frecuencia más baja)

---

### Paso 1.3: Captura de foto y guardado en SD o SPIFFS
**Objetivo:** Tomar una foto real y verificar que la imagen es válida

**Crear archivo:** `src/test_camera_capture.cpp`

```cpp
#include <Arduino.h>
#include "esp_camera.h"
#include "FS.h"
#include "SPIFFS.h"

// [Misma configuración de pines del paso anterior]

void setup() {
  Serial.begin(115200);
  Serial.println("\n=== Test de Captura de Imagen ===");

  // Inicializar SPIFFS
  if(!SPIFFS.begin(true)){
    Serial.println("❌ Error montando SPIFFS");
    return;
  }
  Serial.println("✅ SPIFFS montado");

  // [Misma inicialización de cámara del paso anterior]
  
  // Configurar resolución más alta para prueba
  sensor_t * s = esp_camera_sensor_get();
  s->set_framesize(s, FRAMESIZE_SVGA); // 800x600
  s->set_quality(s, 10); // Calidad JPEG (0-63, menor = mejor)

  Serial.println("📸 Preparado para capturar...");
}

void loop() {
  Serial.println("\n--- Capturando imagen en 3 segundos ---");
  delay(3000);

  // Capturar frame
  camera_fb_t * fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("❌ Error al capturar imagen");
    return;
  }

  Serial.printf("✅ Imagen capturada: %d bytes, %dx%d\n", 
                fb->len, fb->width, fb->height);

  // Guardar en SPIFFS
  File file = SPIFFS.open("/test_photo.jpg", FILE_WRITE);
  if (!file) {
    Serial.println("❌ Error al crear archivo");
  } else {
    file.write(fb->buf, fb->len);
    file.close();
    Serial.println("✅ Imagen guardada en /test_photo.jpg");
    Serial.println("💡 Puedes descargarla vía web server o serial");
  }

  // Liberar buffer
  esp_camera_fb_return(fb);

  delay(10000); // Esperar 10 segundos antes de la siguiente captura
}
```

**Verificación:**
- [ ] Imagen capturada sin errores
- [ ] Tamaño de archivo razonable (20-100KB para 800x600)
- [ ] Archivo guardado en SPIFFS

**Para ver la imagen:** Necesitaremos crear un servidor web simple (siguiente paso)

---

### Paso 1.4: Servidor web para visualizar fotos
**Objetivo:** Ver las imágenes capturadas desde el navegador

**Crear archivo:** `src/camera_webserver.cpp`

```cpp
#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include "esp_camera.h"
#include "SPIFFS.h"

// Configuración WiFi
const char* ssid = "TU_WIFI";  // ⚠️ CAMBIAR
const char* password = "TU_PASSWORD";  // ⚠️ CAMBIAR

WebServer server(80);

// [Misma configuración de pines]

void handleRoot() {
  String html = "<!DOCTYPE html><html><head><title>ESP32-S3 Camera</title>";
  html += "<meta charset='utf-8'></head><body>";
  html += "<h1>📷 Clasificador de Residuos - Test Cámara</h1>";
  html += "<img src='/capture' style='max-width:100%'><br><br>";
  html += "<button onclick='location.reload()'>🔄 Capturar Nueva</button>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

void handleCapture() {
  camera_fb_t * fb = esp_camera_fb_get();
  if (!fb) {
    server.send(500, "text/plain", "Error al capturar");
    return;
  }
  server.sendHeader("Content-Disposition", "inline; filename=capture.jpg");
  server.send_P(200, "image/jpeg", (const char *)fb->buf, fb->len);
  esp_camera_fb_return(fb);
}

void setup() {
  Serial.begin(115200);
  Serial.println("\n=== Camera Web Server ===");

  // Inicializar cámara
  camera_config_t config;
  // [Configuración completa de cámara]
  
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("❌ Error cámara: 0x%x\n", err);
    return;
  }
  Serial.println("✅ Cámara OK");

  // Conectar WiFi
  WiFi.begin(ssid, password);
  Serial.print("Conectando a WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n✅ WiFi conectado");
  Serial.print("📱 URL: http://");
  Serial.println(WiFi.localIP());

  // Configurar servidor
  server.on("/", handleRoot);
  server.on("/capture", handleCapture);
  server.begin();
  Serial.println("✅ Servidor web iniciado");
}

void loop() {
  server.handleClient();
}
```

**Pasos:**
1. Modificar SSID y password del WiFi
2. Compilar y subir
3. Abrir monitor serial
4. Copiar la IP que aparece
5. Abrir navegador en `http://[IP]`

**Verificación:**
- [ ] ESP32 se conecta a WiFi
- [ ] IP mostrada en serial
- [ ] Página web carga correctamente
- [ ] Imagen se muestra en el navegador
- [ ] Imagen tiene contenido reconocible (no solo ruido)

**Troubleshooting:**
- Si la imagen sale oscura: ajustar `s->set_exposure_ctrl(s, 1)` y `s->set_aec_value(s, 300)`
- Si sale con ruido: reducir resolución o mejorar iluminación
- Si no conecta WiFi: verificar credenciales

---

## 🤖 FASE 2: DESARROLLO DEL MODELO DE MACHINE LEARNING

### Paso 2.1: Recolección de dataset
**Objetivo:** Obtener imágenes etiquetadas de residuos para entrenamiento

**Opciones:**

**Opción A - Dataset público (recomendado para empezar):**
```powershell
# Crear carpeta para datasets
mkdir dataset
cd dataset

# Descargar dataset de Kaggle (ejemplo: TrashNet)
# https://github.com/garythung/trashnet
```

**Estructura recomendada:**
```
dataset/
├── train/
│   ├── plastic/
│   │   ├── plastic_001.jpg
│   │   ├── plastic_002.jpg
│   │   └── ...
│   ├── paper/
│   │   ├── paper_001.jpg
│   │   └── ...
│   └── general/
│       ├── general_001.jpg
│       └── ...
├── validation/
│   ├── plastic/
│   ├── paper/
│   └── general/
└── test/
    ├── plastic/
    ├── paper/
    └── general/
```

**Opción B - Recolección propia:**
1. Usar el web server del paso 1.4
2. Tomar ~100 fotos por categoría
3. Etiquetar manualmente
4. Organizar en carpetas

**Cantidad recomendada:**
- Mínimo: 100 imágenes por clase
- Óptimo: 500-1000 imágenes por clase
- Split: 70% train, 15% validation, 15% test

**Verificación:**
- [ ] Dataset descargado/recolectado
- [ ] Estructura de carpetas correcta
- [ ] Al menos 100 imágenes por categoría
- [ ] Imágenes en formato JPG/PNG

---

### Paso 2.2: Entrenamiento de modelo con TensorFlow Lite
**Objetivo:** Entrenar un modelo pequeño y eficiente para ESP32

**Crear archivo:** `ml/train_model.py`

```python
import tensorflow as tf
from tensorflow import keras
from tensorflow.keras import layers
import numpy as np
import matplotlib.pyplot as plt
import os

# Configuración
IMG_SIZE = 96  # Resolución pequeña para ESP32
BATCH_SIZE = 32
EPOCHS = 20
NUM_CLASSES = 3  # plastic, paper, general

# Rutas
DATASET_PATH = './dataset'
MODEL_SAVE_PATH = './models'

# Crear directorios
os.makedirs(MODEL_SAVE_PATH, exist_ok=True)

print("🚀 Iniciando entrenamiento del modelo...")
print(f"📊 Configuración: {IMG_SIZE}x{IMG_SIZE}, {EPOCHS} epochs")

# Cargar dataset con data augmentation
train_ds = tf.keras.preprocessing.image_dataset_from_directory(
    f'{DATASET_PATH}/train',
    image_size=(IMG_SIZE, IMG_SIZE),
    batch_size=BATCH_SIZE,
    label_mode='categorical',
    seed=123
)

val_ds = tf.keras.preprocessing.image_dataset_from_directory(
    f'{DATASET_PATH}/validation',
    image_size=(IMG_SIZE, IMG_SIZE),
    batch_size=BATCH_SIZE,
    label_mode='categorical',
    seed=123
)

# Data augmentation
data_augmentation = keras.Sequential([
    layers.RandomFlip("horizontal"),
    layers.RandomRotation(0.1),
    layers.RandomZoom(0.1),
])

# Modelo MobileNetV2 (optimizado para embebidos)
base_model = keras.applications.MobileNetV2(
    input_shape=(IMG_SIZE, IMG_SIZE, 3),
    include_top=False,
    weights='imagenet',
    alpha=0.35  # Factor de ancho reducido para menor tamaño
)

# Congelar base model
base_model.trainable = False

# Construir modelo completo
inputs = keras.Input(shape=(IMG_SIZE, IMG_SIZE, 3))
x = data_augmentation(inputs)
x = keras.applications.mobilenet_v2.preprocess_input(x)
x = base_model(x, training=False)
x = layers.GlobalAveragePooling2D()(x)
x = layers.Dropout(0.2)(x)
outputs = layers.Dense(NUM_CLASSES, activation='softmax')(x)

model = keras.Model(inputs, outputs)

# Compilar
model.compile(
    optimizer=keras.optimizers.Adam(learning_rate=0.001),
    loss='categorical_crossentropy',
    metrics=['accuracy']
)

print("📐 Arquitectura del modelo:")
model.summary()

# Entrenar
print("\n🏋️ Entrenando modelo...")
history = model.fit(
    train_ds,
    validation_data=val_ds,
    epochs=EPOCHS,
    callbacks=[
        keras.callbacks.EarlyStopping(patience=3, restore_best_weights=True),
        keras.callbacks.ReduceLROnPlateau(patience=2)
    ]
)

# Evaluar
print("\n📊 Evaluando modelo...")
val_loss, val_acc = model.evaluate(val_ds)
print(f"✅ Accuracy en validación: {val_acc*100:.2f}%")

# Guardar modelo Keras
model.save(f'{MODEL_SAVE_PATH}/waste_classifier.h5')
print(f"💾 Modelo guardado en {MODEL_SAVE_PATH}/waste_classifier.h5")

# Gráficas de entrenamiento
plt.figure(figsize=(12, 4))
plt.subplot(1, 2, 1)
plt.plot(history.history['accuracy'], label='Train')
plt.plot(history.history['val_accuracy'], label='Validation')
plt.title('Accuracy')
plt.legend()

plt.subplot(1, 2, 2)
plt.plot(history.history['loss'], label='Train')
plt.plot(history.history['val_loss'], label='Validation')
plt.title('Loss')
plt.legend()

plt.savefig(f'{MODEL_SAVE_PATH}/training_history.png')
print(f"📈 Gráficas guardadas en {MODEL_SAVE_PATH}/training_history.png")
```

**Instalar dependencias:**
```powershell
pip install tensorflow numpy matplotlib pillow
```

**Ejecutar entrenamiento:**
```powershell
cd ml
python train_model.py
```

**Resultado esperado:**
- Accuracy > 85% en validación
- Modelo guardado en `models/waste_classifier.h5`
- Gráficas de entrenamiento generadas

**Verificación:**
- [ ] Script ejecuta sin errores
- [ ] Modelo converge (loss disminuye)
- [ ] Accuracy en validación > 85%
- [ ] Archivo .h5 generado

---

### Paso 2.3: Conversión a TensorFlow Lite
**Objetivo:** Convertir el modelo a formato optimizado para microcontroladores

**Crear archivo:** `ml/convert_to_tflite.py`

```python
import tensorflow as tf
import numpy as np
import os

MODEL_PATH = './models/waste_classifier.h5'
TFLITE_MODEL_PATH = './models/waste_classifier.tflite'
TFLITE_MICRO_PATH = './models/waste_classifier_micro.cc'

print("🔄 Convirtiendo modelo a TensorFlow Lite...")

# Cargar modelo
model = tf.keras.models.load_model(MODEL_PATH)

# Convertir a TFLite con optimizaciones
converter = tf.lite.TFLiteConverter.from_keras_model(model)

# Optimizaciones para ESP32
converter.optimizations = [tf.lite.Optimize.DEFAULT]
converter.target_spec.supported_ops = [
    tf.lite.OpsSet.TFLITE_BUILTINS_INT8
]

# Función representativa para cuantización
def representative_dataset():
    # Genera datos aleatorios del mismo tamaño que la entrada
    for _ in range(100):
        data = np.random.rand(1, 96, 96, 3).astype(np.float32)
        yield [data]

converter.representative_dataset = representative_dataset
converter.inference_input_type = tf.int8
converter.inference_output_type = tf.int8

# Convertir
tflite_model = converter.convert()

# Guardar modelo TFLite
with open(TFLITE_MODEL_PATH, 'wb') as f:
    f.write(tflite_model)

file_size = os.path.getsize(TFLITE_MODEL_PATH) / 1024
print(f"✅ Modelo TFLite guardado: {file_size:.2f} KB")

# Convertir a formato C array para ESP32
print("\n🔄 Generando archivo .cc para ESP32...")

with open(TFLITE_MICRO_PATH, 'w') as f:
    f.write('// Auto-generated model file\n')
    f.write('#include <cstdint>\n\n')
    f.write('alignas(8) const unsigned char model_tflite[] = {\n')
    
    hex_array = [f'0x{byte:02x}' for byte in tflite_model]
    for i in range(0, len(hex_array), 12):
        f.write('  ' + ', '.join(hex_array[i:i+12]) + ',\n')
    
    f.write('};\n')
    f.write(f'const unsigned int model_tflite_len = {len(tflite_model)};\n')

print(f"✅ Archivo C generado: {TFLITE_MICRO_PATH}")

# Verificar el modelo TFLite
print("\n🧪 Verificando modelo TFLite...")
interpreter = tf.lite.Interpreter(model_content=tflite_model)
interpreter.allocate_tensors()

input_details = interpreter.get_input_details()
output_details = interpreter.get_output_details()

print(f"📥 Input shape: {input_details[0]['shape']}")
print(f"📤 Output shape: {output_details[0]['shape']}")
print(f"🔢 Input type: {input_details[0]['dtype']}")
print(f"🔢 Output type: {output_details[0]['dtype']}")

# Test con datos aleatorios
test_input = np.random.randint(-128, 127, 
                               size=input_details[0]['shape'], 
                               dtype=np.int8)
interpreter.set_tensor(input_details[0]['index'], test_input)
interpreter.invoke()
output = interpreter.get_tensor(output_details[0]['index'])

print(f"\n✅ Test de inferencia exitoso!")
print(f"📊 Output shape: {output.shape}")
print("✅ Modelo listo para ESP32!")

# Imprimir resumen
print("\n" + "="*50)
print("📋 RESUMEN DE CONVERSIÓN")
print("="*50)
print(f"Modelo original: {MODEL_PATH}")
print(f"Modelo TFLite: {TFLITE_MODEL_PATH} ({file_size:.2f} KB)")
print(f"Archivo C: {TFLITE_MICRO_PATH}")
print(f"Tipo de entrada: INT8 {input_details[0]['shape']}")
print(f"Tipo de salida: INT8 {output_details[0]['shape']}")
print("="*50)
```

**Ejecutar conversión:**
```powershell
python convert_to_tflite.py
```

**Verificación:**
- [ ] Archivo .tflite generado
- [ ] Archivo .cc generado para ESP32
- [ ] Tamaño del modelo < 500KB (idealmente < 200KB)
- [ ] Test de inferencia exitoso

**Nota importante:** El tamaño del modelo debe caber en la memoria del ESP32-S3. Si es muy grande, reducir:
- Resolución de entrada (ej: 96x96 → 64x64)
- Alpha de MobileNetV2 (ej: 0.35 → 0.25)
- Profundidad del modelo

---

## 🔧 FASE 3: INTEGRACIÓN DEL MODELO EN ESP32-S3

### Paso 3.1: Configurar TensorFlow Lite Micro en PlatformIO
**Objetivo:** Instalar la librería TFLite Micro en el proyecto

**Actualizar `platformio.ini`:**
```ini
[env:esp32-s3-devkitc-1]
platform = espressif32
board = esp32-s3-devkitc-1
framework = arduino
monitor_speed = 115200

board_build.arduino.memory_type = qio_opi
board_build.partitions = huge_app.csv

lib_deps = 
    espressif/esp32-camera @ ^2.0.4
    https://github.com/eloquentarduino/EloquentTinyML.git

build_flags =
    -DBOARD_HAS_PSRAM
    -mfix-esp32-psram-cache-issue
```

**Verificación:**
```powershell
pio lib install
```

- [ ] Librerías descargadas sin errores

---

### Paso 3.2: Implementar inferencia en ESP32
**Objetivo:** Integrar el modelo y realizar clasificación en tiempo real

**Crear archivo:** `src/main.cpp`

```cpp
#include <Arduino.h>
#include "esp_camera.h"
#include <EloquentTinyML.h>
#include "model_tflite.h"  // El archivo .cc generado anteriormente

// Configuración del modelo
#define NUMBER_OF_INPUTS 96*96*3
#define NUMBER_OF_OUTPUTS 3
#define TENSOR_ARENA_SIZE 100*1024  // 100KB para operaciones

Eloquent::TinyML::TfLite<NUMBER_OF_INPUTS, NUMBER_OF_OUTPUTS, TENSOR_ARENA_SIZE> ml;

// Pines de cámara (mismos del paso 1)
// [Incluir configuración de pines]

const char* class_names[] = {"Plástico", "Papel", "General"};

void setup() {
  Serial.begin(115200);
  Serial.println("\n=== Clasificador de Residuos v1.0 ===");

  // Inicializar cámara
  camera_config_t config;
  // [Configuración completa de cámara con FRAMESIZE_96X96]
  config.frame_size = FRAMESIZE_96X96;  // Coincidir con input del modelo
  
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("❌ Error cámara: 0x%x\n", err);
    return;
  }
  Serial.println("✅ Cámara inicializada");

  // Inicializar modelo TFLite
  ml.begin(model_tflite);
  Serial.println("✅ Modelo TFLite cargado");
  Serial.println("🚀 Sistema listo para clasificar!\n");
}

void loop() {
  Serial.println("📸 Capturando imagen...");
  
  // Capturar imagen
  camera_fb_t * fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("❌ Error en captura");
    delay(1000);
    return;
  }

  // Preprocesar imagen (convertir a formato del modelo)
  float input[NUMBER_OF_INPUTS];
  
  // Si la imagen es RGB565, convertir a RGB888 y normalizar
  for (int i = 0; i < fb->len && i < NUMBER_OF_INPUTS; i++) {
    input[i] = (fb->buf[i] - 128.0f) / 128.0f;  // Normalizar [-1, 1]
  }

  // Ejecutar inferencia
  Serial.println("🤖 Clasificando...");
  float output[NUMBER_OF_OUTPUTS];
  
  uint32_t start = millis();
  ml.predict(input, output);
  uint32_t inference_time = millis() - start;

  // Encontrar clase con mayor probabilidad
  int predicted_class = 0;
  float max_prob = output[0];
  
  for (int i = 1; i < NUMBER_OF_OUTPUTS; i++) {
    if (output[i] > max_prob) {
      max_prob = output[i];
      predicted_class = i;
    }
  }

  // Mostrar resultados
  Serial.println("\n📊 RESULTADO:");
  Serial.printf("🎯 Clasificación: %s\n", class_names[predicted_class]);
  Serial.printf("📈 Confianza: %.2f%%\n", max_prob * 100);
  Serial.printf("⏱️  Tiempo de inferencia: %d ms\n", inference_time);
  Serial.println("Probabilidades:");
  for (int i = 0; i < NUMBER_OF_OUTPUTS; i++) {
    Serial.printf("  %s: %.2f%%\n", class_names[i], output[i] * 100);
  }
  Serial.println("─────────────────────────────────\n");

  // Liberar buffer
  esp_camera_fb_return(fb);

  // Esperar antes de siguiente clasificación
  delay(5000);
}
```

**Copiar modelo:** Copiar el archivo `waste_classifier_micro.cc` generado a `src/model_tflite.h`

**Compilar y subir:**
```powershell
pio run --target upload
pio device monitor
```

**Verificación:**
- [ ] Código compila sin errores
- [ ] Modelo carga correctamente
- [ ] Inferencia se ejecuta (tiempo < 1000ms)
- [ ] Clasificación muestra resultados coherentes

**Troubleshooting:**
- Si falta memoria: reducir `TENSOR_ARENA_SIZE` o tamaño del modelo
- Si inferencia muy lenta (>2s): optimizar modelo o reducir resolución
- Si resultados erróneos: revisar preprocesamiento de imagen

---

### Paso 3.3: Optimización de performance
**Objetivo:** Mejorar velocidad y precisión del sistema

**Optimizaciones a probar:**

1. **Ajustar PSRAM:**
```cpp
config.fb_location = CAMERA_FB_IN_PSRAM;
config.grab_mode = CAMERA_GRAB_LATEST;
```

2. **Reducir resolución si es necesario:**
```cpp
config.frame_size = FRAMESIZE_64x64;  // Más rápido pero menos preciso
```

3. **Usar DMA para transferencias:**
```cpp
config.fb_count = 2;  // Double buffering
```

4. **Ajustar frecuencia de CPU:**
```cpp
setCpuFrequencyMhz(240);  // Máxima velocidad
```

**Métricas objetivo:**
- ⏱️ Tiempo de inferencia: < 500ms
- 🎯 Accuracy: > 85%
- 💾 Uso de RAM: < 300KB
- 🔋 Consumo: < 500mA

**Verificación:**
- [ ] Inferencia < 500ms
- [ ] Sin reinicios inesperados
- [ ] Clasificaciones consistentes

---

## 🚀 FASE 4: INTEGRACIÓN AL SISTEMA DE TACHO

### Paso 4.1: Añadir control de actuadores (servo/motor)
**Objetivo:** Controlar la compuerta del tacho según clasificación

**Esquema de funcionamiento:**
```
1. Detectar residuo (sensor de proximidad)
2. Tomar foto
3. Clasificar con ML
4. Activar servo correspondiente
5. Abrir compuerta
6. Esperar a que caiga
7. Cerrar compuerta
```

**Código de control de servos:**
```cpp
#include <ESP32Servo.h>

// Servos para cada compartimento
Servo servoPlastic;
Servo servoPaper;
Servo servoGeneral;

#define SERVO_PLASTIC_PIN 38
#define SERVO_PAPER_PIN 39
#define SERVO_GENERAL_PIN 40

#define SENSOR_PIN 21  // Sensor IR de proximidad

void setupActuators() {
  servoPlastic.attach(SERVO_PLASTIC_PIN);
  servoPaper.attach(SERVO_PAPER_PIN);
  servoGeneral.attach(SERVO_GENERAL_PIN);
  
  pinMode(SENSOR_PIN, INPUT);
  
  // Posición inicial (cerrado)
  servoPlastic.write(0);
  servoPaper.write(0);
  servoGeneral.write(0);
}

void openCompartment(int classification) {
  Servo* servo;
  
  switch(classification) {
    case 0: servo = &servoPlastic; break;
    case 1: servo = &servoPaper; break;
    case 2: servo = &servoGeneral; break;
  }
  
  // Abrir compuerta
  servo->write(90);
  delay(2000);  // Esperar a que caiga
  
  // Cerrar compuerta
  servo->write(0);
}

bool detectWaste() {
  return digitalRead(SENSOR_PIN) == LOW;
}
```

**Actualizar loop principal:**
```cpp
void loop() {
  // Esperar a que se detecte residuo
  if (!detectWaste()) {
    delay(100);
    return;
  }
  
  Serial.println("🔔 Residuo detectado!");
  delay(500);  // Estabilización
  
  // Capturar y clasificar
  int classification = captureAndClassify();
  
  // Abrir compartimento correcto
  openCompartment(classification);
  
  Serial.println("✅ Proceso completado\n");
  delay(2000);  // Cooldown
}
```

**Verificación:**
- [ ] Servos responden correctamente
- [ ] Sensor de proximidad detecta objetos
- [ ] Secuencia completa funciona
- [ ] No hay colisiones mecánicas

---

### Paso 4.2: Sistema de feedback y logging
**Objetivo:** Registrar estadísticas y dar feedback al usuario

**Implementar contador y display:**
```cpp
#include <Wire.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);

// Contadores
int count_plastic = 0;
int count_paper = 0;
int count_general = 0;

void setupDisplay() {
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("❌ Display no encontrado");
    return;
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.display();
}

void updateDisplay(int classification, float confidence) {
  display.clearDisplay();
  
  // Título
  display.setCursor(0, 0);
  display.println("CLASIFICADOR UTEC");
  display.println("─────────────────");
  
  // Resultado
  display.printf("Tipo: %s\n", class_names[classification]);
  display.printf("Conf: %.0f%%\n\n", confidence * 100);
  
  // Estadísticas
  display.println("Estadisticas:");
  display.printf("Plastico: %d\n", count_plastic);
  display.printf("Papel: %d\n", count_paper);
  display.printf("General: %d\n", count_general);
  
  display.display();
}

void incrementCounter(int classification) {
  switch(classification) {
    case 0: count_plastic++; break;
    case 1: count_paper++; break;
    case 2: count_general++; break;
  }
}
```

**Agregar al loop:**
```cpp
// Después de clasificar
incrementCounter(classification);
updateDisplay(classification, confidence);
```

**Verificación:**
- [ ] Display muestra información correctamente
- [ ] Contadores incrementan
- [ ] Feedback visual claro para usuario

---

### Paso 4.3: Sistema de conectividad (WiFi + API)
**Objetivo:** Enviar datos a la nube para análisis

**Implementar cliente HTTP:**
```cpp
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char* server_url = "https://tu-api.com/waste-log";

void sendDataToCloud(int classification, float confidence) {
  if (WiFi.status() != WL_CONNECTED) return;
  
  HTTPClient http;
  http.begin(server_url);
  http.addHeader("Content-Type", "application/json");
  
  // Crear JSON
  StaticJsonDocument<200> doc;
  doc["device_id"] = "TACHO_UTEC_01";
  doc["timestamp"] = millis();
  doc["classification"] = class_names[classification];
  doc["confidence"] = confidence;
  doc["location"] = "Edificio A";
  
  String json;
  serializeJson(doc, json);
  
  // Enviar
  int httpCode = http.POST(json);
  
  if (httpCode > 0) {
    Serial.printf("✅ Datos enviados: %d\n", httpCode);
  } else {
    Serial.printf("❌ Error HTTP: %d\n", httpCode);
  }
  
  http.end();
}
```

**Opciones de backend:**
- Firebase Realtime Database (fácil)
- AWS IoT Core (profesional)
- API REST propia (personalizable)

**Verificación:**
- [ ] Datos se envían correctamente
- [ ] API recibe y almacena datos
- [ ] Dashboard puede visualizar estadísticas

---

## 🧪 FASE 5: PRUEBAS Y VALIDACIÓN FINAL

### Paso 5.1: Pruebas unitarias por componente
**Checklist de pruebas:**

**Cámara:**
- [ ] Captura imágenes claras
- [ ] Funciona con diferentes iluminaciones
- [ ] No se sobrecalienta

**Modelo ML:**
- [ ] Accuracy > 85% con objetos reales
- [ ] Tiempo de inferencia < 500ms
- [ ] Maneja casos ambiguos (confidence < 60%)

**Actuadores:**
- [ ] Servos abren/cierran correctamente
- [ ] Sincronización correcta
- [ ] No hay atascos

**Sistema completo:**
- [ ] Flujo completo funciona 10 veces seguidas
- [ ] Manejo de errores (cámara falla, WiFi pierde conexión)
- [ ] Reinicio automático en caso de error

---

### Paso 5.2: Pruebas de campo
**Objetivo:** Validar en condiciones reales de uso

**Protocolo de prueba:**
1. Preparar 30 objetos (10 de cada categoría)
2. Para cada objeto:
   - Insertar en el tacho
   - Verificar clasificación
   - Registrar resultado (correcto/incorrecto)
3. Calcular accuracy real

**Matriz de confusión esperada:**
```
                Predicho
              Plás  Papel  Gral
Real Plás  [  8     1      1  ]
     Papel [  0     9      1  ]
     Gral  [  1     0      9  ]
```

**Casos edge a probar:**
- Botellas con etiquetas de papel
- Papel arrugado vs papel liso
- Plásticos transparentes
- Objetos sucios o mojados
- Objetos muy pequeños o muy grandes

**Verificación:**
- [ ] Accuracy real > 80%
- [ ] Falsos positivos < 15%
- [ ] Sistema estable durante > 1 hora

---

### Paso 5.3: Optimización final y documentación

**Ajustes finales:**
1. Calibrar threshold de confianza (ej: si confidence < 60%, clasificar como "General")
2. Ajustar tiempos de espera
3. Optimizar consumo energético
4. Añadir modo de suspensión cuando no hay uso

**Código de threshold:**
```cpp
int classification = getClassification(output, &confidence);

if (confidence < 0.60) {
  Serial.println("⚠️  Confianza baja, clasificando como General");
  classification = 2;  // General
}
```

**Documentación final:**
- Crear manual de usuario
- Documentar código
- Crear guía de mantenimiento
- Registrar problemas conocidos y soluciones

---

## 📚 RECURSOS Y REFERENCIAS

### Librerías necesarias:
- **ESP32-Camera**: https://github.com/espressif/esp32-camera
- **TensorFlow Lite Micro**: https://github.com/tensorflow/tflite-micro-arduino-examples
- **EloquentTinyML**: https://github.com/eloquentarduino/EloquentTinyML
- **ESP32Servo**: https://github.com/madhephaestus/ESP32Servo

### Datasets públicos:
- **TrashNet**: https://github.com/garythung/trashnet
- **Garbage Classification**: https://www.kaggle.com/datasets/asdasdasasdas/garbage-classification
- **Waste Classification**: https://www.kaggle.com/datasets/techsash/waste-classification-data

### Documentación técnica:
- ESP32-S3 Datasheet: https://www.espressif.com/sites/default/files/documentation/esp32-s3_datasheet_en.pdf
- OV5640 Datasheet: https://cdn.sparkfun.com/datasheets/Sensors/LightImaging/OV5640_datasheet.pdf
- TFLite Micro Guide: https://www.tensorflow.org/lite/microcontrollers

### Tutoriales recomendados:
- ESP32-CAM con TensorFlow: https://eloquentarduino.com/
- Transfer Learning con MobileNet: https://www.tensorflow.org/tutorials/images/transfer_learning

---

## 🚨 TROUBLESHOOTING COMÚN

### Problema: Cámara no inicializa
**Síntomas:** Error 0x20001 o 0x20002
**Soluciones:**
1. Verificar conexiones físicas (especialmente GND y 3.3V)
2. Reducir `xclk_freq_hz` a 10MHz
3. Verificar que PSRAM está habilitado en platformio.ini
4. Probar con otra cámara (posible hardware defectuoso)

### Problema: Modelo no cabe en memoria
**Síntomas:** Error "tensor arena too small"
**Soluciones:**
1. Reducir `TENSOR_ARENA_SIZE`
2. Usar modelo más pequeño (alpha=0.25)
3. Reducir resolución de entrada (64x64)
4. Habilitar partición `huge_app` en platformio.ini

### Problema: Inferencia muy lenta (>2s)
**Síntomas:** Sistema no responde a tiempo
**Soluciones:**
1. Aumentar frecuencia de CPU a 240MHz
2. Usar cuantización INT8
3. Reducir resolución de imagen
4. Optimizar preprocesamiento

### Problema: Clasificaciones incorrectas
**Síntomas:** Accuracy < 70%
**Soluciones:**
1. Reentrenar con más datos
2. Mejorar iluminación de la cámara
3. Ajustar preprocesamiento (normalización)
4. Aumentar data augmentation en entrenamiento
5. Calibrar colores de la cámara

### Problema: WiFi no conecta
**Síntomas:** Timeout en conexión
**Soluciones:**
1. Verificar SSID y password
2. Acercar ESP32 al router
3. Usar WiFi de 2.4GHz (no 5GHz)
4. Resetear configuración de red

---

## ✅ CHECKLIST FINAL DEL PROYECTO

### Hardware:
- [ ] ESP32-S3 funciona correctamente
- [ ] Cámara OV5640 captura imágenes claras
- [ ] Conexiones físicas seguras
- [ ] Alimentación estable (3.3V)
- [ ] Servos/actuadores responden

### Software:
- [ ] Cámara inicializa sin errores
- [ ] Modelo TFLite cargado correctamente
- [ ] Inferencia < 500ms
- [ ] Accuracy > 85%
- [ ] WiFi conecta y envía datos

### Sistema integrado:
- [ ] Detección de residuos funciona
- [ ] Clasificación precisa en tiempo real
- [ ] Actuadores abren compartimento correcto
- [ ] Display muestra información
- [ ] Datos se registran en la nube

### Pruebas:
- [ ] 30 objetos clasificados correctamente
- [ ] Sistema estable > 1 hora
- [ ] Manejo de errores implementado
- [ ] Documentación completa

---

## 🎯 PRÓXIMOS PASOS Y MEJORAS FUTURAS

1. **Mejora del modelo:**
   - Añadir más categorías (vidrio, metal, orgánico)
   - Implementar detección de objetos múltiples
   - Usar modelo más robusto (EfficientNet)

2. **Interfaz de usuario:**
   - App móvil para monitoreo
   - Dashboard web con analytics
   - Notificaciones cuando el tacho está lleno

3. **Optimizaciones:**
   - Modo de bajo consumo cuando no hay uso
   - Actualización OTA (Over-The-Air) del firmware
   - Auto-calibración del modelo

4. **Escalabilidad:**
   - Red de tachos interconectados
   - Sistema centralizado de gestión
   - Análisis predictivo de llenado

---

## 📝 NOTAS IMPORTANTES

⚠️ **Consideraciones de seguridad:**
- Nunca conectar 5V directamente a la cámara
- Usar fusibles en la alimentación
- Aislar componentes electrónicos del agua/humedad
- Implementar watchdog timer para evitar cuelgues

🔋 **Consumo energético:**
- Modo activo: ~400mA @ 3.3V
- Modo WiFi: +150mA adicional
- Considerar batería o fuente regulada

🌡️ **Temperatura:**
- ESP32-S3 puede calentarse (normal hasta 70°C)
- Añadir ventilación si está en caja cerrada
- Monitorear temperatura con sensor interno

---

## 📧 SOPORTE Y CONTACTO

Si tienes dudas específicas durante la implementación:
1. Revisar este documento primero
2. Consultar documentación oficial de Espressif
3. Buscar en foros de ESP32 (esp32.com, reddit.com/r/esp32)
4. Revisar issues en GitHub de las librerías usadas

---

**¡Éxito con tu proyecto de Clasificador de Residuos! 🚀♻️**

*Última actualización: Noviembre 2025*
