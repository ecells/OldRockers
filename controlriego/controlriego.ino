#include <WiFi101.h> 
#include <SPI.h>  
#include <TelegramBot.h>  
#include <SimpleTimer.h>
 
// Initialize Wifi connection to the router  
char ssid[] = "WLAN_1674";             // your network SSID (name)  
char pass[] = "175e08dec7d27af558b6";            // your network key 
//char ssid[] = "iPhone de Miguel Angel";             // your network SSID (name)  
//char pass[] = "descompresion";            // your network key 
//char ssid[] = "EWA@GUEST";             // your network SSID (name)  
//char pass[] = "trgbxbt6";            // your network key 
int keyIndex = 0;                 // your network key Index number (needed only for WEP)
int status = WL_IDLE_STATUS;
WiFiServer server(80);

// Initialize Telegram BOT  
//const char *BotToken = "399706449:AAEeu-ix49OS3dDMTNzxfYPpPKHCQN_SbLM";    // your Bot Token  
//WiFiSSLClient client;  
//TelegramBot bot(BotToken,client);

const int ledGeneral = 13;  // Estado General
int General = 0; // Variable que contiene el estado general del sistema (0:OFF, 1: Standby, 2: ON)
int estadoLedGeneral = 0; // se usa para hacer que el led parpadee
const int ledVal1 = 12; // Pin valvula 1 
int estadoVal1 = 0; // se usa para hacer que el led parpadee
int Val1 = 0; // Variable que contiene el estado de la valvula 1 (0: OFF; 1: ON pero no riega; 2: ON)
const int ledVal2 = 10; // Pin valvula 2
int estadoVal2 = 0; // se usa para hacer que el led parpadee
int Val2 = 0; // Variable que contiene el estado de la valvula 2 (0: OFF; 1: ON pero no riega; 2: ON)
const int senHum1Pin = A0; // Sensor humedad 1
int senHum1;    // variable to read the value from the analog pin 
const int senHum2Pin = A1; // Sensor humedad 2
int senHum2;    // variable to read the value from the analog pin 
SimpleTimer timer; //the timer object
SimpleTimer timerBot; //the timer object
const int interval = 1000; //interval in miliseconds
const int cicloRiego = 20000 / interval;
const int cicloVal = 10000 / interval;
int cuentaInterval = 0;
int cuentaIntervalVal1 = 0;
int cuentaIntervalVal2 = 0;
int offset;


void printWiFiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
  // print where to go in a browser:
  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);
}

/*
void leeMensajes()
{

 Serial.println("tratamiento mensajes");
 
 message m = bot.getUpdates(); // Read new messages  
 if (m.text.equals("on")) 
       {  
   General = 1; 
   Serial.println("message On received");  
   bot.sendMessage(m.chat_id, "The System is now ON");  
 }  
 else if (m.text.equals("off")) 
       {  
   General = 0; 
   Serial.println("message Off received");  
   bot.sendMessage(m.chat_id, "The Systemd is now OFF");  
 }

 Serial.println(m.chat_id);
}
*/

