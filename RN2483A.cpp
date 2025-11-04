//       1         2         3         4         5         6         7        7
//34567890123456789012345678901234567890123456789012345678901234567890123456789
// IMPRESSION 79 COLONES EN TAILLE 12
//
// ---------------------------------------------------------------------------*
//      _____  _   _ ___  _  _   ___ ____                           
//     |  __ \| \ | |__ \| || | / _ \___ \   /\                     
//     | |__) |  \| |  ) | || || (_) |__) | /  \     ___ _ __  _ __ 
//     |  _  /| . ` | / /|__   _> _ <|__ < / /\ \   / __| '_ \| '_ \
//     | | \ \| |\  |/ /_   | || (_) |__) / ____ \ | (__| |_) | |_) |
//     |_|  \_\_| \_|____|  |_| \___/____/_/    \_(_)___| .__/| .__/
//                                                      | |   | |   
//                                                      |_|   |_|    
// ---------------------------------------------------------------------------*
// Projet Ruche PhLe
//
// RSSI entre 0 et -110 SF7, en dessous limite spread factor 12
//
// ---------------------------------------------------------------------------*
// Datas PROTO 1 envoyées le: 10/05/2020 10:41:03
//
//  Source:   devEUI : 0004A30B0024BF45              (=> carte 2 = ID Rucher)
//            deviceId : urn:lo:nsid:lora:0004A30B0024BF45 
//
//  2032       => 2         // 0  ID Rucher           xx
//  2032312e31 => 21.1      // 2  Temp DHT en °C      xx,xx
//  2035392e33 => 59.3      // 7  Hum DHT en %        xx,xX
//  2039342e39 => 94.9      // 12 %Lum en LUX         xxxxx
//  20302e3030 => 0.00      // 17 Tension BAT en V    x,xxx (pas de batterie)
//  33312e3139 => 31.19     // 22 masse Ruche 1 en kg xx,xx
//  33312e3939 => 31.99     // 27 masse Ruche 2 en kg xx,xx
//  31372e3036 => 17.06     // 32 masse Ruche 3 en kg xx,xx   
//  31382e3437 => 18.47     // 37 masse Ruche 4 en kg xx,xx
//  20332e3337 => 3.37???   // 42 Tension Solaire     x,xxx (Pas de panneau)
//  PB à diagnostiquer sur carte => pas de charge Batterie
//
//    Rssi,   Snr,   Esp,        Sf
//    -103    2      -105.12     9
//
// Gateway: 10
// Lat/Lon: 48.57322,7.655769 
// ---------------------------------------------------------------------------*


// RN 2483 freq par défaut 868.1 868.3 et 868.5 data rate 0 à 5; 30% duty cycle

//#define NO_Tx

#define __INIT_DONE
#include "define.h"

// ---------------------------------------------------------------------------*
// Vide complètement le buffer série du modem
// Paramètres : Aucun
// Retour : void (aucun retour)
// ---------------------------------------------------------------------------*
void clearLoRaBuffer()
{
  while (loraSerial.available())
  {
    loraSerial.read();  // Jeter tous les caractères
  }
  delay(10);  // Petite pause pour être sûr
}

// ---------------------------------------------------------------------------*
// Lit la réponse du modem avec timeout
// Paramètres :
//   - timeoutMs (int) : Timeout en millisecondes
// Retour : String - Réponse complète du modem
// ---------------------------------------------------------------------------*
String readLoRaResponse(int timeoutMs = 1000)
{
  String response = "";
  unsigned long startTime = millis();
  
  while (millis() - startTime < timeoutMs)
  {
    if (loraSerial.available())
    {
      char c = loraSerial.read();
      response += c;
      
      // Arrêter à la fin de ligne
      if (c == '\n')
      {
        break;
      }
    }
  }
  response.trim();  // Enlever \r\n
  return response;
}

