//       1         2         3         4         5         6         7        7
//34567890123456789012345678901234567890123456789012345678901234567890123456789
// IMPRESSION 79 COLONES EN TAILLE 12
//
// ---------------------------------------------------------------------------*
//      _____           _        _                           _     
//     |  __ \         | |      | |                         | |   
//     | |__) | __ ___ | |_ ___ | |_ _   _ _ __   ___  ___  | |__ 
//     |  ___/ '__/ _ \| __/ _ \| __| | | | '_ \ / _ \/ __| | '_ \
//     | |   | | | (_) | || (_) | |_| |_| | |_) |  __/\__ \_| | | |
//     |_|   |_|  \___/ \__\___/ \__|\__, | .__/ \___||___(_)_| |_|
//                                    __/ | |                     
//                                   |___/|_|          
// ---------------------------------------------------------------------------*


// ===== PROTOTYPES DE FONCTIONS =====

// ===== PROTOTYPES DE FONCTIONS DE SETUP.CPP=====
void initDebugSerial(void);
void initLoRaSerial(void);
void softReset(void);
void DHTInit(void);
void setStructDefaultValues(void);


// ===== PROTOTYPES DE FONCTIONS DE POC_ATSAMD.INO=====

// ---------------------------------------------------------------------------*
// DS3231, gestion RTC et temps
void initRTC(void);                       // Initialise le module RTC DS3231
//DateTime getSystemTime(void);             // Retourne l'heure système actuelle
void DS3231setRTCAlarm1(void);                  // Configure alarme 1 du RTC
void DS3231setRTCAlarm2(void);                  // Configure alarme 2 du RTC
void clearRTCAlarms(void);                // Efface les alarmes du RTC
void testConnexionDS3231(void);           // Affiche heure et status Alarmes sur SerialDebug
void forcerSynchronisationDS3231(void);   //  Forcer une synchronisation immédiate
void synchronizeDS3231TimeToMicro(void);  // Synchro. heure µcontrôleur avec DS3231
void copyDS3231TimeToMicro(bool forcer);  // Copie heure DS3231 vers µcontrôleur avec option de forçage
void DS3231hardReset(void); 
void DS3231CompleteReset(void);

// ---------------------------------------------------------------------------*
// encore dans POC...cpp, déplacer?
// Gestion Config et EEPROM
//void setDefaultConfig(void);
void initConfig(void);

// ===== PROTOTYPES DE FONCTIONS EEPROM =====
void loadConfigFromEEPROM(void);
void readConfigFromEEPROM(void);
void saveConfigToEEPROM(void);
uint16_t calculateChecksum(ConfigGenerale_t* cfg);
void writeEEPROMByte(uint16_t address, uint8_t data);
uint8_t readEEPROMByte(uint16_t address);
void writeEEPROMBlock(uint16_t address, uint8_t* data, uint16_t length);
void readEEPROMBlock(uint16_t address, uint8_t* data, uint16_t length);
// void initDefaultConfig(void);  remplacé par  : setStructDefaultValues()
// Dump pour contôles
void dumpConfigToJSON(void);
void printByteArrayJSON(uint8_t* array, uint8_t length);




// ---------------------------------------------------------------------------*
// Fonctions de gestion des menus
void pushMenu(const char* title, const char** menuList, uint8_t menuSize, uint8_t initialIndex);
void popMenu(void); // Retour menu précédent
void backMenu(void); // Réaffiche le menu actuel après execution Fonction.
void backMenuFromList(void); // Réaffiche le menu actuel après Selection LISTE.
void processMenuSelection(uint8_t selectedIndex);

// ---------------------------------------------------------------------------*
// Appels du menu m0
void m0_0E_PageInfosSyst(void);
void m0_0E_PageInfosSystDone(void);
void m0_1M_ConfigSystem(void);
void m0_2M_ConfigLoRa(void);
void m0_3M_CalibTensions(void);
void m0_4M_CalibBal(void);


// ---------------------------------------------------------------------------*
// Appels du menu m01

void m01_0F_GetDate(void);
void m01_0F_GetDateDone(void);

void m01_1F_GetTime(void);
void m01_1F_GetTimeDone(void);

void m01_2F_GetNumRucher(void);
void m01_2F_GetNumRucherDone(void);

void m01_3F_GetNameRucher(void);
void m01_3F_GetNameRucherDone(void);

