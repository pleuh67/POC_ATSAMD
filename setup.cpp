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
void SETUPinitDebugSerial(void)
{
    debugSerial.begin(DEBUG_BAUD);
    while ((!debugSerial) && (millis() < SERIAL_TIMEOUT)) {}
    delay(1000);
    LOG_INFO("Compiled: " __DATE__ ", " __TIME__ ", " VERSION);
    LOG_INFO(PROJECT_NAME);
}


// Port Série LoRa ------------ initialisation -------------------------------*
// ---------------------------------------------------------------------------*
// ---------------------------------------------------------------------------*
void SETUPSETUPinitLoRaSerial(void)
{
  loraSerial.begin(LoRaBee.getDefaultBaudRate());
  LoRaBee.setDiag(debugSerial); // to use debug remove //DEBUG inside library
  LoRaBee.init(loraSerial, LORA_RESET);
}


void SETUPswitchToOperationMode(void)
{
    
// supprimer 3 lignes si pas revu.
debugSerial.println("Passage en mode EXPLOITATION");

    switchToProgrammingMode = true;
    switchToOperationMode = false;
// Reactive l'affichage du menu de démarrage pour sortie mode EXPLOITATION
    startupListActivated = false;
    OLEDClear(); 
    OLEDDrawText(1, 7, 0, "MODE EXPLOITATION   ");

// préciser le statut des menus, retour au propre au PRINCIPAL
//debugSerialPrintMenuStruct(&menuStack[currentMenuDepth]);
//debugSerialListStruct();
/**/
}

// ---------------------------------------------------------------------------*
// out : 0 (Error ou non référencé) / n (Num de carte)  
// @brief Reset immédiat du microcontrôleur

// @param void
// @return void
// ---------------------------------------------------------------------------*
void SETUPsoftReset() 
{
  NVIC_SystemReset();
// Le code ne continue jamais après cette ligne
}

// ---------------------------------------------------------------------------*
//  out : 0 (Error ou non référencé) / n (Num de carte)  
// @brief Find card number
//                Num_Carte (global) dans fonction
// @param : uint8_t *HWEUI
// @return :  0 error or Card number
// ---------------------------------------------------------------------------*
uint8_t non_RN2483Init(uint8_t *HWEUI)  
{ 
/*
RN2483AgetHWEUI(Module_ID_HWEUI);     // get the Hardware DevEUI ex: "0004A30B0020300A"               char Module_ID_HWEUI[20] 
sprintf(serialbuf,"Init_2483()/RN2483AgetHWEUI Module_ID_HWEUI: %s",Module_ID_HWEUI ); 
debugSerial.println(serialbuf);
*/
RN2483AgetHWEUI(config.materiel.DevEUI);     // get the Hardware DevEUI ex: "0004A30B0020300A"   uint8_t HWEUI[8];



//debugSerial.print("Init_2483()/RN2483AgetHWEUI &config.materiel.DevEUI[0]:"); 
//printByteArray(config.materiel.DevEUI, 16);

//OLEDDebugDisplay(OLEDbuf);
//debugSerial.println("------------------------------------------------------------------");

  char Module_ID[17];
  convertToHexString(config.materiel.DevEUI, Module_ID, 8);

//debugSerial.println(Module_ID);
 
  for (config.materiel.Num_Carte=0;config.materiel.Num_Carte< MAX_HWEUI_List; config.materiel.Num_Carte++)
  { 
    if (strncmp(HWEUI_List[config.materiel.Num_Carte],Module_ID,16)==0)   // égalité des 2 
    {
// INITIALISER LES IDENTIFIANTS OTAA de LoRa + Contrôle
  memcpy(config.materiel.DevEUI, SN2483_List[config.materiel.Num_Carte], sizeof(SN2483_List[config.materiel.Num_Carte]));

// Init AppEUI
      memcpy(config.applicatif.AppEUI, AppEUI_List,9);  //  10 avant le 22/01 KKKKK
// Init AppKey
      memcpy(config.applicatif.AppKey, AppKey_List[config.materiel.Num_Carte],17);
       break;  // sortie du for() quand trouvé egalité.... pas terrible!!!  
    }
    LOG_ERROR("HWEUI non trouvé");
  } 

// config.materiel.Noeud_LoRa = config.materiel.Num_Carte;  // par défaut, fonction Module_ID

      if (config.materiel.Num_Carte)
      {
        debugSerial.print(" execution RN2483Init(), done with card : ");
        debugSerial.println(config.materiel.Num_Carte);
       }
      else
      {
        LOG_ERROR("NO 2483 present.");
        OLEDDebugDisplay("2483A   STP   Failed");
      }
  return((config.materiel.Num_Carte == MAX_HWEUI_List) ? 0 : config.materiel.Num_Carte); // retourne 0 error or Card number
}



