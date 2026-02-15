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
  "SF7",   // 
  "SF9",   //
  "SF12",  //
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
  "CALIB. Bal.  (m04)",  // m0_4M_CalibBal() BAL A/B/C/D
};

#define M01_ITEM 7 // Maintenir dans Define.h
const char* m01_ConfigSystem[] = {
  "SAISIE DATE    (S)",  // m01_0F_GetDate()
  "SAISIE HEURE   (S)",  // m01_1F_GetTime()
  "NUM. RUCHER    (S)",  // m01_2F_NumRucher()
  "NOM  RUCHER    (S)",  // m01_3F_NomRucher()
  "Lire EEPROM    (F)",  // m01_4F_readConfig()
  "Ecrire EEPROM  (F)",  // m01_5F_writeConfig()
  "RET  popMenu(m0)"     // popMenu()
};

#define M02_ITEM 8 // Maintenir dans Define.h
const char* m02_ConfigLoRa[] = {
  "INFOS LoRa     (P)",  // m02_0E_PageInfosLoRa() // afficher prochain Payload
  "AppKEY         (S)",  // m02_1F_AppKey()
  "AppEUI         (S)",  // m02_2F_AppEUI()
  "Spread Fact    (S)",  // m02_3F_SpreadFact()
  "PayLoad time   (S)",  // m02_4F_PayLoad() // Afficher prochain Payload
  "Join LoRa      (S)",  // m02_5F_Join() // Connexion LoRa
  "Send Payload   (S)",  // m02_6F_SendPayLoad() // Envoyer Payload  
  "RET  popMenu(m0)"     // m02_7M_PopMenu()
};

#define M03_ITEM 5 // Maintenir dans Define.h
const char* m03_CalibTensions[] = {
  "Calib. VBAT    (F)",  // m03_0F_CalibVBat() appel de scalingVBat() à écrire avec startNumInput("--CALIBRATION VBAT--", char* initialNum (VBatScale_List [10]), 6, 0, 1, 0, 1); ex:0.0032252
  "Calib. VSOL    (F)",  // m03_1F_CalibVSol()
  "Calib. LUM     (F)",  // m03_2F_CalibVLum()
  "Affiche Tens.  (P)",  // m03_3F_Reserve()
  "RET  popMenu(m0)"     // m03_4M_PopMenu()
};


/*
Modification en cours du menu des calib tensions
L1 affiche un ecran des tensions V1, V2, V3 et Scales correspondantes
L2 saisie scale V1
L3 saisie scale V2
L4 saisie scale V3
*/


#define M03_ITEM 5 // Maintenir dans Define.h
const char* m03_Future_CalibTensions[] = {
  "Affiche Tens.  (S)",  // m03_0E_DisplayVoltageInput()
  "Calib. VBAT    (F)",  // m03_1F_CalibVBat() appel de scalingVBat() à écrire avec startNumInput("--CALIBRATION VBAT--", char* initialNum (VBatScale_List [10]), 6, 0, 1, 0, 1); ex:0.0032252
  "Calib. VSOL    (F)",  // m03_2F_CalibVSol()
  "Calib. LUM     (F)",  // m03_3F_CalibVLum()
  "RET  popMenu(m0)"     // m03_4M_PopMenu()
};



#define M033_ITEM 5 // Maintenir dans Define.h
const char* m033_Reserve[] = {
  "menu033-0      (F)",      // Mise à Echelle VBat
  "menu033-1      (F)",      // Mise à Echelle VSol
  "menu033-2      (F)",      // Mise à Echelle VLum
  "menu033-3      (F)",      // Libre
  "RET  popMenu (m03)"       // PopMenu()
};

#define M04_ITEM 7 // Maintenir dans Define.h
const char* m04_CalibBalances[] = {
  "info.  Balances(F)",  // m04_0F_InfoBal() Affichage rafraichi du poids des balances
  "Poids  Tare    (F)",  // m04_1F_PoidsTare()  (chiffre entre 0 et 99000)
  "Calib. Bal A  m042",  // m04_2M_CalibBal(int Bal=0)
  "Calib. Bal B  m043",  // m04_2M_CalibBal(int Bal=1)
  "Calib. Bal C  m044",  // m04_2M_CalibBal(int Bal=2)
  "Calib. Bal D m045",  // m04_2M_CalibBal(int Bal=3)
  "RET  popMenu  (m0)"   // PopMenu()
};


#define M04x_ITEM 5 // Maintenir dans Define.h
const char* m04x_CalibBal[] = {
  "Num. Balance    (S)",    // m04x_0F_numBalx()
  "Tare Balance    (F)",    // m04x_1F_tareBal(int numJauge); enregistre la tare et temperature
  "Echelle Balance (F)",    // m04x_2F_echBal(int numJauge); calcule la mise à l'echelle
  "Saisie Comp. Temp. ",    // m04x_3F_adjustTempBal(int numJauge); saisie d'un coefficient de correction
  "RET   popMenu (m04)"     // PopMenu()
};

#else

extern const char* m0_Demarrage[];
extern const char* m01_ConfigSystem[];
extern const char* m02_ConfigLoRa[];
extern const char* m03_CalibTensions[];
extern const char* m033_Reserve[];
extern const char* m04_CalibBalances[];
extern const char* m04x_CalibBal[];

extern const char* List_SF[];
extern const char* List_Ruchers[];

#endif
