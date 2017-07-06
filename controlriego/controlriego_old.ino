//#include <WiFi101.h> 
//#include <SPI.h>  
//#include <TelegramBot.h>  
#include <SimpleTimer.h>
 
// Initialize Wifi connection to the router  
//char ssid[] = "xxxx";             // your network SSID (name)  
//char pass[] = "yyyy";            // your network key 
// Initialize Telegram BOT  
const char* BotToken = "399706449:AAEeu-ix49OS3dDMTNzxfYPpPKHCQN_SbLM";    // your Bot Teken  
//WiFiSSLClient client;  
//TelegramBot bot(BotToken,client);  
const int ledGeneral = 13;  // Estado General
int General = 1; // Variable que contiene el estado general del sistema (0:OFF, 1: Standby, 2: ON)
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
const int interval = 1000; //interval in miliseconds
const int cicloRiego = 15000 / interval;
const int cicloVal = 10000 / interval;
int cuentaInterval = 0;
int cuentaIntervalVal1 = 0;
int cuentaIntervalVal2 = 0;

void setup() 
{  
 Serial.begin(115200);  
 while (!Serial) {}  //Start running when the serial is open 
 delay(3000);  
 // attempt to connect to Wifi network:  
// xSerial.print("Connecting Wifi: ");  
// Serial.println(ssid);  
// while (WiFi.begin(ssid, pass) != WL_CONNECTED) 
//       {  
//   Serial.print(".");  
//   delay(500);  
// }  
// Serial.println("");  
// Serial.println("WiFi connected");  
// bot.begin();  
 pinMode(ledGeneral, OUTPUT);
 pinMode(ledVal1, OUTPUT); 
 pinMode(ledVal2, OUTPUT); 
 timer.setInterval(interval, bucleRiego); // call Riega every "interval" miliseconds

}  


void bucleRiego()
{

  // Aqui va a ir la parte de leer mensaje de ON / OFF
  //
  //
  //

  if (General == 0)
  {
    cuentaInterval = 0;
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
    if (analogRead(senHum1Pin) < 500) // humedad suficiente
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

void loop() 
{  
// message m = bot.getUpdates(); // Read new messages  
// if (m.text.equals("ON")) 
//       {  
//   General = 1; // Standby
   //digitalWrite(ledGeneral, HIGH);  
//   Serial.println("ON message received");  
//   bot.sendMessage(m.chat_id, "The Led is now STANDBY");  
// }  
// else if (m.text.equals("OFF")) 
//       {  
   //digitalWrite(ledGeneral, LOW);  
//   General = 0;
//   Serial.println("OFF message received");  
//   bot.sendMessage(m.chat_id, "The Led is now OFF");  
// }  

 timer.run();

}  
