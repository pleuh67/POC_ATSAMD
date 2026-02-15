//       1         2         3         4         5         6         7        7
//34567890123456789012345678901234567890123456789012345678901234567890123456789
// IMPRESSION 79 COLONES EN TAILLE 12
//
// ---------------------------------------------------------------------------*
//      __  __                      ______               _
//     |  \/  |                    |  ____|             | |
//     | \  / | ___ _ __  _   _ ___| |__ ___  _ __   ___| |_   ___ _ __  _ __
//     | |\/| |/ _ \ '_ \| | | / __|  __/ _ \| '_ \ / __| __| / __| '_ \| '_ \
//     | |  | |  __/ | | | |_| \__ \ | | (_) | | | | (__| |_ | (__| |_) | |_) |
//     |_|  |_|\___|_| |_|\__,_|___/_|  \___/|_| |_|\___|\__(_)___| .__/| .__/
//                                                                | |   | |
//                                                                |_|   |_|
// ---------------------------------------------------------------------------*
#define __INIT_DONE
#include "define.h"

// info afficher NextPayload
// info afficher MODE PE


// Fonctions lancées par les menus
//
// lancement : menuNNN_FN_WhatToDo()     // ex: menu000_F7_GetTime()
// Traitement: menuNNN_FN_WhatToDoDone() // ex: menu000_F7_GetTimeDone()
//
//
//
// menu000_F6_GetDate()
// menu000_F7_GetTime() + Done
// menu000_F8_GetHex()  + Done
//
// prévoir inits param Lora :
//  uint8_t HWEUI [20];       // ID RN2483: "0004A30B00EEEE01"
//  uint8_t AppEUI [10];      // AppEUI: {0x41, 0x42, 0x45, 0x49, 0x4C, 0x4C, 0x45, 0x31, 0x00} menuxxx_xx_GetHex()
//  uint8_t AppKey [18];      // AppKEY: // 5048494C495050454C4F564542454553 - PHILIPPELOVEBEES
//// {0x50, 0x48, 0x49, 0x4C, 0x49, 0x50, 0x50, 0x45, 0x4C, 0x4F, 0x56, 0x45, 0x42, 0x45, 0x45, 0x53, 0x00}
//  uint8_t SpreadingFactor;  // 7, 9 et 12 echec freudeneck
//  uint8_t SendingPeriod;    // 15 minutes = 500 sans IT
//
// Prévoir inits generaux + Bal
//  Date:  menuxxx_xx_GetDate()
//  Heure: menuxxx_xx_GetTime()
//  uint8_t Balance_ID;           // ID Rucher           xx  uint8_t
//  char    RucherName [20];      // Localisation Rucher (saisir direct ou liste + "autre")
//  version, mail (pour le fun),
//
// Prévoir Config Hardware
//  uint8_t HX711Clk_0;           // HX711#0 parameters
//  uint8_t HX711Dta_0;
//  float   HX711ZeroValue_0;
//  float   HX711Scaling_0;
//  float   HX711Cor_Temp_0;
//  uint8_t HX711Clk_1;           // HX711#1 parameters
//  uint8_t HX711Dta_1;
//  float   HX711ZeroValue_1;
//  float   HX711Scaling_1;
//  float   HX711Cor_Temp_1;
//  uint8_t HX711Clk_2;           // HX711#2 parameters
//  uint8_t HX711Dta_2;
//  float   HX711ZeroValue_2;
//  float   HX711Scaling_2;
//  float   HX711Cor_Temp_2;
//  uint8_t HX711Clk_3;           // HX711#3 parameters
//  uint8_t HX711Dta_3;
//  float   HX711ZeroValue_3;
//  float   HX711Scaling_3;
//  float   HX711Cor_Temp_3;
//  float   LDRBrightnessScale;   //
//  float   VSolScale;            //
//  float   VBatScale;
//
// ---------------------------------------------------------------------------*


// ---------------------------------------------------------------------------*
// ------------------------------- m0_Demarrage ------------------------------
// ---------------------------------------------------------------------------*
// "Page INFOS     (P)"
void m0_0E_PageInfosSyst()
{
  debugSerial.println("CONFIG. SYSTEME - Ecran INFOS demandé");
  InfoScreenRefreshTime = true;
  OLEDdisplayInfoScreenSyst();
}

// trouver ou appeler
void m0_0E_PageInfosSystDone()   // appel dans Handle.cpp en fonction de saisieActive
{
  // voir si pas désactivé de façon globale?????  azerty
  InfoScreenRefreshTime = false;
}

// "CONFIG. SYSTEME(F)"
void m0_1M_ConfigSystem() //
{
  //  infoScreenState = INFO_SCREEN_ACTIVE;  // pour eviter KKKKK met en mode ecran info, continu par appuii touche "VALIDE"
  debugSerial.println("Appel du Menu: CONFIG. SYSTEME");
  pushMenu("-- CONFIG Systeme --", m01_ConfigSystem, M01_ITEM, 0);
  //OLEDDrawText(1, 7, 0, "VALIDE pour retour");
}

//  "CONFIG. LoRa  (F)",
void m0_2M_ConfigLoRa()    //
{
  // infoScreenState = INFO_SCREEN_ACTIVE;  // pour eviter KKKKK met en mode ecran info, continu par appuii touche "VALIDE"
  debugSerial.println("Appel du Menu: CONFIG. LoRa   (M)");
  pushMenu("--- CONFIG LoRa ----", m02_ConfigLoRa, M02_ITEM, 0);
}


//  "CALIB. Volt (M030)",
void m0_3M_CalibTensions()   //
{
  debugSerial.println("Appel d'un sous menu");
  pushMenu("CALIB. TENSIONS:", m03_CalibTensions, M03_ITEM, 0);
}


//  "CALIB. BALANCES(F)",
void m0_4M_CalibBal()   //
{
  // debugSerial.print("Appel d'un sous menu:");
  sprintf(serialbuf, "Appel d'un sous menu: CONFIG. BALANCE %1d", bal);
  debugSerial.println(serialbuf);
  //  pushMenu(serialbuf, m04_CalibBalances, M04_ITEM, 0);
  pushMenu("- CALIB. BALANCE # -", m04_CalibBalances, M04_ITEM, 0);
}

// ---------------------------------------------------------------------------*
// ----------------------------- m01_ConfigSystem ---------------------------
// ---------------------------------------------------------------------------*

