//       1         2         3         4         5         6         7        7
//34567890123456789012345678901234567890123456789012345678901234567890123456789
// IMPRESSION 79 COLONES EN TAILLE 12
//
// ---------------------------------------------------------------------------*
//      __  __                    _    
//     |  \/  |                  | |   
//     | \  / | ___ _ __  _   _  | |__ 
//     | |\/| |/ _ \ '_ \| | | | | '_ \
//     | |  | |  __/ | | | |_| |_| | | |
//     |_|  |_|\___|_| |_|\__,_(_)_| |_|
//                                     
// ---------------------------------------------------------------------------*


#ifdef __MAIN__
// Penser à les déclarer en extern


// Exemple de liste de valeurs alphanumériques

#define LIST_SF 4 // Maintenir dans Define.h
const char* List_SF[] = {
  "7",   // 
  "9",   //
  "12",  //
};

#define LIST_RUCHERS 12 // Maintenir dans Define.h
const char* List_Ruchers[] = {
  "Loess",   // 
  "Antenne",   //
  "Mathieu",  //
  "Verger",  //
  "Jachere",  //
  "Dachstein",  //  
  "Dangolsheim",  //
  "Ottrott",  //
  "MF Spitzberg",  //
  "Freudeneck",  //
  "Wolfisheim",  //  
  "Autre",  //
};

// Définition des menus

#define M0_ITEM 5 // Maintenir dans Define.h
const char* m0_Demarrage[] = {
  "Page INFOS     (P)",  // m0_0E_PageInfosSyst()
  "CONFIG. SYSTEME(M)",  // m0_1M_ConfigSystem() Date, Time, N° LoRa
  "CONFIG. LoRa   (M)",  // m0_2M_ConfigLoRa()    DevEUI, AppEUI, SF, Délai
  "CALIB. Tens. (m03)",  // m0_3M_CalibTensions() VBat, Vsol, VLum
  "CALIB. Bal.  (m04)",  // m0_4M_CalibBal() VBat, Vsol, VLum
};

#define M01_ITEM 7 // Maintenir dans Define.h
const char* m01_ConfigSystem[] = {
  "SAISIE DATE    (S)",  // m01_0F_GetDate()
  "SAISIE HEURE   (S)",  // m01_1F_GetTime()
  "NUM. RUCHER    (S)",  // m01_2F_NumRucher()
  "NOM. RUCHER    (S)",  // m01_3F_NomRucher()
  "Lire EEPROM    (F)",  // m01_4F_readConfig()
  "Ecrire EEPROM  (F)",  // m01_5F_writeConfig()
  "RET  popMenu(m0)"     // m01_6M_PopMenu()
};

#define M02_ITEM 8 // Maintenir dans Define.h
const char* m02_ConfigLoRa[] = {
  "INFOS LoRa     (P)",  // m02_0E_PageInfosLoRa() // afficher prochain Payload
  "DevEUI         (S)",  // m02_1F_DevEUI()
  "AppEUI         (S)",  // m02_2F_AppEUI()
  "Spread Fact    (S)",  // m02_3F_SpreadFact()
  "PayLoad time   (S)",  // m02_4F_PayLoad() // Afficher prochain Payload
  "Join LoRa      (S)",  // m02_5F_Join() // Connexion LoRa
  "Send Payload   (S)",  // m02_6F_SendPayLoad() // Envoyer Payload  
  "RET  popMenu(m0)"     // m02_7M_PopMenu()
};

#define M03_ITEM 5 // Maintenir dans Define.h
const char* m03_CalibTensions[] = {
  "Calib. VBAT    (F)",  // m03_0F_CalibVBat()
  "Calib. VSOL    (F)",  // m03_1F_CalibVSol()
  "Calib. LUM     (F)",  // m03_2F_CalibVLum()
  "Reserve     (m033)",  // m03_3F_Reserve()
  "RET  popMenu(m0)"     // m03_4M_PopMenu()
};

#define M033_ITEM 5 // Maintenir dans Define.h
const char* m033_Reserve[] = {
  "menu033-0       (F)",      // Mise à Echelle VBat
  "menu033-1       (F)",      // Mise à Echelle VSol
  "menu033-2       (F)",      // Mise à Echelle VLum
  "menu033-3       (F)",    // Libre
  "RET   popMenu (m03)"     // Retour menu principal
};

#define M04_ITEM 7 // Maintenir dans Define.h
const char* m04_CalibBalances[] = {
  "Calib. Bal #1  (F)",  // m04_0F_CalibBal_1()
  "Calib. Bal #2  (F)",  // m04_1F_CalibBal_2()
  "Calib. Bal #3  (F)",  // m05_2F_CalibBal_3()
  "Calib. Bal #4  (F)",  // m05_3F_CalibBal_4()
  "info.  Balances(F)",  // m04_4F_CalibBal_1()  /!\ sortie info, c'est la pagaille!!!!
  "Poids  Balances(F)",  // m04_5F_PoidsBal()  /!\ sortie info, c'est la pagaille!!!!  
  "RET  popMenu  (m0)"   // m05_6M_PopMenu()
};


#define M04x_ITEM 4 // Maintenir dans Define.h
const char* m04x_CalibBal[] = {
//  "Num. Balance    (S)", // m04x_0F_numBalx()
  "Tare Balance    (F)", // m04x_1F_tareBalx()
  "Echelle Balance (F)", // m04x_2F_echBalx()
  "Comp. Temp.     (F)", // m04x_3F_tempBalx()
  "RET   popMenu (m04)"  // Retour menu principal
};

#else

extern const char* m0_Demarrage[];
extern const char* m01_ConfigSystem[];
extern const char* m02_ConfigLoRa[];
extern const char* m03_CalibTensions[];
extern const char* m033_Reserve[];
extern const char* m04_CalibBalances[];
extern const char* m04x_CalibBal[];

#endif
