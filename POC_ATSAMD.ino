
//Virer 3 x colonne?0:0 valider sur 0.96"
// https://claude.ai/chat/75e1ba53-8b90-4f09-97a5-cfc366e36a8e
// 16:40:59.416 -> Alarme 2 (payload) programmée pour: 16:39:16  afficher a chaque reprogrammation


/**
 * @file POC_IRQ_LOW_POWER.ino
 * @brief Programme de démonstration pour SODAQ EXPLORER avec gestion basse consommation
 * @author Votre nom
 * @version 1.1.1-PL
 * @date 2025
 * 
 * Description : Gestion de deux modes de fonctionnement avec interruptions RTC
 * - MODE EXPLOITATION : réveil périodique pour LED rouge
 * - MODE PROGRAMMATION : réveil périodique + clignotement LED builtin + interface utilisateur
 */

#define __MAIN__

// ===== INCLUDES PROJET =====
#include ".\define.h"


// =====  PROGRAMME =====
// ===== SETUP =====
void setup() 
{
    
    // Initialisation des LEDs
    initLEDs();
    
    // Initialisation du mode selon PIN_PE
    pinMode(PIN_PE, INPUT_PULLUP);
    modeExploitation = digitalRead(PIN_PE);
    
    debugSerial.print("Mode détecté: ");
    debugSerial.println(modeExploitation ? "EXPLOITATION" : "PROGRAMMATION");
    
    // Initialisation I2C
    Wire.begin();
    
    // Initialisation OLED
    if (OLED) 
    {
        initOLED();
        debugDisplayOLED("initOLED OK"); // scrolling lors du setup(); 
    }

  init2483A();
  initLoRa();

    // Initialisation RTC
    initRTC();
    debugDisplayOLED("initRTC OK");

    // Initialisation configuration
    initConfig();
    debugDisplayOLED("initConfig OK");
        
    // Configuration des alarmes RTC
    setRTCAlarms();
    debugDisplayOLED("setRTCAlarms OK");
    
    // Configuration interruption externe
    pinMode(RTC_INTERRUPT_PIN, INPUT_PULLUP);

// AJOUTÉ: Debug configuration
debugSerial.print("Configuration interruption sur pin ");
debugSerial.println(RTC_INTERRUPT_PIN);

// AJOUTÉ: Test état initial
debugSerial.print("État initial pin RTC: ");
debugSerial.println(digitalRead(RTC_INTERRUPT_PIN) ? "HIGH" : "LOW");

// AJOUTÉ: Configuration DS3231 AVANT interruption
rtc.writeSqwPinMode(DS3231_OFF);
rtc.clearAlarm(1);
rtc.clearAlarm(2);
    
    LowPower.attachInterruptWakeup(RTC_INTERRUPT_PIN, onRTCAlarm, FALLING);
    
 // AJOUTÉ: Tests supplémentaires
debugSerial.println("Interruption RTC attachée");
delay(1000);
debugSerial.print("État pin après config: ");
debugSerial.println(digitalRead(RTC_INTERRUPT_PIN) ? "HIGH" : "LOW");



debugSerial.println("Initialisation terminee");

    void forcerSynchronisationDS3231();

 debugSerial.println("Mise à l'heure");
    
    if (OLED) 
    {
        debugDisplayOLED("Init terminee");
        delay(2000);
        clearOLED();
    }

//  vider cache : debugDisplayOLED()  
 debugDisplayOLEDReset();
}

// ===== LOOP PRINCIPAL =====
void loop() 
{ 
debugSerial.print("b");
    // *** TRAITEMENT CLAVIER NON-BLOQUANT ***
    processContinuousKeyboard();
    // *** UTILISATION DES TOUCHES ***
    key_code_t touche = readKeyNonBlocking();
    if (touche != KEY_NONE)
    {
        // Traiter la touche
        debugSerial.print("Touche pressée: ");
        debugSerial.println(keyToString(touche));
    }
    
    
    // Vérification du mode
    modeExploitation = digitalRead(PIN_PE);
    if (modeExploitation)         // OK, validé, GreenLED => couleur Red
    {
        blinkGreenLED();          // PCB=> RedLED
        handleOperationMode();
    } 
    else                          // OK, validé, BlueLED
    {
        blinkBlueLED();
        handleProgrammingMode();  // fait : drawScreenTime(0, 0);
    }
    
    // Gestion des interruptions
    if (wakeupPayload) 
    {
        wakeupPayload = false;
        debugSerial.println("PROG/Réveil payload");
        
        if (OLED) 
        {
            debugDisplayOLED("PROG/Reveil payload");
        }
        
        turnONRedLED();
        delay(RED_LED_DURATION);
        turnOffRedLED();
    }
    if (wakeup1Sec && !modeExploitation) 
    {
        wakeup1Sec = false;
        debugSerial.println("PROG/Réveil 1 sec");
/*
// Debug moins verbeux
      static int debugCounter = 0;
      debugCounter++;
      if (debugCounter % 10 == 0) // ← AFFICHER SEULEMENT TOUTES LES 10 SECONDES
      {
          debugSerial.print("Réveil 1 sec - Alarme 1 (#");
          debugSerial.print(debugCounter);
          debugSerial.println(")");
      }
*/      
        blinkBuiltinLED();
        if (OLED) 
        {
            drawScreenTime(0, 0); // refresh Time/Date every second
        }
    }
    
    // Entrée en veille si activée
    if (DEBUG_LOW_POWER) 
    {
      debugSerial.flush();  
  //    sleep();
    }
    delay(100); // Sécurité pour éviter la surcharge
}

// ===== IMPLÉMENTATION DES FONCTIONS =====

/**
 * @brief Initialise le module RTC DS3231
 * @param Aucun
 * @return void
 */
void initRTC(void) 
{
    if (!rtc.begin()) 
    {
        debugSerial.println("Erreur: RTC introuvable");
        while (1) delay(10);
    }
    
    if (rtc.lostPower()) 
    {
        debugSerial.println("RTC a perdu l'heure, mise à jour avec l'heure de compilation");
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }
    
    debugSerial.println("RTC initialisé");
}

/**
 * @brief Retourne l'heure système actuelle
 * @param Aucun
 * @return DateTime Heure actuelle
 */
DateTime getSystemTime(void) 
{
    return rtc.now();
}

 /**
 * @brief Configure les alarmes du RTC - ALARMES ÉCHANGÉES
 * Alarme 1 : Toutes les secondes (mode programmation)
 * Alarme 2 : Payload toutes les X minutes
 * @param Aucun
 * @return void
 */
void setRTCAlarms(void) 
{
/*
Fonctions modifiées :

setRTCAlarms() : Configuration inversée

Alarme 1 → DS3231_A1_PerSecond pour chaque seconde
Alarme 2 → DS3231_A2_Minute pour payload
 */

  
    clearRTCAlarms();

     
     debugSerial.print(DEBUG_INTERVAL_1SEC);
     debugSerial.println("=== CONFIGURATION ALARMES RTC + INTERRUPTIONS ===");
     // ALARME 1 : Toutes les secondes en mode programmation
     // AJOUTÉ: Commentaires critiques sur interruptions
    // CRITIQUE: Activer les interruptions dans le registre de contrôle du DS3231
    // Sans cela, le pin INT/SQW ne changera pas d'état
   if (DEBUG_INTERVAL_1SEC && !modeExploitation) 
    {
        DateTime nextSecond = rtc.now() + TimeSpan(0, 0, 0, 10);
        rtc.setAlarm1(nextSecond, DS3231_A1_PerSecond);
        debugSerial.print("Alarme 1 (1 sec) programmée pour: ");
        debugSerial.print(nextSecond.hour()); debugSerial.print(":");
        debugSerial.print(nextSecond.minute()); debugSerial.print(":");
        debugSerial.println(nextSecond.second());
        debugSerial.print("Alarme 1 (CHAQUE seconde) programmée...");       
        debugSerial.println("Interruption Alarme 1 activée");
      }
    
  // ALARME 2 : Payload toutes les X minutes
    if (DEBUG_WAKEUP_PAYLOAD) 
    {
        DateTime nextPayload = rtc.now() + TimeSpan(0, 0, WAKEUP_INTERVAL_PAYLOAD, 0);
        rtc.setAlarm2(nextPayload, DS3231_A2_Minute);
                // AJOUTÉ: Debug et confirmation
        debugSerial.println("Interruption Alarme 2 activée");
debugSerial.print("Alarme 2 (payload) programmée pour: ");
        debugSerial.print(nextPayload.hour()); debugSerial.print(":");
        debugSerial.print(nextPayload.minute()); debugSerial.print(":");
        debugSerial.println(nextPayload.second());
    }

    
    // AJOUTÉ: Test état pin après configuration
    debugSerial.print("État pin RTC après config alarmes: ");
    debugSerial.println(digitalRead(RTC_INTERRUPT_PIN) ? "HIGH" : "LOW");
    
    debugSerial.println("=== FIN CONFIGURATION ALARMES + INTERRUPTIONS ===");

    
   // Activer les interruptions
    rtc.writeSqwPinMode(DS3231_OFF); 
}

