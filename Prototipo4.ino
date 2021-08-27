#include <ESP8266HTTPClient.h>
#include <ESP8266WiFiMulti.h>

const char* ssid ="Sparky";
const char* password ="Silvestre";
// Variables para lectura del DHT 11
float t;
float h;
float f;
float hif;//Omitir
float hic;//Omitir

#include <DHT.h>
//#include "DHT.h" // Para otras versiones de IDE de Arduino
#define DHTPIN 2     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT 11
DHT dht(DHTPIN, DHTTYPE, 11);

void setup() {
  Serial.begin(115200);
  Serial.println(F("DHT 11 prueba de conexión con el servidor"));
  dht.begin();

  WiFi.begin(ssid, password);
  Serial.print("Conectando...");
  while (WiFi.status()!= WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Conexión OK!");
  Serial.print("IP Local: ");
  Serial.println(WiFi.localIP());
  

 
}

void loop() {

  LecturaTH();
  // funcion  de envio por POST
  EnvioDatos();

}


// -------------------- funcion de lectura de temperatura y humedad --------------------
void LecturaTH(){

  h = dht.readHumidity();
  t = dht.readTemperature();
  f = dht.readTemperature(true);

  //Verifica si la lectura existe falla o lo intenta de nuevo
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  //hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
//  hic = dht.computeHeatIndex(t, h, false);

 
}

// rutina de envio de datos por POST
void EnvioDatos(){
  if (WiFi.status() == WL_CONNECTED){ 
     HTTPClient http;  // creo el objeto http

     //Si se desea trabajar con más prototipos se cambia la variable temperatura y humedad
     // por ejemplo ahora sería "temperatura2"
     String datos_a_enviar = "temperatura_3=" + String(t) + "&humedad_3=" + String(h); 
      
    //Aquí dentro se pone la ubicacion del public html del dominio que estén usando
    // http.begin("http://proyectoug.ga//EspPost.php");
     http.begin("http://proyectosensores.ml//EspPost.php");
     http.addHeader("Content-Type", "application/x-www-form-urlencoded"); // defino texto plano..

     int codigo_respuesta = http.POST(datos_a_enviar);

     if (codigo_respuesta>0){
      Serial.println("Código HTTP: "+ String(codigo_respuesta));
        if (codigo_respuesta == 200){
          String cuerpo_respuesta = http.getString();
          Serial.println("El servidor respondió: ");
          Serial.println(cuerpo_respuesta);
        }
     } else {
        Serial.print("Error enviado POST, código: ");
        Serial.println(codigo_respuesta);
     }

       http.end();  // libero recursos
       
  } else {
     Serial.println("Error en la conexion WIFI");
  }
  delay(60000); //espera 60s
}
