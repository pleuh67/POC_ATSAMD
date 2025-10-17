//       1         2         3         4         5         6         7        7
//34567890123456789012345678901234567890123456789012345678901234567890123456789
// IMPRESSION 79 COLONES EN TAILLE 12
//
// ---------------------------------------------------------------------------*

// ---------------------------------------------------------------------------*
#define __INIT_DONE
#include "define.h"

// info afficher NextPayload
// info afficher MODE PE


/*
 * Fonctions lancées par les menus
 * 
 * lancement : menuNNN_FN_WhatToDo()     // ex: menu000_F7_GetTime()
 * Traitement: menuNNN_FN_WhatToDoDone() // ex: menu000_F7_GetTimeDone()    
 * 
 * 
 * 
 * menu000_F6_GetDate()
 * menu000_F7_GetTime() + Done
 * menu000_F8_GetHex()  + Done
 * 
 * prévoir inits param Lora :
//  uint8_t HWEUI [20];       // ID RN2483: "0004A30B00EEEE01"
//  uint8_t AppEUI [10];      // AppEUI: {0x41, 0x42, 0x45, 0x49, 0x4C, 0x4C, 0x45, 0x31, 0x00} menuxxx_xx_GetHex()
//  uint8_t AppKey [18];      // AppKEY: // 5048494C495050454C4F564542454553 - PHILIPPELOVEBEES
//// {0x50, 0x48, 0x49, 0x4C, 0x49, 0x50, 0x50, 0x45, 0x4C, 0x4F, 0x56, 0x45, 0x42, 0x45, 0x45, 0x53, 0x00} 
//  uint8_t SpreadingFactor;  // 7, 9 et 12 echec freudeneck
//  uint8_t SendingPeriod;    // 15 minutes = 500 sans IT
 * 
 * Prévoir inits generaux + Bal
//  Date:  menuxxx_xx_GetDate()
//  Heure: menuxxx_xx_GetTime()
//  uint8_t Balance_ID;           // ID Rucher           xx  uint8_t
//  char    RucherName [20];      // Localisation Rucher (saisir direct ou liste + "autre")
//  version, mail (pour le fun), 
 * 
 * Prévoir Config Hardware
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
 * 
 */


// ---------------------------------------------------------------------------*
// ----------------------------- menu000Demarrage ------------------------
// ---------------------------------------------------------------------------*

 
// "Page INFOS     (P)"
void m0_0E_PageInfos()
{
//
  debugSerial.println("CONFIG. SYSTEME - Ecran INFOS demandé");   
  infoScreenRefreshTime = true;  
  OLEDdisplayInfoScreen();     // part dans KKKKKKKKK
}

// "CONFIG. SYSTEME(F)"
void m0_1M_ConfigSysteme() //
{
  infoScreenState = INFO_SCREEN_ACTIVE;  // pour eviter KKKKK met en mode ecran info, continu par appuii touche "VALIDE"
  debugSerial.println("Appel d'un Menu");
  debugSerial.println("CONFIG. SYSTEME - Fonction a implementer");

  OLEDDrawText(1, 7, 0, "VALIDE pour retour");
}

//  "CONFIG. LoRa  (F)",
void m0_2M_ConnexLoRa()    //
{
  infoScreenState = INFO_SCREEN_ACTIVE;  // pour eviter KKKKK met en mode ecran info, continu par appuii touche "VALIDE"
  debugSerial.print("Appel d'un Menu: ");      
  debugSerial.println("CONFIG. LoRa   (M) - a implementer");
  debugSerial.println("faire Menu : menu030ConfigLoRa pour : DevEUI, AppEUI, SF, Délai et RETOUR");

// creer sous menu  
  OLEDDrawText(1, 1, 0, "  CREER SOUS MENU");
  OLEDDrawText(1, 2, 0, "DevEUI: ");
  OLEDDrawText(1, 3, 0, "AppEUI: ");
  OLEDDrawText(1, 4, 0, "SF    : ");
  OLEDDrawText(1, 5, 0, "Delay : ");
  OLEDDrawText(1, 7, 0, "VALIDE pour retour");
}  


//  "CALIB. Volt (M030)", 
void m0_3M_CalibTensions()   //  
{
  debugSerial.println("Appel d'un sous menu");    
  pushMenu("CALIB. TENSIONS:", m03_CalibTensions, 5, 0);
}
  
  
//  "CALIB. BALANCES(F)",
void m0_4M_CalibBalances()   //
{
  debugSerial.print("Appel d'un sous menu:");
  debugSerial.println("CONFIG. BALANCES");   
// pushMenu(const char* title, const char** menuList, uint8_t menuSize, uint8_t initialIndex)
  debugSerial.println("Appel d'un sous menu");    
  pushMenu("CALIB. TENSIONS:", m04_CalibBalances, 5, 0);
}


void m0_5F_GetDate()
{ DateTime systemTime;     

debugSerial.println("Lancement saisie DATE");
debugSerial.println("CONFIG. SYSTEME - Demande saisie DATE");
  systemTime = rtc.now();
sprintf(serialbuf, "%02d:%02d:%02d    %02d/%02d/%02d", 
        systemTime.hour(), systemTime.minute(), systemTime.second(),
        systemTime.day(), systemTime.month(), systemTime.year()-2000);
debugSerial.println(serialbuf);
  sprintf(serialbuf, "%02d/%02d/%04d",systemTime.day(), systemTime.month(), systemTime.year());
  startDateInput(serialbuf); 
}               