/**
 * @brief Efface les alarmes du RTC
 * @param Aucun
 * @return void
 */
void clearRTCAlarms(void) 
{
    rtc.clearAlarm(1);
    rtc.clearAlarm(2);
}



/**
 * @brief Lecture instantanée d'une touche du clavier analogique
 * @param Aucun
 * @return key_code_t Code de la touche ou état d'erreur
 */
key_code_t readKeyOnce(void) 
{
    static const int levels[NB_KEYS] = {10, 149, 332, 501, 735};
    static const key_code_t keycodes[NB_KEYS] = {KEY_1, KEY_2, KEY_3, KEY_4, KEY_5};
    
    int val = analogRead(KBD_ANA);
    
    if (val > 1000) return KEY_NONE;
  //  if (val < 5) return KEY_INVALID;
    
    for (int i = 0; i < NB_KEYS; i++) 
    {
        if (val >= levels[i] - TOL && val <= levels[i] + TOL) 
        {
            return keycodes[i];
        }
    }
    
    return KEY_INVALID;
}

/**
 * @brief Lecture avec anti-rebond du clavier analogique
 * @param Aucun
 * @return key_code_t Code de touche stable après confirmation
 */
key_code_t readKey(void) 
{
    key_code_t key, key2;
    int stable_count = 0;
    
    key = readKeyOnce();
    
    do 
    {
        delay(DEBOUNCE_DELAY_MS);
        key2 = readKeyOnce();
        
        if (key == key2) 
        {
            stable_count++;
        } 
        else 
        {
            stable_count = 0;
            key = key2;
        }
    } while (stable_count < DEBOUNCE_COUNT);
    
    return key;
}

/**
 * @brief Convertit un code de touche en chaîne de caractères
 * @param key Code de la touche
 * @return const char* Nom de la touche
 */
const char* keyToString(key_code_t key) 
{
    switch (key) 
    {
        case KEY_NONE: return "NONE";
        case KEY_1: return "KEY_1";
        case KEY_2: return "KEY_2";
        case KEY_3: return "KEY_3";
        case KEY_4: return "KEY_4";
        case KEY_5: return "KEY_5";
        case KEY_INVALID: return "INVALID";
        default: return "UNKNOWN";
    }
}

/**
 * @brief Fonction de test du clavier analogique
 * @param Aucun
 * @return void
 */
void testKbd(void) 
{
    key_code_t key = readKey();
    
    switch (key) 
    {
        case KEY_NONE:
            break;
            
        case KEY_1:
            debugSerial.println("Touche 1 pressée");
            break;
            
        case KEY_2:
            debugSerial.println("Touche 2 pressée");
            break;
            
        case KEY_3:
            debugSerial.println("Touche 3 pressée");
            break;
            
        case KEY_4:
            debugSerial.println("Touche 4 pressée");
            break;
            
        case KEY_5:
            debugSerial.println("Touche 5 pressée");
            break;
            
        case KEY_INVALID:
            debugSerial.print("Erreur de lecture clavier - valeur: ");
            debugSerial.println(analogRead(KBD_ANA));
            break;
            
        default:
            debugSerial.println("Code de touche inconnu");
            break;
    }
    delay(100);
}

/**
 * @brief Version non-bloquante de readKey() - Retourne immédiatement
 * @param Aucun
 * @return key_code_t Touche disponible ou KEY_NONE si aucune touche
 * @description Récupère une touche stabilisée sans attendre
 */
key_code_t readKeyNonBlocking(void)
{
    if (clavierContext.toucheDisponible)
    {
        clavierContext.toucheDisponible = false; // Consommer la touche
        return clavierContext.toucheStable;
    }
    
    return KEY_NONE; // Pas de touche disponible
}

/**
 * @brief Traite le clavier en continu (à appeler dans loop)
 * @param Aucun
 * @return void
 * @description Gère l'anti-rebond et la détection des touches de manière non-bloquante
 */
void processContinuousKeyboard(void)
{
    unsigned long currentTime = millis();
    
    // Vérifier l'intervalle minimum entre lectures
    if (currentTime - clavierContext.derniereLecture < DEBOUNCE_DELAY_MS)
    {
        return; // Trop tôt pour une nouvelle lecture
    }
    
    clavierContext.derniereLecture = currentTime;
    
    // Lire la touche actuelle
    key_code_t toucheActuelle = readKeyOnce();
    
    // Vérifier la stabilité
    if (toucheActuelle == clavierContext.derniereTouche)
    {
        clavierContext.compteStable++;
        
        // Si suffisamment stable et différente de la précédente
        if (clavierContext.compteStable >= DEBOUNCE_COUNT)
        {
            if (toucheActuelle != clavierContext.toucheStable)
            {
                clavierContext.toucheStable = toucheActuelle;
                
                // Nouvelle touche disponible (pas KEY_NONE)
                if (toucheActuelle != KEY_NONE)
                {
                    clavierContext.toucheDisponible = true;
                    
                    // Debug optionnel
                    static int toucheCount = 0;
                    toucheCount++;
                    if (toucheCount % 5 == 0)
                    {
                        debugSerial.print("Touche détectée #");
                        debugSerial.print(toucheCount);
                        debugSerial.print(": ");
                        debugSerial.println(keyToString(toucheActuelle));
                    }
                }
            }
        }
    }
    else
    {
        // Touche différente, recommencer le comptage
        clavierContext.derniereTouche = toucheActuelle;
        clavierContext.compteStable = 0;
    }
}

/**
 * @brief Initialise l'écran OLED selon le type sélectionné
 * @param Aucun
 * @return void
 */
void initOLED(void) 
{
#ifdef OLED096
    initOLED096();
#endif
#ifdef OLED130
    initOLED130();
#endif
#ifdef OLED154
    initOLED154();
#endif
    debugSerial.print("LOGO ");
    delay(500);
    display.clearDisplay();
    debugSerial.println(" done");
}

/**
 * @brief Initialise l'écran OLED 0.96"
 * @param Aucun
 * @return void
 */
void initOLED096(void) 
{
    if (OLED) 
    {
        debugSerial.println("INIT OLED096");
#define SSD1306_SWITCHCAPVCC 0x02
        display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS);

        display.setTextSize(1);
        display.setTextColor(WHITE);
        
        display.display();
    }
}

/**
 * @brief Initialise l'écran OLED 1.30"
 * @param Aucun
 * @return void
 */
void initOLED130(void) 
{
    if (OLED) 
    {
        debugSerial.println("INIT OLED130");
        display.begin(OLED_ADDRESS, true);

        display.setTextSize(1);
        display.setTextColor(WHITE);
       
        display.display();
    }
}

/**
 * @brief Initialise l'écran OLED 1.54"
 * @param Aucun
 * @return void
 */
void initOLED154(void) 
{
    if (OLED) 
    {
        debugSerial.println("INIT OLED154");
        display.begin(OLED_ADDRESS, true);

        display.setTextSize(1);
        display.setTextColor(WHITE);
        
        display.display();
    }
}

/**
 * @brief Efface tout le contenu visuel de l'écran OLED
 * @param Aucun
 * @return void
 */
void clearOLED(void) 
{
  display.clearDisplay();
  display.display();
//  debugDisplayOLED("clearOLED()");
}


/**
 * @brief Efface du texte à une position donnée
 * @param col Colonne de départ
 * @param lig Ligne de départ
 * @param Ncar Nombre de caractères à effacer
 * @return void
 */
void erasetext(int16_t colonne, int16_t lig, int16_t Ncar)
{
    char TL_ColPixel = (colonne*OLED_Col)-(colonne?0:0); // 1:0
    char TL_LigPixel = lig * OLED_L1;
    
    if (OLED)
    {
      display.fillRect(TL_ColPixel, TL_LigPixel, OLED_Col * Ncar, OLED_L1, 0);
      display.display(); // Ajoutez ceci pour forcer l'affichage
    }
}


/**
 * @brief Affiche l'heure et la date sur l'écran OLED
 * @param col Colonne de départ
 * @param lig Ligne de départ
 * @return void
 */
void drawScreenTime(uint8_t ligne, uint8_t colonne)
{ static DateTime oldSystemTime(1692712245);  //1692712245);  // timestamp Unix

    if (OLED) 
    {  
        DateTime systemTime = getSystemTime();
        sprintf(OLEDbuf, "%02d:%02d:%02d - %02d/%02d/%d", 
                systemTime.hour(), systemTime.minute(), systemTime.second(),
                systemTime.day(), systemTime.month(), systemTime.year());
              
         if (systemTime != oldSystemTime)
         { 
            drawtext(1, ligne, colonne, OLEDbuf);
            oldSystemTime = systemTime;
         }
    }
}


/**
 * @brief Affiche l'heure et la date sur l'écran OLED
 * @param col Colonne de départ
 * @param lig Ligne de départ
 * @return void
 */
