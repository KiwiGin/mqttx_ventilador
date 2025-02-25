# Control de Ventilador con ESP32 y MQTT

## Descripción
Este proyecto utiliza un ESP32 para monitorear temperatura, humedad y presión atmosférica con un sensor BME280 y enviar estos datos a un servidor MQTT. Además, permite el control remoto de un relé para activar o desactivar un ventilador según la temperatura medida.

## Tecnologías Utilizadas
- **ESP32**
- **BME280** (Sensor de temperatura, humedad y presión)
- **WiFi** (Para la conexión a la red inalámbrica)
- **MQTT** (Protocolo de comunicación liviano)
- **MQTT X** (Cliente MQTT para pruebas y monitoreo)
- **Mosquitto** (Broker MQTT público: test.mosquitto.org)

## Requisitos de Hardware
- ESP32
- Sensor BME280
- Módulo relé
- Cables de conexión

## Configuración del Entorno
1. **Instalar Arduino IDE** y agregar la placa ESP32 en el Gestor de Tarjetas.
2. **Instalar las librerías necesarias**:
   - `PubSubClient` para la comunicación MQTT
   - `Adafruit BME280` para el sensor
3. **Configurar credenciales WiFi y MQTT** en el código:
   ```cpp
   #define WIFI_SSID "TuSSID"
   #define WIFI_PASSWORD "TuContraseña"
   #define MQTT_SERVER "test.mosquitto.org"
   ```

## Esquema de Conexión
| ESP32 | BME280 | Relé |
|-------|--------|------|
| 3.3V  | VCC    | VCC  |
| GND   | GND    | GND  |
| 21    | SDA    | -    |
| 22    | SCL    | -    |
| 5     | -      | IN   |

## Funcionalidades
1. **Monitoreo de sensores**: El ESP32 lee los valores de temperatura, humedad y presión cada 5 segundos y los publica en los siguientes tópicos MQTT:
   - `esp32/sensores/temperatura`
   - `esp32/sensores/humedad`
   - `esp32/sensores/presion`
2. **Control del ventilador**: Se puede encender o apagar el relé enviando `1` o `0` al tópico `esp32/rele` desde MQTT X u otro cliente MQTT.
3. **Activación automática**: Si la temperatura supera los 28°C, el ESP32 enciende el relé automáticamente y lo apaga cuando baja de este umbral.

## Uso con MQTT X
1. Descargar e instalar [MQTT X](https://mqttx.app/).
2. Configurar una conexión con el broker `test.mosquitto.org` y puerto `1883`.
3. Suscribirse a los tópicos:
   - `esp32/sensores/#` para ver los datos de los sensores.
   - `esp32/rele` para recibir el estado del relé.
4. Publicar `1` o `0` en `esp32/rele` para activar o desactivar el ventilador manualmente.

## Posibles Mejoras
- Implementar autenticación en el broker MQTT.
- Usar un broker local para mayor seguridad.
- Agregar una interfaz web para el control del sistema.

## Autor
Proyecto desarrollado por el grupo 1:
-Aldair Rivera
-Davis Cartagena
-Eva Bayes
-José Sernaqué
-Said Najarro