void m01_4F_readConfig(void);
void m01_4F_readConfigDone(void);

void m01_5F_writeConfig(void);
void m01_5F_writeConfigDone(void);

// ---------------------------------------------------------------------------*
// Appels du menu m02
void m02_0E_PageInfosLoRa(void);     // Page info
void m02_0E_PageInfosLoRaDone(void);

void m02_1F_AppKEY(void);       // AppKEY
void m02_1F_AppKEYDone(void);

void m02_2F_AppEUI(void);       // AppEUI
void m02_2F_AppEUIDone(void);

void m02_3L_GetSF(void);        // SF d'après ListeSF
void m02_3L_GetSFDone(void);

void m02_4F_GetPayloadDelay(void);  // Periodicité Payload
void m02_4F_GetPayloadDelayDone(void);

void m02_5F_Join(void); // Connexion LoRa
void m02_5F_JoinDone(void); // Connexion LoRa
  
void m02_6F_SendPayLoad(void); // Envoyer Payload  

// ---------------------------------------------------------------------------*
// Appels du menu m03

// modif en cours:
void m03_0E_DisplayVoltageInput(void);



void m03_0F_CalibVBat(void);      // appel écran de calibration
void m03_0F_CalibVBatDone(void);
void m03_1F_CalibVSol(void);      // appel écran de calibration
void m03_1F_CalibVSolDone(void);
void m03_2F_CalibVLum(void);      // appel écran de calibration
void m03_2F_CalibVLumDone(void);   // 
void m03_3F_Reserve(void);  
void m03_3F_ReserveDone(void); 

/*
// Appels du menu 033
void m033_0(void);
void m033_1(void);
void m033_2(void);
void m033_3(void);
void m033_4(void);
*/

// ---------------------------------------------------------------------------*
// Appels du menu m04 => appel calibrations de bal sélectionnée

void m04_nM_CalibBal_bal(void); // appel menu calib#bal des paramètre

void m04_2F_CalibBal_1(void); // appel menu calib#1 des paramètre
void m04_1F_CalibBal_2(void); // appel menu calib#2 des paramètre
void m04_2F_CalibBal_3(void); // appel menu calib#3 des paramètre
void m04_3F_CalibBal_4(void); // appel menu calib#4 des paramètre
void m04_0F_InfoBal(void);    // 
void m04_0F_InfoBalDone(void);   
void m04_1F_PoidsBal(void);   //
void m04_1F_PoidsBalDone(void);

// ---------------------------------------------------------------------------*
// Appels du menu m04x => appel écrans de calibrations
void m04x_0F_tareBal_1(void);    // appel écran de calibration
void m04x_1F_echelleBal_2(void); // appel écran de calibration
void m04x_2F_tempBal_3(void);    // appel écran de calibration

// ---------------------------------------------------------------------------*
// Fonctions de gestion des calibrations
// VBat

// VLum

// VSol

// tare

// echelle

// temp




// ---------------------------------------------------------------------------*
// Fonctions de gestion des ecrans infos
void displayInfoScreen(void);
infoScreenState_t processInfoScreen(void);
bool isInfoScreenActive(void);

// ---------------------------------------------------------------------------*
// Fonctions de sélection dans liste
void initStartupList(void);
void startListInput(const char* title, const char** itemList, uint8_t numItems, uint8_t initialIndex, unsigned long timeoutMs);
listInputState_t processListInput(void);
uint8_t finalizeListInput(char* outputItem);
void cancelListInput(void);
bool isListInputActive(void);
void refreshListDisplay(void);
void updateListInputCursorBlink(void);


// ---------------------------------------------------------------------------*
// Fonctions de saisie numérique
void startNumInput(const char* title, const char* initialNum, uint8_t maxLen, bool allowNeg, bool allowDec, long minVal, long maxVal);
numInputState_t processNumInput(void);
void finalizeNumInput(char* outputNum);
void cancelNumInput(void);
bool isNumInputActive(void);
void refreshNumDisplay(void);
void updateNumInputCursorBlink(void);
void updateNumDisplayOffset(void);
bool isNumValid(const char *num, bool allowNeg, bool allowDec, long minVal, long maxVal);
void insertNumCharAtPosition(char *num, uint8_t *length, uint8_t pos, char c);
void deleteCharAtPosition(char *num, uint8_t *length, uint8_t pos);
char getNextNumChar(char current, int delta, bool allowNeg, bool allowDec);