void drawScreenRefreshTime(uint8_t ligne, uint8_t colonne)
{ static DateTime oldSystemTime(1692712245);  //1692712245);  // timestamp Unix

    if (OLED) 
    {  
        DateTime systemTime = getSystemTime();
        sprintf(OLEDbuf, "%02d:%02d:%02d - %02d/%02d/%d", 
                systemTime.hour(), systemTime.minute(), systemTime.second(),
                systemTime.day(), systemTime.month(), systemTime.year());

// Check changes and refresh with:
// printVarOLED(uint8_t ligne, uint8_t col, const void *valeur, char type)
/* @param ligne Ligne sur l'écran OLED où afficher la variable
 * @param col Colonne sur l'écran OLED où commencer l'affichage
 * @param valeur Pointeur vers la valeur à afficher
 * @param type Type de la variable ('c', 's', 'i', 'I', 'f', 'F')
*/

/*
Police par défaut (built-in font)
Taille fixe : 6x8 pixels par caractère (incluant 1 pixel d'espacement)
Monospace : chaque caractère occupe exactement la même largeur
Positionnement prévisible : le curseur avance de 6 pixels horizontalement pour chaque caractère
*/

int val;
               if (oldSystemTime.hour() != systemTime.hour())
               { val = systemTime.hour();
                 printVarOLED(ligne, 0, &val, 'i');
               }
               if (oldSystemTime.minute() != systemTime.minute())
               { val = systemTime.minute();
                 printVarOLED(ligne, 3, &val, 'i');
               }
               if (oldSystemTime.second() != systemTime.second())
               { val = systemTime.second();
                 printVarOLED(ligne, 6, &val, 'i');
               }       
               if (oldSystemTime.day() != systemTime.day())
               { val = systemTime.day();
   //              printVarOLED(ligne, 11, &val, 'i');
               }
               if (oldSystemTime.month() != systemTime.month())
               { val = systemTime.month();
        //         printVarOLED(ligne, 14, &val, 'i');
               }               
               if (oldSystemTime.year() != systemTime.year())
               { val = systemTime.year();
     //            printVarOLED(ligne, 17, &val, 'i');
               }               
               
         if (systemTime != oldSystemTime)
         { 
  //          drawtext(1, ligne, colonne, OLEDbuf);
            oldSystemTime = systemTime;
         }
    }
}


/**
 * @brief Ajoute un message avec scrolling si besoin
 * @param message Message à afficher
 * @return void
 */
void debugDisplayOLED(char* message) 
{
    if (!modeDebugActif) return;
    
    for (int i = 0; i < MAX_LIGNES - 1; i++) 
    {
        strcpy(lignesDebug[i], lignesDebug[i+1]);
    }
    strcpy(lignesDebug[MAX_LIGNES - 1], message);
    
    display.clearDisplay();
    for (uint8_t i = 1; i < MAX_LIGNES; i++) // uint8_t i = 0 => reserve L0 pour Time/Date
    {
        display.setCursor(0, i * TAILLE_LIGNE);
        drawtext(1,i, 0, lignesDebug[i]);
    }
    display.display();
    delay(delaiAffichage);
}


/**
 * @brief efface les messages
 * @param message Message à afficher
 * @return void
 */
