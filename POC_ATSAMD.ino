
//Virer 3 x colonne?0:0 valider sur 0.96"
// https://claude.ai/chat/75e1ba53-8b90-4f09-97a5-cfc366e36a8e
// 16:40:59.416 -> Alarme 2 (payload) programmée pour: 16:39:16  afficher a chaque reprogrammation
//  Compiled: Aug 22 2025, 08:34:58, 1.1.1-PL


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

#define __SendLoRa
//#define __SerialDebugPoc      // decommenter pour afficher messages debug

// =====  PROGRAMME =====
// ===== SETUP =====
void setup() 
{

// INITIALISE LA STRUCTURE A ENVOYER
Data_LoRa.rucher_ID = 67;   // compris entre 0 et 99
sprintf(Data_LoRa.RucherName,"LPCZ");
Data_LoRa.DHT_Temp = 25;
Data_LoRa.DHT_Hum = 69;
Data_LoRa.Brightness = 91;
Data_LoRa.Bat_Voltage = 3.69;
Data_LoRa.Solar_Voltage = 4.69;
Data_LoRa.HX711Weight[0] = 12.34; // 
Data_LoRa.HX711Weight[1] = 23.45;
Data_LoRa.HX711Weight[2] = 34.56;
Data_LoRa.HX711Weight[3] = 45.67;
Data_LoRa.ProcessorTemp = 21.18;   // temp µC, ne sera pas conservé 

  
  initDebugSerial(); 
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
  OLEDInit();
  OLEDDebugDisplay("OLEDInit OK"); // scrolling lors du setup(); 
  debugSerial.println("OLEDInit OK");
 
  // Initialisation configuration
  initConfig();
  OLEDDebugDisplay("initConfig OK");

  init2483A();
  sprintf(OLEDbuf,"ID: %s",Module_ID);
  OLEDDebugDisplay(OLEDbuf);

// config balance connue 10 lecture + moyenne
// poids peson doit être de 0 à 3 et GetPoids de 1..4 confirmé 06/06/2025
  Poids_Peson(0) = GetPoids(1,10);
  OLEDDebugDisplay("getWeight 1 Done");

  float temp = ((analogRead(TEMP_SENSOR) * 3300.0 / 1023.0) - 500.0) / 10.0; // Lecture temp µC  
            // put the ADC in sleep mode
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// temp : utiliser DHT22 si existe sinon temperature interne µC
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// Rappel:
// #define Poids_Peson(num)      Data_LoRa.HX711Weight[num]   //  Data_LoRa de type LoRa_Var (ligne 38)
// #define Tare_Peson(num)       Jauge[Peson[Ruche.Num_Carte][num]][0]
// #define Echelle_Peson(num)    Jauge[Peson[Ruche.Num_Carte][num]][1]
// #define BalPoids(num) (Contrainte_List[num]-Tare_Peson(num))/Echelle_Peson(num)/1000 //retourne float
// float GetPoids(int num)  // ?????   
// lecture brute => poids en gr
int num = 0;
float pesonTare  = Jauge[Peson[Ruche.Num_Carte][num]][0];  // correction pour avoir valeur balance sans charge.
float pesonScale = Jauge[Peson[Ruche.Num_Carte][num]][1];  // Mise à l'Echelle de pesonValue vers peson Poids
float pesonValue = Poids_Peson(num) - pesonTare;           // valeur à convertir
float pesonPoids = pesonValue / pesonScale;                // poids correspondant
float pesee;                                               // 
  //  Poids_List [num] = pesee*(1-Jauge[Peson[carte][num]][3]*(Jauge[Peson[carte][num]][2]/temp));
  Contrainte_List [num] = pesee; // *(1-Jauge[Peson[carte][num]][3]*(Jauge[Peson[carte][num]][2]/temp));
  // (peson-tare)/echelle
  pesee = BalPoids(num);   // ( pesee - Jauge[Peson[Ruche.Num_Carte][num]][0] ) / Jauge[Peson[Ruche.Num_Carte][num]][1] / 1000; 
  sprintf(serialbuf,"temp %5.2f, lu %5.2f, tare %5.2f, echelle %5.2f,  tare en moins %5.2f, poids %5.2f ",
          temp,Poids_Peson(num) , pesonTare, pesonScale,  pesonValue, pesonPoids  );
  debugSerial.println(serialbuf);
/*
15:20:10.162 ->  7892  7936  7884  7852  7785  7839  7999  8066  8076  7969  M 7929 
15:20:10.162 -> temp 28.39, lu 7929.00, tare 22005.70, echelle 97.49,  tare en moins -14076.70, poids -144.39 
*/


// poids peson doit être de 0 à 3 et GetPoids de 1..4 confirmé 06/06/2025
Poids_Peson(1) = GetPoids(2,10);
  OLEDDebugDisplay("getWeight 2 Done");
Poids_Peson(2) = GetPoids(3,10);
  OLEDDebugDisplay("getWeight 3 Done"); 
Poids_Peson(3) = GetPoids(4,10);
  OLEDDebugDisplay("getWeight 4 Done");

  initLoRa();

// void DS3231CompleteReset() si DS3231 out!



// Initialisation RTC
  initRTC();
// Configuration des alarmes RTC 1 et 2
if (DEBUG_INTERVAL_1SEC)
{
  debugSerial.println("Initialisation IRQ1");
  DS3231setRTCAlarm1();
}
if (DEBUG_WAKEUP_PAYLOAD)
{
  debugSerial.println("Initialisation IRQ2");
  DS3231setRTCAlarm2();
}
  OLEDDebugDisplay("Set RTC Alarms OK");
// Configuration interruption externe
  pinMode(RTC_INTERRUPT_PIN, INPUT_PULLUP);
// AJOUTÉ: Debug configuration
debugSerial.print("Configuration interruption sur pin ");
debugSerial.println(RTC_INTERRUPT_PIN);

/*
 * 
 * //bool DEBUG_WAKEUP_PAYLOAD = true;    // Activer/désactiver réveil payload
bool DEBUG_WAKEUP_PAYLOAD = false;    // Activer/désactiver réveil payload

//bool DEBUG_INTERVAL_1SEC = true;     // Activer/désactiver réveil 1 seconde
bool DEBUG_INTERVAL_1SEC = false;     // Activer/désactiver réveil 1 seconde

*/
// Configuration DS3231 AVANT interruption
//  rtc.writeSqwPinMode(DS3231_OFF);

  
  rtc.clearAlarm(1);
  rtc.clearAlarm(2);
  LowPower.attachInterruptWakeup(RTC_INTERRUPT_PIN, onRTCAlarm, FALLING);
debugSerial.println("Initialisation terminee");

//  forcerSynchronisationDS3231();
debugSerial.println("Mise à l'heure");

 
  dht.begin();
  read_DHT(dht);
//  sprintf(serialbuf,"Humidité : %.2f %%\nTemperature : %.2f °C",Data_LoRa.DHT_Hum,Data_LoRa.DHT_Temp);
//  debugSerial.println(serialbuf); 
  OLEDDebugDisplay("DHT Done");

getVBatMoy();   // calcul moyenne de 10 lectures
getVSolMoy();   // calcul moyenne de 10 lectures

 OLEDDebugDisplayReset();

 debugSerial.println("Start loop(); =====================================");

buildLoraPayload();
sendLoRaPayload((uint8_t*)payload,sizeof(payload)); //19);   // hex
setupDone = true;
}