void m01_0F_GetDate()
{ DateTime systemTime;

  debugSerial.println("Lancement saisie DATE");
  debugSerial.println("CONFIG. SYSTEME - Demande saisie DATE");
  systemTime = rtc.now();
  sprintf(serialbuf, "%02d:%02d:%02d    %02d/%02d/%02d",
          systemTime.hour(), systemTime.minute(), systemTime.second(),
          systemTime.day(), systemTime.month(), systemTime.year() - 2000);
  debugSerial.println(serialbuf);
  sprintf(serialbuf, "%02d/%02d/%04d", systemTime.day(), systemTime.month(), systemTime.year());
  startDateInput("-- SAISIR LA DATE --", serialbuf);
}


void m01_0F_GetDateDone()   // appel dans Handle.cpp en fonction de saisieActive
{ DateTime systemTime;
  byte jour, mois, annee;
  static char dateBuffer[11] = "11/11/2025"; // Buffer pour la date 11/11/1965

  finalizeDateInput(dateBuffer); // Récupérer l'heure finale en jj/mm/aaaa
  debugSerial.print("Nouvelle Date: ");
  debugSerial.println(dateBuffer);        // Ici vous pouvez traiter la date et revenir au menu
  // jj/mm/aaaa
  jour = ((byte)dateBuffer[0] - 48) * 10;
  jour += (byte)dateBuffer[1] - 48;
  mois = ((byte)dateBuffer[3] - 48) * 10;
  mois += (byte)dateBuffer[4] - 48;
  annee = ((byte)dateBuffer[8] - 48) * 10;
  annee += (byte)dateBuffer[9] - 48;

  // Lecture avant modif pour init hh:mm:ss
  systemTime = rtc.now();
  // mise à date yy/mm/dd du DS3231
  rtc.adjust(DateTime(annee, mois, jour, systemTime.hour(), systemTime.minute(), systemTime.second()));
  // mise à date modifiée du µc
  systemTime = rtc.now();

  /*
    rtc.adjust(DateTime(systemTime.year()-2000, systemTime.month(), systemTime.day(), hour, minute, second));


    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  */


  sprintf(serialbuf, "mise à la date DS3231 %d/%d/%d", annee, mois, jour);
  debugSerial.println(serialbuf);
  DS3231copyTimeToMicro(1);
  DS3231synchronizeTimeToMicro();

//  debugSerial.println("Reprogramme IRQ2");
//  DS3231setRTCAlarm2(); // Reprogrammer prochaine alarme dans n min
  // Activer la liste de démarrage quand fin saisie Date : void finalizeDateInput(char* outputDate)

  backMenu();
}

void m01_1F_GetTime()
{ DateTime systemTime;

  debugSerial.println("Lancement saisie TIME");
  debugSerial.println("CONFIG. SYSTEME - Demande saisie TIME");
  systemTime = rtc.now();
  sprintf(serialbuf, "%02d:%02d:%02d    %02d/%02d/%02d",
          systemTime.hour(), systemTime.minute(), systemTime.second(),
          systemTime.day(), systemTime.month(), systemTime.year() - 2000);
  debugSerial.println(serialbuf);
  sprintf(serialbuf, "%02d:%02d:%02d", systemTime.hour(), systemTime.minute(), systemTime.second());
  startTimeInput("-- SAISIR L HEURE --", serialbuf);
}

void m01_1F_GetTimeDone()   // appel dans Handle.cpp en fonction de saisieActive
{ DateTime systemTime;
  byte hour, minute, second;
  static char timeBuffer[9] = ""; // Buffer pour l'heure

  finalizeTimeInput(timeBuffer); // Récupérer l'heure finale
  debugSerial.print("Nouvelle heure: ");
  debugSerial.println(timeBuffer);        // Ici vous pouvez traiter l'heure et revenir au menu
  // hh:mm:ss
  hour = ((byte)timeBuffer[0] - 48) * 10;
  hour += (byte)timeBuffer[1] - 48;
  minute = ((byte)timeBuffer[3] - 48) * 10;
  minute += (byte)timeBuffer[4] - 48;
  second = ((byte)timeBuffer[6] - 48) * 10;
  second += (byte)timeBuffer[7] - 48;


  // Lecture avant modif pour init hh:mm:ss
  systemTime = rtc.now();
  // mise à l'heure hh:mm:ss du DS3231
  rtc.adjust(DateTime(systemTime.year() - 2000, systemTime.month(), systemTime.day(), hour, minute, second));
  // mise à date modifiée du µc
  systemTime = rtc.now();

  debugSerial.println("mise à l'heure DS3231");
  DS3231copyTimeToMicro(1);
  DS3231synchronizeTimeToMicro();


//debugSerial.println("Reprogramme IRQ2");
  DS3231setRTCAlarm2(); // Reprogrammer prochaine alarme dans n min
  // Activer la liste de démarrage quand fin saisie TIME : void finalizeTimeInput(char* outputTime)
  backMenu();
}

// Saisie de config.materiel.Noeud_LoRa
void m01_2F_GetNumRucher()
{ static char number[4];

  saisieActive = 12; // pour identifier variable saisie lors de l'affectation

  debugSerial.print("Appel d'une Fonction: m01_2F_GetNumRucher()");
  sprintf(number, "%d", config.applicatif.RucherID);
  debugSerial.println(config.applicatif.RucherID);
  debugSerial.println(number);
  startNumInput("-NUM. SYSTEME LoRa-", number, 4, false, false, 0, 10);
}

// Sauvegarde de config.materiel.Noeud_LoRa
void m01_2F_GetNumRucherDone()   // appel dans Handle.cpp en fonction de saisieActive
{ static char number[9] = ""; // Buffer pour le numérique

  finalizeNumInput(number); // Récupérer saisie finale
  config.applicatif.RucherID = atoi(number);
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
char msg[80];
  snprintf(msg,80,"Num. de rucher mémorisé: %d",config.applicatif.RucherID);  
  LOG_INFO(msg);
//  LOG_WARNING(msg)
//  LOG_ERROR(msg)
//  LOG_DEBUG(msg); 
//  OLEDDrawText(1,2, 0, msg );
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  E24C32saveConfig();
  backMenu();
}


