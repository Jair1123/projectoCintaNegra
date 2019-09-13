#include <ESP8266WiFi.h>
#include <PubSubClient.h>

//-------------------VARIABLES GLOBALES--------------------------
int contconexion = 0;

const char *ssid = "BlackPhone";
const char *password = " ";
char   SERVER[50]   = "3.83.223.148"; //"soldier.cloudmqtt.com"
int    SERVERPORT   = 14591;
String USERNAME = "enaeteby";   
char   PASSWORD[50] = "frF-bTU_Ct51";     

unsigned long previousMillis = 0;

char charPulsador [15];
String strPulsador;
String strPulsadorUltimo;

char PLACA[50];

char valueStr[15];
String strtemp = "";
char TEMPERATURA[50];
char PULSADOR[50];
char SALIDADIGITAL[50];
char SALIDAANALOGICA[50];


//-------------------------------------------------------------------------
WiFiClient espClient;
PubSubClient client(espClient);

//------------------------CALLBACK-----------------------------
void callback(char* topic, byte* payload, unsigned int length) {

  char PAYLOAD[5] = "    ";
  
  Serial.print("Mensaje Recibido: [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    PAYLOAD[i] = (char)payload[i];
  }
  Serial.println(PAYLOAD);

  if (String(topic) ==  String(SALIDADIGITAL)) {
    if (payload[1] == 'N'){
     digitalWrite(2, LOW);
    }
    if (payload[1] == 'F'){
      digitalWrite(2, HIGH);
    }
  }

  if (String(topic) ==  String(SALIDAANALOGICA)) {
    if (payload[1] == 'N'){
     digitalWrite(13, LOW);
    }
    if (payload[1] == 'F'){
      digitalWrite(13, HIGH);
    }
  }
}

//------------------------RECONNECT-----------------------------
void reconnect() {
  uint8_t retries = 3;
  // Loop hasta que estamos conectados
  while (!client.connected()) {
    Serial.print("Intentando conexion MQTT...");
    // Crea un ID de cliente al azar
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    USERNAME.toCharArray(PLACA, 50);
    if (client.connect("", PLACA, PASSWORD)) {
      Serial.println("conectado");
      client.subscribe(SALIDADIGITAL);
      client.subscribe(SALIDAANALOGICA);
    } else {
      Serial.print("fallo, rc=");
      Serial.print(client.state());
      Serial.println(" intenta nuevamente en 5 segundos");
      // espera 5 segundos antes de reintentar
      delay(5000);
    }
    retries--;
    if (retries == 0) {
      // esperar a que el WDT lo reinicie
      while (1);
    }
  }
}

//------------------------SETUP-----------------------------
void setup() {

  //prepara GPI13 y 12 como salidas 
  pinMode(13, OUTPUT); // D7 salida analógica
  digitalWrite(13, LOW); // analogWrite(pin, value);
  pinMode(2, OUTPUT); // D6 salida digital
  digitalWrite(2, LOW);

  // Entradas
  pinMode(14, INPUT); // D5

  // Inicia Serial
  Serial.begin(115200);
  Serial.println("");

  // Conexión WIFI
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED and contconexion <50) { //Cuenta hasta 50 si no se puede conectar lo cancela
    ++contconexion;
    delay(500);
    Serial.print(".");
  }
  if (contconexion <50) {
      //para usar con ip fija
      //IPAddress ip(192,168,1,156); 
      //IPAddress gateway(192,168,1,1); 
      //IPAddress subnet(255,255,255,0); 
      //WiFi.config(ip, gateway, subnet); 
      
      Serial.println("");
      Serial.println("WiFi conectado");
      Serial.println(WiFi.localIP());
  }
  else { 
      Serial.println("");
      Serial.println("Error de conexion");
  }
  
  client.setServer(SERVER, SERVERPORT);
  client.setCallback(callback);

  String temperatura = "/" + USERNAME + "/" + "temperatura"; 
  temperatura.toCharArray(TEMPERATURA, 50);
  String pulsador = "/" + USERNAME + "/" + "pulsador"; 
  pulsador.toCharArray(PULSADOR, 50);
  String salidaDigital = "/" + USERNAME + "/" + "salidaDigital"; 
  salidaDigital.toCharArray(SALIDADIGITAL, 50);
  String salidaAnalogica = "/" + USERNAME + "/" + "salidaAnalogica"; 
  salidaAnalogica.toCharArray(SALIDAANALOGICA, 50);
  
}

//--------------------------LOOP--------------------------------
void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  unsigned long currentMillis = millis();
    
  if (currentMillis - previousMillis >= 10000) { //envia la temperatura cada 10 segundos
    previousMillis = currentMillis;
    int analog = analogRead(17);
    float temp = analog*0.322265625;
    strtemp = String(temp, 1); //1 decimal
    strtemp.toCharArray(valueStr, 15);
    Serial.println("Enviando: [" +  String(TEMPERATURA) + "] " + strtemp);
    client.publish(TEMPERATURA, valueStr);
  }
  
  if (digitalRead(14) == 0) {
    strPulsador = "Cerrada";
  } else {
    strPulsador = "Abierta";
  }

  if (strPulsador != strPulsadorUltimo) { //envia el estado del pulsador solamente cuando cambia.
    strPulsadorUltimo = strPulsador;
    strPulsador.toCharArray(valueStr, 15);
    Serial.println("Enviando: [" +  String(PULSADOR) + "] " + strPulsador);
    client.publish(PULSADOR, valueStr);
  }
}