/*
// 
// 
// void startNumberInput(const char* title, const char* initialNumber, uint8_t maxLength, bool allowNegative);

// WithTimeout
void startNumberInputWithTimeout(const char* title, const char* initialNumber, uint8_t maxLength, bool allowNegative, unsigned long timeoutMs);
numberInputState_t processNumberInput(void);
void finalizeNumberInput(char* outputNumber);
void cancelNumberInput(void);
bool isNumberInputActive(void);
void refreshNumberDisplay(void);
void updateNumberInputCursorBlink(void);
char getNextNumericChar(char current, int delta, bool allowNegative, uint8_t position);
void modifyNumberChar(char* str, uint8_t pos, int delta);
bool isValidNumber(const char* number);
*/

// ---------------------------------------------------------------------------*
// Fonctions de saisie alphanumérique
void startStringInput(const char* title, const char* initialString, uint8_t maxLength);
// WithTimeout
void startStringInputWithTimeout(const char* title, const char* initialString, uint8_t maxLength, unsigned long timeoutMs);

stringInputState_t processStringInput(void);
void finalizeStringInput(char* outputString);
void cancelStringInput(void);
bool isStringInputActive(void);
void refreshStringDisplay(void);
void updateStringInputCursorBlink(void);
char getNextAlphaNumChar(char current, int delta);
void modifyStringChar(char* str, uint8_t pos, int delta);

// ---------------------------------------------------------------------------*
// Fonctions de saisie hexadécimale
void startHexInput(const char* title, const char* initialHex, uint8_t maxLen);
hexInputState_t processHexInput(void);
void finalizeHexInput(char* outputHex);
void cancelHexInput(void);
bool isHexInputActive(void);
void refreshHexDisplay(void);
void updateHexInputCursorBlink(void);
void updateHexDisplayOffset(void);
char getNextHexChar(char current, int delta);
void modifyHexDigit(char *hex, uint8_t pos, int delta);
bool isHexStringValid(const char *hex, uint8_t expectedLength);

// ---------------------------------------------------------------------------*
// Fonctions de saisie Time
void startTimeInput(const char* title, const char* initialTime);
timeInputState_t processTimeInput(void);
void finalizeTimeInput(char* outputTime);
void cancelTimeInput(void);
bool isTimeInputActive(void);
void refreshTimeDisplay(void);
void updateTimeInputCursorBlink(void);
uint8_t getNextValidTimePosition(uint8_t currentPos, bool forward);
bool isTimeValid(const char *t);
void modifyTimeDigit(char *t, uint8_t pos, int delta);


// ---------------------------------------------------------------------------*
// Fonctions de saisie de date
void startDateInput(const char* title, const char* initialDate);
dateInputState_t processDateInput(void);
void finalizeDateInput(char* outputDate);
void cancelDateInput(void);
bool isDateInputActive(void);
void refreshDateDisplay(void);
void updateDateInputCursorBlink(void);
uint8_t getNextValidDatePosition(uint8_t currentPos, bool forward);
bool isDateValid(const char *d);
void modifyDateDigit(char *d, uint8_t pos, int delta);


// ---------------------------------------------------------------------------*
// Fonctions de saisie email
void startEmailInput(const char* title, const char* initialEmail);
emailInputState_t processEmailInput(void);
void finalizeEmailInput(char* outputEmail);
void cancelEmailInput(void);
bool isEmailInputActive(void);
void refreshEmailDisplay(void);
void updateEmailInputCursorBlink(void);
void updateEmailDisplayOffset(void);
bool isEmailValid(const char *email);
void insertEmailCharAtPosition(char *email, uint8_t *length, uint8_t pos, char c);
void deleteEmailCharAtPosition(char *email, uint8_t *length, uint8_t pos);
char getNextEmailChar(char current, int delta);


// ---------------------------------------------------------------------------*
// Fonctions de saisie IP
void startIPInput(const char* title, const char* initialIP);
ipInputState_t processIPInput(void);
void finalizeIPInput(char* outputIP);
void cancelIPInput(void);
bool isIPInputActive(void);
void refreshIPDisplay(void);
void updateIPInputCursorBlink(void);
uint8_t getNextValidIPPosition(uint8_t currentPos, bool forward);
bool isIPValid(const char *ip);
void modifyIPDigit(char *ip, uint8_t pos, int delta);