// Saisie de config.applicatif.RucherName
void m01_3F_GetNameRucher()  // Saisir dans la liste List_Ruchers[], 12 elements
{ static char currentString[21];
  debugSerial.println("Appel d'une Fonction: m01_4F_GetNameRucher()");

  sprintf(currentString, "%s", config.applicatif.RucherName);
  debugSerial.print("Appel avec: "); debugSerial.println(currentString);
  //sprintf(currentString,Data_LoRa.RucherName); // pointeur sur valeur courante traiter
  startStringInput("---- NOM RUCHER ----", currentString, 20);
}

// Sauvegarde de config.applicatif.RucherName
void m01_3F_GetNameRucherDone()   // appel dans Handle.cpp en fonction de saisieActive
{ static char currentString[21];

  finalizeStringInput(currentString); // Récupérer saisie finale
//  debugSerial.print("Nouveau nom: ");
//  debugSerial.println(currentString);
  sprintf(config.applicatif.RucherName, "%s", currentString);
// debugSerial.println(config.applicatif.RucherName);
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
char msg[80];
  snprintf(msg,80,"Nom de rucher mémorisé: %s",config.applicatif.RucherName);  
  LOG_INFO(msg);
//  LOG_WARNING(msg)
//  LOG_ERROR(msg)
//  LOG_DEBUG(msg); 
//  OLEDDrawText(1,2, 0, msg );
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  E24C32saveConfig();
  backMenu();
}

void m01_4F_readConfig()
{
  debugSerial.print("Appel d'une Fonction: ");
  debugSerial.println("m01_5F_readConfig()");

  //
E24C32readConfig();
E24C32DumpConfigToJSON();
 
  // Si pas fonction_Done();
  // Activer la liste de démarrage quand fin saisie
  backMenu();
}

void non_m01_4F_readConfigDone()   // appel dans Handle.cpp en fonction de saisieActive
{
  debugSerial.println("Appel d'une Fonction: m01_5F_readConfigDone() à developper");
  backMenu(); // supprimer dans pas Done
}

void m01_5F_writeConfig()
{
  debugSerial.println("Appel d'une Fonction: m01_5F_writeConfig()");

E24C32saveConfig();
E24C32DumpConfigToJSON();

  // Si pas fonction_Done();
  // Activer la liste de démarrage quand fin saisie
  backMenu();
}

void non_m01_5F_writeConfigDone()   // appel dans Handle.cpp en fonction de saisieActive
{
  debugSerial.println("Appel d'une Fonction: m01_5F_writeConfigDone()");
  backMenu(); // supprimer dans pas Done
}

// ---------------------------------------------------------------------------*
// ------------------------------ m02_ConfigLoRa -----------------------------
// ---------------------------------------------------------------------------*
/*
  Data_LoRa.
  uint8_t rucher_ID;  // 0:non affecté, 1: Fleurs, 2: Acacia, 3: Tilleul, 4: Chataignier
                      // 5: Sapin 6: Jachère 7: Forêt  8: Phacélie 9: Pommes
  char    RucherName [20];      // Localisation Rucher (saisir direct ou liste
  float   DHT_Temp;         // Temp DHT en °C    xx,x  Float
  float   DHT_Hum;          // Hum DHT en %      xx,x  Float
  float   Brightness;       // %Lum en LUX      xxxxx  uint16_t
  float   Bat_Voltage;      // Tension BAT en V     xx,xx  Float (uint16_t)
  float   Solar_Voltage;    // Tension BAT en V     xx,xx  Float (uint16_t)
  float   HX711Weight[4];    // masse Ruche 1 en kg xxx,xx (précision affich. 10g)
  float   ProcessorTemp;    // temp µC, conservé en backup DHT22
  } LoRa_Var;   //
*/

// "Page INFOS     (P)"
void m02_0E_PageInfosLoRa()
{
 saisieActive = 20; // pour désactivation Refresh en sortie
  
  debugSerial.println("CONFIG. LoRa - Ecran INFOS demandé");
  OLEDdisplayInfoScreenLoRa();
  LoRaScreenRefreshTime = true;   // active rafraichissement date/heure
  LoraScreenRefreshNextPayload = true;  // active rafraichissement heure payload
}


void m02_0E_PageInfosLoRaDone()    // appel dans Handle.cpp en fonction de saisieActive
{
  LoRaScreenRefreshTime = false;
  LoraScreenRefreshNextPayload = false;
}

// AppKey => {0x50,0x48,0x49,0x4C,0x49,0x50,0x50,0x45,0x4C,0x4F,0x56,0x45,0x42,0x45,0x45,0x53,0x00}
// 5048494C495050454C4F564542454553 - PHILIPPELOVEBEES
// AppKey = (uint8_t *)AppKey_List[Ruche.Num_Carte];
// appel de Menus.cpp
void m02_1F_AppKEY()  // AppKEY
{ static char hexBuffer[33]; // Buffer pour l'hexa

  saisieActive = 21;
  debugSerial.println("CONFIG. SYSTEME - Demande saisie HEXA AppKEY");
  byteArrayToHexString(&AppKey_List [config.materiel.Num_Carte][0], 16, hexBuffer, 33);
  startHexInput("-- SAISIR APPKEY ---", hexBuffer, 32);
}


void m02_1F_AppKEYDone()    // appel dans Handle.cpp en fonction de saisieActive
{ static char hexBuffer[33] = ""; // Buffer pour l'hexa

  finalizeHexInput(hexBuffer); // Récupérer chaine HEXA
  debugSerial.print("Nouvelle chaine AppKEY: ");
  debugSerial.println(hexBuffer);        // Ici vous pouvez traiter AppKEY et revenir au menu
  hexStringToByteArray(hexBuffer, &AppKey_List [config.materiel.Num_Carte][0], strlen(hexBuffer));
  backMenu();
}

// AppEUI => {0x41, 0x42, 0x45, 0x49, 0x4C, 0x4C, 0x45, 0x35, 0x00} = ABEILLE5
// appel de Menus.cpp
void m02_2F_AppEUI() // AppEUI
{ static char hexBuffer[10]; // Buffer pour l'hexa

  saisieActive = 22;
  debugSerial.println("CONFIG. SYSTEME - Demande saisie HEXA AppEUI");
  byteArrayToHexString(&AppEUI_List[0], 8, hexBuffer, 17);                                      // &[0]  ???
  startHexInput("-- SAISIR APPEUI ---", hexBuffer, 16);
}


