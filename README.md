Estamos creando un Clasificador de Residuos Automático usando los tachos de UTEC. El hardware disponible es un ESP32-S3 WROOM N16R8 con WiFi+Bluetooth y una cámara OV5640.
El objetivo es que el ESP32-S3 pueda tomar fotos y clasificar residuos en 3 tipos: Plástico, Papel, General (todo lo que no sea plástico o papel), usando un modelo de Machine Learning que se pueda ejecutar en el módulo local.

Objetivo del prompt:
Queremos primero probar que la cámara OV5640 funciona, y luego desarrollar el clasificador. Me preocupa especialmente la mecánica de cómo se hará todo.

Solicito:

Crear un plan detallado paso a paso, incluyendo pruebas de hardware, instalación de software, pruebas de cámara y desarrollo del modelo.

Ejecutar y documentar cada paso uno por uno, confirmando que funciona y obteniendo el output esperado antes de continuar al siguiente paso.

Revisar cada etapa y sugerir ajustes si algo falla o no es óptimo.

Consideraciones adicionales:

Priorizar simulación o prueba local antes de integrar al tacho.

Indicar cómo verificar el resultado de cada paso.

Sugerir librerías y herramientas específicas para ESP32-S3 y OV5640.