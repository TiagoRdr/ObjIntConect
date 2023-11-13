// Inclusão das bibliotecas do DHT11 e o módulo WiFi do ESP8266
#include <DHT.h>  
#include <ESP8266WiFi.h>
 
// Chave da API do ThingSpeak utilizado para enviar os dados do DHT11
String apiKey = "UEF7YM3SSFOGBTIR";   

// Dados da rede WIFi
const char *ssid =  "MARCIO RODRIGUES";     
const char *pass =  "15132128";

// Host da plataforma ThingSpeak
const char* server = "api.thingspeak.com";
 
// O pino onde o DHT11 está conectado e a instancia dele.
#define DHTPIN 0  
DHT dht(DHTPIN, DHT11);

//Objeto da conexão WiFi
WiFiClient client;
 

// Conectividade com Wifi 
void setup() 
{
       Serial.begin(115200);
       delay(10);
       dht.begin();    
 
 
       WiFi.begin(ssid, pass);
 
      while (WiFi.status() != WL_CONNECTED) 
     {
            delay(500);
            Serial.print(".");
     }
      Serial.println("");
      Serial.println("Conectado ao Wifi");
 
}
 
void loop() 
{  
      // Verifica a temperatura e umidade do DHT11
      float h = dht.readHumidity();
      float t = dht.readTemperature();

              
              if (isnan(h) || isnan(t)) 
                 {
                     Serial.println("Falha ao ler dados do sensor");
                      return;
                 }
                        // Se conectado ao host do ThingSpeak
                         if (client.connect(server,80))   
                      {  
                            
                             String postStr = apiKey;
                             postStr +="&field1=";
                             postStr += String(t);
                             postStr +="&field2=";
                             postStr += String(h);
                             postStr += "\r\n\r\n";
 
                             client.print("POST /update HTTP/1.1\n");
                             client.print("Host: api.thingspeak.com\n");
                             client.print("Connection: close\n");
                             client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
                             client.print("Content-Type: application/x-www-form-urlencoded\n");
                             client.print("Content-Length: ");
                             client.print(postStr.length());
                             client.print("\n\n");
                             client.print(postStr);
 
                             Serial.print("Temperatura: ");
                             Serial.print(t);
                             Serial.print("Graus Celsius, Umidade: ");
                             Serial.print(h);
                             Serial.println("%. Send to Thingspeak.");
                        }
          client.stop();
 
          Serial.println("Aguardando...");
  
  // Delay de 60 segundos entre as atualizações de informações no ThingSpeak
  delay(60000);
}