void m02_2F_AppEUIDone()    // appel dans Handle.cpp en fonction de saisieActive
{ static char hexBuffer[17] = ""; // Buffer pour l'hexa

  finalizeHexInput(hexBuffer); // Récupérer chaine HEXA
  debugSerial.print("Nouvelle chaine AppEUI: ");
  debugSerial.println(hexBuffer);        // Ici vous pouvez traiter AppEUI et revenir au menu
  hexStringToByteArray(hexBuffer, &AppEUI_List[0], strlen(hexBuffer));         // &[0]  ???
// paramètres LoRa
//  uint8_t AppEUI [9];        // AppEUI: {0x41, 0x42, 0x45, 0x49, 0x4C, 0x4C, 0x45, 0x31}
//  uint8_t AppKey [17];        // AppKEY: // 5048494C495050454C4F564542454553 - PHILIPPELOVEBEES

hexStringToByteArray(hexBuffer, config.applicatif.AppEUI, 9);
  
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
char msg[80];
  snprintf(msg,80,"AppEUI mémorisé: %s",hexBuffer);
  LOG_INFO(msg);
//  LOG_WARNING(msg);
//  LOG_ERROR(msg);
//  OLEDDrawText(1,2, 0, msg );
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  E24C32saveConfig();
  backMenu();
}

// appel de Menus.cpp
void m02_3L_GetSF() // Spread Factor, saisir dans la liste List_SF[], 3 elements
{
  saisieActive = 23; //
  debugSerial.println("Appel m02_3L_GetSF");
  int i;
  i = config.applicatif.SpreadingFactor; //

  i = i == 7 ? 0 : i == 9 ? 1 : 2 ;

  static char hexBuffer[17] = ""; // Buffer pour l'hexa
  sprintf(hexBuffer, "SF: %d  i: %d", config.applicatif.SpreadingFactor, i);
  debugSerial.println(hexBuffer);

  startListInput("---- SELECT SF -----", List_SF, 3, i, TIMEOUT_SAISIE);
}

void m02_3L_GetSFDone()    // appel dans Handle.cpp en fonction de saisieActive
{ static char selectedSF[21] = "";
  static char localBuffer[40] = "";

  debugSerial.println("Appel m02_3L_GetSFDone");
  uint8_t index = finalizeListInput(selectedSF);

  debugSerial.print("SF selectionne: ");
  debugSerial.println(selectedSF);

  // Extraire la valeur numérique
  if (strcmp(selectedSF, "SF7") == 0)
    config.applicatif.SpreadingFactor = 7;
  else if (strcmp(selectedSF, "SF9") == 0)
    config.applicatif.SpreadingFactor = 9;
  else if (strcmp(selectedSF, "SF12") == 0)
    config.applicatif.SpreadingFactor = 12;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
char msg[80];
  snprintf(msg,80,"SpreadingFactor mémorisé: %d",config.applicatif.SpreadingFactor);
  LOG_INFO(msg);
//  LOG_WARNING(msg)
//  LOG_ERROR(msg)
//  OLEDDrawText(1,2, 0, msg );
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  E24C32saveConfig();
  backMenuFromList(); // Fin de gestion hors Timeout
}

void m02_4F_GetPayloadDelay() // Delais Payload
{ static char number[4] = "010";
  int i;

  debugSerial.println("Appel m02_4F_GetPayloadDelay - a implementer");
  saisieActive = 24; // pas de cas d'usage


  i = config.applicatif.SendingPeriod;
  char localSerialbuf[81];
  sprintf(localSerialbuf, "SendingPeriod: %d  i: %d number : ", config.applicatif.SendingPeriod, i);
  debugSerial.print(localSerialbuf);

  sprintf(localSerialbuf, "%04d", config.applicatif.SendingPeriod);
  debugSerial.println(localSerialbuf);
  //config.applicatif.SendingPeriod = atoi(number);
  startNumInput("-Sending Period LoRa", localSerialbuf, 4, false, false, 0, 360); // 0 à 6h
}

void m02_4F_GetPayloadDelayDone()                          // appel de Handle.cpp
{ static char numBuffer[21] = ""; // Buffer pour le Num
  static char strNumber[5] = "";    // 0 à 360
  static char localBuffer[40] = "";

  debugSerial.println("Appel m02_4F_GetPayloadDelayDone - a implementer");
  finalizeNumInput(strNumber);  // récupérer Chaine TXT

  debugSerial.print("PayloadDelay selectionne: ");
  debugSerial.println(strNumber);
  config.applicatif.SendingPeriod = atoi(strNumber);
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
char msg[80];
  snprintf(msg,80,"PayloadDelay mémorisé: %d",config.applicatif.SendingPeriod);  
  LOG_INFO(msg);
//  LOG_WARNING(msg)
//  LOG_ERROR(msg)
//  OLEDDrawText(1,2, 0, msg );
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  E24C32saveConfig();
  DS3231setRTCAlarm2();
  backMenu();
}


void m02_5F_Join() // Connexion LoRa
{
  debugSerial.println("Appel m02_5F_Join - a tester");
  GestionEnCours("m02_5Fa");

  SETUPinit2483A(config.materiel.DevEUI);  // Réinitialise DevEUI, AppEUI et AppKey
  
  debugSerial.println("Fin SETUPinit2483A");
  GestionEnCours("m02_5Fb");
  if (SETUPinitLoRa())   // Reconnecte et envoie Payload + affiche résultat
  {
debugSerial.println("Init LoRa done.");
debugSerial.println("Test sending LoRa testPayload (7) (Restart)..."); 
    RN2483AsendLoRaPayload((uint8_t*)testPayload,7);
    OLEDDebugDisplay("LoRa    Initialized");
  }
  else
  {
    OLEDDebugDisplay("LoRa Failed");  
  }
  
  GestionEnCours("m02_5Fc");
  debugSerial.println("Appel m02_5F_Join - FIN");


  // Relance la navigation menu execution
  backMenu();
}


void m02_5F_JoinDone()     // appel dans Handle.cpp en fonction de saisieActive
{
  // pas besoin ???
}