// ---------------------------------------------------------------------------*
//  out : 0 (Error ou non référencé) / n (Num de carte)  
// @brief Find card number
//                Num_Carte (global) dans fonction
// @param void
// @return :  0 error or Card number
// ---------------------------------------------------------------------------*

// appel avec  config.materiel.Num_Carte = SETUPinit2483A(config.materiel.DevEUI);   // return :  0 error or Card number

uint8_t SETUPinit2483A(uint8_t *HWEUI)  // Init 2483 ---- Init 2483 ---- Init 2483 ---- Init 2483 
{ uint8_t tryy = 1 , rc;

  uint8_t DevEUI[8];
  uint8_t len = LoRaBee.getHWEUI(DevEUI, sizeof(DevEUI));
   // Print the Hardware EUI
  LOG_DEBUG("SETUPinit2483A()/LoRaBee.RN2483AgetHWEUI => DevEUI: ");
  for (uint8_t i = 0; i < sizeof(DevEUI); i++) 
  {
      debugSerial.print((char)NIBBLE_TO_HEX_CHAR(HIGH_NIBBLE(DevEUI[i])));
      debugSerial.print((char)NIBBLE_TO_HEX_CHAR(LOW_NIBBLE(DevEUI[i])));
  }
  debugSerial.println();  


/*
   do
   { rc = RN2483Init(HWEUI); // 0 = erreur ou Num de carte
//debugSerial.println(rc ? "rc Init_2483() true":"rc Init_2483() false");
// Reset_LoRa();  // initialise pas sur reset chaud.
 */     
// nnnnnnnnnnnnnnnnn

// passer uint_ DEVEUI initialisé en char *Module_ID           DevEUI: 0004A30B00EEEE01
  char Module_ID[17];
 
 // recopié 
 //                          source             destination
  convertToHexString(DevEUI, Module_ID, 8);
 /*for (uint8_t i = 0; i < len; i++)
  {
    // Convertir chaque octet en deux caractères hexadécimaux
    sprintf(&Module_ID[i * 2], "%02X", DevEUI[i]);
  }
  // Terminer la chaîne avec un caractère nul
  Module_ID[len * 2] = '\0';

// fin recopié
*/
debugSerial.print("SETUPinit2483A()/Module_ID => ");
debugSerial.println(Module_ID);
 
  for (config.materiel.Num_Carte=0;config.materiel.Num_Carte< MAX_HWEUI_List; config.materiel.Num_Carte++)
  { 
    if (strncmp(HWEUI_List[config.materiel.Num_Carte],Module_ID,16)==0)   // égalité des 2 
    {
// INITIALISER LES IDENTIFIANTS OTAA de LoRa + Contrôle
  memcpy(config.materiel.DevEUI, SN2483_List[config.materiel.Num_Carte], sizeof(SN2483_List[config.materiel.Num_Carte]));

// Init AppEUI
      memcpy(config.applicatif.AppEUI, AppEUI_List,9);  //  10 avant le 22/01 KKKKK
// Init AppKey
      memcpy(config.applicatif.AppKey, AppKey_List[config.materiel.Num_Carte],17);
       break;  // sortie du for() quand trouvé egalité.... pas terrible!!!  
    }
 //debugSerial.println("HWEUI non trouvé");
  } 

  rc = config.materiel.Num_Carte == MAX_HWEUI_List ? 0 : config.materiel.Num_Carte; // retourne 0 error or Card number

// nnnnnnnnnnnnnnnnn 
      if (config.materiel.Num_Carte)
      {char msg[80];
        snprintf(msg,80," Init 2483 done with card : %2d",config.materiel.Num_Carte);
        LOG_DEBUG(msg);
        OLEDDebugDisplay("RN2483A STP       OK");
      }
      else
      {
        LOG_DEBUG("NO 2483 present.");
        OLEDDebugDisplay("RN2483A STP   Failed");
      }
  /*    
      tryy--;
    }  
    while (!rc && tryy); // erreur et max 4 fois
 */ 
 /*  
    else 
    {
      debugSerial.println(" Init 2483 failed");    
      OLEDDebugDisplay("2483A   Failed");
    }  
*/
// debugSerialPrintLoRaStatus();

  LOG_DEBUG("SETUPinit2483A() => fin Fonction");
  return(rc);
}