// ---------------------------------------------------------------------------*
// @brief Lit les infos du modem
// @param Aucun
//   - timeoutMs (int) : Timeout en millisecondes
// @return String - Réponse complète du modem
// ---------------------------------------------------------------------------*
uint8_t RN2483Version(void)   
{ uint8_t len;

  clearLoRaBuffer();
  loraSerial.println("sys get ver"); // RN2483 1.0.5 Oct 31 2018 15:06:52
   delay(100);
  len = loraSerial.readBytesUntil(0x20, serialbuf, 33); //sizeof(Module_ID));
// envoi sur moniteur RS résultat getHWEUI()
debugSerial.print("Version: ");  debugSerial.print(len);
debugSerial.print(" car. lus: "); debugSerial.println(serialbuf);
//  clearLoRaBuffer(); // normalement buffer vide.
// Fin test communic ation avec RN2483A
}


// ---------------------------------------------------------------------------*
//  out : 0 (OK) / 1 (Error)                          
// ---------------------------------------------------------------------------*
uint8_t Init_2483(void)   
{
/*
uint8_t len;

  clearLoRaBuffer();
  loraSerial.println("sys get ver"); // RN2483 1.0.5 Oct 31 2018 15:06:52
   delay(100);
  len = loraSerial.readBytesUntil(0x20, serialbuf, 33); //sizeof(Module_ID));
// envoi sur moniteur RS résultat getHWEUI()
debugSerial.print("Version: ");  debugSerial.print(len);
debugSerial.print(" car. lus: "); debugSerial.println(serialbuf);
*/
 
//  clearLoRaBuffer(); // normalement buffer vide.
// Fin test communic ation avec RN2483A

  getHWEUI(&Module_ID[0]);     // get the Hardware DevEUI

  sprintf(serialbuf,"Init_2483() ID. module: %s",Module_ID ); 
debugSerial.println(serialbuf);


//OLEDDebugDisplay(OLEDbuf);
//debugSerial.println("------------------------------------------------------------------");
 
  for (Ruche.Num_Carte=0;Ruche.Num_Carte< MAX_HWEUI_List; Ruche.Num_Carte++)
  { 
    if (strncmp(HWEUI_List[Ruche.Num_Carte],Module_ID,16)==0)   // égalité des 2 
    {
// INITIALISER LES IDENTIFIANTS OTAA de LoRa + Contrôle
      DevEUI = (uint8_t *)SN2483_List[Ruche.Num_Carte];
// Init AppEUI
      AppEUI = (uint8_t *)AppEUI_List[Ruche.Num_Carte];
// Init AppKey
      AppKey = (uint8_t *)AppKey_List[Ruche.Num_Carte];
       break;  // sortie du for() quand trouvé egalité.... pas terrible!!!	
	  }
 //debugSerial.println("HWEUI non trouvé");
  } 
  return((Ruche.Num_Carte == MAX_HWEUI_List) ? 1 : 0); 
}


// ---------------------------------------------------------------------------*
// Gets and stores the LoRa module's HWEUI                              
// ---------------------------------------------------------------------------*
void getHWEUI(char *Module_ID)
{ uint8_t len;

  clearLoRaBuffer();
  loraSerial.println("sys get hweui");  
   delay(200);
  len = loraSerial.readBytesUntil(0x20, Module_ID, 16); //sizeof(Module_ID));
// envoi sur moniteur RS résultat getHWEUI()
debugSerial.print("getHWEUI(): ");  debugSerial.print(len);
debugSerial.print(" car. lus: "); debugSerial.println(Module_ID);
}
// testée OK le 12/02/2020


// appelé par Send_Lora_Mess() et setup()
bool setupLoRa()
{ bool result;
 
  result=setupLoRaOTAA();
  return(result);
}

// ---------------------------------------------------------------------------*
// connexion au reseau
//  in : 
//  out : 1 OK, 0 Echec                             
// ---------------------------------------------------------------------------*
bool setupLoRaOTAA()
{ if (LoRaBee.initOTA(loraSerial, DevEUI, AppEUI, AppKey, true))
  {
    debugSerial.println("setupLoRaOTAA(), Network connection successfull");
    LoRaBee.setSpreadingFactor(LoRa_Config.SpreadingFactor); // 7, 9 et 12 echec freudeneck
    return(1);
  }
  else  // ordre des lignes changé le 07/04/25, 
  { 
    debugSerialPrintLoRaStatus();
    debugSerial.println("setupLoRaOTAA(), Network connection failed!");
    return(0);
  }
}

