#define __INIT_DONE
#include "define.h"


/**
 * @brief Définit la configuration par défaut
 * @param Aucun
 * @return void
 */
void setDefaultConfig(void) 
{
    config.materiel.version = CONFIG_VERSION;
    config.materiel.adresseRTC = DS3231_ADDRESS;    // Adresse RTC Module DS3231
    config.materiel.adresseOLED = OLED_ADDRESS;     // Adresse écran OLED
    config.materiel.adresseEEPROM = EEPROM_ADDRESS; // Adresse EEPROM Module DS3231
   
    config.applicatif.redLedDuration = RED_LED_DURATION;
    config.applicatif.greenLedDuration = GREEN_LED_DURATION;
    config.applicatif.blueLedDuration = BLUE_LED_DURATION;
    config.applicatif.builtinLedDuration = BUILTIN_LED_DURATION;
    config.applicatif.SendingPeriod = WAKEUP_INTERVAL_PAYLOAD;    // mieux: wakeupIntervalPayload
    config.applicatif.OLEDRefreshPeriod = INTERVAL_1SEC;               //
    
    config.checksum = calculateChecksum(&config);
}


/**
 * @brief Initialise la configuration par défaut
 * @param Aucun
 * @return void
 */
void initConfig(void) 
{
    loadConfigFromEEPROM();
    debugSerial.println("Configuration chargée");
}

/**
 * @brief Charge la configuration depuis l'EEPROM
 * @param Aucun
 * @return void
 */
void loadConfigFromEEPROM(void) 
{
    // Simulation de lecture EEPROM - à implémenter avec bibliothèque I2C EEPROM
    setDefaultConfig();
}

/**
 * @brief Sauvegarde la configuration dans l'EEPROM
 * @param Aucun
 * @return void
 */
void saveConfigToEEPROM(void) 
{
    config.checksum = calculateChecksum(&config);
    // Simulation de sauvegarde EEPROM - à implémenter avec bibliothèque I2C EEPROM
    debugSerial.println("Configuration sauvegardée");
}

/**
 * @brief Calcule le checksum de la configuration
 * @param cfg Pointeur vers la configuration
 * @return uint16_t Checksum calculé
 */
uint16_t calculateChecksum(ConfigGenerale_t* cfg) 
{
    uint16_t sum = 0;
    uint8_t* data = (uint8_t*)cfg;
    for (int i = 0; i < sizeof(ConfigGenerale_t) - sizeof(uint16_t); i++) 
    {
        sum += data[i];
    }
    return sum;
}
