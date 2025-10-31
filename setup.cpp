//       1         2         3         4         5         6         7        7
//34567890123456789012345678901234567890123456789012345678901234567890123456789
// IMPRESSION 79 COLONES EN TAILLE 12
//
// ---------------------------------------------------------------------------*
//       _____      _                               
//      / ____|    | |                              
//     | (___   ___| |_ _   _ _ __   ___ _ __  _ __ 
//      \___ \ / _ \ __| | | | '_ \ / __| '_ \| '_ \
//      ____) |  __/ |_| |_| | |_) | (__| |_) | |_) |
//     |_____/ \___|\__|\__,_| .__(_)___| .__/| .__/
//                           | |        | |   | |   
//                           |_|        |_|   |_|    
// ---------------------------------------------------------------------------*
#define __INIT_DONE
#include ".\define.h"
    
// Port Série Debug ----- initialisation ----- infos projet -----
// ---------------------------------------------------------------------------*
// ---------------------------------------------------------------------------*
void initDebugSerial(void)
{
    debugSerial.begin(DEBUG_BAUD);
    while ((!debugSerial) && (millis() < SERIAL_TIMEOUT)) {}
    delay(1000);
    debugSerial.println("Compiled: " __DATE__ ", " __TIME__ ", " VERSION);
    debugSerial.println(PROJECT_NAME);
}

// ---------------------------------------------------------------------------*
// ---------------------------------------------------------------------------*
void softReset() 
{
// Reset immédiat du microcontrôleur
  NVIC_SystemReset();
// Le code ne continue jamais après cette ligne
}

// ---------------------------------------------------------------------------*
// ---------------------------------------------------------------------------*
void init2483A(void)  // Init 2483 ---- Init 2483 ---- Init 2483 ---- Init 2483 
{
debugSerial.println("------------------------------------------------------------------");
debugSerial.println("INIT 2483...");

//  LoRaBee.setDiag(debugSerial);  // Debug des commandes
//  OLEDDebugDisplay("Initializing 2483");
  
  loraSerial.begin(LoRaBee.getDefaultBaudRate());
  
//debugSerial.print("LoRaBee.getDefaultBaudRate: ");    debugSerial.println(LoRaBee.getDefaultBaudRate());
// GETHWEUI

   if (!Init_2483())
      if (Ruche.Num_Carte)
      {
        debugSerial.print(" Init 2483 done with card : ");
        debugSerial.println(Ruche.Num_Carte);
        OLEDDebugDisplay("2483A    Initialized");
      }
      else
      {
        debugSerial.println(" NO 2483 present.");
        OLEDDebugDisplay("2483A   Failed");
      }
    else 
    {
      debugSerial.println(" Init 2483 failed");    
      OLEDDebugDisplay("2483A   Failed");
    }  
// debugSerialPrintLoRaStatus();
}


// ---------------------------------------------------------------------------*
// ---------------------------------------------------------------------------*
void initLoRa(void)
{
// INIT LoRa ---- INIT LoRa ---- INIT LoRa ---- INIT LoRa ---- INIT LoRa 
debugSerial.println("------------------------------------------------------------------");
debugSerial.println("INIT LoRa...");
//  Reset_LoRa();  // initialise pas sur reset chaud.

  if (setupLoRa())
  {
debugSerial.println("Init LoRa done.");
debugSerial.println("Test sending LoRa testPayload (7) (Restart)..."); 
    sendLoRaPayload((uint8_t*)testPayload,7);
    OLEDDebugDisplay("LoRa    Initialized");
  }
  else
  {
    OLEDDebugDisplay("LoRa Failed");  
  }

/*
  blink(LED_BUILTIN,200);
// Flash ---- Flash ---- Flash ---- Flash ---- Flash ---- Flash ---- Flash 
// set FLASH to deep sleep & reset SPI pins for min. energy consumption
  DFlashUltraDeepSleep(); 
*/
//  OLEDDebugDisplay("---- SETUP DONE ----");
//delay(1000);
//  display.clearDisplay();
//  display.display();
}

// INIT DHT22 ---- INIT DHT22 ---- INIT DHT22 ---- INIT DHT22 ---- 
// ---------------------------------------------------------------------------*
// ---------------------------------------------------------------------------*
void DHTInit(void)
{
debugSerial.println("------------------------------------------------------------------");
debugSerial.println("INIT DHT22...");
  dht.begin(); // temperature
  debugSerial.println("done.");
OLEDDebugDisplay("DHT22   Initialized");
}