// ---------------------------------------------------------------------------*
// Construit le message HEXA                              
// faire: 
// in : pointeur Payload , Ptr Struct datas
// out : -
// #define PAYLOADSIZE 27
// int payloadSize = PAYLOADSIZE; 
// byte payload[PAYLOADSIZE];
//  => hexPayload sera envoyé???? est ce bien juste?
// à quoi correspond payload ????
// ---------------------------------------------------------------------------*
void buildLoraPayload(void)
{ int indice = 0, i;

debugSerial.println("buildLoraPayload, datas:");
  
  payload[indice++] = Data_LoRa.rucher_ID;
//debugSerial.println(Data_LoRa.rucher_ID);

  int temperature = (int)(Data_LoRa.DHT_Temp * 100); // We multiple the values below because we then divide them on AllThingsTalk and don't need to send floats which use more data
  payload[indice++] = ((uint8_t*)&temperature)[0];          // Temperature: Least significant byte first, little endian
  payload[indice++] = ((uint8_t*)&temperature)[1];
//debugSerial.println(temperature);
  
  int humidity = (int)Data_LoRa.DHT_Hum * 100;
  payload[indice++] = ((uint8_t*)&humidity)[0]; // Humidity
  payload[indice++] = ((uint8_t*)&humidity)[1];
//debugSerial.println(humidity);

  sprintf(serialbuf,"Rucher: %d  [RucherName: %s] temp: %d, Hum.: %d",Data_LoRa.rucher_ID, Data_LoRa.RucherName, temperature, humidity);
  debugSerial.println(serialbuf);
  
  int Brightness = (int)Data_LoRa.Brightness;
  payload[indice++] = ((uint8_t*)&Brightness)[0]; // Brightness
  payload[indice++] = ((uint8_t*)&Brightness)[1];
//debugSerial.println(Brightness);
    
  // tension moyenne des 10 dernières lectures
  int VBat = (int)(Data_LoRa.Bat_Voltage * 100); // We multiple the values below because we then divide them on AllThingsTalk and don't need to send floats which use more data
  payload[indice++] = ((uint8_t*)&VBat)[0];  // Battery Voltage
  payload[indice++] = ((uint8_t*)&VBat)[1];
//debugSerial.print(VBat);
  
  // tension moyenne des 10 dernières lectures
  int VSol = (int)(Data_LoRa.Solar_Voltage * 100); // multiple then divide: don't need to send floats
  payload[indice++] = ((uint8_t*)&VSol)[0];  // Solar Panel Voltage
  payload[indice++] = ((uint8_t*)&VSol)[1];
//debugSerial.println(VSol);

  sprintf(serialbuf,"Lum:    %d Vbat: %d,  Vsol: %d",Brightness, VBat, VSol);
  debugSerial.println(serialbuf);
 

  for (i = 0; i < 4; i++) 
  {
//debugSerial.print(i);  debugSerial.print("/");debugSerial.println(indice);
    int Masse = Data_LoRa.HX711Weight[i] *100; //(int)(Data_LoRa.HX711Weight[i]// 100); 
    payload[indice++] = ((uint8_t*)&Masse)[0];	// Hive1 to 4 Weight
    payload[indice++] = ((uint8_t*)&Masse)[1];
//debugSerial.println(Masse);

  sprintf(serialbuf,"Masse%d: %d / ",i, Masse);
  debugSerial.print(serialbuf);
  }
debugSerial.println("");
  
//debugSerial.print("final i/payload[indice]"); debugSerial.print(i);  debugSerial.print("/");
//debugSerial.println(indice);

// c'est quoi la suite?
// rappel : char hexPayload[]

  for (i = 0; i < payloadSize; i++) 
  {
    sprintf(&hexPayload[i * 2], "%02X.", payload[i]);  // pourquoi le . dans "%02X."
  }
// remplissage Payload de '7'                 // 7 car visible, mettre 0 quand validé
  for (i = payloadSize * 2; i < hexPayloadSize; i++) // complete si size HEXPAYLOAD > size Payload*2
  {
    hexPayload[i] = '7'; // Pad with leading zeros to reach the required nn bytes
  }
  hexPayload[hexPayloadSize - 1] = '\0'; // Null terminate
//debugSerial.print("final hexpayload[i]"); debugSerial.println(i); 
debugSerial.print("(fin buildLoraPayload) hexPayload: "); debugSerial.println(hexPayload); // chaine de caractère HEXA de Payload
}