// ---------------------------------------------------------------------------*
// Fonctions de saisie à definir
// 


// ---------------------------------------------------------------------------*
// Gestion LEDs
void initLEDs(void);        // Initialise les LEDs RGB et builtin
void turnOnRedLED(void);    // Allume la LED rouge                        (vert)
void turnOffRedLED(void);   // Éteint la LED rouge                        (vert)
void blinkRedLED(void);     // Fait clignoter la LED Rouge pendant 300ms  (vert)
void turnOnGreenLED(void);  // Allume la LED verte                        (rouge)
void turnOffGreenLED(void); // Éteint la LED verte                        (rouge)
void blinkGreenLED(void);   // Fait clignoter la LED verte pendant 300 ms (rouge)
void turnOnBlueLED(void);   // Allume la LED bleue
void turnOffBlueLED(void);  // Éteint la LED bleue
void blinkBlueLED(void);    // Fait clignoter la LED bleue pendant 300 ms 
void turnOnBuiltinLED(void);  // Allume la LED builtin (bleue)
void turnOffBuiltinLED(void); // Éteint la LED builtin (bleue)
void blinkBuiltinLED(void); //  Fait clignoter la LED builtin pendant BUILTIN_LED_DURATION ms
// Led non Blocking
void LEDStartRed(void);
void LEDStartBlue(void);
void LEDStartGreen(void);
void demarrerLEDBuiltin(void);
void gererLEDsNonBloquant(void);


// ---------------------------------------------------------------------------*
// Fonctions de conversions
void TestConvert(void);
// Conversions hexadécimales
uint8_t hexCharToNibble(char c);
char nibbleToHexChar(uint8_t nibble);
bool hexStringToByteArray(const char* hexString, uint8_t* byteArray, uint8_t maxBytes);
bool byteArrayToHexString(const uint8_t* byteArray, uint8_t numBytes, char* hexString, uint8_t maxChars);
void convertByteArray(const char *source, uint8_t *destination, uint8_t len);
void convertToHexString(const uint8_t *source, char *destination, uint8_t len);

// Conversions décimales uint8_t ↔ char buffer
bool uint8ToDecimalString(uint8_t value, char* buffer, uint8_t maxChars);
bool decimalStringToUint8(const char* buffer, uint8_t* value);
bool isValidLoRaWanSF(uint8_t sf);
bool validateLoRaWanSF(const char* sfString, uint8_t* sfValue);

// Affichage sur port Serie
void printByteArray(const uint8_t* byteArray, uint8_t length);
void printHexString(const char* hexString);


// ---------------------------------------------------------------------------*
// Gestion clavier
key_code_t readKeyOnce(void);   // Lecture instantanée d'une touche du clavier analogique
key_code_t readKey(void);       // Lecture avec anti-rebond du clavier analogique
const char* keyToString(key_code_t key);    //Convertit un code de touche en chaîne de caractères
key_code_t readKeyNonBlocking(void);    // Version non-bloquante de readKey() - Retourne immédiatement
void processContinuousKeyboard(void);   // Traite le clavier en continu (à appeler dans loop)

// ---------------------------------------------------------------------------*
// Gestion OLED
void OLEDInit(void);
void OLEDInit096(void);
void OLEDInit130(void);
void OLEDInit154(void);
void OLEDClear(void);
void nonprintOLED(uint8_t ligne, uint8_t colonne, const char *message);
void OLEDClearLine(uint8_t ligne);
void OLEDPrintChar(uint8_t ligne, uint8_t colonne, char c);
void OLEDPrintVar(uint8_t ligne, uint8_t colonne, const void *valeur, char type);
void OLEDPrintFormatted(uint8_t ligne, uint8_t colonne, const void *valeur, char type, const char *unite, int precision, char align);
void OLEDDisplayMessageL8(const char* message, bool defilant, bool inverse);
void OLEDDebugDisplay(char* message);
void OLEDDebugDisplayReset(void);
void OLEDDrawScreenNextPayload(uint8_t ligne, uint8_t colonne, DateTime nextPayload );
void OLEDDrawScreenTime(uint8_t ligne, uint8_t colonne);        // sous format: hh:mm:ss - dd/mm/yyyy
void OLEDDrawScreenRefreshTime(uint8_t ligne, uint8_t colonne); // que les valeurs modifiées
void OLEDDrawText(int8_t Txt_Size, uint8_t ligne, uint8_t colonne,const char *text);
void OLEDEraseText(int16_t col, int16_t lig, int16_t Ncar);   // !!!!!!!!!!!!!!!!!!! ordre param
void OLEDDisplayDate(char *d, uint8_t pos);
void OLEDDisplayTime(char *h, uint8_t pos);
void OLEDSetDebug(bool actif);
void OLEDDisplayHivesDatas(void);
void OLEDRefreshDisplay(void);
void OLEDRefreshlum(uint8_t ligne, uint8_t colonne);
void OLEDRefreshVlum(uint8_t ligne, uint8_t colonne);
void setCursorAdjusted(int16_t x, int16_t y);

