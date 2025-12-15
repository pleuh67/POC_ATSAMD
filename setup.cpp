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
//  out : 0 (Error ou non référencé) / n (Num de carte)  
// @brief Find card number
//                Num_Carte (global) dans fonction
// @param void
// @return :  0 error or Card number
// ---------------------------------------------------------------------------*
uint8_t init2483A(uint8_t *HWEUI)  // Init 2483 ---- Init 2483 ---- Init 2483 ---- Init 2483 
{ uint8_t tryy = 3 , rc;

debugSerial.println("------------------------------------------------------------------");
debugSerial.println("INIT 2483...");

  loraSerial.begin(LoRaBee.getDefaultBaudRate());
   do
   { rc = Init_2483(HWEUI); // 0 = erreur ou Num de carte
debugSerial.println(rc ? "rc Init_2483() true":"rc Init_2483() false");
// Reset_LoRa();  // initialise pas sur reset chaud.
      if (ConfigMateriel.Num_Carte)
      {
        debugSerial.print(" Init 2483 done with card : ");
        debugSerial.println(ConfigMateriel.Num_Carte);
        OLEDDebugDisplay("2483A    Initialized");
      }
      else
      {
        debugSerial.println(" NO 2483 present.");
        OLEDDebugDisplay("2483A   Failed");
      }
      tryy--;
    }  
    while (rc && tryy);
  /*  
    else 
    {
      debugSerial.println(" Init 2483 failed");    
      OLEDDebugDisplay("2483A   Failed");
    }  
*/
// debugSerialPrintLoRaStatus();
  return(rc);
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
