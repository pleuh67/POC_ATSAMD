#define __INIT_DONE
#include ".\define.h"
    
// Port Série Debug ----- initialisation ----- infos projet -----
void initDebugSerial(void)
{
    debugSerial.begin(DEBUG_BAUD);
    while ((!debugSerial) && (millis() < SERIAL_TIMEOUT)) {}
    delay(1000);
    debugSerial.println("Compiled: " __DATE__ ", " __TIME__ ", " VERSION);
    debugSerial.println(PROJECT_NAME);
}



void init2483A(void)
{
// Init 2483 ---- Init 2483 ---- Init 2483 ---- Init 2483 ---- Init 2483 
debugSerial.println("------------------------------------------------------------------");
debugSerial.println("INIT 2483...");

//  LoRaBee.setDiag(debugSerial);  // Debug des commandes
  OLEDDebugDisplay("Initializing 2483");
  
  loraSerial.begin(LoRaBee.getDefaultBaudRate());
  
//debugSerial.print("LoRaBee.getDefaultBaudRate: ");    debugSerial.println(LoRaBee.getDefaultBaudRate());
// GETHWEUI


/* */
   if (!Init_2483())
      if (Ruche.Num_Carte)
      {
        debugSerial.print(" Init 2483 done with card : ");
        debugSerial.println(Ruche.Num_Carte);
        OLEDDebugDisplay("2483    Initialized");
      }
      else
      {
        debugSerial.println(" NO 2483 present.");
        OLEDDebugDisplay("2483    Failed");
      }
    else 
    {
      debugSerial.println(" Init 2483 failed");    
      OLEDDebugDisplay("2483    Failed");
    }  
// debugSerialPrintLoRaStatus();
}


void initLoRa(void)
{
// INIT LoRa ---- INIT LoRa ---- INIT LoRa ---- INIT LoRa ---- INIT LoRa 
debugSerial.println("------------------------------------------------------------------");
debugSerial.println("INIT LoRa...");
//  Reset_LoRa();  // initialise pas sur reset chaud.
  setupLoRa();

  if (setupLoRa())
  {
//debugSerial.println("Reboot message SENT");
debugSerial.println("Init LoRa done.");
debugSerial.println("------------------------------------------------------------------");
debugSerial.println("Test sending LoRa testPayload (7) (Restart)..."); 
  OLEDDebugDisplay("Sending testPayload");
  Send_LoRa_Mess((uint8_t*)testPayload,7);
  OLEDDebugDisplay("LoRa    Initialized");
  }
  else
  {
//debugSerial.println("Reboot message SENT");
  OLEDDebugDisplay("LoRa    Failed");  
  }

/*
  blink(LED_BUILTIN,200);
// Flash ---- Flash ---- Flash ---- Flash ---- Flash ---- Flash ---- Flash 
// set FLASH to deep sleep & reset SPI pins for min. energy consumption
  DFlashUltraDeepSleep(); 
*/
  OLEDDebugDisplay("---- SETUP DONE ----");
delay(1000);
  display.clearDisplay();
  display.display();
}

// INIT DHT22 ---- INIT DHT22 ---- INIT DHT22 ---- INIT DHT22 ---- 
void DHTInit(void)
{
debugSerial.println("------------------------------------------------------------------");
debugSerial.println("INIT DHT22...");
  dht.begin(); // temperature
  debugSerial.println("done.");
OLEDDebugDisplay("DHT22   Initialized");
}
