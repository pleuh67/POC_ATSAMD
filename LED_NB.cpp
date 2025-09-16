
#define __INIT_DONE
#include "define.h"

//#define __debugSerial_NonBlockingLED  


/**
 * @brief Initialise les LEDs RGB et builtin
 * @param Aucun
 * @return void
 */
void initLEDs(void) 
{
  pinMode(LED_RED, OUTPUT);         // vert
  pinMode(LED_GREEN, OUTPUT);       // Rouge
  pinMode(LED_BLUE, OUTPUT);        // bleue
  pinMode(LED_BUILTIN, OUTPUT);
    
  digitalWrite(LED_RED, LED_OFF);
  digitalWrite(LED_GREEN, LED_OFF);
  digitalWrite(LED_BLUE, LED_OFF);
  digitalWrite(LED_BUILTIN, LOW);

  blinkRedLED();  // vert
  delay(500);
  blinkGreenLED();  // Rouge
  delay(500);
  blinkBlueLED();  // bleue
debugSerial.println("LEDs initialisées");
}

/**
 * @brief Allume la LED rouge
 * @param Aucun
 * @return void
 */
void turnOnRedLED(void) 
{
  digitalWrite(LED_RED, LED_ON);
}

/**
 * @brief Éteint la LED rouge
 * @param Aucun
 * @return void
 */
void turnOffRedLED(void) 
{
  digitalWrite(LED_RED, LED_OFF);
}
/**
 * @brief Fait clignoter la LED Rouge pendant 300 ms
 * @param Aucun
 * @return void
 */
void blinkRedLED(void) 
{
  turnOnRedLED();
  delay(300);
  turnOffRedLED();
}

/**
 * @brief Allume la LED bleue
 * @param Aucun
 * @return void
 */
void turnOnBlueLED(void) 
{
  digitalWrite(LED_BLUE, LED_ON);
}
/**
 * @brief Éteint la LED bleue
 * @param Aucun
 * @return void
 */
void turnOffBlueLED(void) 
{
  digitalWrite(LED_BLUE, LED_OFF);
}

/**
 * @brief Fait clignoter la LED bleue pendant 300 ms
 * @param Aucun
 * @return void
 */
void blinkBlueLED(void) 
{
  turnOnBlueLED();
  delay(300);
  turnOffBlueLED();
}
/**
 * @brief Allume la LED verte
 * @param Aucun
 * @return void
 */
void turnOnGreenLED(void) 
{
  digitalWrite(LED_GREEN, LED_ON);
}

/**
 * @brief Éteint la LED verte
 * @param Aucun
 * @return void
 */
void turnOffGreenLED(void) 
{
  digitalWrite(LED_GREEN, LED_OFF);
}

/**
 * @brief Fait clignoter la LED verte pendant 300 ms
 * @param Aucun
 * @return void
 */
void blinkGreenLED(void) 
{
  turnOnGreenLED();
  delay(300);
  turnOffGreenLED();
}

/**
 * @brief Allume la LED builtin
 * @param Aucun
 * @return void
 */
void turnOnBuiltinLED(void) 
{
  digitalWrite(LED_BUILTIN, HIGH);
}

/**
 * @brief Éteint la LED builtin
 * @param Aucun
 * @return void
 */
void turnOffBuiltinLED(void) 
{
  digitalWrite(LED_BUILTIN, LOW);
}

/**
 * @brief Fait clignoter la LED builtin pendant BUILTIN_LED_DURATION ms
 * @param Aucun
 * @return void
 */
void blinkBuiltinLED(void) 
{
  turnOnBuiltinLED();
  delay(BUILTIN_LED_DURATION);
  turnOffBuiltinLED();
}


// ***    gestion LEDs Non Bloquant ***

/**
 * @brief Démarre le cycle LED rouge (non-bloquant)
 * @param Aucun
 * @return void
 */
void LEDStartRed(void)
{
    redLedActive = true;
    redLedStartTime = millis();
    turnOnRedLED();
#ifdef __debugSerial_NonBlockingLED    
    debugSerial.print("LED Rouge démarrée pour ");
    debugSerial.print(config.applicatif.redLedDuration);
    debugSerial.println("ms");
#endif
    
}

/**
 * @brief Démarre le cycle LED rouge (non-bloquant)
 * @param Aucun
 * @return void
 */
void LEDStartGreen(void)
{
    greenLedActive = true;
    greenLedStartTime = millis();
    turnOnGreenLED();
#ifdef __debugSerial_NonBlockingLED     
    debugSerial.print("LED Verte démarrée pour ");
    debugSerial.print(config.applicatif.redLedDuration);
    debugSerial.println("ms");
#endif    
}

/**
 * @brief Démarre le cycle LED rouge (non-bloquant)
 * @param Aucun
 * @return void
 */
void LEDStartBlue(void)
{
    blueLedActive = true;
    blueLedStartTime = millis();
    turnOnBlueLED();
#ifdef __debugSerial_NonBlockingLED     
    debugSerial.print("LED Bleue démarrée pour ");
    debugSerial.print(config.applicatif.redLedDuration);
    debugSerial.println("ms");
#endif    
}

/**
 * @brief Démarre le cycle LED builtin (non-bloquant)
 * @param Aucun
 * @return void
 */
void demarrerLEDBuiltin(void)
{
    builtinLedActive = true;
    builtinLedStartTime = millis();
    turnOnBuiltinLED();  
    
#ifdef __debugSerial_NonBlockingLED 
    // Debug moins verbeux
    static int builtinCount = 0;
    builtinCount++;
    if (builtinCount % 10 == 0)
    {
        debugSerial.print("LED Builtin #");
        debugSerial.println(builtinCount);
    }
#endif    
}


/**
 * @brief Gère les LEDs de manière non-bloquante
 * @param Aucun
 * @return void
 */
void gererLEDsNonBloquant(void)
{
    unsigned long currentTime = millis();
    
    // Gestion LED Rouge
    if (redLedActive)
    {
        if (currentTime - redLedStartTime >= config.applicatif.redLedDuration)
        {
            turnOffRedLED();
            redLedActive = false;      
#ifdef __debugSerial_NonBlockingLED                
debugSerial.println("LED Rouge éteinte (fin cycle)");
#endif
       }
    }

      if (greenLedActive)
    {
        if (currentTime - greenLedStartTime >= config.applicatif.greenLedDuration)
        {
            turnOffGreenLED();
            greenLedActive = false;
#ifdef __debugSerial_NonBlockingLED 
  debugSerial.println("LED verte éteinte (fin cycle)");            
#endif
        }
    }
    
     if (blueLedActive)
    {
        if (currentTime - blueLedStartTime >= config.applicatif.blueLedDuration)
        {
            turnOffBlueLED();
            blueLedActive = false;    
#ifdef __debugSerial_NonBlockingLED 
  debugSerial.println("LED bleue éteinte (fin cycle)");
#endif        
        }
    }
   // Gestion LED Builtin
    if (builtinLedActive)
    {
        if (currentTime - builtinLedStartTime >= config.applicatif.builtinLedDuration)
        {
            turnOffBuiltinLED();
            builtinLedActive = false;
        }
    }
}

    