// ---------------------------------------------------------------------------*
// envoi toutes les IRQ2
// ---------------------------------------------------------------------------*
void sendLoRaPayload(uint8_t *Datas,uint8_t len)
{ 
debugSerialPrintLoraPayload(Datas,len);
debugSerial.println("appel LoRaBee.send");

    switch (LoRaBee.send(1,Datas,len))
    {
    case NoError:
      debugSerial.println("\nSuccessful transmission in Send_LoRa_Mess().");
      break;
    case NoResponse:
      debugSerial.println("\nThere was no response from the device in Send_LoRa_Mess().");
      break;
    case Timeout:
      debugSerial.println("\nConnection timed-out in Send_LoRa_Mess(). Check your serial connection to the device! Sleeping for 20sec.");
      delay(20000);
      break;
    case PayloadSizeError:
      debugSerial.println("\nThe size of the payload is greater than allowed in Send_LoRa_Mess(). Transmission failed!");
    break;
    case InternalError:
      debugSerial.print("\nOh No! This shouldn't happen in Send_LoRa_Mess(). Something is really wrong! The program will reset the RN module...");
      setupLoRa();
      // renvoyer MESSAGE
   LoRaBee.send(1,Datas, len);
    break;
    case Busy:
      debugSerial.println("\nThe device is busy in Send_LoRa_Mess(). Sleeping for 10 extra seconds.");
      delay(10000);
      break;
    case NetworkFatalError:
      debugSerial.println("\nThere is a non-recoverable error with the network connection in Send_LoRa_Mess(). The program will reset the RN module...");
      setupLoRa();
       // renvoyer MESSAGE
   LoRaBee.send(1,Datas, len);
       break;
    case NotConnected:
      debugSerial.print("\nThe device is not connected to the network in Send_LoRa_Mess(). The program will reset the RN module...");
      setupLoRa();
      // renvoyer MESSAGE
  LoRaBee.send(1,Datas, len);
      break;
    case NoAcknowledgment:
      debugSerial.print("\nThere was no acknowledgment sent back! in Send_LoRa_Mess()");
      break;
    default:
      break;
    }  // Fin Switch
}



// ---------------------------------------------------------------------------*
// INITIALISE LA CHAINE TEXTE A ENVOYER                   
// ---------------------------------------------------------------------------*
String Build_Lora_String(String dataGrafana)
{ 
// N° carte
    sprintf(serialbuf, "%2c",'3');         // ????? %2c = '3' ?????
    dataGrafana +=serialbuf; 
    
// température DHT
    sprintf(serialbuf, "%5.1f",Data_LoRa.DHT_Temp);
    dataGrafana +=serialbuf;  
/*
// température Peson
    sprintf(serialbuf,"%5.1f",Temp_Peson(2));        // DHT_Hum); sp
    dataGrafana +=serialbuf; 
*/
// Luminance LDR
    sprintf(serialbuf, "%5.1f",Data_LoRa.Brightness);        // DHT_Hum);sp
    dataGrafana +=serialbuf;//Data_LoRa.Brightness;  
 
// Tension BAT
   sprintf(serialbuf, "%5.2f",Data_LoRa.Bat_Voltage);  // 2 fois???
    dataGrafana +=serialbuf; 
// MASSE A
   sprintf(serialbuf, "%5.2f",Poids_Peson(4));   // au lieu de 1
    dataGrafana +=serialbuf; 


// MASSE B
   sprintf(serialbuf, "%5.2f",Poids_Peson(2));   
     dataGrafana +=serialbuf;  
     
// MASSE C
   sprintf(serialbuf, "%5.2f",Poids_Peson(3));  
   dataGrafana +=serialbuf; 
   
// MASSE D   peson 2 brut
   sprintf(serialbuf,"%5.2f",Contrainte_List [1]); //Data_LoRa.HX711Weight_4);   

   sprintf(serialbuf,"%5.2f",Data_LoRa.Bat_Voltage); // 2 fois???
    dataGrafana +=serialbuf;  

// tension panneau solaire
   sprintf(serialbuf, "%5.2f",Data_LoRa.Solar_Voltage);
    dataGrafana +=serialbuf; 

// remplace pour débug
// Température
//    readingT = getTemperature(); // lecture Temp en String
//    sprintf(&testDataBase[37],"/%5.1f",readingT.toFloat());  

return(dataGrafana);
//return(" 3 17.0 99.0 72.7 4.0813.5453.24 8.02 4.08 3.30");  // 47 car
}