void m02_6F_SendPayLoad() // Envoyer Payload
{
  alarm1_enabled = false;  // Bloquer alarme 1
  alarm2_enabled = false;  // Bloquer alarme 2
  debugSerial.println("Appel m02_6F_SendPayLoad - a tester");
  turnOnRedLED();     // PCB donne GREEN?
  MESUREStake_All_Measure();
  buildLoraPayload();
#ifdef __SendLoRaInProgrammationMode
  RN2483AsendLoRaPayload((uint8_t*)payload, 19);  // hex
#endif
  turnOffRedLED();
  debugSerial.println("Fin Payload, Reactive IRQ1");
  alarm1_enabled = true;   // Réactiver alarme 1
  alarm2_enabled = true;   // Réactiver alarme 2

  backMenu(); // Relance la navigation menu après execution
}


//  "RET  popMenu(M000)"       // 5: Retour menu principal
void m02_7M_PopMenu()  // retour menu000Demarrage
{
  debugSerial.print("Appel d'une Fonction: ");
  debugSerial.println("PopMenu()");
  popMenu();
}


// ---------------------------------------------------------------------------*
// ----------------------------- m03_CalibTensions --------------------
// ---------------------------------------------------------------------------*

void m03_0E_DisplayVoltageInput()
{ char localOLEDbuf[21] = "12345678901234567890";

  infoScreenState = INFO_SCREEN_ACTIVE;  // pour eviter KKKKK met en mode ecran info, continu par appuii touche "VALIDE"
  debugSerial.print("Appel d'une Fonction: ");
  debugSerial.println("CalibVBat - Fonction a implementer");

  // Passer en mode lecure ANA avec rafraichissement rapide (10 lect /appel)
  // Activer rafraichissement VBat, VSol, VLum


  // creer sous menu
  OLEDDrawText(1, 1, 0, "Tension Lues");
  sprintf(localOLEDbuf, "Bat : %3.2f / %7.6f", VBat[11], VBatScale_List [bal]);    // 16
  OLEDDrawText(1, 3, 0, localOLEDbuf);          // Affichage à chaque seconde, sans effet si valid

  sprintf(localOLEDbuf, "Sol : %3.2f / %7.6f", VSol[11], VSolScale_List [bal]);    // 16
  OLEDDrawText(1, 4, 0, localOLEDbuf);          // Affichage à chaque seconde, sans effet si valid

  sprintf(localOLEDbuf, "Lum : %3.2f / %7.6f", readingL, VLumScale_List [bal]);    // 16
  OLEDDrawText(1, 5, 0, localOLEDbuf);          // Affichage à chaque seconde, sans effet si valid

  OLEDDrawText(1, 7, 0, "VALIDE pour retour");  // fin
}


// "Calib. VBAT    (F)",      // 0: Mise à Echelle VBat
//  float  Ruche.VBatScale;
void m03_0F_CalibVBat() //
{ static char number[9];

  saisieActive = 30; // pour identifier variable saisie lors de l'affectation
  debugSerial.print("Appel d'une Fonction: m03_0F_CalibVBat()");
  sprintf(number, "%f", config.materiel.VBatScale);

  debugSerial.println(config.materiel.VBatScale); //11:18:22.650 -> Appel d'une Fonction: m03_2F_CalibVLum()1.13
  debugSerial.println(number);  //  11:18:22.650 -> -536870912

  sprintf(number, "%1.6f", config.materiel.VBatScale);
  startNumInput("--- CALIBRER VBat --", number, 8, false, true, 0, 1000); // allow dec
  // texte hors startNumInput() affiché qu'une fois, à rafraichir dans loop()
  // ligne 0: Title
  OLEDDrawText(1, 1, 0, "Mise a l'echelle:   ");
  // ligne 2: Num en cours de modification
  // ligne 3: Gestion du curseur
  OLEDDrawText(1, 4, 0, "VBat :  ");            // Affichage à chaque seconde, sans effet si valid
  OLEDDrawText(1, 5, 0, "Bat???:  ");         // saisie Num nouvelle val de VLumScale => menu041_F0_GetNum()
  // ligne 6:
  // ligne 7: OLEDDrawText(1, 7, 0, "+/- Char VALIDE: OK"); ou timeout // Instructions fixes
  InfoVBatScreenRefresh = true;             // active rafraichissement des mesures de Vlum
  InfoVBatScreenRefreshTime = true;
}


// Sauvegarde de config.materiel.Noeud_LoRa
void m03_0F_CalibVBatDone()                        // appel de Handle.cpp
{ static char number[9] = ""; // Buffer pour le numérique

  InfoVBatScreenRefresh = false;           // desactive rafraichissement de Vlum
  InfoVBatScreenRefreshTime = false;
  debugSerial.print("Appel d'une Fonction: ");
  debugSerial.println("m03_0F_CalibVBatDone()");
  finalizeNumInput(number); // Récupérer saisie finale
  debugSerial.print("Nouveau nombre: ");
  debugSerial.println(number);
  config.materiel.VBatScale = atof(number);   // atof????
  debugSerial.print("Nombre memorisé: ");
  debugSerial.println(config.materiel.VBatScale);
  backMenu();
}



//  "Calib. VSOL    (F)",       // 1: Mise à Echelle VSol
//  float   Ruche.VSolScale;
void m03_1F_CalibVSol()  //
{ static char number[9];

  saisieActive = 31; // pour identifier variable saisie lors de l'affectation

  debugSerial.print("Appel d'une Fonction: m03_0F_CalibVSol()");
  sprintf(number, "%f", config.materiel.VSolScale);

  debugSerial.println(config.materiel.VSolScale); //11:18:22.650 -> Appel d'une Fonction: m03_2F_CalibVLum()1.13
  debugSerial.println(number);  //  11:18:22.650 -> -536870912

  sprintf(number, "%1.6f", config.materiel.VSolScale);
  startNumInput("--- CALIBRER VSol --", number, 8, false, true, 0, 1000); // allow dec
  // texte hors startNumInput() affiché qu'une fois, à rafraichir dans loop()
  // ligne 0: Title
  OLEDDrawText(1, 1, 0, "Mise a l'echelle:   ");
  // ligne 2: Num en cours de modification
  // ligne 3: Gestion du curseur
  OLEDDrawText(1, 4, 0, "VSol :  ");            // Affichage à chaque seconde, sans effet si valid
  OLEDDrawText(1, 5, 0, "Sol???:  ");         // saisie Num nouvelle val de VLumScale => menu041_F0_GetNum()
  // ligne 6:
  // ligne 7: OLEDDrawText(1, 7, 0, "+/- Char VALIDE: OK"); ou timeout // Instructions fixes
  InfoVSolScreenRefresh = true;             // active rafraichissement des mesures de Vlum
  InfoVSolScreenRefreshTime = true;
}