void debugDisplayOLEDReset(void) 
{ char message[21];
   
    if (!modeDebugActif) return;
    
    for (int i = 1; i < MAX_LIGNES; i++) // int i = 0, on garde time/date dispo
    {
 
 //           sprintf(message, " --- %02d --- ",i);     
        strcpy(lignesDebug[i], "                    ");
    }
    
    display.clearDisplay();
    for (uint8_t i = 0; i < MAX_LIGNES; i++) 
    {
        display.setCursor(0, i * TAILLE_LIGNE);
        drawtext(1,i, 0, lignesDebug[i]);
    }
    display.display();
    delay(delaiAffichage);
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

/**
 * @brief Définit la configuration par défaut
 * @param Aucun
 * @return void
 */
void setDefaultConfig(void) 
{
    config.materiel.version = CONFIG_VERSION;
    config.materiel.adresseRTC = 0x68;
    config.materiel.adresseOLED = 0x3C;
    config.materiel.adresseEEPROM = 0x57;
    
    config.applicatif.redLedDuration = RED_LED_DURATION;
    config.applicatif.builtinLedDuration = BUILTIN_LED_DURATION;
    config.applicatif.wakeupIntervalPayload = WAKEUP_INTERVAL_PAYLOAD;
    config.applicatif.interval1Sec = INTERVAL_1SEC;
    
    config.checksum = calculateChecksum(&config);
}

/**
 * @brief Gestion du mode exploitation
 * @param Aucun
 * @return void
 */
void handleOperationMode(void) 
{ // Mode exploitation : seulement réveil payload
  if (OLED) 
  {
    if (switchToOperationMode)    // affiche qu'une fois
    { 
      switchToProgrammingMode = true;
      clearOLED(); 
      drawtext(1, 7, 0, "MODE EXPLOITATION");
      switchToOperationMode = false;
    }  
 //   drawScreenRefreshTime(0, 0);
  }

//  sendPayload();
  Send_LoRa_Mess((uint8_t*)testPayload,7);
  
}

/**
 * @brief Gestion du mode programmation
 * @param Aucun
 * @return void
 */
void handleProgrammingMode(void) 
{    // Mode programmation : réveil payload + 1 sec + interface utilisateur
  if (OLED) 
  {
    if (switchToProgrammingMode)    // affiche qu'une fois
    {  
      switchToOperationMode = true;
      clearOLED();
      drawScreenTime(0,0);
      drawtext(1, 7, 0, "MODE PROGRAMMATION");
      switchToProgrammingMode = false;
    }  
    drawScreenRefreshTime(0, 0);
  }
}

/**
 * @brief Fonction d'interruption RTC
 * @param Aucun
 * @return void
 */
void onRTCAlarm(void) 
{ static int counter1=0,counter2=0;
  char buffer[21];

 //   debugDisplayOLED("onRTCAlarm()");

    // ALARME 1 : Toutes les secondes (mode programmation)
    if (rtc.alarmFired(1)) 
    {
        wakeup1Sec = true;
        rtc.clearAlarm(1);
snprintf(buffer, sizeof(buffer), "IRQ 1s %d", counter1++);
debugDisplayOLED(buffer);   
debugSerial.println(buffer);        
/* en mode  DS3231_A1_PerSecond, ne pas reprogrammer l'alarme
// L'alarme DS3231_A1_PerSecond se répète automatiquement
// Seulement reprogrammer si le mode change
// reprogramer si DS3231_A1_Second, faire :  
// Reprogrammer l'alarme 1 seconde si en mode programmation
// ici toutes les minutes quand la seconded programmée survient
        if (DEBUG_INTERVAL_1SEC && !modeExploitation) 
        {
            DateTime nextSecond = rtc.now() + TimeSpan(0, 0, 0, 1);
            rtc.setAlarm1(nextSecond, DS3231_A1_PerSecond);
           
        }
*/ 
//       setRTCAlarms(); // Reprogrammer l'alarme
    }
    // ALARME 2 : Payload périodique    
    if (rtc.alarmFired(2)) 
    {
        wakeupPayload = true;
        rtc.clearAlarm(2);
snprintf(buffer, sizeof(buffer), "IRQ Payload %d", counter2++);
debugDisplayOLED(buffer);  
debugSerial.println(buffer);  
        // Reprogrammer l'alarme payload
        if (DEBUG_WAKEUP_PAYLOAD) 
        {
            DateTime nextPayload = rtc.now() + TimeSpan(0, 0, config.applicatif.wakeupIntervalPayload, 0);
            rtc.setAlarm2(nextPayload, DS3231_A2_Minute);
        }
/*
        if (!modeExploitation) 
        {
            setRTCAlarms(); // Reprogrammer l'alarme
        }
*/        
    }
}


void non_forceTestAlarm(void)
{
    debugSerial.println("=== TEST FORCÉ ALARME (10 SECONDES) ===");
    
    // Alarme dans 10 secondes
    DateTime testTime = rtc.now() + TimeSpan(0, 0, 0, 10);
    rtc.clearAlarm(1);
    rtc.setAlarm1(testTime, DS3231_A1_Minute);
    
    debugSerial.print("Alarme test programmée pour: ");
    debugSerial.print(testTime.hour()); debugSerial.print(":");
    debugSerial.print(testTime.minute()); debugSerial.print(":");
    debugSerial.println(testTime.second());
    
    debugSerial.println("Surveillez le pin et les messages...");
}

void testConnexionDS3231(void)
{
    debugSerial.println("=== TEST CONNEXION DS3231 ===");
    
    // Test lecture heure
    DateTime now = rtc.now();
    debugSerial.print("Heure lue: ");
    debugSerial.print(now.hour()); debugSerial.print(":");
    debugSerial.print(now.minute()); debugSerial.print(":");
    debugSerial.println(now.second());
    
    // Test état alarmes
    debugSerial.print("Alarme 1 fired: ");
    debugSerial.println(rtc.alarmFired(1) ? "OUI" : "NON");
    debugSerial.print("Alarme 2 fired: ");
    debugSerial.println(rtc.alarmFired(2) ? "OUI" : "NON");
    
    // Test pin interruption
    debugSerial.print("Pin interruption état: ");
    debugSerial.println(digitalRead(RTC_INTERRUPT_PIN) ? "HIGH" : "LOW");
    
    debugSerial.println("=== FIN TEST DS3231 ===");
}


/**
 * @brief Met le microcontrôleur en veille
 * @param Aucun
 * @return void
 */
void sleep(void) 
{     static int sleepCount = 0;
    
    
    sleepCount++;
  debugDisplayOLED("LowPower.sleep()");
    // AJOUTÉ: Debug entrée en veille
    debugSerial.print("=== ENTRÉE EN VEILLE #");
    debugSerial.print(sleepCount);
    debugSerial.println(" ===");
    
    // AJOUTÉ: État pin avant veille
    debugSerial.print("État pin RTC avant veille: ");
    debugSerial.println(digitalRead(RTC_INTERRUPT_PIN) ? "HIGH" : "LOW");
    
    // AJOUTÉ: État des alarmes
    debugSerial.print("Alarmes actives: ");
    debugSerial.print("A1="); debugSerial.print((DEBUG_INTERVAL_1SEC && !modeExploitation) ? "ON" : "OFF");
    debugSerial.print(" A2="); debugSerial.println(DEBUG_WAKEUP_PAYLOAD ? "ON" : "OFF");
    
    // AJOUTÉ: Forcer envoi série avant veille
    debugSerial.flush();
    delay(10);
    
    LowPower.sleep();  // ← VEILLE
    
    // AJOUTÉ: Debug réveil
    debugSerial.println("=== RÉVEIL DÉTECTÉ ===");
    
    // AJOUTÉ: État pin après réveil
    debugSerial.print("État pin RTC après réveil: ");
    debugSerial.println(digitalRead(RTC_INTERRUPT_PIN) ? "HIGH" : "LOW");
    
    // AJOUTÉ: Stabilisation
    delay(10);
}


/**
 * @brief Affiche un nombre avec 2 chiffres minimum
 * @param number Nombre à afficher
 * @return void
 */
void print2digits(int number) 
{
  if (number >= 0 && number < 10) 
  {
      debugSerial.write('0');
  }
  debugSerial.print(number);
}

/**
 * @brief Synchronise l'heure du microcontrôleur avec celle du DS3231
 * @param Aucun
 * @return void
 */
void synchronizeDS3231TimeToMicro(void)
{
    debugSerial.println("=== SYNCHRONISATION DS3231 -> MICRO ===");
    
    // Lecture de l'heure du DS3231
    DateTime heureDS3231 = rtc.now();
    
    // Affichage de l'heure lue
    debugSerial.print("Heure DS3231: ");
    debugSerial.print(heureDS3231.day()); debugSerial.print("/");
    debugSerial.print(heureDS3231.month()); debugSerial.print("/");
    debugSerial.print(heureDS3231.year()); debugSerial.print(" ");
    debugSerial.print(heureDS3231.hour()); debugSerial.print(":");
    debugSerial.print(heureDS3231.minute()); debugSerial.print(":");
    debugSerial.println(heureDS3231.second());
    
    // Note: Sur ATSAMD21, il n'y a pas de RTC interne séparée
    // L'heure système est déjà synchronisée avec le DS3231
    // Cette fonction sert principalement pour le debug et la vérification
    
    debugSerial.println("Synchronisation terminée");
    
    if (OLED)
    {
        displayMessageL8("Heure synchronisee", false, false);
    }
}

/**
 * @brief Copie l'heure du DS3231 vers le microcontrôleur avec option de forçage
 * @param forcer True pour forcer la copie même si les heures sont proches
 * @return void
 */
void copyDS3231TimeToMicro(bool forcer)
{
    static unsigned long derniereCopie = 0;
    const unsigned long INTERVALLE_COPIE_MS = 60000; // 1 minute
    
    // Vérifier s'il faut faire la copie
    if (!forcer && (millis() - derniereCopie < INTERVALLE_COPIE_MS))
    {
        return; // Pas encore temps de recopier
    }
    
    debugSerial.println("=== COPIE DS3231 -> MICRO ===");
    
    // Lire l'heure actuelle du DS3231
    DateTime heureDS3231 = rtc.now();
    
    // Obtenir l'heure système actuelle pour comparaison
    DateTime heureSysteme = getSystemTime();
    
    // Calculer la différence
    long difference = heureDS3231.unixtime() - heureSysteme.unixtime();
    
    debugSerial.print("Différence DS3231-Système: ");
    debugSerial.print(difference);
    debugSerial.println(" secondes");
    
    // Si la différence est significative ou si forcé, synchroniser
    if (forcer || abs(difference) > 2) // Plus de 2 secondes de différence
    {
        debugSerial.println("Synchronisation nécessaire...");
        
        // Affichage avant synchronisation
        debugSerial.print("Avant - DS3231: ");
        debugSerial.print(heureDS3231.hour()); debugSerial.print(":");
        debugSerial.print(heureDS3231.minute()); debugSerial.print(":");
        debugSerial.print(heureDS3231.second());
        debugSerial.print(" | Système: ");
        debugSerial.print(heureSysteme.hour()); debugSerial.print(":");
        debugSerial.print(heureSysteme.minute()); debugSerial.print(":");
        debugSerial.println(heureSysteme.second());
        
        // Sur ATSAMD21, le "système" utilise déjà le DS3231
        // Cette fonction simule la copie pour des microcontrôleurs avec RTC interne
        
        // Pour un vrai microcontrôleur avec RTC interne, vous ajouteriez ici :
        // setClock(heureDS3231.year(), heureDS3231.month(), heureDS3231.day(),
        //          heureDS3231.hour(), heureDS3231.minute(), heureDS3231.second());
        
        debugSerial.println("Heure copiée du DS3231 vers micro");
        
        if (OLED)
        {
            char message[21];
            sprintf(message, "Sync: %02d:%02d:%02d", 
                    heureDS3231.hour(), heureDS3231.minute(), heureDS3231.second());
            displayMessageL8(message, false, false);
        }
        
        derniereCopie = millis();
    }
    else
    {
        debugSerial.println("Synchronisation non nécessaire (différence < 2s)");
    }
}

/**
 * @brief Fonction utilitaire pour forcer une synchronisation immédiate
 * @param Aucun
 * @return void
 */
void forcerSynchronisationDS3231(void)
{
    debugSerial.println("=== SYNCHRONISATION FORCÉE ===");
    copyDS3231TimeToMicro(true);
    
    // Vérification après synchronisation
    delay(100);
    printTimeComparison();
}


/**
 * @brief Affiche l'heure système et l'heure RTC côte à côte pour comparaison
 * @param Aucun
 * @return void
 */
void printTimeComparison(void)
{
    DateTime systemTime = getSystemTime();
    DateTime rtcTime = rtc.now();
    debugSerial.println();
    debugSerial.println("=== COMPARAISON DES HORLOGES ===");
    
    debugSerial.print("Système: ");
    debugSerial.print(systemTime.day()); debugSerial.print("/");
    debugSerial.print(systemTime.month()); debugSerial.print("/");
    debugSerial.print(systemTime.year()); debugSerial.print(" ");
    debugSerial.print(systemTime.hour()); debugSerial.print(":");
    debugSerial.print(systemTime.minute()); debugSerial.print(":");
    debugSerial.println(systemTime.second());
    
    debugSerial.print("RTC:     ");
    debugSerial.print(rtcTime.day()); debugSerial.print("/");
    debugSerial.print(rtcTime.month()); debugSerial.print("/");
    debugSerial.print(rtcTime.year()); debugSerial.print(" ");
    debugSerial.print(rtcTime.hour()); debugSerial.print(":");
    debugSerial.print(rtcTime.minute()); debugSerial.print(":");
    debugSerial.println(rtcTime.second());
    
    long diff = systemTime.unixtime() - rtcTime.unixtime();
    debugSerial.print("Différence: ");
    debugSerial.print(diff);
    debugSerial.println(" secondes");
    debugSerial.println("===============================");
}


/**
 * @brief Affiche l'heure et la date système sur le port série
 * @param Aucun
 * @return void
 */
void printTimeOndebugSerial(void)
{
    DateTime systemTime = getSystemTime();
    debugSerial.println();
    debugSerial.println("=== HEURE SYSTÈME ===");
    
    debugSerial.print("Système: ");
    debugSerial.print(systemTime.day()); debugSerial.print("/");
    debugSerial.print(systemTime.month()); debugSerial.print("/");
    debugSerial.print(systemTime.year()); debugSerial.print(" ");
    debugSerial.print(systemTime.hour()); debugSerial.print(":");
    debugSerial.print(systemTime.minute()); debugSerial.print(":");
    debugSerial.println(systemTime.second());
    
    print2digits(systemTime.hour());
    debugSerial.print(":");
    print2digits(systemTime.minute());
    debugSerial.print(":");
    print2digits(systemTime.second());
    debugSerial.print(" ");
    print2digits(systemTime.day());
    debugSerial.print("/");
    print2digits(systemTime.month());
    debugSerial.print("/");
    print2digits(systemTime.year());
    debugSerial.println("");
}

/**
 * @brief Efface une ligne entière en la remplissant d'espaces
 * @param ligne Ligne à effacer (0 à 7)
 * @return void
 */
void non_clearLine(uint8_t ligne) 
{
    drawtext(1,ligne, 0, "                    "); // 20 espaces
delay(1000);
////    
}

/**
 * @brief Affiche un seul caractère à une position donnée
 * @param ligne Ligne (0 à 7)
 * @param colonne Colonne (0 à ~21)
 * @param c Caractère à afficher
 * @return void
 */
void printCharOLED(uint8_t ligne, uint8_t colonne, char c) 
{
    display.setCursor(colonne * 6, ligne * 8);
    display.write(c);
    display.display();
}

/**
 * @brief Affiche une variable formatée sur l'écran OLED à une position donnée
 * @param ligne Ligne sur l'écran OLED où afficher la variable
 * @param col Colonne sur l'écran OLED où commencer l'affichage
 * @param valeur Pointeur vers la valeur à afficher
 * @param type Type de la variable ('c', 's', 'i', 'I', 'f', 'F')
 * @return void
 */
void printVarOLED(uint8_t ligne, uint8_t col, const void *valeur, char type) 
{   char buffer[21];
    switch (type) 
    {
        case 'c':
            snprintf(buffer, sizeof(buffer), "%c", *((char *)valeur));
  
            break;
        case 's':
            snprintf(buffer, sizeof(buffer), "%s", (char *)valeur);
            break;
        case 'i':
        case 'I':
            snprintf(buffer, sizeof(buffer), "%02d", *((int *)valeur));
 // %02d arrangement temporaire           
            break;
        case 'f':
        case 'F':
            snprintf(buffer, sizeof(buffer), "%.2f", *((float *)valeur));
            break;
        default:
            snprintf(buffer, sizeof(buffer), "<inconnu>");
            break;
    }
    drawtext(1,ligne, col, buffer);
}

/**
 * @brief Affiche une variable formatée avec unité et alignement sur l'écran OLED
 * @param ligne Ligne sur l'écran OLED
 * @param col Colonne sur l'écran OLED
 * @param valeur Pointeur vers la donnée à afficher
 * @param type Type de la variable
 * @param unite Chaîne représentant l'unité
 * @param precision Nombre de chiffres après la virgule pour les float
 * @param align Mode d'alignement ('L', 'R', 'C')
 * @return void
 */
void printFormattedOLED(uint8_t ligne, uint8_t col, const void *valeur, char type,
                        const char *unite, int precision, char align) 
{
    char buffer[22];
    char valeurStr[16] = "";
    
    switch (type) 
    {
        case 'c':
            snprintf(valeurStr, sizeof(valeurStr), "%c", *((char *)valeur));
            break;
        case 's':
            snprintf(valeurStr, sizeof(valeurStr), "%s", (char *)valeur);
            break;
        case 'i':
        case 'I':
            snprintf(valeurStr, sizeof(valeurStr), "%d", *((int *)valeur));
            break;
        case 'f':
        case 'F': 
        {
            char format[8];
            snprintf(format, sizeof(format), "%%.%df", precision);
            snprintf(valeurStr, sizeof(valeurStr), format, *((float *)valeur));
            break;
        }
        default:
            snprintf(valeurStr, sizeof(valeurStr), "<invalide>");
            break;
    }
    
    snprintf(buffer, sizeof(buffer), "%s %s", valeurStr, unite ? unite : "");
    
    int len = strlen(buffer);
    char ligneFinale[22] = "";
    
    switch (align) 
    {
        case 'R':
            snprintf(ligneFinale, sizeof(ligneFinale), "%*s", 20, buffer);
            break;
        case 'C':
            snprintf(ligneFinale, sizeof(ligneFinale), "%*s", (20 + len)/2, buffer);
            break;
        default:
            snprintf(ligneFinale, sizeof(ligneFinale), "%-20s", buffer);
            break;
    }
    
    drawtext(1,ligne, col, ligneFinale);
}

/**
 * @brief Affiche un message pendant 1s sur la ligne 8
 * @param message Message à afficher
 * @param defilant True pour faire défiler le message si trop long
 * @param inverse True pour affichage inversé
 * @return void
 */
void displayMessageL8(const char* message, bool defilant, bool inverse) 
{
    debugSerial.print("displayMessageL8() => "); 
    debugSerial.println(message);
    
    display.clearDisplay();
    display.setTextSize(1);
    
    if (inverse) 
    {
        display.setTextColor(BLACK, WHITE);
    }
    else 
    {
        display.setTextColor(WHITE);
    }
    
    int len = strlen(message);
    
    if (defilant && len > (SCREEN_WIDTH / 6)) 
    {
        for (int offset = 0; offset <= (len * 6 - SCREEN_WIDTH); offset += 2) 
        {
            display.clearDisplay();
            display.setCursor(-offset, pixelLine);
            display.print(message);
            display.display();
            delay(30);
        }
        delay(500);
    } 
    else 
    {
        display.setCursor((SCREEN_WIDTH - len * 6) / 2, pixelLine);
        display.print(message);
        display.display();
        delay(1000);
    }
    
    display.setTextColor(WHITE);
}

/**
 * @brief Active ou désactive l'affichage au boot
 * @param actif True pour activer, false pour désactiver
 * @return void
 */
void setDebug(bool actif) 
{
    modeDebugActif = actif;
}

/**
 * @brief Affiche du texte avec taille et position spécifiées
 * @param Txt_Size Taille du texte
 * @param col Colonne de départ
 * @param lig Ligne de départ
 * @param text Texte à afficher
 * @return void
 */
void drawtext(int8_t Txt_Size, uint8_t ligne, uint8_t colonne, const char *text)
{
    if (OLED)
    {
        display.setTextSize(Txt_Size);
        erasetext(colonne, ligne, strlen(text));
//   debugdrawtext =1;     
        if (debugdrawtext)
        {
            debugSerial.print("txt: "); debugSerial.println(text);
            debugSerial.print("NB Car: "); debugSerial.print(strlen(text));
            debugSerial.print("dColonne: "); debugSerial.print(colonne); 
            debugSerial.print("/"); debugSerial.println((colonne*OLED_Col)-(colonne?0:0));
        }
//   debugdrawtext=0;     
        display.setTextColor(WHITE); 
        display.setCursor((colonne*OLED_Col)-(colonne?0:0), (ligne*OLED_L1));
        
        for (uint8_t i = 0; i < strlen(text); i++)
        {
            display.write(text[i]);
        }      
        display.display();
    }
}


/**
 * @brief Vérifie si une date est valide
 * @param d Chaîne de caractères représentant la date (format jj/mm/aaaa)
 * @return bool True si la date est valide
 */
bool isDateValid(const char *d) 
{
    int jj = (d[0]-'0')*10 + (d[1]-'0');
    int mm = (d[3]-'0')*10 + (d[4]-'0');
    int yyyy = (d[6]-'0')*1000 + (d[7]-'0')*100 + (d[8]-'0')*10 + (d[9]-'0');
    
    if (mm < 1 || mm > 12 || yyyy < 1900 || yyyy > 2099) 
        return false;
    
    int joursMax[] = {31,28,31,30,31,30,31,31,30,31,30,31};
    
    if ((yyyy % 4 == 0 && yyyy % 100 != 0) || (yyyy % 400 == 0)) 
        joursMax[1] = 29;
    
    return jj >= 1 && jj <= joursMax[mm - 1];
}

/**
 * @brief Modifie un chiffre d'une date
 * @param d Chaîne de caractères de la date
 * @param pos Position du chiffre à modifier
 * @param delta Valeur à ajouter (+1 ou -1)
 * @return void
 */
void modifyDateDigit(char *d, uint8_t pos, int delta) 
{
    if (d[pos] >= '0' && d[pos] <= '9') 
    {
        int val = d[pos] - '0' + delta;
        if (val >= 0 && val <= 9) 
        {
            d[pos] = val + '0';
            displayMessageL8(isDateValid(d) ? "Date valide" : "Date invalide !", false, false);
        }
    }
}

/**
 * @brief Affiche la date en cours de saisie sur l'écran
 * @param d Chaîne de caractères de la date
 * @param pos Position du curseur
 * @return void
 */
void displayDateOnScreen(char *d, uint8_t pos) 
{
    if (millis() - lastBlink >= 500) 
    {
        lastBlink = millis();
        blinkState = !blinkState;
    }
    
    char ligne[11];
    for (int i = 0; i < 10; i++) 
        ligne[i] = (i == pos && blinkState) ? '_' : d[i];
    ligne[10] = '\0';
    
    clearOLED();
    drawtext(1,0, 0, "Saisir la date :");
    drawtext(1,1, 0, ligne);
}

/**
 * @brief Interface de saisie d'une date
 * @param d Chaîne de caractères pour stocker la date saisie
 * @return void
 */
void inputDate(char *d) 
{
    uint8_t pos = 0;
    bool done = false;
    
    while (!done) 
    {
        displayDateOnScreen(d, pos);
        key_code_t t = readKey();
        
        switch (t) 
        {
            case LEFT:  
                if (pos > 0) pos--; 
                break;
            case RIGHT: 
                if (pos < 9) pos++; 
                break;
            case UP:    
                modifyDateDigit(d, pos, +1); 
                break;
            case DOWN:  
                modifyDateDigit(d, pos, -1); 
                break;
            case VALIDE: 
                if (isDateValid(d)) done = true; 
                break;
        }
        delay(20);
    }
}

/**
 * @brief Vérifie si une heure est valide
 * @param h Chaîne de caractères représentant l'heure (format hh:mm:ss)
 * @return bool True si l'heure est valide
 */
bool isTimeValid(const char *h) 
{
    int hh = (h[0]-'0')*10 + (h[1]-'0');
    int mm = (h[3]-'0')*10 + (h[4]-'0');
    int ss = (h[6]-'0')*10 + (h[7]-'0');
    return hh <= 23 && mm <= 59 && ss <= 59;
}

/**
 * @brief Modifie un chiffre d'une heure
 * @param h Chaîne de caractères de l'heure
 * @param pos Position du chiffre à modifier
 * @param delta Valeur à ajouter (+1 ou -1)
 * @return void
 */
void modifyTimeDigit(char *h, uint8_t pos, int delta) 
{
    if (h[pos] >= '0' && h[pos] <= '9') 
    {
        int val = h[pos] - '0' + delta;
        if (val >= 0 && val <= 9) 
        {
            h[pos] = val + '0';
            displayMessageL8(isTimeValid(h) ? "Heure valide" : "Heure invalide !", false, false);
        }
    }
}

/**
 * @brief Affiche l'heure en cours de saisie sur l'écran
 * @param h Chaîne de caractères de l'heure
 * @param pos Position du curseur
 * @return void
 */
void displayTimeOnScreen(char *h, uint8_t pos) 
{
    if (millis() - lastBlink >= 500) 
    {
        lastBlink = millis();
        blinkState = !blinkState;
    }
    
    char ligne[9];
    for (int i = 0; i < 8; i++) 
        ligne[i] = (i == pos && blinkState) ? '_' : h[i];
    ligne[8] = '\0';
    
    clearOLED();
    drawtext(1,0, 0, "Saisir l'heure :");
    drawtext(1,1, 0, ligne);
}

/**
 * @brief Interface de saisie d'une heure
 * @param h Chaîne de caractères pour stocker l'heure saisie
 * @return void
 */
void inputTime(char *h) 
{
    uint8_t pos = 0;
    bool done = false;
    
    while (!done) 
    {
        displayTimeOnScreen(h, pos);
        key_code_t t = readKey();
        
        switch (t) 
        {
            case LEFT:  
                if (pos > 0) pos--; 
                break;
            case RIGHT: 
                if (pos < 7) pos++; 
                break;
            case UP:    
                modifyTimeDigit(h, pos, +1); 
                break;
            case DOWN:  
                modifyTimeDigit(h, pos, -1); 
                break;
            case VALIDE: 
                if (isTimeValid(h)) done = true; 
                break;
        }
        delay(20);
    }
}

/**
 * @brief Permet de sélectionner une valeur parmi une liste fixe
 * @param label Texte à afficher (ligne 0)
 * @param liste Tableau de valeurs possibles
 * @param nbValeurs Nombre d'éléments dans la liste
 * @param valeurSelectionnee Pointeur vers la variable qui recevra la valeur choisie
 * @return void
 */
void inputListValue(const char *label, const int *liste, uint8_t nbValeurs, int *valeurSelectionnee) 
{
    uint8_t index = 0;
    bool valide = false;
    
    while (!valide) 
    {
        clearOLED();
        drawtext(1,0, 0, label);
        
        char buffer[20];
        sprintf(buffer, "Valeur : %d", liste[index]);
        drawtext(1,1, 0, buffer);
        displayMessageL8("Valider: VALIDE/UP/DOWN", false, false);
        
        key_code_t touche = readKey();
        switch (touche) 
        {
            case UP:
                if (index > 0) index--;
                break;
            case DOWN:
                if (index < nbValeurs - 1) index++;
                break;
            case VALIDE:
                *valeurSelectionnee = liste[index];
                displayMessageL8("Valeur confirmée !", false, false);
                valide = true;
                break;
        }
        delay(150);
    }
}

/**
 * @brief Permet de sélectionner une valeur avec libellé parmi une liste
 * @param label Texte à afficher
 * @param valeurs Tableau des valeurs
 * @param libelles Tableau des libellés correspondants
 * @param nbValeurs Nombre d'éléments
 * @param valeurSelectionnee Pointeur vers la valeur sélectionnée
 * @param valide Pointeur vers le flag de validation
 * @return void
 */
void inputListValueLibelle(const char *label, const int *valeurs, const char **libelles,
                               uint8_t nbValeurs, int *valeurSelectionnee, bool *valide) 
{
    uint8_t index = 0;
    *valide = false;
    bool enCours = true;
    
    while (enCours) 
    {
        clearOLED();
        drawtext(1,0, 0, label);
        
        char ligne1[21];
        snprintf(ligne1, sizeof(ligne1), "%s (%d)", libelles[index], valeurs[index]);
        drawtext(1,1, 0, ligne1);
        drawtext(1,3, 0, "T3: ↑  T4: ↓");
        drawtext(1,4, 0, "T5: Valider  T2: Annuler");
        
        key_code_t touche = readKey();
        switch (touche) 
        {
            case UP:
                if (index > 0) index--;
                break;
            case DOWN:
                if (index < nbValeurs - 1) index++;
                break;
            case VALIDE:
                *valeurSelectionnee = valeurs[index];
                *valide = true;
                displayMessageL8("Valeur validée !", false, false);
                enCours = false;
                break;
            case ANNULER_4:
                displayMessageL8("Saisie annulée", false, false);
                enCours = false;
                break;
        }
        delay(150);
    }
}

/**
 * @brief Interface de saisie d'une valeur hexadécimale 64 bits
 * @param variable Nom de la variable (affichage)
 * @param valeurInitiale Valeur initiale à afficher
 * @return uint64_t Valeur hexadécimale saisie
 */
uint64_t inputHex(const char* variable, uint64_t valeurInitiale) 
{
    char digits[17];
    uint8_t pos = 0;
    
    debugSerial.print("inputHex()  ");
    
    for (int i = 15; i >= 0; i--) 
    {
        digits[i] = "0123456789ABCDEF"[valeurInitiale & 0xF];
        valeurInitiale >>= 4;
    }
    digits[16] = '\0';
    
    debugSerial.print(digits);
    
    bool valide = false;
    while (!valide) 
    {
        display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(WHITE);
        display.setCursor(0, 0);
        display.print("Saisie HEXA");
        display.setCursor(0, 8);
        display.print(variable);
        display.setCursor(0, 28);
        
        for (int i = 0; i < 16; i++) 
        {
            if (i == pos) 
            {
                display.setTextColor(BLACK, WHITE);
                display.print(digits[i]);
                display.setTextColor(WHITE);
            } 
            else 
            {
                display.print(digits[i]);
            }
        }
        display.display();
        
        key_code_t touche = readKey();
        if (touche == MOINS) 
        {
            if (digits[pos] == '0') digits[pos] = 'F';
            else if (digits[pos] == 'A') digits[pos] = '9';
            else digits[pos]--;
        }
        else if (touche == PLUS) 
        {
            if (digits[pos] == '9') digits[pos] = 'A';
            else if (digits[pos] == 'F') digits[pos] = '0';
            else digits[pos]++;
        }
        else if (touche == LEFT && pos > 0) 
        {
            pos--;
        }
        else if (touche == RIGHT && pos < 15) 
        {
            pos++;
        }
        else if (touche == VALIDE) 
        {
            valide = true;
        }
        delay(200);
    }
    
    uint64_t resultat = 0;
    for (int i = 0; i < 16; i++) 
    {
        char c = digits[i];
        resultat <<= 4;
        if (c >= '0' && c <= '9') resultat += (c - '0');
        else if (c >= 'A' && c <= 'F') resultat += (c - 'A' + 10);
    }
    
    debugSerial.print(" => ");debugSerial.println(digits);
    return resultat;
}

/**
 * @brief Interface de saisie d'une valeur décimale 32 bits
 * @param variable Nom de la variable (affichage)
 * @param valeurInitiale Valeur initiale à afficher
 * @return uint32_t Valeur décimale saisie
 */
uint32_t inputDecimal(const char* variable, uint32_t valeurInitiale) 
{
    char digits[7];
    uint8_t pos = 0;
    
    debugSerial.print("inputDecimal()  ");
    debugSerial.print(valeurInitiale);
    
    for (int i = 5; i >= 0; i--)
    {
        digits[i] = '0' + (valeurInitiale % 10);
        valeurInitiale /= 10;
    }
    digits[6] = '\0';
    
    bool valide = false;
    
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.print("Saisie DEC.");
    display.setCursor(0, 8);
    display.print(variable);
    
    while (!valide) 
    {
        display.setCursor((SCREEN_WIDTH - 6 * 6) / 2, 28);
        for (int i = 0; i < 6; i++)
        {
            if (i == pos) 
            {
                display.setTextColor(BLACK, WHITE);
                display.print(digits[i]);
                display.setTextColor(WHITE);
            } 
            else 
            {
                display.print(digits[i]);
            }
        }
        display.display();
        
        key_code_t touche = readKey();
        switch(touche)
        { 
            case MOINS:
                digits[pos] = (digits[pos] == '0') ? '9' : digits[pos] - 1;
                break;
            case PLUS:
                digits[pos] = (digits[pos] == '9') ? '0' : digits[pos] + 1;      
                break;
            case LEFT:
                if (pos > 0) 
                {
                    pos--;
                } 
                break;
            case RIGHT:
                if (pos < 5) 
                {
                    pos++;
                } 
                break;
            case VALIDE: 
                valide = true;
                break;
        }
        delay(200);
    }
    
    uint32_t resultat = 0;
    for (int i = 0; i < 6; i++) 
    {
        resultat = resultat * 10 + (digits[i] - '0');
    }
    
    debugSerial.print(" => ");debugSerial.println(resultat);
    return resultat;
}

/**
 * @brief Construit et affiche le message HEXA sur le port série de debug
 * @param payload Pointeur vers les données
 * @param len Longueur des données
 * @return void
 */
void printLoraPayloadOndebugSerial(uint8_t *payload, uint8_t len)
{
    int i;
    for (i = 0; i < len; i++) 
    {
        sprintf(&hexPayload[i * 2], "%02X", payload[i]);  
    }
    hexPayload[hexPayloadSize - 1] = '\0';
    
    debugSerial.print("hexPayload: "); 
    debugSerial.print(hexPayload);
    debugSerial.print(" len : "); 
    debugSerial.println(len);
}

/**
 * @brief Affiche des données au format hexadécimal sur le port série
 * @param txt Pointeur vers les données
 * @param len Longueur des données
 * @return void
 */
void printOndebugSerial(char *txt, char len)
{
    int i;
    char buf[256];
    
    if (COM_DebugSerial)
    {
        memset(buf, 0, 256);
        for (i = 0; i < len; i++)  
        { 
            buf[2*i] = ((txt[i]>>4) < 10) ? (txt[i]>>4) + 48 : (txt[i]>>4) + 55;
            buf[2*i+1] = ((txt[i]&0x0F) < 10) ? (txt[i]&0x0F) + 48 : (txt[i]&0x0F) + 55;
        }
        debugSerial.print(" ");
        buf[2*i] = 0;
        debugSerial.println(buf);
    }
}

// ===== FONCTIONS UTILITAIRES SUPPLÉMENTAIRES =====

/**
 * @brief Configure les registres de bas niveau pour la basse consommation
 * @param Aucun
 * @return void
 */
void configureLowPowerMode(void)
{
    // Désactiver les périphériques non utilisés pour économiser l'énergie
    // Configuration spécifique ATSAMD21
    
    // Désactiver l'USB si non utilisé en mode exploitation
    if (modeExploitation && DEBUG_LOW_POWER)
    {
        // USB->DEVICE.CTRLA.bit.ENABLE = 0;
    }
    
    debugSerial.println("Mode basse consommation configuré");
}

/**
 * @brief Vérifie l'état des alarmes RTC et met à jour les flags - ALARMES ÉCHANGÉES
 * @param Aucun
 * @return void
 */
void checkRTCStatus(void)
{
    // Vérification de l'alarme 1 (1 seconde)
    if (rtc.alarmFired(1))
    {
        debugSerial.println("*** ALARME 1 DÉTECTÉE (1 SECONDE) ***");
        wakeup1Sec = true;
        rtc.clearAlarm(1);
 // même remarque que dans : void onRTCAlarm(void)    
 /*
 // Reprogrammer l'alarme 1 seconde
        if (DEBUG_INTERVAL_1SEC && !modeExploitation)
        {
            DateTime nextSecond = rtc.now() + TimeSpan(0, 0, 0, 1);
            rtc.setAlarm1(nextSecond, DS3231_A1_PerSecond);
        }
*/        
    }
    
    // Vérification de l'alarme 2 (payload)
    if (rtc.alarmFired(2))
    {
        debugSerial.println("*** ALARME 2 DÉTECTÉE (PAYLOAD) ***");
        wakeupPayload = true;
        rtc.clearAlarm(2);
        
        // Reprogrammer l'alarme payload
        if (DEBUG_WAKEUP_PAYLOAD)
        {
            DateTime nextPayload = rtc.now() + TimeSpan(0, 0, config.applicatif.wakeupIntervalPayload, 0);
            rtc.setAlarm2(nextPayload, DS3231_A2_Minute);
            
            debugSerial.print("Prochaine alarme payload: ");
            debugSerial.print(nextPayload.hour()); debugSerial.print(":");
            debugSerial.print(nextPayload.minute()); debugSerial.print(":");
            debugSerial.println(nextPayload.second());
        }
    }
    
    // Debug périodique de l'état des alarmes (toutes les 60 secondes)
    static unsigned long lastAlarmDebug = 0;
    if (millis() - lastAlarmDebug > 60000)
    {
        lastAlarmDebug = millis();
        
        debugSerial.println("=== ÉTAT ALARMES RTC (ÉCHANGÉES) ===");
        debugSerial.print("Alarme 1 (1 sec) activée: ");
        debugSerial.println((DEBUG_INTERVAL_1SEC && !modeExploitation) ? "OUI" : "NON");
        debugSerial.print("Alarme 2 (payload) activée: ");
        debugSerial.println(DEBUG_WAKEUP_PAYLOAD ? "OUI" : "NON");
        debugSerial.print("Mode: ");
        debugSerial.println(modeExploitation ? "EXPLOITATION" : "PROGRAMMATION");
        
        // Afficher l'heure actuelle
        DateTime now = rtc.now();
        debugSerial.print("Heure actuelle: ");
        debugSerial.print(now.hour()); debugSerial.print(":");
        debugSerial.print(now.minute()); debugSerial.print(":");
        debugSerial.println(now.second());
    }
}


/**
 * @brief Gère l'affichage et les actions du mode exploitation
 * @param Aucun
 * @return void
 */
void executeOperationMode(void)     // pas appelé à la base....
{
    static unsigned long lastModeDisplay = 0;
    
    // Affichage périodique du mode (toutes les 10 secondes)
    if (millis() - lastModeDisplay > 10000)
    {
        lastModeDisplay = millis();
        
        if (OLED)
        {
            clearOLED();
            drawtext(1,0, 0, PROJECT_NAME);
            drawtext(1,1, 0, "Mode: EXPLOITATION");
            drawtext(1,2, 0, "PIN_PE = HIGH");
            
            char buffer[21];
            sprintf(buffer, "Reveil: %d min", config.applicatif.wakeupIntervalPayload);
            drawtext(1,4, 0, buffer);
            
            drawScreenTime(0, 0);   // 0, 6 ????
            
            // Affichage état debug
            drawtext(1, 7, 0, DEBUG_LOW_POWER ? "Sleep: ON" : "Sleep: OFF");
        }
        
        debugSerial.println("=== MODE EXPLOITATION ===");
        debugSerial.print("Prochain réveil payload dans: ");
        debugSerial.print(config.applicatif.wakeupIntervalPayload);
        debugSerial.println(" minutes");
    }
}

/**
 * @brief Gère l'affichage et les actions du mode programmation
 * @param Aucun
 * @return void
 */
void executeProgrammingMode(void)
{
    static unsigned long lastModeDisplay = 0;
    static bool menuDisplayed = false;
    
    // Affichage initial du menu
    if (!menuDisplayed || (millis() - lastModeDisplay > 30000))
    {
        lastModeDisplay = millis();
        menuDisplayed = true;
        
        if (OLED)
        {
            clearOLED();
            drawtext(1,0, 0, "MODE PROGRAMMATION");
            drawtext(1,1, 0, "PIN_PE = LOW");
            drawtext(1,3, 0, "T1: Test clavier");
            drawtext(1,4, 0, "T2: Config");
            drawtext(1,5, 0, "T3: Heure/Date");
            drawtext(1,6, 0, "T4: Debug");
            drawtext(1,7, 0, "T5: Infos");
        }
        
        debugSerial.println("=== MODE PROGRAMMATION ===");
        debugSerial.println("Menu principal affiché");
    }
    
    // Traitement des touches du menu principal
    key_code_t touche = readKey();
    if (touche != KEY_NONE)
    {
        menuDisplayed = false; // Forcer le réaffichage du menu après action
        
        switch (touche)
        {
            case KEY_1:
                debugSerial.println("Test clavier sélectionné");
                if (OLED)
                {
                    displayMessageL8("Test clavier...", false, false);
                }
                // La fonction testKbd() sera appelée automatiquement dans la boucle
                break;
                
            case KEY_2:
                debugSerial.println("Configuration sélectionnée");
                handleConfigurationMenu();
                break;
                
            case KEY_3:
                debugSerial.println("Heure/Date sélectionnée");
                handleTimeDateRegisterMenu();
                break;
                
            case KEY_4:
                debugSerial.println("Debug sélectionné");
                handleDebugMenu();
                break;
                
            case KEY_5:
                debugSerial.println("Infos sélectionnées");
                displaySystemInfo();
                break;
                
            default:
                break;
        }
    }
    
    // Test clavier en continu en mode programmation
    testKbd();
}

/**
 * @brief Gère le menu de configuration
 * @param Aucun
 * @return void
 */
void handleConfigurationMenu(void)
{
    bool menuConfig = true;
    
    while (menuConfig)
    {
        if (OLED)
        {
            clearOLED();
            drawtext(1,0, 0, "=== CONFIGURATION ===");
            drawtext(1,2, 0, "T1: Duree LED rouge");
            drawtext(1,3, 0, "T2: Duree LED builtin");
            drawtext(1,4, 0, "T3: Intervalle payload");
            drawtext(1,5, 0, "T4: Sauvegarder");
            drawtext(1,7, 0, "T5: Retour");
        }
        
        key_code_t touche = readKey();
        switch (touche)
        {
            case KEY_1:
            {
                uint32_t nouvelleDuree = inputDecimal("Duree LED rouge (ms)", config.applicatif.redLedDuration);
                config.applicatif.redLedDuration = nouvelleDuree;
                displayMessageL8("Duree LED modifiee", false, false);
                break;
            }
            
            case KEY_2:
            {
                uint32_t nouvelleDuree = inputDecimal("Duree LED builtin (ms)", config.applicatif.builtinLedDuration);
                config.applicatif.builtinLedDuration = nouvelleDuree;
                displayMessageL8("Duree LED modifiee", false, false);
                break;
            }
            
            case KEY_3:
            {
                uint32_t nouvelIntervalle = inputDecimal("Intervalle payload (min)", config.applicatif.wakeupIntervalPayload);
                config.applicatif.wakeupIntervalPayload = nouvelIntervalle;
                displayMessageL8("Intervalle modifie", false, false);
                break;
            }
            
            case KEY_4:
                saveConfigToEEPROM();
                displayMessageL8("Configuration sauvee!", false, false);
                break;
                
            case KEY_5:
                menuConfig = false;
                break;
                
            default:
                break;
        }
        
        delay(100);
    }
}

/**
 * @brief Gère le menu de configuration Heure/Date
 * @param Aucun
 * @return void
 */
void handleTimeDateRegisterMenu(void)
{
    bool menuHeure = true;
    
    while (menuHeure)
    {
        if (OLED)
        {
            clearOLED();
            drawtext(1,0, 0, "=== HEURE / DATE ===");
            drawtext(1,2, 0, "T1: Saisir heure");
            drawtext(1,3, 0, "T2: Saisir date");
            drawtext(1,4, 0, "T3: Afficher temps");
            drawtext(1,5, 0, "T4: Comparer horloges");
            drawtext(1,7, 0, "T5: Retour");
        }
        
        key_code_t touche = readKey();
        switch (touche)
        {
            case KEY_1:
            {
                char heureStr[9] = "12:34:56";
                DateTime maintenant = rtc.now();
                sprintf(heureStr, "%02d:%02d:%02d", maintenant.hour(), maintenant.minute(), maintenant.second());
                inputTime(heureStr);
                
                // Mettre à jour le RTC avec la nouvelle heure
                int hh = (heureStr[0]-'0')*10 + (heureStr[1]-'0');
                int mm = (heureStr[3]-'0')*10 + (heureStr[4]-'0');
                int ss = (heureStr[6]-'0')*10 + (heureStr[7]-'0');
                
                DateTime nouvelleHeure(maintenant.year(), maintenant.month(), maintenant.day(), hh, mm, ss);
                rtc.adjust(nouvelleHeure);
                
                displayMessageL8("Heure mise a jour!", false, false);
                break;
            }
            
            case KEY_2:
            {
                char dateStr[11] = "01/01/2025";
                DateTime maintenant = rtc.now();
                sprintf(dateStr, "%02d/%02d/%04d", maintenant.day(), maintenant.month(), maintenant.year());
                inputDate(dateStr);
                
                // Mettre à jour le RTC avec la nouvelle date
                int jj = (dateStr[0]-'0')*10 + (dateStr[1]-'0');
                int mm = (dateStr[3]-'0')*10 + (dateStr[4]-'0');
                int yyyy = (dateStr[6]-'0')*1000 + (dateStr[7]-'0')*100 + (dateStr[8]-'0')*10 + (dateStr[9]-'0');
                
                DateTime nouvelleDate(yyyy, mm, jj, maintenant.hour(), maintenant.minute(), maintenant.second());
                rtc.adjust(nouvelleDate);
                
                displayMessageL8("Date mise a jour!", false, false);
                break;
            }
            
            case KEY_3:
                printTimeOndebugSerial();
                displayMessageL8("Temps affiche serie", false, false);
                break;
                
            case KEY_4:
                printTimeComparison();
                displayMessageL8("Comparaison serie", false, false);
                break;
                
            case KEY_5:
                menuHeure = false;
                break;
                
            default:
                break;
        }
        
        delay(100);
    }
}

/**
 * @brief Gère le menu de debug
 * @param Aucun
 * @return void
 */
void handleDebugMenu(void)
{
    bool menuDebug = true;
    
    while (menuDebug)
    {
        if (OLED)
        {
            clearOLED();
            drawtext(1,0, 0, "=== DEBUG ===");
            
            char ligne[21];
            sprintf(ligne, "Payload: %s", DEBUG_WAKEUP_PAYLOAD ? "ON" : "OFF");
            drawtext(1,2, 0, ligne);
            
            sprintf(ligne, "1Sec: %s", DEBUG_INTERVAL_1SEC ? "ON" : "OFF");
            drawtext(1,3, 0, ligne);
            
            sprintf(ligne, "Sleep: %s", DEBUG_LOW_POWER ? "ON" : "OFF");
            drawtext(1,4, 0, ligne);
            
            drawtext(1,6, 0, "T1/T2/T3: Toggle");
            drawtext(1,7, 0, "T5: Retour");
        }
        
        key_code_t touche = readKey();
        switch (touche)
        {
            case KEY_1:
                DEBUG_WAKEUP_PAYLOAD = !DEBUG_WAKEUP_PAYLOAD;
                debugSerial.print("DEBUG_WAKEUP_PAYLOAD: ");
                debugSerial.println(DEBUG_WAKEUP_PAYLOAD ? "ON" : "OFF");
                if (!DEBUG_WAKEUP_PAYLOAD) clearRTCAlarms();
                else setRTCAlarms();
                break;
                
            case KEY_2:
                DEBUG_INTERVAL_1SEC = !DEBUG_INTERVAL_1SEC;
                debugSerial.print("DEBUG_INTERVAL_1SEC: ");
                debugSerial.println(DEBUG_INTERVAL_1SEC ? "ON" : "OFF");
                if (!DEBUG_INTERVAL_1SEC) clearRTCAlarms();
                else setRTCAlarms();
                break;
                
            case KEY_3:
                DEBUG_LOW_POWER = !DEBUG_LOW_POWER;
                debugSerial.print("DEBUG_LOW_POWER: ");
                debugSerial.println(DEBUG_LOW_POWER ? "ON" : "OFF");
                configureLowPowerMode();
                break;
                
            case KEY_5:
                menuDebug = false;
                break;
                
            default:
                break;
        }
        
        delay(100);
    }
}

/**
 * @brief Affiche les informations système
 * @param Aucun
 * @return void
 */
void displaySystemInfo(void)
{
    if (OLED)
    {
        clearOLED();
        drawtext(1,0, 0, "=== INFOS SYSTEME ===");
        drawtext(1,1, 0, PROJECT_NAME);
        drawtext(1,2, 0, "Version: " VERSION);
        
        char ligne[21];
        sprintf(ligne, "Mode: %s", modeExploitation ? "EXPLOIT" : "PROGRAM");
        drawtext(1,3, 0, ligne);
        
        sprintf(ligne, "Config v: %d.%02d", config.materiel.version/100, config.materiel.version%100);
        drawtext(1,4, 0, ligne);
        
        drawtext(1,6, 0, "Appuyer pour continuer");
    }
    
    debugSerial.println("=== INFORMATIONS SYSTEME ===");
    debugSerial.println("Projet: " PROJECT_NAME);
    debugSerial.println("Version: " VERSION);
    debugSerial.print("Mode actuel: ");
    debugSerial.println(modeExploitation ? "EXPLOITATION" : "PROGRAMMATION");
    debugSerial.print("Configuration version: ");
    debugSerial.print(config.materiel.version/100);
    debugSerial.print(".");
    debugSerial.println(config.materiel.version%100);
    debugSerial.print("Compilation: ");
    debugSerial.print(__DATE__);
    debugSerial.print(" ");
    debugSerial.println(__TIME__);
    
    // Attendre une touche pour continuer
    while (readKey() == KEY_NONE)
    {
        delay(100);
    }
    
    displayMessageL8("Retour menu principal", false, false);
}