// ---------------------------------------------------------------------------*
//                               
// ---------------------------------------------------------------------------*
String Build_CSV_String(String dataString)
{  // INITIALISE LA CHAINE TEXTE A ENVOYER


//2021.03.28 19:10:53; 311.90;13.31;  1.9; 4.04;13.36;52.91; 8.03;1222475.25; 0.00;
// Date; carte; DHT_T; temp2; LDR; VBat; masse1; masse2; masse3; Contrainte 2; VSol

// N° carte
    sprintf(serialbuf, "%2c;",'3');
    dataString +=serialbuf; 
    
// température DHT
    sprintf(serialbuf,"%5.2f;",Data_LoRa.DHT_Temp);
    dataString +=serialbuf; 
/*    
// température Peson
    sprintf(serialbuf,"%5.2f;",Temp_Peson(2));        // DHT_Hum); sp
    dataString +=serialbuf;
*/
    
// Luminance LDR
    sprintf(serialbuf, "%5.1f;",Data_LoRa.Brightness);        // DHT_Hum);sp
    dataString +=serialbuf; //Data_LoRa.Brightness; 
 
// Tension BAT
   sprintf(serialbuf, "%5.2f;",Data_LoRa.Bat_Voltage);  // 2 fois???
   dataString +=serialbuf;

// MASSE A
   sprintf(serialbuf, "%5.2f;",Poids_Peson(1));   
   dataString +=serialbuf;

// MASSE B
   sprintf(serialbuf, "%5.2f;",Poids_Peson(2));   
   dataString +=serialbuf;

// MASSE C
   sprintf(serialbuf, "%5.2f;",Poids_Peson(3));  
   dataString +=serialbuf;  

// MASSE D   peson 2 brut
   sprintf(serialbuf, "%5.2f;",Contrainte_List [1]); //Poids(4));   
   dataString +=serialbuf;

// tension panneau solaire
   sprintf(serialbuf,"%5.2f;",Data_LoRa.Solar_Voltage);
   dataString +=serialbuf;

// remplace pour débug
// Température
//    readingT = getTemperature(); // lecture Temp en String
//    sprintf(&testDataBase[37],"/%5.1f",readingT.toFloat());  
return(dataString);
}

