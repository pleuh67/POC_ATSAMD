// ===== PROTOTYPES DE FONCTIONS =====

// ===== PROTOTYPES DE FONCTIONS DE SETUP.CPP=====
void initDebugSerial(void);
void softReset(void);
void DHTInit(void);


// ===== PROTOTYPES DE FONCTIONS DE POC_ATSAMD.INO=====
// DS3231, gestion RTC et temps
void initRTC(void);                       // Initialise le module RTC DS3231
DateTime getSystemTime(void);             // Retourne l'heure système actuelle
void DS3231setRTCAlarm1(void);                  // Configure alarme 1 du RTC
void DS3231setRTCAlarm2(void);                  // Configure alarme 2 du RTC
void clearRTCAlarms(void);                // Efface les alarmes du RTC
void testConnexionDS3231(void);           // Affiche heure et status Alarmes sur SerialDebug
void forcerSynchronisationDS3231(void);   //  Forcer une synchronisation immédiate
void synchronizeDS3231TimeToMicro(void);  // Synchro. heure µcontrôleur avec DS3231
void copyDS3231TimeToMicro(bool forcer);  // Copie heure DS3231 vers µcontrôleur avec option de forçage
void DS3231hardReset(void); 
void DS3231CompleteReset(void);

// encore dans POC...cpp, déplacer?
// Gestion Config et EEPROM
void setDefaultConfig(void);
void initConfig(void);
void loadConfigFromEEPROM(void);
void readConfigFromEEPROM(void);
void saveConfigToEEPROM(void);
uint16_t calculateChecksum(ConfigGenerale_t* cfg);


// Fonctions de sélection dans une liste
void startListInput(const char* title, const char** itemList, uint8_t numItems, uint8_t initialIndex);
listInputState_t processListInput(void);
uint8_t finalizeListInput(void);
void cancelListInput(void);
bool isListInputActive(void);
void refreshListDisplay(void);
void updateListInputCursorBlink(void);


// Fonctions de saisie numérique
void startNumberInput(const char* title, const char* initialNumber, uint8_t maxLength, bool allowNegative);
numberInputState_t processNumberInput(void);
void finalizeNumberInput(char* outputNumber);
void cancelNumberInput(void);
bool isNumberInputActive(void);
void refreshNumberDisplay(void);
void updateNumberInputCursorBlink(void);
char getNextNumericChar(char current, int delta, bool allowNegative, uint8_t position);
void modifyNumberChar(char* str, uint8_t pos, int delta);
bool isValidNumber(const char* number);


// Fonctions de saisie alphanumérique
void startStringInput(const char* title, const char* initialString, uint8_t maxLength);
stringInputState_t processStringInput(void);
void finalizeStringInput(char* outputString);
void cancelStringInput(void);
bool isStringInputActive(void);
void refreshStringDisplay(void);
void updateStringInputCursorBlink(void);
char getNextAlphaNumChar(char current, int delta);
void modifyStringChar(char* str, uint8_t pos, int delta);


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


// Gestion clavier
key_code_t readKeyOnce(void);   // Lecture instantanée d'une touche du clavier analogique
key_code_t readKey(void);       // Lecture avec anti-rebond du clavier analogique
const char* keyToString(key_code_t key);    //Convertit un code de touche en chaîne de caractères
key_code_t readKeyNonBlocking(void);    // Version non-bloquante de readKey() - Retourne immédiatement
void processContinuousKeyboard(void);   // Traite le clavier en continu (à appeler dans loop)

// Gestion OLED
void OLEDInit(void);
void OLEDInit096(void);
void OLEDInit130(void);
void OLEDInit154(void);
void OLEDClear(void);
void nonprintOLED(uint8_t ligne, uint8_t colonne, const char *message);
void OLEDClearLine(uint8_t ligne);
void OLEDPrintChar(uint8_t ligne, uint8_t colonne, char c);
void OLEDPrintVar(uint8_t ligne, uint8_t col, const void *valeur, char type);
void OLEDPrintFormatted(uint8_t ligne, uint8_t col, const void *valeur, char type, const char *unite, int precision, char align);
void OLEDDisplayMessageL8(const char* message, bool defilant, bool inverse);
void OLEDDebugDisplay(char* message);
void OLEDDebugDisplayReset(void);
void OLEDDrawScreenNextPayload(uint8_t ligne, uint8_t colonne, DateTime nextPayload );
void OLEDDrawScreenTime(uint8_t ligne, uint8_t colonne);        // sous format: hh:mm:ss - dd/mm/yyyy
void OLEDDrawScreenRefreshTime(uint8_t ligne, uint8_t colonne); // que les valeurs modifiées
void OLEDDrawText(int8_t Txt_Size, uint8_t ligne, uint8_t colonne,const char *text);
void OLEDEraseText(int16_t col, int16_t lig, int16_t Ncar);
void OLEDDisplayDate(char *d, uint8_t pos);
void OLEDDisplayTime(char *h, uint8_t pos);
void OLEDSetDebug(bool actif);
void OLEDDisplayHivesDatas(void);
void OLEDDisplaySystemInfo(void);

// Gestion saisies

char* strToChar(String s);

// Gestion serialDebug 
void debugSerialListStruct(void);
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

// Gestion modes et interruptions
void handleOperationMode(void);
void executeOperationMode(void);
void handleProgrammingMode(void);
void executeProgrammingMode(void);


void onRTCAlarm(void);

// Gestion modes basse consommation
void sleep(void);
void configureLowPowerMode(void);

// Gestion RN2483 LoRa
// RS ?????
uint8_t RN2483Version(void);    // Lit les infos du modem


void init2483A(void);
uint8_t Init_2483(void);
void initLoRa(void);
void getHWEUI(char *);      // Gets and stores the LoRa module's HWEUI    
void Reset_LoRa(void);
void clearLoRaBuffer(void); // Vide complètement le buffer série du modem
String readLoRaResponse(int timeoutMs); // Lit la réponse du modem avec timeout
 
bool setupLoRa(void);   // appelé par Send_Lora_Mess() et setup()
bool setupLoRaOTAA(void);

void Send_DATA_LoRa(void);

void buildLoraPayload(void);
void sendLoRaPayload(uint8_t *, uint8_t);


// basse conso.
void sleep_LoRa(void);
void wake_LoRa(void);

// Gestion menus
void displaySystemInfo(void);
void handleConfigurationMenu(void);
void handleTimeDateRegisterMenu(void);
void handleDebugMenu(void);

// DHT22
// Read temp and Humidity with DHT22      
void read_DHT(DHT dht);


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
