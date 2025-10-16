#ifdef __MAIN__
// Penser à les déclarer en extern


// Exemple de liste de valeurs alphanumériques
// Définition des menus

#define M0_ITEM 8
const char* m0_Demarrage[] = {
  "Page INFOS     (P)",  // m0_0E_PageInfos()
  "CONFIG. SYSTEME(M)",  // m0_1M_ConfigSysteme() Date, Time, N° LoRa
  "CONFIG. LoRa   (M)",  // m0_2M_ConnexReseau() DevEUI, AppEUI, SF, Délai
  "CALIB. Volt  (m03)",  // m0_3M_CalibVolt() VBat, Vsol, VLum
  "CALIB. BAL.  (m04)",  // m0_4M_calibBal() Bal: 1, 2, 3, 4
  "SAISIE DATE    (S)",  // m0_5F_GetDate()5 : test rapide date
  "SAISIE HEURE   (S)",  // m0_6F_GetTime()6 : Test rapide Time
  "SAISIE HEXA    (S)"   // m0_7F_GetHex()7 : Test saisie HEXA
};

#define M01_ITEM 7
const char* m01_ConfigSysteme[] = {
  "SAISIE DATE    (S)",  // m01_0F_GetDate()
  "SAISIE HEURE   (S)",  // m01_1F_GetTime()
  "NUM. RUCHER    (S)",  // m01_3F_Rucher()
  "Lire EEPROM    (F)",  // m01_4F_readConfig()
  "Ecrire EEPROM  (F)",  // m01_5F_writeConfig()
  "Reserve     (m013)",  // m01_6F_Reserve()
  "RET  popMenu(m0)"     // m01_7M_PopMenu()
};

#define M02_ITEM 5
const char* m02_ConfigLoRa[] = {
  "DevEUI         (S)",  // m02_0F_DevEUI()
  "AppEUI         (S)",  // m02_1F_AppEUI()
  "Spread Fact    (S)",  // m02_2F_SpreadFact()
  "PayLoad time   (S)",  // m02_3F_PayLoad()
  "RET  popMenu(m0)"     // m02_4M_PopMenu()
};

#define M03_ITEM 5
const char* m03_CalibTensions[] = {
  "Calib. VBAT    (F)",  // m03_0F_CalibVBat()
  "Calib. VSOL    (F)",  // m03_1F_CalibVSol()
  "Calib. LUM     (F)",  // m03_2F_CalibVLum()
  "Reserve     (m033)",  // m03_3F_Reserve()
  "RET  popMenu(m0)"     // m03_4M_PopMenu()
};

#define M033_ITEM 5
const char* m033_Reserve040[] = {
  "menu033-0       (F)",      // Mise à Echelle VBat
  "menu033-1       (F)",      // Mise à Echelle VSol
  "menu033-2       (F)",      // Mise à Echelle VLum
  "menu033-3       (F)",    // Libre
  "RET   popMenu (m03)"     // Retour menu principal
};

#define M04_ITEM 5
const char* m04_CalibBalances[] = {
  "Calib. Bal #1  (F)",  // m04_0F_CalibBal_1()
  "Calib. Bal #2  (F)",  // m04_1F_CalibBal_2()
  "Calib. Bal #3  (F)",  // m05_2F_CalibBal_3()
  "Calib. Bal #4  (F)",  // m05_3F_CalibBal_4()
  "RET  popMenu  (m0)"   // m05_4M_PopMenu()
};


#define M04x_ITEM 5
const char* m04x_CalibBal[] = {
  "Num. Balance    (S)", // m04x_0F_numBalx()
  "Tare Balance    (F)", // m04x_1F_tareBalx()
  "Echelle Balance (F)", // m04x_2F_echBalx()
  "Comp. Temp.     (F)", // m04x_3F_tempBalx()
  "RET   popMenu (m04)"     // Retour menu principal
};

#else

extern const char* m0_Demarrage[];
extern const char* m01_ConfigSysteme[];
extern const char* m02_ConfigLoRa[];
extern const char* m03_CalibTensions[];
extern const char* m033_Reserve040[];
extern const char* m04_CalibBalances[];
extern const char* m04x_CalibBal[];

#endif