// ---------------------------------------------------------------------------*
//                               
// ---------------------------------------------------------------------------*
void Send_DATA_LoRa()
{
  static char Num_Mess=0;
  uint8_t lum=0;
  String readingL ,readingT; 
//String Local_LoRa_Buffer="";
  String dataString = "";
  String dataGrafana = "";

/*
// pourquoi pas dans un struct.h (mis en commentaire!)?????
typedef struct   // Mess2
{
  char rucher_ID[2];       // ID Rucher         xx  1
  char readingDHT_T[5];    // Temp DHT en °C   xx,xx 2 
  char readingDHT_H[5];    // Hum DHT en %      xx,x
  char readingL[5];        // %Lum en LUX      xxxxx
  char readingBat[5];      // Tension BAT en V     x,xxx
  char reading_M1[5];      // masse Ruche 1 en kg xx,xx
  char reading_M2[5];      // masse Ruche 2 en kg xx,xx
  char reading_M3[5];      // masse Ruche 3 en kg xx,xx
  char reading_M4[5];      // masse Ruche 4 en kg xx,xx
 // char readingT[5];        // temp µC, ne sera pas conservé xx.xx
} LoRa_VarText; 

LoRa_VarText LoRa_Dwata;
*/
 
/*
// INITIALISE LA STRUCTURE A ENVOYER
Data_LoRa.rucher_ID = 67;   // compris entre 0 et 99
Data_LoRa.DHT_Temp = 25;
Data_LoRa.DHT_Hum = 04;
Data_LoRa.Brightness = 91;
Data_LoRa.Bat_Voltage = 3,69;
Data_LoRa.Solar_Voltage = 4.69;
Data_LoRa.temp_1=111;
Data_LoRa.HX711Weight_1 = 12.34;
Data_LoRa.temp_2=222;
Data_LoRa.HX711Weight_2 = 23.45;
Data_LoRa.temp_3=333;
Data_LoRa.HX711Weight_3 = 34.56;
Data_LoRa.temp_4=444;
Data_LoRa.HX711Weight_4 = 45.67;

Data_LoRa.ProcessorTemp=21,18;   // temp µC, ne sera pas conservé 


  read_DHT(dht); // initialise : Data_LoRa.DHT_Temp et Data_LoRa.DHT_Hum
  Data_LoRa.Brightness = getLuminance();
//  Data_LoRa.Lux = ???();
  Data_LoRa.ProcessorTemp = getTemperature(); // lecture Temp en String
  Data_LoRa.Solar_Voltage=getVSolMoy();
  Data_LoRa.Bat_Voltage=getVBatMoy();
  Data_LoRa.temp_2 = get_DS();
#ifdef WEIGHT_YES
  Data_LoRa.HX711Weight_1 = GetPoids(1); // à ranger dans Structure
  Data_LoRa.HX711Weight_2 = GetPoids(2); // à ranger dans Structure
  Data_LoRa.HX711Weight_3 = GetPoids(3); // à ranger dans Structure
  Data_LoRa.HX711Weight_4 = GetPoids(4); // à ranger dans Structure
#endif // WEIGHT_YES
*/

// ENVOI LA STRUCTURE ou temporairement sa version lisible par l'humain...


dataGrafana=Build_Lora_String(dataGrafana);
dataString=Build_CSV_String(dataString);

char chardata[256]="";

 dataGrafana.toCharArray(chardata,dataGrafana.length()); //48); //sizeof(dataGrafana));
 
 debugSerial.println("-------------------------------------------------------------------------------------------------------------------------------------------------");
 debugSerial.println(dataString);
 debugSerial.println(dataGrafana);
 debugSerial.println(chardata); 
 debugSerial.println("-------------------------------------------------------------------------------------------------------------------------------------------------");
  sendLoRaPayload((uint8_t *)chardata,47);
}

/*
// ---------------------------------------------------------------------------*
//                               
// ---------------------------------------------------------------------------*
void Reset_LoRa(void)  //Hardreset the RN module
{ 
  debugSerial.println("Hardreset the RN module LoRa");
  digitalWrite(LORA_RESET, LOW);
  delay(100);
  digitalWrite(LORA_RESET, HIGH);
  delay(1000);
  clearLoRaBuffer();
}

bool LoRa_sleeps = false;

// ---------------------------------------------------------------------------*
// Sleep commands
// ---------------------------------------------------------------------------*
//                               
// ---------------------------------------------------------------------------*
void sleep_LoRa()
{
    loraSerial.flush();
    LoRaBee.sleep();
    LoRa_sleeps=true;
    sodaq_wdt_safe_delay(5); // without this, it doesn’t sleep.. don’t know why
}

// ---------------------------------------------------------------------------*
//                               
// ---------------------------------------------------------------------------*
void wake_LoRa()
{
    LoRa_sleeps = false;
    LoRaBee.wakeUp();
}
*/
