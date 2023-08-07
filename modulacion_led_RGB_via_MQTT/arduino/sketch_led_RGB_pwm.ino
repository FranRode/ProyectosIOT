#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h >
#include <WiFiManager.h>
#include <Ticker.h>
#include <String.h>
#include <queue>


// Objetos WiFiManager y PubSubClient
WiFiManager wifiManager;
WiFiClient wifiClient;
PubSubClient client(wifiClient);
bool system_available = false;
bool responseReceived = false;

// Variables para el Broker MQTT
const char *mqtt_broker = "test.mosquitto.org";
const char *topic_rojo = "datos/ledRGB/rojo";
const char *topic_verde = "datos/ledRGB/verde";
const char *topic_azul = "datos/ledRGB/azul";

const int mqtt_port = 1883;


void setup() {
  // D4 --> Color verde
  // D5 --> Color azul
  // D6 --> Color rojo
  pinMode(D4, INPUT);
  pinMode(D5, INPUT);
  pinMode(D6, INPUT);
  // Establecer la velocidad del puerto serie en 9600;
  Serial.begin(9600);
  // Establecer conexion
  nodeConnect();
  system_available = true;

}

void nodeConnect(){
  Serial.println(" - - - - - - - - - - - - - - - - - -");
  Serial.println("CONEXIÓN WIFI");
  wifiConnect();
  Serial.println("CONEXIÓN AL SERVIDOR MQTT");
  mqttServerConnect();
}

void wifiConnect() {
  // Descomenta esta línea para restablecer todas las configuraciones de WiFi
  // wifiManager.resetSettings();

  // Si no se puede conectar automáticamente, crea un punto de acceso y portal cautivo y verifica si se establece la conexión
  if (!wifiManager.autoConnect("ESP8266Temp")) {
    Serial.println("Conexión fallida (tiempo de espera)");
    ESP.reset();
    delay(1000);
  }
  Serial.println("Ahora estás conectado a WiFi");
  Serial.println("- - - - - - - - - - - - - - - - - -");
}

void mqttServerConnect() {
  
  // Conectar el cliente al servidor
  client.setServer(mqtt_broker, mqtt_port);
  client.setCallback(callback);
  // Crear el ID del cliente y el mensaje a publicar
  String pub_msg;
  String client_id = "ESP8266-Client-";
  client_id += String(WiFi.macAddress());

  // Repetir hasta que se establezca la conexión
  // Intentar conectarse
  while (!client.connected()) {
    Serial.print("Intentando conexión MQTT...");
    // Intentar conectar
    if (client.connect(client_id.c_str())) {
      Serial.println("¡Conectado!");
      // Si se conecta correctamente, suscribirse a topicos
      client.subscribe(topic_rojo);
      client.subscribe(topic_verde);
      client.subscribe(topic_azul);
      
      Serial.println(" - - - - - - - - - - - - - - - - - -");      
      system_available = true;
    } else {
      Serial.print("Falló con estado: ");
      Serial.print(client.state());
      Serial.println("\nIntentando de nuevo en 5 segundos");
      // Esperar 5 segundos antes de volver a intentar
      delay(5000);
    }
  }
}


void callback(char* topic, byte* payload, unsigned int length){

  responseReceived = true;
  Serial.print("Porcentaje recibido:");
  // Convertir la carga útil a una cadena de caracteres
  char message[length+1];
  for (int i = 0; i < length; i++) {
    message[i] = (char)payload[i];
  }
  message[length] = '\0'; // Agregar un terminador nulo al final de la cadena
  // Imprimir el contenido de la carga útil en el puerto serie
  Serial.println(message);
  Serial.println("- - - - - - - - - - - - - - -");

  if (strcmp(topic, "datos/ledRGB/rojo") == 0) {
    
     // Convierto porcentaje a flotante
    float porcentaje = atof(message);

    float valor_en_bits = 102.3*porcentaje;


    float outputValue = map(valor_en_bits, 0, 1023, 0, 255);

    analogWrite(D6, outputValue);
  
  }

  if (strcmp(topic, "datos/ledRGB/verde") == 0) {
    
     // Convierto porcentaje a flotante
    float porcentaje = atof(message);

    float valor_en_bits = 102.3*porcentaje;


    float outputValue = map(valor_en_bits, 0, 1023, 0, 255);

    analogWrite(D4, outputValue);
  
  }

  if (strcmp(topic, "datos/ledRGB/azul") == 0) {
    
     // Convierto porcentaje a flotante
    float porcentaje = atof(message);

    float valor_en_bits = 102.3*porcentaje;


    float outputValue = map(valor_en_bits, 0, 1023, 0, 255);

    analogWrite(D5, outputValue);
  
  }


  
}


void loop() {
  if(system_available) {  
    client.loop();
  }else{
    while(!system_available){   
      Serial.println("Configuración en proceso...");
    }    
  }
  // Dar una breve pausa para permitir que se ejecuten otros procesos
  yield();
}