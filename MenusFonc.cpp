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
  PageInfosSystRefresh = true;  
  OLEDdisplayInfoScreenSyst(); 
}

// trouver ou appeler
void m0_0E_PageInfosSystDone()
{
  PageInfosSystRefresh = false;
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
  sprintf(serialbuf,"Appel d'un sous menu: CONFIG. BALANCE %1d", bal);   
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
        systemTime.day(), systemTime.month(), systemTime.year()-2000);
debugSerial.println(serialbuf);
  sprintf(serialbuf, "%02d/%02d/%04d",systemTime.day(), systemTime.month(), systemTime.year());
  startDateInput("-- SAISIR LA DATE --", serialbuf); 
}               


void m01_0F_GetDateDone()  
{ DateTime systemTime;
  byte jour, mois, annee;
  static char dateBuffer[11] = "11/11/2025"; // Buffer pour la date 11/11/1965

  finalizeDateInput(dateBuffer); // Récupérer l'heure finale en jj/mm/aaaa
debugSerial.print("Nouvelle Date: ");
debugSerial.println(dateBuffer);        // Ici vous pouvez traiter la date et revenir au menu
// jj/mm/aaaa
  jour = ((byte)dateBuffer[0] -48)*10;
  jour += (byte)dateBuffer[1] -48;
  mois = ((byte)dateBuffer[3] -48)*10;
  mois += (byte)dateBuffer[4] -48;
  annee = ((byte)dateBuffer[8] -48)*10;
  annee += (byte)dateBuffer[9] -48;
  
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
  

sprintf(serialbuf, "mise à la date DS3231 %d/%d/%d",annee,mois,jour);
debugSerial.println(serialbuf);
  copyDS3231TimeToMicro(1);
  synchronizeDS3231TimeToMicro();

// utile ?????????????
debugSerial.println("Reprogramme IRQ2");  
  DS3231setRTCAlarm2(); // Reprogrammer prochaine alarme dans n min

  
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
        systemTime.day(), systemTime.month(), systemTime.year()-2000);
debugSerial.println(serialbuf);
  sprintf(serialbuf, "%02d:%02d:%02d",systemTime.hour(), systemTime.minute(), systemTime.second());
  startTimeInput("-- SAISIR L HEURE --",serialbuf); 
}           

void m01_1F_GetTimeDone()    
{ DateTime systemTime;
  byte hour, minute, second;
  static char timeBuffer[9] = ""; // Buffer pour l'heure

  finalizeTimeInput(timeBuffer); // Récupérer l'heure finale
debugSerial.print("Nouvelle heure: ");
debugSerial.println(timeBuffer);        // Ici vous pouvez traiter l'heure et revenir au menu
// hh:mm:ss
  hour = ((byte)timeBuffer[0] -48)*10;
  hour += (byte)timeBuffer[1] -48;
  minute = ((byte)timeBuffer[3] -48)*10;
  minute += (byte)timeBuffer[4] -48;
  second = ((byte)timeBuffer[6] -48)*10;
  second += (byte)timeBuffer[7] -48;

  
// Lecture avant modif pour init hh:mm:ss
 systemTime = rtc.now();
  // mise à l'heure hh:mm:ss du DS3231
  rtc.adjust(DateTime(systemTime.year()-2000, systemTime.month(), systemTime.day(), hour, minute, second));  
  // mise à date modifiée du µc
  systemTime = rtc.now();

//   rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  
//  systemTime = rtc.now();
//  rtc.adjust(DateTime(systemTime.year()-2000, systemTime.month(), systemTime.day(), hour, minute, second));  
debugSerial.println("mise à l'heure DS3231");
  copyDS3231TimeToMicro(1);
  synchronizeDS3231TimeToMicro();

  
debugSerial.println("Reprogramme IRQ2");  
  DS3231setRTCAlarm2(); // Reprogrammer prochaine alarme dans n min
// Activer la liste de démarrage quand fin saisie TIME : void finalizeTimeInput(char* outputTime)
  backMenu(); 
}