// appelé par Send_Lora_Mess() et setup()
bool SETUPsetupLoRa()
{ bool result;
  result=SETUPsetupLoRaOTAA();
  return(result);
}



// ---------------------------------------------------------------------------*
// ---------------------------------------------------------------------------*
bool SETUPinitLoRa(void)
{
//  Reset_LoRa();  // initialise pas sur reset chaud.
  return(SETUPsetupLoRa());
}

// INIT DHT22 ---- INIT DHT22 ---- INIT DHT22 ---- INIT DHT22 ---- 
// ---------------------------------------------------------------------------*
// ---------------------------------------------------------------------------*
void SETUPDHTInit(void)
{
debugSerial.println("--------------------------------- SETUP - INIT DHT22 ---------------------------------");
  dht.begin(); // temperature
  debugSerial.println("done.");
OLEDDebugDisplay("DHT22 STP         OK");
}

//==============================================================================
// SOLUTION 2 : Utiliser un Watchdog Timer (plus radical)
//==============================================================================

// Sur ATSAMD21, on peut utiliser le WDT pour forcer un reset si blocage



// ---------------------------------------------------------------------------
// @brief Initialise le HX711
// @param num, numéro du HX711 à initialiser
// @return void
// ---------------------------------------------------------------------------
// exemple appel: SETUP_HX711initWithWatchdog(numJauge);
void SETUP_HX711initWithWatchdog(int num)
{

  for ( int numGauge=0;numGauge<4;numGauge++)                                 // Z  .. 3
  {     
    if (Peson[config.materiel.Num_Carte][numGauge])
    {   
       switch (numGauge)
       {
         case 0 : scaleA.begin(config.materiel.HX711Dta_0, config.materiel.HX711Clk_0); // DOUT, SCK
  //                scaleA.power_down(); // mis en basse conso après initialisation
                  break;
         case 1 : scaleB.begin(config.materiel.HX711Dta_1, config.materiel.HX711Clk_1); // DOUT, SCK
    //              scaleB.power_down(); // mis en basse conso après initialisation
                  break;
         case 2 : scaleC.begin(config.materiel.HX711Dta_2, config.materiel.HX711Clk_2); // DOUT, SCK
      //            scaleC.power_down(); // mis en basse conso après initialisation
                  break;
         case 3 : scaleD.begin(config.materiel.HX711Dta_3, config.materiel.HX711Clk_3); // DOUT, SCK
        //          scaleD.power_down(); // mis en basse conso après initialisation
                  break;
       }
     }
   }   
// voir porposition Claude : SOLUTION_COMPLETE_HX711_BLOCAGE SODAQ.txt
}

// ATTENTION : Si scale.begin() bloque, le µC va reset après 4 secondes !
// Pas idéal, mais au moins le programme ne reste pas bloqué indéfiniment




