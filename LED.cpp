
#define __INIT_DONE
#include "define.h"





/**
 * @brief Initialise les LEDs RGB et builtin
 * @param Aucun
 * @return void
 */
void initLEDs(void) 
{
    pinMode(LED_RED, OUTPUT);
    pinMode(LED_GREEN, OUTPUT);
    pinMode(LED_BLUE, OUTPUT);
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
void turnONRedLED(void) 
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
 * @brief Fait clignoter la LED builtin pendant BUILTIN_LED_DURATION ms
 * @param Aucun
 * @return void
 */
void blinkRedLED(void) 
{
    turnONRedLED();
    delay(300);
    turnOffRedLED();
}

/**
 * @brief Allume la LED rouge
 * @param Aucun
 * @return void
 */
void turnOnBlueLED(void) 
{
    digitalWrite(LED_BLUE, LED_ON);
}

/**
 * @brief Éteint la LED rouge
 * @param Aucun
 * @return void
 */
void turnOffBlueLED(void) 
{
    digitalWrite(LED_BLUE, LED_OFF);
}

/**
 * @brief Fait clignoter la LED builtin pendant BUILTIN_LED_DURATION ms
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
 * @brief Allume la LED rouge
 * @param Aucun
 * @return void
 */
void turnOnGreenLED(void) 
{
    digitalWrite(LED_GREEN, LED_ON);
}

/**
 * @brief Éteint la LED rouge
 * @param Aucun
 * @return void
 */
void turnOffGreenLED(void) 
{
    digitalWrite(LED_GREEN, LED_OFF);
}

/**
 * @brief Fait clignoter la LED builtin pendant BUILTIN_LED_DURATION ms
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
