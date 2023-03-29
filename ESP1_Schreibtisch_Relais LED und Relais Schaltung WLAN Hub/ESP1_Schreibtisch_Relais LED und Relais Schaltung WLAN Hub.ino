/*
Das Programm ist per WLAN mit dem Heimnetzwerk verbunden.
Es schaltet die LED am Schreibtisch und den WLAN Hub.
Die LED wird anhand der berechneten Sonnenuntergangszeit über NodeRed geschaltet.
Das Relais für den WLAN Hub wird einmalig nachts um 4 Uhr geschaltet um diesen neu zu starten und zu initialisieren.
*/


#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>

//----------------------------------------------------------------------------------------------------------------------

#define wifi_ssid "Vodafone-5E34"
#define wifi_password "fbNeGY4n6s7h42eK"
#define mqtt_server "192.168.0.59"

//----------------------------------------------------------------------------------------------------------------------

//Test
int LED = D6;
int WLAN_Router  = D1;

//----------------------------------------------------------------------------------------------------------------------

WiFiClient espClient;  
PubSubClient client(espClient);
 

//#######################################################################################################################

void setup() 
{  

    pinMode(LED, OUTPUT);
    digitalWrite(LED, HIGH);

    pinMode(WLAN_Router, OUTPUT);
    digitalWrite(WLAN_Router, HIGH);

    Serial.begin(9600); 
    dht.begin();  
    setup_wifi();  
    client.setServer(mqtt_server, 1883); 
    client.setCallback(callback);  

}  
//#######################################################################################################################

void setup_wifi() 
{  
    delay(10);  
    // We start by connecting to a WiFi network  
    Serial.println();  
    Serial.print("Connecting to ");  
    Serial.println(wifi_ssid);  
    WiFi.begin(wifi_ssid, wifi_password);  
    while (WiFi.status() != WL_CONNECTED) 
    {  
        Serial.print("."); 
        delay(500);  
    }  
    Serial.println("");  
    Serial.println("WiFi connected");  
    Serial.println("IP address: ");  
    Serial.println(WiFi.localIP());  
}  

//#######################################################################################################################

void callback(char* topic, byte* message, unsigned int length) 
{
    Serial.print("Message arrived on topic: ");
    Serial.print(topic);
    Serial.print(". Message: ");
    String messageTemp;


    for (int i = 0; i < length; i++) 
    {
        Serial.print((char)message[i]);
        messageTemp += (char)message[i];
    }

    client.subscribe("esp3/LED");
    client.subscribe("esp1/WLAN_aus");


    delay(100);


    if(messageTemp == 1)
    {
        digitalWrite(LED, LOW);

    }
    else if(messageTemp == 2)
    {
        digitalWrite(LED, HIGH);
    }

    else if (messageTemp == 3)
    {
        Serial.println("Geht");
        //digitalWrite(WLAN_Router, LOW);
        //delay(10000)
        //digitalWrite(WLAN_Router, HIGH);
    }

}

//#######################################################################################################################

void reconnect() 
{
  
    while (!client.connected()) 
    {
        WiFi.begin(wifi_ssid, wifi_password); 
        Serial.print("Attempting MQTT connection...");
    
        String clientId= "ESP8266-";
        clientId += String(random(0xffff), HEX);
        // Attempt to connect
        delay(100);

        if (client.connect(clientId.c_str()))  
        {
            Serial.println("connected");

            client.subscribe("esp3/LED");
            client.subscribe("esp1/WLAN_aus");

      
        } 
        else 
        {

            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
 
            //Serial.println(ResetCounter);

            // Wait 5 seconds before retrying
            delay(5000);
        }
   }
}
//#######################################################################################################################
   
void loop() 
{  
    if (!client.connected()) 
    {  
        reconnect();  
  
    }
    client.loop();
    
    delay(50);
  
 }  
//#######################################################################################################################