// Sauvegarde de config.materiel.Noeud_LoRa
void m03_1F_CalibVSolDone()                        // appel de Handle.cpp
{ static char number[9] = ""; // Buffer pour le numérique

  InfoVSolScreenRefresh = false;           // desactive rafraichissement de Vlum
  InfoVSolScreenRefreshTime = false;
  debugSerial.print("Appel d'une Fonction: ");
  debugSerial.println("m03_1F_CalibVSolDone()");
  finalizeNumInput(number); // Récupérer saisie finale
  debugSerial.print("Nouveau nombre: ");
  debugSerial.println(number);
  config.materiel.VSolScale = atof(number);   // atof????
  debugSerial.print("Nombre memorisé: ");
  debugSerial.println(config.materiel.VSolScale);
  backMenu();
}

//  "Calib. LUM     (F)",       // 2: Mise à Echelle VLum
//  float   Ruche.LDRBrightnessScale;
void m03_2F_CalibVLum()   //
{ static char number[9];

  saisieActive = 32; // pour identifier variable saisie lors de l'affectation

  debugSerial.print("Appel d'une Fonction: m03_2F_CalibVLum()");
  sprintf(number, "%f", config.materiel.LDRBrightnessScale);

  debugSerial.println(config.materiel.LDRBrightnessScale); //11:18:22.650 -> Appel d'une Fonction: m03_2F_CalibVLum()1.13
  debugSerial.println(number);  //  11:18:22.650 -> -536870912

  sprintf(number, "%1.6f", config.materiel.LDRBrightnessScale);
  startNumInput("--- CALIBRER LDR ---", number, 8, false, true, 0, 1000); // allow dec
  // texte hors startNumInput() affiché qu'une fois, à rafraichir dans loop()
  // ligne 0: Title
  OLEDDrawText(1, 1, 0, "Mise a l'echelle:   ");
  // ligne 2: Num en cours de modification
  // ligne 3: Gestion du curseur
  OLEDDrawText(1, 4, 0, "VLum :  ");            // Affichage à chaque seconde, sans effet si valid
  OLEDDrawText(1, 5, 0, "Lum  :  ");         // saisie Num nouvelle val de VLumScale => menu041_F0_GetNum()
  // ligne 6:
  // ligne 7: OLEDDrawText(1, 7, 0, "+/- Char VALIDE: OK"); ou timeout // Instructions fixes
  InfoVLumScreenRefresh = true;             // active rafraichissement des mesures de Vlum
InfoVLumScreenRefreshTime = true; 
}

// Sauvegarde de config.materiel.Noeud_LoRa
void m03_2F_CalibVLumDone()                        // appel de Handle.cpp
{ static char number[9] = ""; // Buffer pour le numérique

  InfoVLumScreenRefresh = false;           // desactive rafraichissement de Vlum
  InfoVLumScreenRefreshTime = false; 
  debugSerial.print("Appel d'une Fonction: ");
  debugSerial.println("m03_2F_CalibVLumDone()");
  finalizeNumInput(number); // Récupérer saisie finale
  debugSerial.print("Nouveau nombre: ");
  debugSerial.println(number);
  config.materiel.LDRBrightnessScale = atof(number);   // atof????
  debugSerial.print("Nombre memorisé: ");
  debugSerial.println(config.materiel.LDRBrightnessScale);
  backMenu();
}



//  "Reserve     (M043)",       // 3: Libre
void m03_3F_Reserve()   //
{
  infoScreenState = INFO_SCREEN_ACTIVE;  // pour eviter KKKKK met en mode ecran info, continu par appuii touche "VALIDE"
  debugSerial.print("Appel d'un Menu: ");
  debugSerial.println("Menu043 - a implementer");

  OLEDDrawText(1, 7, 0, "VALIDE pour retour");
}

//  "RET  popMenu(M000)"       // 4: Retour menu principal
void m03_4M_PopMenu()  // retour menu000Demarrage
{
  debugSerial.print("Appel d'une Fonction: ");
  debugSerial.println("PopMenu()");
  popMenu();
}


// ---------------------------------------------------------------------------*
// ----------------------------- menu04_CalibBalances --------------------
// ---------------------------------------------------------------------------*

// Affichage rafraichi du poids des balances
//  "info.  Balances(F)",      //   Page info Balances
void m04_0F_InfoBal(void)    //
{
  saisieActive=40;                 // appel desactivation screenRefresh
  InfoBalScreenRefreshTime = true; // active rafraichissement time/date
  if (Peson[config.materiel.Num_Carte][0]) 
    InfoBalScreenRefreshBal_1 = true; // Active rafraichissement Balance 1 
  if (Peson[config.materiel.Num_Carte][1]) 
    InfoBalScreenRefreshBal_2 = true; // Active rafraichissement Balance 2
  if (Peson[config.materiel.Num_Carte][2]) 
    InfoBalScreenRefreshBal_3 = true; // Active rafraichissement Balance 3
  if (Peson[config.materiel.Num_Carte][3]) 
    InfoBalScreenRefreshBal_4 = true; // Active rafraichissement Balance 4

  OLEDdisplayInfoBal();
  OLEDDrawText(1, 7, 0, "VALIDE pour retour");
  //  backMenu(); // Relance la navigation menu après saisie  ou timeout
}


// Appelé dans Handle.cpp en fonction de saisieActive
// Désactive rafraichissement dynamique des poids
void m04_0F_InfoBalDone()     // 
{
  InfoBalScreenRefreshTime = false; // desactive rafraichissement time/date
  InfoBalScreenRefreshBal_1 = false; // desactive rafraichissement Balance 1
  InfoBalScreenRefreshBal_2 = false; // desactive rafraichissement Balance 2
  InfoBalScreenRefreshBal_3 = false; // desactive rafraichissement Balance 3
  InfoBalScreenRefreshBal_4 = false; // desactive rafraichissement Balance 4
  backMenu(); // Relance la navigation menu après saisie  ou timeout
}

