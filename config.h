//       1         2         3         4         5         6         7        7
//34567890123456789012345678901234567890123456789012345678901234567890123456789
// IMPRESSION 79 COLONES EN TAILLE 12
//
// ---------------------------------------------------------------------------*
//       _____             __ _         _    
//      / ____|           / _(_)       | |   
//     | |     ___  _ __ | |_ _  __ _  | |__ 
//     | |    / _ \| '_ \|  _| |/ _` | | '_ \
//     | |___| (_) | | | | | | | (_| |_| | | |
//      \_____\___/|_| |_|_| |_|\__, (_)_| |_|
//                               __/ |       
//                              |___/        
// ---------------------------------------------------------------------------*


// ---------------------------------------------------------------------------*

/*  WDT
#include "Sodaq_wdt.h"

#define DIAG_STREAM SerialUSB

void setup() 
{
  // Startup delay
  delay(5000);
  
  // Supported periods on both platforms
  // WDT_PERIOD_1DIV64 = 1/64s
  // WDT_PERIOD_1DIV32 = 1/32s
  // WDT_PERIOD_1DIV16 = 1/16s
  // WDT_PERIOD_1DIV8  = 1/8s
  // WDT_PERIOD_1DIV4  = 1/4s
  // WDT_PERIOD_1DIV2  = 1/2s
  // WDT_PERIOD_1X     = 1s
  // WDT_PERIOD_2X     = 2s
  // WDT_PERIOD_4X     = 4s
  // WDT_PERIOD_8X     = 8s
  // Default parameter is set to WDT_PERIOD_1X = 1s

  // Enable WDT
  sodaq_wdt_enable(WDT_PERIOD_8X);

  // Startup message (also seen after a reset)
  DIAG_STREAM.begin(57600);
  DIAG_STREAM.println("Start up");
}

void loop() 
{
  // If the WDT interrupt has been triggered
  if (sodaq_wdt_flag) {
    sodaq_wdt_flag = false;
    sodaq_wdt_reset();
    
    DIAG_STREAM.println("WDT interrupt has been triggered");
  }
}
*/


/* SLEEP
// C:\Users\h179132\Documents\Arduino\libraries\Sodaq_wdt\src
#include "Sodaq_wdt.h"

// Gardé source de : ARDUINO_ARCH_SAMD 

#define DIAG_STREAM SerialUSB
#define SLEEP_LED LED_BUILTIN
#define WDT_LED 4

// Length of LED flash
#define LED_FLASH_MS 100

void setup() 
{
  // Startup delay
  delay(5000);
  
  // Supported periods on both platforms
  // WDT_PERIOD_1DIV64 = 1/64s
  // WDT_PERIOD_1DIV32 = 1/32s
  // WDT_PERIOD_1DIV16 = 1/16s
  // WDT_PERIOD_1DIV8  = 1/8s
  // WDT_PERIOD_1DIV4  = 1/4s
  // WDT_PERIOD_1DIV2  = 1/2s
  // WDT_PERIOD_1X     = 1s
  // WDT_PERIOD_2X     = 2s
  // WDT_PERIOD_4X     = 4s
  // WDT_PERIOD_8X     = 8s
  // Default parameter is set to WDT_PERIOD_1X = 1s

  // Enable WDT
  sodaq_wdt_enable(WDT_PERIOD_1X);

  // Setup LED's IO pins
  pinMode(SLEEP_LED, OUTPUT);                          // P13 = LED_BUILTIN
  pinMode(WDT_LED, OUTPUT);                            // P04

  // Startup message (also seen after a reset)
  DIAG_STREAM.begin(57600);
  DIAG_STREAM.println("Start up");
}

void loop() 
{
  // If the WDT interrupt has been triggered
  if (sodaq_wdt_flag) {
    sodaq_wdt_flag = false;
    sodaq_wdt_reset();
    
    DIAG_STREAM.println("WDT interrupt has been triggered");

    // Flash LED
    digitalWrite(WDT_LED, HIGH);
    sodaq_wdt_safe_delay(LED_FLASH_MS);
    digitalWrite(WDT_LED, LOW);
  }

  // Try and enter sleep mode
  systemSleep();
}

void systemSleep()
{
  // Only go to sleep if there was no watchdog interrupt.
  if (!sodaq_wdt_flag)
  {
    // Power on LED before sleep
    digitalWrite(SLEEP_LED, HIGH);                //P13
    
    // Disable USB
    USBDevice.detach();

    // Set the sleep mode
    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
    
    // SAMD sleep
    __WFI();
    
    // Power off LED after sleep
    digitalWrite(SLEEP_LED, LOW);
    sodaq_wdt_safe_delay(LED_FLASH_MS);
  }
}
 */