void m0_6F_GetTime()
{
DateTime systemTime;     
debugSerial.println("Lancement saisie TIME");
debugSerial.println("CONFIG. SYSTEME - Demande saisie TIME");
  systemTime = rtc.now();
sprintf(serialbuf, "%02d:%02d:%02d    %02d/%02d/%02d", 
        systemTime.hour(), systemTime.minute(), systemTime.second(),
        systemTime.day(), systemTime.month(), systemTime.year()-2000);
debugSerial.println(serialbuf);
  sprintf(serialbuf, "%02d:%02d:%02d",systemTime.hour(), systemTime.minute(), systemTime.second());
  startTimeInput(serialbuf); 
}           

void m0_6F_GetTimeDone()    
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
  systemTime = rtc.now();
  rtc.adjust(DateTime(systemTime.year()-2000, systemTime.month(), systemTime.day(), hour, minute, second));  
debugSerial.println("mise à l'heure DS3231");
  copyDS3231TimeToMicro(1);
  synchronizeDS3231TimeToMicro();
debugSerial.println("Reprogramme IRQ2");  
  DS3231setRTCAlarm2(); // Reprogrammer prochaine alarme dans n min
// Activer la liste de démarrage quand fin saisie TIME : void finalizeTimeInput(char* outputTime)
  if (currentMenuDepth > 0)
  {
    menuLevel_t* currentMenu = &menuStack[currentMenuDepth - 1];
    startListInputWithTimeout(currentMenu->title, currentMenu->menuList, currentMenu->menuSize, currentMenu->selectedIndex, 0);
  }
}


void m0_7F_GetHex()
{ static char hexBuffer[41] = "0123456789ABCDEF0123456789ABCDEF01234567"; // Buffer pour l'hexa

debugSerial.println("Lancement saisie HEXA");
debugSerial.println("CONFIG. SYSTEME - Demande saisie HEXA");
  startHexInput(hexBuffer); 
}           

void m0_7F_GetHexDone()    
{ static char hexBuffer[41] = "0123456789ABCDEF0123456789ABCDEF01234567"; // Buffer pour l'hexa

  finalizeHexInput(hexBuffer); // Récupérer chaine HEXA
debugSerial.print("Nouvelle chaine: ");
debugSerial.println(hexBuffer);        // Ici vous pouvez traiter l'heure et revenir au menu
  if (currentMenuDepth > 0)
  {
    menuLevel_t* currentMenu = &menuStack[currentMenuDepth - 1];
    startListInputWithTimeout(currentMenu->title, currentMenu->menuList, currentMenu->menuSize, currentMenu->selectedIndex, 0);
  }
}

// ---------------------------------------------------------------------------*
// ----------------------------- menu030CalibTensions --------------------
// ---------------------------------------------------------------------------*
//  float   LDRBrightnessScale;   // 
//  float   VSolScale;            //  
//  float   VBatScale;


// "Calib. VBAT    (F)",      // 0: Mise à Echelle VBat
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
// ----------------------------- menu043 => menu040_F3_-------------------
// ---------------------------------------------------------------------------*



// ---------------------------------------------------------------------------*
// ----------------------------- menu050CalibBalances --------------------
// ---------------------------------------------------------------------------*

//  "Calib. Bal #1  (F)",      // 0: Calibration Balance 1
void m04_0F_CalibBal_1()  // 
{
  debugSerial.print("Appel d'une Fonction: ");      
  debugSerial.println("CalibBal_1 - a implementer");   

  OLEDDrawText(1, 7, 0, "VALIDE pour retour");
}


//  "Calib. Bal #2  (F)",      // 1: Calibration Balance 2
void m04_1F_CalibBal_2()  // 
{
  debugSerial.print("Appel d'une Fonction: ");      
  debugSerial.println("CalibBal_2 - a implementer");   

  OLEDDrawText(1, 7, 0, "VALIDE pour retour");
}


//  "Calib. Bal #3  (F)",      // 2:  Calibration Balance 3
void m04_2F_CalibBal_3()  // 
{
  debugSerial.print("Appel d'une Fonction: ");      
  debugSerial.println("CalibBal_3 - a implementer");   

  OLEDDrawText(1, 7, 0, "VALIDE pour retour");
}


//  "Calib. Bal #4  (F)",      // 3:  Calibration Balance 4
void m04_3F_CalibBal_4()  // 
{
  debugSerial.print("Appel d'une Fonction: ");      
  debugSerial.println("CalibBal_4 - a implementer");    

  OLEDDrawText(1, 7, 0, "VALIDE pour retour");
}

//  "RET  popMenu(M000)"       // 4: Retour menu principal
void m04_4M_PopMenu()  // retour menu000Demarrage
{
  debugSerial.print("Appel d'une Fonction: ");      
  debugSerial.println("PopMenu()");    
  popMenu();
}