void m04_1F_PoidsTare()
{ static char number[6]; // 0..99999

  saisieActive = 41; // pour identifier variable saisie lors de l'affectation

  debugSerial.print("Appel d'une Fonction: m04_1F_PoidsTare()");
  sprintf(number, "%d", config.materiel.poidsTare);
  debugSerial.println(config.materiel.poidsTare);
  debugSerial.println(number);
  startNumInput("- MASSE DE LA TARE -", number, 6, false, false, 0, 65535);
}

void m04_1F_PoidsTareDone()
{ static char number[9] = ""; // Buffer pour le numérique

  debugSerial.print("Appel d'une Fonction: ");
  debugSerial.println("m04_1F_PoidsTareDone()");
  finalizeNumInput(number); // Récupérer saisie finale
  debugSerial.print("Nouveau nombre: ");
  debugSerial.println(number);
  config.materiel.poidsTare = atoi(number);
  debugSerial.print("Nombre memorisé: ");
  debugSerial.println(config.materiel.poidsTare);
// sauvegarde en EEPROM
char msg[80];
  snprintf(msg, 21, "poidsTare:%d",config.materiel.poidsTare);
  LOG_INFO(msg);
  OLEDDrawText(1,2, 0, msg );
  E24C32saveConfig();
  backMenu();  
}


void m04_nM_CalibBal_bal()    // utiliser la var gloale bal
{

 //   saisieActive = 4n; // pour identifier variable saisie lors de l'affectation
  LOG_DEBUG("Appel m04_nM_CalibBal_bal - a implementer");
  //  OLEDDrawText(1, 7, 0, "VALIDE pour retour");
  OLEDDrawText(1, 7, 0, "m04_nM_CalibBal_bal");
  /*
    ScreenRefreshed = true;         // active rafraichissement en général
    BalScreenRefreshTime = true;   // active rafraichissement date/heure
    BalScreenRefreshWeights = true;  // active rafraichissement poids
    OLEDdisplayInfoBal();     // part dans KKKKKKKKK
  */
  // fonction implementer



  
  OLEDdisplayCalibBal();
  InfoScreenRefreshTime = false;
  //  backMenu(); // Relance la navigation menu après saisie  ou timeout
}

void m04_2M_CalibBal(int numJauge)
{
/*
menu qui appelle :
  "Tare Balance    (F)",    // m04x_1F_tareBal(int numJauge); enregistre la tare et temperature
  "Echelle Balance (F)",    // m04x_2F_echBal(int numJauge); calcule la mise à l'echelle
  "Saisie Comp. Temp. ",    // m04x_3F_adjustTempBal(int numJauge); saisie d'un coefficient de correction

m04_2F_CalibBal_1();   devient tare/echelle?  

  */
}




void m04_2F_CalibBal_1()  //
{
  LOG_DEBUG("Appel m04_nM_CalibBal_1 - a implementer");
  saisieActive = 42; // pour identifier variable saisie lors de l'affectation

  InfoScreenRefreshTime = true;
  OLEDdisplayInfoBal();     // part dans KKKKKKKKK

  // fonction implementer

    if (Peson[config.materiel.Num_Carte][0])//  Poids_Peson(3) = GetPoids(4, 1);
    {
      Contrainte_List[0]= MESURESHX711GetStrainGauge(0,scaleA,AVR_10);  // renvoi la moyenne des 10 dernières mesures 
      HiveSensor_Data.HX711Weight[0] =  calculePoids(0); // kg     
    }

config.materiel.HX711NoloadValue_0 = Contrainte_List[0];

  
/*
  uint8_t Peson_0;              //  N° Peson
  uint8_t HX711Clk_0;           
  uint8_t HX711Dta_0;
  float   HX711NoloadValue_0;   // valeur sans charge (relevée par opérateur )
  float   HX711Tare_Temp_0;     // Température lors de la tare (mesurée lors de la tare)  
  float   HX711Scaling_0;       // Facteur de mise à l'echelle
  float   HX711Cor_Temp_0;      // Facteur de compensation en température
*/
    
char msg[80];
  snprintf(msg, 21, "tareJaugeA:%8f",Contrainte_List[0]);
  
  LOG_INFO(msg);
//  LOG_WARNING(msg)
//  LOG_ERROR(msg)
  OLEDDrawText(1,2, 0, msg );

char rc = read_DHT(dht); 
config.materiel.HX711Tare_Temp_0 = rc ? 99 : HiveSensor_Data.DHT_Temp;
E24C32saveConfig();

    InfoScreenRefreshTime = false;
  //  backMenu(); // Relance la navigation menu après saisie  ou timeout
}


//  "Calib. Bal #2  (F)",      // 1: Calibration Balance 2
void m04_1F_CalibBal_2()  //
{
  debugSerial.print("Appel d'une Fonction: ");
  debugSerial.println("m04_1F_CalibBal_2 - a implementer");
  
  
  OLEDDrawText(1, 7, 0, "VALIDE pour retour");

  backMenu(); // Relance la navigation menu après saisie  ou timeout


}


//  "Calib. Bal #3  (F)",      // 2:  Calibration Balance 3
void m04_2F_CalibBal_3()  //
{
  debugSerial.print("Appel d'une Fonction: ");
  debugSerial.println("m04_2F_CalibBal_3 - a implementer");

  OLEDDrawText(1, 7, 0, "VALIDE pour retour");

  backMenu(); // Relance la navigation menu après saisie  ou timeout
}


//  "Calib. Bal #4  (F)",      // 3:  Calibration Balance 4
void m04_3F_CalibBal_4()  //
{
  debugSerial.print("Appel d'une Fonction: ");
  debugSerial.println("m04_3F_CalibBal_4 - a implementer");

  OLEDDrawText(1, 7, 0, "VALIDE pour retour");
  backMenu(); // Relance la navigation menu après saisie  ou timeout
}


//  "RET  popMenu(M000)"       // 4: Retour menu principal
void m04_6M_PopMenu()  // retour menu000Demarrage
{
  debugSerial.print("Appel d'une Fonction: ");
  debugSerial.println("PopMenu()");
  popMenu();
}


// ---------------------------------------------------------------------------*
// --------------------------------- menu04x_CalibBal ------------------------
// ---------------------------------------------------------------------------*
//  "Calib. Bal #1  (F)",      // 0: Calibration Balance 1
// HX711#0 parameters
//  uint8_t Ruche.HX711Clk_0;
//  uint8_t Ruche.HX711Dta_0;
//  float   Ruche.HX711ZeroValue_0;
//  float   Ruche.HX711Scaling_0;
//  float   Ruche.HX711Cor_Temp_0;