//void OLEDDisplaySystemInfo(void);  // voir si pas remplacée par suivante.
void OLEDdisplayInfoScreenSyst(void);
void OLEDdisplayInfoScreenLoRa(void); 
void OLEDdisplayInfoBal(void);
void OLEDdisplayWeightBal(void);
void OLEDdisplayCalibBal(void);


// Gestion saisies

char* strToChar(String s);

// ---------------------------------------------------------------------------*
// Gestion serialDebug 
void debugSerialListStruct(void);
void debugSerialPrintMenuStruct(menuLevel_t* menu);
void debugSerialPrintNumberStruct(void);
void debugSerialPrintStringStruct(void);
void debugSerialTestConnexionDS3231(void);
void debugSerialPrintTimeComparison(void);     // Affiche heure système et heure RTC côte à côte pour comparaison
void debugSerialPrintTime(void); // Affiche heure et date système sur le port serialDebug
void debugSerialPrint2digits(int number); 
void debugSerialPrintLoraPayload(uint8_t *payload, uint8_t len);
void debugSerialPrintText(char *txt, char len);
void debugSerialPrintLoRaStatus();
void debugSerialPrintNextAlarm(DateTime nextPayload, int IRQ);
void debugSerialPrintKbdKey(void);
void debugSerialPrintSystemInfo(void); 
void debugSerialPrintHEXA(void);  // non appelée ???
void debugSerialDisplayScaledSensorState(int num);

// ---------------------------------------------------------------------------*
// Gestion modes et interruptions
void handleOperationMode(void);
void executeOperationMode(void);
void handleProgrammingMode(void);
void executeProgrammingMode(void);
// fonctions debug :
void GestionEnCours(char *from);   // affiche le type de traitement en cours de gestion par le handler
void restartGestionSaisieOLED(void);  // entrée dans mode Programmation
void onRTCAlarm(void);

// ---------------------------------------------------------------------------*
// Gestion modes basse consommation
void sleep(void);
void configureLowPowerMode(void);

// ---------------------------------------------------------------------------*
// Gestion RN2483 LoRa
// RS ?????
uint8_t RN2483Version(void);    // Lit les infos du modem
uint8_t RN2483GetCardNumber(void);

uint8_t init2483A(uint8_t *HWEUI); //   => dans setup.cpp
uint8_t RN2483Init(uint8_t *HWEUI);
void initLoRa(void);
//void getHWEUI(char *);      // Gets and stores the LoRa module's HWEUI   

// tests en cours
void getHWEUI(uint8_t *AppEUI); 



// void Reset_LoRa(void);
void clearLoRaBuffer(void); // Vide complètement le buffer série du modem
String readLoRaResponse(int timeoutMs); // Lit la réponse du modem avec timeout
 
bool setupLoRa(void);   // appelé /*par Send_Lora_Mess() et*/ setup()
bool setupLoRaOTAA(void);

//void no_Send_DATA_LoRa(void);

void buildLoraPayload(void);
void sendLoRaPayload(uint8_t *, uint8_t);


// basse conso.
//void sleep_LoRa(void);
//void wake_LoRa(void);

// ---------------------------------------------------------------------------*
// DHT22
// Read temp and Humidity with DHT22      
char read_DHT(DHT dht);


// ---------------------------------------------------------------------------*
// Gestion mesures
void take_All_Measure(void);
// HX711
float Set_Scale_Bal(char num, float poids_en_grammes);    // N° de jauges des balances 1 à 4
float GetPoids(int num,int sample);    // N° de jauges des balances 1 à 4

// µC
float getTemperature(void);

// µC ANA
float getVBatMoy(void);
float getVSolMoy(void);

// LDR ANA
float getLuminance(void);
