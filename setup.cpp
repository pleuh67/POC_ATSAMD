#define __INIT_DONE
#include ".\define.h"
    
// Port Série Debug ----- initialisation ----- infos projet -----
void initDebugSerial(void)
{
    debugSerial.begin(DEBUG_BAUD);
    while ((!debugSerial) && (millis() < SERIAL_TIMEOUT)) {}
    debugSerial.println("Compiled: " __DATE__ ", " __TIME__ ", " VERSION);
    debugSerial.println(PROJECT_NAME);
}



void init2483A(void)
{
// Init 2483 ---- Init 2483 ---- Init 2483 ---- Init 2483 ---- Init 2483 
debugSerial.println("------------------------------------------------------------------");
debugSerial.println("INIT 2483...");

//  LoRaBee.setDiag(debugSerial);  // Debug des commandes
  debugDisplayOLED("Initializing 2483");
  
  loraSerial.begin(LoRaBee.getDefaultBaudRate());
  
//debugSerial.print("LoRaBee.getDefaultBaudRate: ");    debugSerial.println(LoRaBee.getDefaultBaudRate());
// GETHWEUI
    if (!Init_2483())
      if (Ruche.Num_Carte)
      {
        debugSerial.print(" Init 2483 done with card : ");
        debugSerial.println(Ruche.Num_Carte);
        debugDisplayOLED("2483    Initialized");
      }
      else
      {
        debugSerial.println(" NO 2483 present.");
        debugDisplayOLED("2483    Failed");
      }
    else 
    {
      debugSerial.println(" Init 2483 failed");    
      debugDisplayOLED("2483    Failed");
    }  
//   blink(LED_BUILTIN,200);
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
  debugDisplayOLED("Sending testPayload");
  Send_LoRa_Mess((uint8_t*)testPayload,7);
  debugDisplayOLED("LoRa    Initialized");
  }
  else
  {
//debugSerial.println("Reboot message SENT");
  debugDisplayOLED("LoRa    Failed");  
  }

/*
  blink(LED_BUILTIN,200);
// Flash ---- Flash ---- Flash ---- Flash ---- Flash ---- Flash ---- Flash 
// set FLASH to deep sleep & reset SPI pins for min. energy consumption
  DFlashUltraDeepSleep(); 
*/
  debugDisplayOLED("---- SETUP DONE ----");
delay(1000);
  display.clearDisplay();
  display.display();
}