// Saisie de ConfigMateriel.Noeud_LoRa
void m01_2F_GetNumRucher()  
{ static char number[4];

debugSerial.print("Appel d'une Fonction: m01_2F_GetNumRucher()");   
  sprintf(number,"%d", ConfigMateriel.Noeud_LoRa);
debugSerial.println(ConfigMateriel.Noeud_LoRa);   
debugSerial.println(number);   
  startNumInput("-NUM. SYSTEME LoRa-",number, 4, false, false, 0, 10);
}

// Sauvegarde de ConfigMateriel.Noeud_LoRa
void m01_2F_GetNumRucherDone()  
{ static char number[9] = ""; // Buffer pour le numérique

debugSerial.print("Appel d'une Fonction: ");      
debugSerial.println("m01_2F_GetNumRucherDone()");   
  finalizeNumInput(number); // Récupérer saisie finale  
debugSerial.print("Nouveau nombre: ");
debugSerial.println(number);
  ConfigMateriel.Noeud_LoRa = atoi(number);
debugSerial.println(ConfigMateriel.Noeud_LoRa);   
  backMenu(); 
}


// Saisie de ConfigApplicatif.RucherName
void m01_3F_GetNameRucher()  // Saisir dans la liste List_Ruchers[], 12 elements
{ static char currentString[21];
debugSerial.println("Appel d'une Fonction: m01_4F_GetNameRucher()");   

  sprintf(currentString,"%s", ConfigApplicatif.RucherName);
  debugSerial.print("Appel avec: "); debugSerial.println(currentString);     
//sprintf(currentString,Data_LoRa.RucherName); // pointeur sur valeur courante traiter
  startStringInput("---- NOM RUCHER ----", currentString, 20);
}

// Sauvegarde de ConfigApplicatif.RucherName
void m01_3F_GetNameRucherDone()  
{ static char currentString[21];
  debugSerial.print("Appel d'une Fonction: ");      
  debugSerial.println("m01_4F_GetNameRucherDone()");       

  finalizeStringInput(currentString); // Récupérer saisie finale  
debugSerial.print("Nouveau nom: ");
debugSerial.println(currentString);
  sprintf(ConfigApplicatif.RucherName,"%s", currentString);
debugSerial.println(ConfigApplicatif.RucherName);   
  backMenu(); 
}

void m01_4F_readConfig()  
{
  debugSerial.print("Appel d'une Fonction: ");      
  debugSerial.println("m01_5F_readConfig() à developper");     

// 


// Si pas fonction_Done();
// Activer la liste de démarrage quand fin saisie
  backMenu(); 
}

void m01_4F_readConfigDone()  
{
  debugSerial.print("Appel d'une Fonction: ");      
  debugSerial.println("m01_5F_readConfigDone() à developper");       
  backMenu(); // supprimer dans pas Done  
}

void m01_5F_writeConfig()  
{
  debugSerial.print("Appel d'une Fonction: ");      
  debugSerial.println("m01_6F_writeConfig()");       

// Si pas fonction_Done();
// Activer la liste de démarrage quand fin saisie
  backMenu(); 
}

void m01_5F_writeConfigDone()  
{
  debugSerial.print("Appel d'une Fonction: ");      
  debugSerial.println("m01_6F_writeConfigDone()");       
  backMenu(); // supprimer dans pas Done
}

//  "RET  popMenu(M000)"       // 6: Retour menu principal
void m01_6M_PopMenu()  // retour menu000Demarrage
{
  debugSerial.print("Appel d'une Fonction: ");      
  debugSerial.println("PopMenu()");  
  popMenu();
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
  debugSerial.println("CONFIG. LoRa - Ecran INFOS demandé");   
  PageInfosLoRaRefresh = true;  
  OLEDdisplayInfoScreenLoRa(); 
}