// HX711#0 parameters    
  uint8_t Peson_0;              //  N° Peson
  uint8_t HX711Clk_0;           
  uint8_t HX711Dta_0;
  float   HX711NoloadValue_0;   // valeur sans charge (relevée par opérateur )
  float   HX711Tare_Temp_0;     // Température lors de la tare (mesurée lors de la tare)  
  float   HX711Scaling_0;       // Facteur de mise à l'echelle
  float   HX711Cor_Temp_0;      // Facteur de compensation en température


// ---------------------------------------------------------------------------*
// @brief 
// @param Aucun
// @return void
// @description 
// ---------------------------------------------------------------------------*
void m04x_0F_numBalx(void)
{ static char number[3]; // Buffer pour le numérique 0..99
 
  saisieActive = 490; // pour identifier variable saisie lors de l'affectation
  debugSerial.print("Appel d'une Fonction: m04x_0F_numBalx()");
  sprintf(number, "%d", config.materiel.Peson_0);
  debugSerial.println(config.materiel.Peson_0);
  debugSerial.println(number);
  startNumInput("- PESON DE BALANCE -", number, 2, false, false, 0, 99);
//  backMenu(); // Relance la navigation menu après saisie  ou timeout  
}

void m04x_0F_numBalxDone(void)
{ static char number[3] = ""; // Buffer pour le numérique 0..99

  LOG_INFO("Appel d'une Fonction: m04x_0F_numBalxDone()");
  finalizeNumInput(number); // Récupérer saisie finale
char msg[80];
  snprintf(msg,80,"Nouveau nombre: %d",number);
  LOG_INFO(msg);
  config.materiel.Peson_0 = atoi(number);
//  modifier struct pour tableau [num] vs switch (num)/case 1..3
  snprintf(msg,80,"Nombre memorisé: %d",config.materiel.Peson_0);
  LOG_INFO(msg);

// sauvegarde en EEPROM
  snprintf(msg, 21, "Bal %c => Jauge %d",bal+65,config.materiel.Peson_0);
  LOG_INFO(msg);
  OLEDDrawText(1,2, 0, msg );
  E24C32saveConfig();
E24C32DumpConfigToJSON();    
  backMenu();                                            // erreur retour menu
/*  
09:49:53.653 -> Nombre valide
09:49:53.653 -> OLEDDisplayMessageL8() => Nombre accepte
09:49:54.127 -> [DEBUG] Default dans isListInputActive()/saisieActive/default, pourquoi ???????????????????
09:49:54.127 -> [DEBUG] !!!else!!! : 146106 GestionEnCours 
09:49:54.127 -> [DEBUG] from handleProgrammingModea: 0
09:49:54.127 -> [ERROR] Perte de la Gestion courante dans Handle.cpp (Saisies_NB)
09:49:54.175 -> Lancement Menu principal
09:49:54.175 -> pushMenu()\title: -- MENU PRINCIPAL --
*/
  LOG_ERROR("m04x_0F_numBalxDone: erreur retour menu");
}

void m04x_0F_tareBal_n(void)    // appel écran de calibration
{
  debugSerial.println("Appel m04x_0F_tareBal_n - a implementer");
  saisieActive = 491; // pour identifier variable saisie lors de l'affectation
// selectionner numBal et affectation DEFAULT_VAL + Write EEPROM
// ou timeout
  InfoScreenRefreshTime = true;
  OLEDdisplayInfoBal();     // part dans KKKKKKKKK

  // fonction implementer

    if (Peson[config.materiel.Num_Carte][0])//  Poids_Peson(3) = GetPoids(4, 1);
    {
      Contrainte_List[0]= MESURESHX711GetStrainGauge(0,scaleA,AVR_10);  // renvoi la moyenne des 10 dernières mesures 
      HiveSensor_Data.HX711Weight[0] =  calculePoids(0); // kg     
    }

config.materiel.HX711NoloadValue_0 = Contrainte_List[0];

  
/*
  uint8_t Peson_0;              //  N° Peson
  uint8_t HX711Clk_0;           
  uint8_t HX711Dta_0;
  float   HX711NoloadValue_0;   // valeur sans charge (relevée par opérateur )
  float   HX711Tare_Temp_0;     // Température lors de la tare (mesurée lors de la tare)  
  float   HX711Scaling_0;       // Facteur de mise à l'echelle
  float   HX711Cor_Temp_0;      // Facteur de compensation en température
*/
    
char msg[80];
  snprintf(msg, 21, "tareJaugeA:%8f",Contrainte_List[0]);
  
  LOG_INFO(msg);
//  LOG_WARNING(msg)
//  LOG_ERROR(msg)
  OLEDDrawText(1,2, 0, msg );

char rc = read_DHT(dht); 
config.materiel.HX711Tare_Temp_0 = rc ? 99 : HiveSensor_Data.DHT_Temp;
E24C32saveConfig();

//delay(10000);

  InfoScreenRefreshTime = false;
  //  backMenu(); // Relance la navigation menu après saisie  ou timeout
}




void m04x_1F_echelleBal_n(void) // appel écran de calibration
{
  debugSerial.print("Appel d'une Fonction: ");
  debugSerial.println("m04x_1F_echelleBal_n()");

  saisieActive = 492; // pour identifier variable saisie lors de l'affectation
// selectionner numBal et affectation DEFAULT_VAL + Write EEPROM
// ou timeout
  
  backMenu(); // Relance la navigation menu après saisie  ou timeout
}

void m04x_2F_tempBal_n(void)    // appel écran de calibration
{
  debugSerial.print("Appel d'une Fonction: ");
  debugSerial.println("m04x_2F_tempBal_n()");
  saisieActive = 493; // pour identifier variable saisie lors de l'affectation
// selectionner numBal et affectation DEFAULT_VAL + Write EEPROM
// ou timeout
  backMenu(); // Relance la navigation menu après saisie  ou timeout
}

void m04x_3M_PopMenu(void)   // Retour menu précédent m04_CalibBalances
{
  debugSerial.print("Appel d'une Fonction: ");
  debugSerial.println("PopMenu()");
  popMenu();
}