// -----------------------------------------------------------------------------
// ===== LOOP PRINCIPAL =====
// -----------------------------------------------------------------------------
void loop() 
{  static int index=0; 
   static int counter1s=0,counter15m=0;   

//debugSerial.println("M");  // MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM

// Utilisable dans les 2 Modes
// *** APPEL OBLIGATOIRE À CHAQUE CYCLE ***
  gererLEDsNonBloquant(); 


// seulement en PROG  
// *** UTILISATION DES TOUCHES ***
// *** TRAITEMENT CLAVIER NON-BLOQUANT ***
  processContinuousKeyboard();
  touche = readKeyNonBlocking();      // lecture du clavier
  if (touche != KEY_NONE)               // Affiche la touche pressée  (KEY 1 à 5)
  {
    // Traiter la touche
    sprintf(serialbuf,"Touche pressée: %s",keyToString(touche));
    debugSerial.println(serialbuf);
  }
 
  
#ifdef __SerialDebugPoc
debugSerial.print("2");   // 22222222222222222222222
#endif  

// Affiche l'heure du prochain PayLoad si displayNextPayload == true     
 if (displayNextPayload)
 {
   displayNextPayload = false;
   debugSerialPrintNextAlarm(nextPayload,2);
//OLEDDrawScreenNextPayload(7, 0, nextPayload );  // Status message
 }


    
#ifdef __SerialDebugPoc
debugSerial.print("3");   // 333333333333333333333333333
#endif  


// Vérification du mode
  modeExploitation = digitalRead(PIN_PE);
  if (modeExploitation)         // OK, validé, GreenLED => couleur Red
  {
    handleOperationMode();    // normalement rien à faire dans ce mode.
  } 
  else                          // OK, validé, BlueLED
  {
    handleProgrammingMode();  // faire gestion Clavier et actions associées
  }

  
#ifdef __SerialDebugPoc  
debugSerial.print("4");   // 444444444444444444444444444444
#endif

if (!DEBUG_INTERVAL_1SEC)
{
static unsigned long noIRQSecLast=0;
  if (millis() - noIRQSecLast > 1000) // Clignotement toutes les 1s
    {
      noIRQSecLast = millis();
      wakeup1Sec = true;
    }
}


  if (wakeup1Sec) // && !modeExploitation)                  // màj heure, blink LED
  {    
#ifdef __SerialDebugPoc     
debugSerial.print("I1$");   //   I1$ I1$ I1$ I1$ I1$ I1$ I1$ I1$
#endif
    wakeup1Sec = false;   
    counter1s++;
    if (!(counter1s %60)) 
    {     
//      debugSerialPrintReprogNextAlarm(2);
      debugSerialPrintNextAlarm(nextPayload,2);
      debugSerial.print("Min/ menuDeep: ");    // MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
      debugSerial.println(currentMenuDepth);
    }
    switch (counter1s %10)
    {
      case 0 :    // Rafraichir OLED
  //             OLEDDisplayHivesDatas();  // ne pas executer quans saisies en cours....
               break;
      case 1 :   //  read_DHT(dht);  // Reading temperature or humidity takes about 250 milliseconds!
               read_DHT(dht); // initialise : Data_LoRa.DHT_Temp et Data_LoRa.DHT_Hum
               break;
     case 2 :
               Data_LoRa.Brightness = getLuminance();
//  Data_LoRa.Lux = ???();
//  Data_LoRa.ProcessorTemp = getTemperature(); // lecture Temp en String
               Data_LoRa.Bat_Voltage=getVBatMoy();
              Data_LoRa.Solar_Voltage=getVSolMoy();
               break;
     case 3 :
               Poids_Peson(0) =  GetPoids(1,1); // à ranger dans Structure 
               break;
     case 4 :
               Poids_Peson(1) = GetPoids(2,1); // à ranger dans Structure 
               break;
     case 5 :
               Poids_Peson(2) = GetPoids(3,1); // à ranger dans Structure 
               break;
     case 6 :
               Poids_Peson(3) = GetPoids(4,1); // à ranger dans Structure 
              break;
     case 7 :
              readingT=getTemperature();
               break;
      case 8 :

              break;
      case 9 :      // Alive: '.' sur OLED
              debugSerial.print(".");
              break;
      default : // WTF
               debugSerial.print("WTF");  
               break;
    }

#ifdef __SerialDebugPoc      
 debugSerial.print("5");    // 55555555555555555555555555555555555
#endif 
    config.applicatif.blueLedDuration = 100;  // Clignotement Blue = 100 ms
    LEDStartBlue();                           //Clignotement 100ms
//    OLEDDrawScreenRefreshTime(0, 0); // partial refresh Time/Date every second
  }
#ifdef __SerialDebugPoc  
debugSerial.print("6");   // 666666666666666666666666666666666666666666666
#endif
  if (wakeupPayload)                                    // Envoi LoRa, LED Activité LoRa
  {
    wakeupPayload = false;
    counter15m++;
//#ifdef __SerialDebugPoc    
sprintf(serialbuf, "I2£%d ", counter15m);  // I2£n I2£n I2£n I2£n I2£n I2£n I2£n 
debugSerial.println(serialbuf); 
//#endif
    turnOnRedLED();     // PCB donne GREEN?
    buildLoraPayload();
#ifdef __SendLoRa
  sendLoRaPayload((uint8_t*)payload,19);   // hex
#endif    
    turnOffRedLED();
    OLEDDrawScreenTime(0, 0); // Affiche Time/Date au complet
debugSerial.println("Fin Payload, Reactive IRQ1");    
    alarm1_enabled = true;   // Réactiver alarme 1 
#ifdef __SerialDebugPoc  
debugSerial.print("7");   // 777777777777777777777777777777777777
#endif

//tentative d'appel partout pour saisie d'une variable.... bloque, voir pourquoi
/*
startStringInput("SAISIE TEXTE:", Data_LoRa.RucherName, 20);
*/
  }
#ifdef __SerialDebugPoc    
debugSerial.print("8");   // 888888888888888888888888888888888888
#endif
// lire une grandeur à chaque seconde 
// 00 :  Rafraichir OLED
// 01 :  read_DHT(dht);  // Reading temperature or humidity takes about 250 milliseconds!
// 02 :  LDR, VBat, VSol   // lectures ANA (une seule lecture, moyenne calculée quand nécéssaire).
// 03 :  Poids Balance 1
// 04 :  Poids Balance 2
// 05 :  Poids Balance 3
// 06 :  Poids Balance 4
// 07 :  Température µC
// 08 : 
// 09 : '.' sur OLED
// rafraichir horloge à chaque seconde
// Flash LED 100 ms

  
// Entrée en veille si activée
  if (DEBUG_LOW_POWER && modeExploitation) // pas de low power en config
  {
debugSerial.println("Low");     
    debugSerial.flush();  
    sleep();
debugSerial.println("low"); 
  }
}