// trouver ou appeler
void m02_0E_PageInfosLoRaDone()
{
  PageInfosLoRaRefresh = false;
}

// AppKey => {0x50,0x48,0x49,0x4C,0x49,0x50,0x50,0x45,0x4C,0x4F,0x56,0x45,0x42,0x45,0x45,0x53,0x00} 
// 5048494C495050454C4F564542454553 - PHILIPPELOVEBEES
// AppKey = (uint8_t *)AppKey_List[Ruche.Num_Carte];
// appel de Menus.cpp
void m02_1F_AppKEY()  // AppKEY
{ static char hexBuffer[33]; // Buffer pour l'hexa

  saisieActive=21;
debugSerial.println("CONFIG. SYSTEME - Demande saisie HEXA AppKEY");
  byteArrayToHexString(&AppKey_List [Ruche.Num_Carte][0], 16, hexBuffer, 33);
  startHexInput("-- SAISIR APPKEY ---", hexBuffer,32); 
}           

// appel de Handle.cpp avec saisieActive=21;
void m02_1F_AppKEYDone()    // AppKEY
{ static char hexBuffer[33] = ""; // Buffer pour l'hexa

  finalizeHexInput(hexBuffer); // Récupérer chaine HEXA
debugSerial.print("Nouvelle chaine AppKEY: ");
debugSerial.println(hexBuffer);        // Ici vous pouvez traiter AppKEY et revenir au menu
  hexStringToByteArray(hexBuffer, &AppKey_List [Ruche.Num_Carte][0], strlen(hexBuffer));
  backMenu(); 
}

// AppEUI => {0x41, 0x42, 0x45, 0x49, 0x4C, 0x4C, 0x45, 0x35, 0x00} = ABEILLE5
// appel de Menus.cpp
void m02_2F_AppEUI() // AppEUI
{ static char hexBuffer[10]; // Buffer pour l'hexa

  saisieActive=22;
debugSerial.println("CONFIG. SYSTEME - Demande saisie HEXA AppEUI");
  byteArrayToHexString(&AppEUI_List [Ruche.Num_Carte][0], 8, hexBuffer, 17);
  startHexInput("-- SAISIR APPEUI ---", hexBuffer,16); 
}

// appel de Handle.cpp avec saisieActive=22;
void m02_2F_AppEUIDone()    // AppEUI
{  static char hexBuffer[17] = ""; // Buffer pour l'hexa

  finalizeHexInput(hexBuffer); // Récupérer chaine HEXA
debugSerial.print("Nouvelle chaine AppEUI: ");
debugSerial.println(hexBuffer);        // Ici vous pouvez traiter AppEUI et revenir au menu
  hexStringToByteArray(hexBuffer, &AppEUI_List [Ruche.Num_Carte][0], strlen(hexBuffer));
  backMenu(); 
}

// appel de Menus.cpp
void m02_3L_GetSF() // Spread Factor, saisir dans la liste List_SF[], 3 elements
{
  saisieActive=23;   // 
debugSerial.println("Appel m02_3L_GetSF");   
int i;
 i = LoRa_Config.SpreadingFactor;

 i = i == 7 ? 0 : i == 9 ? 1 : 2 ;

static char hexBuffer[17] = ""; // Buffer pour l'hexa
sprintf(hexBuffer,"SF: %d  i: %d",LoRa_Config.SpreadingFactor,i);
debugSerial.println(hexBuffer);   

  startListInput("---- SELECT SF -----", List_SF, 3, i, TIMEOUT_SAISIE);
}