// ---------------------------------------------------------------------------
// @brief Initialise la configuration avec des valeurs par défaut
// @param void
// @return void
// ---------------------------------------------------------------------------
// exemple appel: SETUPSetStructDefaultValues();
void SETUPSetStructDefaultValues()
{ char msg[80];
  snprintf(msg,80,"SETUPSetStructDefaultValues() executé: RESET STRUCTURE en EEPROM"); 
  LOG_INFO(msg);   
  
// ---------------------------------------------------------------------------
// ========================= Configuration Applicatif ========================
// ---------------------------------------------------------------------------
  config.applicatif.version = CONFIG_VERSION; 
  
// Paramètres LED
  config.applicatif.redLedDuration = RED_LED_DURATION;
  config.applicatif.greenLedDuration = GREEN_LED_DURATION;
  config.applicatif.blueLedDuration = BLUE_LED_DURATION;
  config.applicatif.builtinLedDuration = BUILTIN_LED_DURATION;
  
// Paramètres Rucher
  config.applicatif.RucherID = 0; 
  strcpy(config.applicatif.RucherName, "Rucher Test       *"); // 19 car max

//debugSerial.println("SETUPinit2483A()"); 
 config.materiel.Num_Carte = SETUPinit2483A(config.materiel.DevEUI);   // return :  0 error or Card number
   
 memcpy(config.materiel.DevEUI, SN2483_List [config.materiel.Num_Carte], 9);
 memcpy(config.applicatif.AppEUI, AppEUI_List, 8);
 memcpy(config.applicatif.AppKey, AppKey_List[config.materiel.Num_Carte], 17);

  config.applicatif.SpreadingFactor = DEFAULT_SF;
  config.applicatif.SendingPeriod = WAKEUP_INTERVAL_PAYLOAD;
  config.applicatif.OLEDRefreshPeriod = INTERVAL_1SEC;
  
// ---------------------------------------------------------------------------
// ========================== Configuration Matériel =========================
// ---------------------------------------------------------------------------
  config.materiel.version = 3;  // PCB2
  config.materiel.adresseRTC = DS3231_ADDRESS;
  config.materiel.adresseOLED = OLED_ADDRESS;
  config.materiel.adresseEEPROM = EEPROM_ADDRESS;
  config.materiel.poidsTare = TARE;              // valeur de la tare en grammes


//Conditionner que pas déjà lu  
  
// DevEUI par défaut initialisé ci dessus
// memcpy(config.materiel.DevEUI, defaultDevEUI, 8);
  
// Détection périphériques
  config.materiel.Rtc = true;
  config.materiel.KBD_Ana = true;
  config.materiel.Oled = 96;
  config.materiel.SDHC = false;
  config.materiel.LiPo = true;
  config.materiel.Solaire = true;
  
// Facteurs d'échelle analogiques
  config.materiel.LDRBrightnessScale = 1.0;
  config.materiel.VSolScale = 1.0;
  config.materiel.VBatScale = 1.0;
  
// Initialisation HX711#0
  config.materiel.Peson_0 = Peson[config.materiel.Num_Carte][0];
  config.materiel.HX711Clk_0 = HX711_SENSOR_SCK;
  config.materiel.HX711Dta_0 = HX711_ASENSOR_DOUT;
// les données sont extraites de la base de données des pesons
  config.materiel.HX711NoloadValue_0 = Jauge[config.materiel.Peson_0][0];
  config.materiel.HX711Scaling_0 = Jauge[config.materiel.Peson_0][1];
  config.materiel.HX711Tare_Temp_0 = Jauge[config.materiel.Peson_0][2];
  config.materiel.HX711Cor_Temp_0 = Jauge[config.materiel.Peson_0][3];
  
// Initialisation HX711#1
  config.materiel.Peson_1 = Peson[config.materiel.Num_Carte][1];
  config.materiel.HX711Clk_1 = HX711_SENSOR_SCK;
  config.materiel.HX711Dta_1 = HX711_BSENSOR_DOUT;
// les données sont extraites de la base de données des pesons
  config.materiel.HX711NoloadValue_1 = Jauge[config.materiel.Peson_1][0];
  config.materiel.HX711Scaling_1 = Jauge[config.materiel.Peson_1][1];
  config.materiel.HX711Tare_Temp_1 = Jauge[config.materiel.Peson_1][2];
  config.materiel.HX711Cor_Temp_1 = Jauge[config.materiel.Peson_1][3];
  
// Initialisation HX711#2
  config.materiel.Peson_2 = Peson[config.materiel.Num_Carte][2];
  config.materiel.HX711Clk_2 = HX711_SENSOR_SCK;
  config.materiel.HX711Dta_2 = HX711_CSENSOR_DOUT;
// les données sont extraites de la base de données des pesons
  config.materiel.HX711NoloadValue_2 = Jauge[config.materiel.Peson_2][0];
  config.materiel.HX711Scaling_2 = Jauge[config.materiel.Peson_2][1];
  config.materiel.HX711Tare_Temp_2 = Jauge[config.materiel.Peson_2][2];
  config.materiel.HX711Cor_Temp_2 = Jauge[config.materiel.Peson_2][3];
  
// Initialisation HX711#3
  config.materiel.Peson_3 = Peson[config.materiel.Num_Carte][3];
  config.materiel.HX711Clk_3 = HX711_SENSOR_SCK;
  config.materiel.HX711Dta_3 = HX711_DSENSOR_DOUT;
// les données sont extraites de la base de données des pesons
  config.materiel.HX711NoloadValue_3 = Jauge[config.materiel.Peson_3][0];
  config.materiel.HX711Scaling_3 = Jauge[config.materiel.Peson_3][1];
  config.materiel.HX711Tare_Temp_3 = Jauge[config.materiel.Peson_3][2];
  config.materiel.HX711Cor_Temp_3 = Jauge[config.materiel.Peson_3][3];

// ---------------------------------------------------------------------------
// ========================== Configuration du reste =========================
// ---------------------------------------------------------------------------
// Initialisation magicNumber
  config.magicNumber = CONFIG_MAGIC_NUMBER;  
// Calcul et stockage du checksum
  config.checksum = EPR_24C32calcChecksum(&config);
// 58 elements dans struct
  LOG_DEBUG(F("Config par defaut initialisee"));
// E24C32DumpConfigToJSON();

}