void bucleRiego()
{

 Serial.println("bucle riego");


  if (General == 0)
  {
      cuentaInterval = 0;
      cuentaIntervalVal1 = 0;
      cuentaIntervalVal2 = 0;
      digitalWrite(ledGeneral, LOW);
      digitalWrite(ledVal1, LOW);
      digitalWrite(ledVal2, LOW);
  }
  else if ((General == 1) && (cuentaInterval < cicloRiego))
  {
    cuentaInterval = cuentaInterval + 1;
    Serial.println(cuentaInterval);

    // Standby - el led general parpadea
    if (estadoLedGeneral == 0)
    {
      digitalWrite(ledGeneral, LOW);
      estadoLedGeneral = 1;
    }
    else
    {
      digitalWrite(ledGeneral, HIGH);
      estadoLedGeneral = 0;
    }
  }
  else if ((General == 1) && (cuentaInterval == cicloRiego))
  {
    // Se dan las condiciones para empezar un ciclo de riego
    digitalWrite(ledGeneral, HIGH); //el led general queda encendido

    // Riego Valvula 1
    // Primero se determina si hay que abrirla o no por humedad
    Serial.println(analogRead(senHum1Pin));
    if (analogRead(senHum1Pin) < 700) // humedad suficiente
    {
      Val1 = 1; // humedad suficiente, led parpadea
    }
    else
    {
      Val1 = 2; // humedad insuficiente, led fijo
    }
    Serial.print("Val1: ");
    Serial.println(Val1);

    Serial.print("Interval Val1: ");
    Serial.println(cuentaIntervalVal1);
    
    if (cuentaIntervalVal1 < cicloVal)
    {
      cuentaIntervalVal1 = cuentaIntervalVal1 + 1;
      if (Val1 == 1) // Hacemos que el led parpadee
      {
        if (estadoVal1 == 0)
        {
          digitalWrite(ledVal1, LOW);
          estadoVal1 = 1;          
        }
        else
        {
          digitalWrite(ledVal1, HIGH);
          estadoVal1 = 0;             
        }
      }
      else if (Val1 == 2)
      {
        digitalWrite(ledVal1, HIGH);
        estadoVal1 = 1;
      }
    }
    else // Ha acabado el ciclo de la valvula 1
    {
      Val1 = 0;
      estadoVal1 = 0;
      digitalWrite(ledVal1, LOW);
    }
/*
    // Riego Valvula 2
    // Primero se determina si hay que abrirla o no por humedad
    if (analogRead(senHum2Pin) < 500) // humedad suficiente
    {
      Val2 = 1; // humedad suficiente, led parpadea
    }
    else
    {
      Val2 = 2; // humedad insuficiente, led fijo
    }
    if (cuentaIntervalVal2 < cicloVal)
    {
      cuentaIntervalVal2 = cuentaIntervalVal2 + 1;
      if (Val2 == 1) // Hacemos que el led parpadee
      {
        if (estadoVal2 == 0)
        {
          digitalWrite(ledVal2, LOW);
          estadoVal2 = 1;          
        }
        else
        {
          digitalWrite(ledVal2, HIGH);
          estadoVal2 = 0;             
        }
      }
      else if (Val2 == 2)
      {
        digitalWrite(ledVal2, HIGH);
        estadoVal2 = 1;
      }
    }
    else // Ha acabado el ciclo de la valvula 1
    {
      Val2 = 0;
      estadoVal2 = 0;
      digitalWrite(ledVal2, LOW);
    }    
*/    
    // Termina el ciclo de riego
//    if ((cuentaIntervalVal1 == cicloVal) && (cuentaIntervalVal2 == cicloVal))
    if (cuentaIntervalVal1 == cicloVal)
    {
      General = 1;
      cuentaInterval = 0;
      cuentaIntervalVal1 = 0;
      cuentaIntervalVal2 = 0;
      digitalWrite(ledVal1, LOW);
      digitalWrite(ledVal2, LOW);
    }
  }
}

void setup() 
{  
 Serial.begin(115200);  
 while (!Serial) {}  //Start running when the serial is open 
 delay(3000);  
 // attempt to connect to Wifi network:  
 Serial.print("Connecting Wifi: ");  
 Serial.println(ssid);  
 while (WiFi.begin(ssid, pass) != WL_CONNECTED) 
       {  
   Serial.print(".");  
   delay(500);  
 }  
 Serial.println("");  
 Serial.println("WiFi connected");  
 pinMode(ledGeneral, OUTPUT);
 pinMode(ledVal1, OUTPUT); 
 pinMode(ledVal2, OUTPUT); 

 timer.setInterval(interval, bucleRiego); // call Riega every "interval" miliseconds
 //timerBot.setInterval(10000, leeMensajes); // call Riega every "interval" miliseconds

  server.begin();                           // start the web server on port 80
  printWiFiStatus();                        // you're connected now, so print out the status
//  bot.begin();

}  



void loop() 
{  
  WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    Serial.println("new client");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // the content of the HTTP response follows the header:
            client.print("Click <a href=\"/H\">here</a> turn the system on<br>");
            client.print("Click <a href=\"/L\">here</a> turn the system off<br>");

            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          }
          else {      // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        }
        else if (c != '\r') {    // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        // Check to see if the client request was "GET /H" or "GET /L":
        if (currentLine.endsWith("GET /H")) {
          General = 1;
        }
        if (currentLine.endsWith("GET /L")) {
          General = 0;
        }
        Serial.println(currentLine);
      }
    }
    // close the connection:
    client.stop();
    Serial.println("client disconnected");


  }

   timer.run();
   //timerBot.run();
}  