void m02_3L_GetSFDone()    // Spread Factor
{ static char selectedSF[21] = "";
  static char localBuffer[40] = "";
  
debugSerial.println("Appel m02_3L_GetSFDone");   
    uint8_t index = finalizeListInput(selectedSF);
    
debugSerial.print("SF selectionne: ");
debugSerial.println(selectedSF);
    
    // Extraire la valeur numérique
    if (strcmp(selectedSF, "SF7") == 0)
      LoRa_Config.SpreadingFactor = 7;
    else if (strcmp(selectedSF, "SF9") == 0)
      LoRa_Config.SpreadingFactor = 9;
    else if (strcmp(selectedSF, "SF12") == 0)
      LoRa_Config.SpreadingFactor = 12;

sprintf(localBuffer,"Nouvelle chaine RC: %d / item: %s / SF: %d", index, selectedSF, LoRa_Config.SpreadingFactor );
debugSerial.println(localBuffer);        // Ici vous pouvez traiter SF et revenir au menu
  backMenu(); 
}

void m02_4F_GetPayloadDelay() // Delais Payload
{
 debugSerial.println("Appel m02_4F_GetPayloadDelay - a implementer");   
  saisieActive=23;   // pas de cas d'usage

}

void m02_4F_GetPayloadDelayDone()    // Delais Payload
{  static char numBuffer[21] = ""; // Buffer pour le Num
debugSerial.println("Appel m02_4F_GetPayloadDelayDone - a implementer");   

  finalizeNumInput(numBuffer);
  backMenu();       
}


void m02_5F_Join() // Connexion LoRa
{
debugSerial.println("Appel m02_5F_Join - a tester");   

GestionEnCours("m02_5Fa");
  init2483A();  // Réinitialise DevEUI, AppEUI et AppKey
debugSerial.println("Fin init2483A");   
 GestionEnCours("m02_5Fb");
  initLoRa();   // Reconnecte et envoie Payload + affiche résultat
GestionEnCours("m02_5Fc");
debugSerial.println("Appel m02_5F_Join - FIN"); 


// Relance la navigation menu execution
  backMenu(); 
}


void m02_5F_JoinDone() // Connexion LoRa
{
    // pas besoin ???
}
  
  
void m02_6F_SendPayLoad() // Envoyer Payload  
{
    alarm1_enabled = false;  // Bloquer alarme 1
    alarm2_enabled = false;  // Bloquer alarme 2
debugSerial.println("Appel m02_6F_SendPayLoad - a tester");   
    turnOnRedLED();     // PCB donne GREEN?
    buildLoraPayload();
#ifdef __SendLoRa
  sendLoRaPayload((uint8_t*)payload,19);   // hex
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

// "Calib. VBAT    (F)",      // 0: Mise à Echelle VBat
//  float  Ruche.VBatScale;
void m03_0F_CalibVBat() //
{
  infoScreenState = INFO_SCREEN_ACTIVE;  // pour eviter KKKKK met en mode ecran info, continu par appuii touche "VALIDE"
  debugSerial.print("Appel d'une Fonction: ");      
  debugSerial.println("CalibVBat - Fonction a implementer");  

// creer sous menu  
  OLEDDrawText(1, 1, 0, "  CALIBRER VBat");
  OLEDDrawText(1, 3, 0, "VBat :  ");            // Affichage à chaque seconde, sans effet si valid
  OLEDDrawText(1, 4, 0, "VBatScale: ");         // affichage, sans effet si valid
  OLEDDrawText(1, 5, 0, "New : ");              // saisie Num nouvelle val de VBatScale => menu041_F0_GetNum()
  OLEDDrawText(1, 7, 0, "VALIDE pour retour");  // fin
}
  
//  "Calib. VSOL    (F)",       // 1: Mise à Echelle VSol
//  float   Ruche.VSolScale; 
void m03_1F_CalibVSol()  //
{
  infoScreenState = INFO_SCREEN_ACTIVE;  // pour eviter KKKKK met en mode ecran info, continu par appuii touche "VALIDE"
  debugSerial.print("Appel d'une Fonction: ");      
  debugSerial.println("CalibVSol - Fonction a implementer");  
// creer sous menu  
  OLEDDrawText(1, 1, 0, "  CALIBRER VSol");
  OLEDDrawText(1, 3, 0, "VSol :  ");            // Affichage à chaque seconde, sans effet si valid
  OLEDDrawText(1, 4, 0, "VSolScale: ");         // affichage, sans effet si valid
  OLEDDrawText(1, 5, 0, "New : ");              // saisie Num nouvelle val de VSolScale => menu041_F0_GetNum()
  OLEDDrawText(1, 7, 0, "VALIDE pour retour");  // fin
}

//  "Calib. LUM     (F)",       // 2: Mise à Echelle VLum
//  float   Ruche.LDRBrightnessScale; 
void m03_2F_CalibVLum()   //
{
  infoScreenState = INFO_SCREEN_ACTIVE;  // pour eviter KKKKK met en mode ecran info, continu par appuii touche "VALIDE"
  debugSerial.print("Appel d'une Fonction: ");      
  debugSerial.println("CalibVLum - Fonction a implementer");  
// creer sous menu  
  OLEDDrawText(1, 1, 0, "  CALIBRER VLum");
  OLEDDrawText(1, 3, 0, "VLum :  ");            // Affichage à chaque seconde, sans effet si valid
  OLEDDrawText(1, 4, 0, "VLumScale: ");         // affichage, sans effet si valid
  OLEDDrawText(1, 5, 0, "New : ");              // saisie Num nouvelle val de VLumScale => menu041_F0_GetNum()
  OLEDDrawText(1, 7, 0, "VALIDE pour retour");  // fin
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


void m04_nM_CalibBal_bal()
{
  debugSerial.println("Appel m04_nM_CalibBal_bal - a implementer");   
//  OLEDDrawText(1, 7, 0, "VALIDE pour retour");  
  OLEDDrawText(1, 7, 0, "m04_nM_CalibBal_bal");  
  
  infoScreenRefreshTime = true;  
  OLEDdisplayInfoBal();     // part dans KKKKKKKKK
  backMenu(); // Relance la navigation menu après saisie  ou timeout
}



void m04_0F_CalibBal_1()  // 
{
debugSerial.println("Appel m04_nM_CalibBal_1 - a implementer");   

  infoScreenRefreshTime = true;  
  OLEDdisplayInfoBal();     // part dans KKKKKKKKK

  backMenu(); // Relance la navigation menu après saisie  ou timeout
   
// listInputCtx.state == LIST_INPUT_ACTIVE;  // reste dans les 
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

//  "info.  Balances(F)",      //   Page info Balances 
void m04_4F_InfoBal(void)    // 
{

  OLEDDrawText(1, 7, 0, "VALIDE pour retour");
  backMenu(); // Relance la navigation menu après saisie  ou timeout
}

 
// "Poids  Balances(F)"   // Affichage rafraichi du poids des balances
void m04_5F_PoidsBal(void)   //
{
  
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

void m04x_0F_tareBal_1(void)    // appel écran de calibration
{
  debugSerial.print("Appel d'une Fonction: ");      
  debugSerial.println("m04x_0F_tareBal_1()");      
  backMenu(); // Relance la navigation menu après saisie  ou timeout
}


void m04x_1F_echelleBal_2(void) // appel écran de calibration
{
  debugSerial.print("Appel d'une Fonction: ");      
  debugSerial.println("m04x_1F_echelleBal_2()");        
  backMenu(); // Relance la navigation menu après saisie  ou timeout
}  
  
void m04x_2F_tempBal_3(void)    // appel écran de calibration
{
  debugSerial.print("Appel d'une Fonction: ");      
  debugSerial.println("m04x_2F_tempBal_3()");      
  backMenu(); // Relance la navigation menu après saisie  ou timeout
}

void m04x_3M_PopMenu(void)   // Retour menu précédent m04_CalibBalances
{
  debugSerial.print("Appel d'une Fonction: ");      
  debugSerial.println("PopMenu()");    
  popMenu();  
}
