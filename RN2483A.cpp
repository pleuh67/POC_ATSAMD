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

//#define NO_Tx

#define __INIT_DONE
#include "define.h"

// ---------------------------------------------------------------------------*
// Vide complètement le buffer série du modem
// Paramètres : Aucun
// Retour : void (aucun retour)
// ---------------------------------------------------------------------------*
void RN2483AclearLoRaBuffer()
{
  while (loraSerial.available())
  {
    loraSerial.read();  // Jeter tous les caractères
  }
  delay(10);  // Petite pause pour être sûr
}

// ---------------------------------------------------------------------------*
//  out : 0 (Error ou non référencé) / n (Num de carte)  
// @brief Find card number
//                Num_Carte (global) dans fonction
// @param : uint8_t *HWEUI =>SN2483_List
// @return :  0 error or Card number
// ---------------------------------------------------------------------------*
uint8_t RN2483GetCardNumber(void)  
{ 
debugSerial.println("RN2483AgetHWEUI()");
// Print the Hardware EUI
  debugSerial.print("RN2483GetCardNumber/LoRa DevEUI avant : ");
  for (uint8_t i = 0; i < sizeof(config.materiel.DevEUI); i++) 
  {
      debugSerial.print((char)NIBBLE_TO_HEX_CHAR(HIGH_NIBBLE(config.materiel.DevEUI[i])));
      debugSerial.print((char)NIBBLE_TO_HEX_CHAR(LOW_NIBBLE(config.materiel.DevEUI[i])));
  }
  debugSerial.println();  
  
  RN2483AgetHWEUI(config.materiel.DevEUI);     // get the Hardware DevEUI ex: "0004A30B0020300A"   uint8_t HWEUI[8];

// ici pas bon
// Print the Hardware EUI
  debugSerial.print("RN2483GetCardNumber/LoRa DevEUI après : ");
  for (uint8_t i = 0; i < sizeof(config.materiel.DevEUI); i++) 
  {
      debugSerial.print((char)NIBBLE_TO_HEX_CHAR(HIGH_NIBBLE(config.materiel.DevEUI[i])));
      debugSerial.print((char)NIBBLE_TO_HEX_CHAR(LOW_NIBBLE(config.materiel.DevEUI[i])));
  }
  debugSerial.println(" ici pas bon");  

debugSerial.print("RN2483GetCardNumber()/RN2483AgetHWEUI config.materiel.DevEUI:"); 
printByteArray(config.materiel.DevEUI, 16);

char Module_ID[17];
  convertToHexString(config.materiel.DevEUI, Module_ID, 8);

char msg[80];
  snprintf(msg,80,"Module %s",Module_ID);
  LOG_INFO(msg);

  for (config.materiel.Num_Carte=0;config.materiel.Num_Carte< MAX_HWEUI_List; config.materiel.Num_Carte++)
  { 
    if (strncmp(HWEUI_List[config.materiel.Num_Carte],Module_ID,16)==0)   // égalité des 2 
    {
// INITIALISER LES IDENTIFIANTS OTAA de LoRa + Contrôle
// Init AppEUI
      memcpy(config.applicatif.AppEUI, AppEUI_List,9);  //  10 avant le 22/01 KKKKK
// Init AppKey
      memcpy(config.applicatif.AppKey, AppKey_List[config.materiel.Num_Carte],17);
       break;  // sortie du for() quand trouvé egalité.... pas terrible!!!  
    }
 //debugSerial.println("HWEUI non trouvé");
  } 

// config.materiel.Noeud_LoRa = config.materiel.Num_Carte;  // par défaut, fonction Module_ID

      if (config.materiel.Num_Carte)
      {
        debugSerial.print(" execution Init_2483(), done with card : ");
        debugSerial.println(config.materiel.Num_Carte);
        OLEDDebugDisplay("2483A    Initialized");
      }
      else
      {
        LOG_ERROR("NO 2483 present.");
        OLEDDebugDisplay("2483A   Failed");
      }
  return((config.materiel.Num_Carte == MAX_HWEUI_List) ? 0 : config.materiel.Num_Carte); // retourne 0 error or Card number
}


const char* RN2483AloRaSendErrorToString(uint8_t rc)
{
  switch (rc)
  {
    case NoError:            return "NoError";
    case NoResponse:         return "NoResponse";
    case Timeout:            return "Timeout";
    case PayloadSizeError:   return "PayloadSizeError";
    case InternalError:      return "InternalError";
    case Busy:               return "Busy";
    case NetworkFatalError:  return "NetworkFatalError";
    case NotConnected:       return "NotConnected";
    case NoAcknowledgment:   return "NoAcknowledgment";
    default:                 return "Unknown";
  }
}


// ---------------------------------------------------------------------------*
// Gets and stores the LoRa module's HWEUI                              
// ---------------------------------------------------------------------------*
void RN2483AgetHWEUI(uint8_t *DecvEUI)
{ uint8_t len;
  char localModule_ID[20]="";
uint8_t DevEUI[8];


debugSerial.println("LoRaBee.RN2483AgetHWEUI()");
  len = LoRaBee.getHWEUI(DevEUI, sizeof(DevEUI));
debugSerial.print("RN2483AgetHWEUI(): ");  debugSerial.print(len);debugSerial.println(" car. lus: ");



// Print the Hardware EUI
  debugSerial.print("LoRa DevEUI: ");
  for (uint8_t i = 0; i < sizeof(DevEUI); i++) 
  {
      debugSerial.print((char)NIBBLE_TO_HEX_CHAR(HIGH_NIBBLE(DevEUI[i])));
      debugSerial.print((char)NIBBLE_TO_HEX_CHAR(LOW_NIBBLE(DevEUI[i])));
  }
  debugSerial.println();  



 // RN2483AclearLoRaBuffer();
//  loraSerial.println("sys get hweui");  
//   delay(200);
//  len = loraSerial.readBytesUntil(0x20, localModule_ID, 16); 

// envoi sur moniteur RS résultat RN2483AgetHWEUI()
//debugSerial.print("RN2483AgetHWEUI(): ");  debugSerial.print(len);
//debugSerial.print(" car. lus: "); debugSerial.print(localModule_ID);debugSerial.println(" dans localModule_ID  ");


// wtf CONVERTfconvertByteArray("0004A30B00EEEE01", DevEUI, 16);  // 0004A30B00EEEE01000EEC0000000000
 
  CONVERTfconvertByteArray(localModule_ID, DevEUI, 16);  // 0004A30B00EEA5D5 => 
  
}
// non testée 


// ---------------------------------------------------------------------------*
// connexion au reseau
//  in : 
//  out : 1 OK, 0 Echec                             
// ---------------------------------------------------------------------------*
bool SETUPsetupLoRaOTAA()
{ 
uint8_t DevEUI[8];
uint8_t AppEUI[8];
uint8_t AppKey[16]; 

// INITIALISER LES IDENTIFIANTS OTAA de LoRa + Contrôle
  memcpy(DevEUI, SN2483_List[config.materiel.Num_Carte], 8);
// Init AppEUI
      memcpy(AppEUI, AppEUI_List,8);  //  10 avant le 22/01 KKKKK
// Init AppKey
      memcpy(AppKey, AppKey_List[config.materiel.Num_Carte],16);


//  if (LoRaBee.initOTA(loraSerial, DevEUI, AppEUI, AppKey, true)) // titi

  if (LoRaBee.initOTA(loraSerial, SN2483_List[config.materiel.Num_Carte] , (const uint8_t *)AppEUI_List[config.materiel.Num_Carte] , AppKey_List[config.materiel.Num_Carte], true)) // titi

  
 //  if (LoRaBee.initOTA(loraSerial, config.materiel.DevEUI, config.applicatif.AppEUI, config.applicatif.AppKey, true)) // titi
  {
    LOG_INFO("SETUPsetupLoRaOTAA(), Network connection successfull");
    LoRaBee.setSpreadingFactor(config.applicatif.SpreadingFactor); // 7, 9 et 12 echec freudeneck
    return(1);
  }
  else  // ordre des lignes changé le 07/04/25, 
  { 
    LOG_ERROR("SETUPsetupLoRaOTAA(), Network connection failed!");
    debugSerialPrintLoRaStatus();
    return(0);
  }
}

// ---------------------------------------------------------------------------*
// Construit le message HEXA                              
// faire: 
// in : pointeur Payload , Ptr Struct datas
// out : -
// #define PAYLOADSIZE     19
// #define HEXPAYLOADSIZE  38
// int payloadSize = PAYLOADSIZE; 
// byte payload[PAYLOADSIZE];
//  => hexPayload sera envoyé
// à quoi correspond payload : 38 Caractères HEX
/*
  payload[indice++] = config.applicatif.RucherID;

  payload[indice++] = ((uint8_t*)&temperature)[0];    // Temperature: Least significant byte first, little endian
  payload[indice++] = ((uint8_t*)&temperature)[1];

  payload[indice++] = ((uint8_t*)&humidity)[0];       // Humidity
  payload[indice++] = ((uint8_t*)&humidity)[1];

  payload[indice++] = ((uint8_t*)&Brightness)[0];     // Brightness
  payload[indice++] = ((uint8_t*)&Brightness)[1];

  payload[indice++] = ((uint8_t*)&VBat)[0];           // Battery Voltage
  payload[indice++] = ((uint8_t*)&VBat)[1];

  payload[indice++] = ((uint8_t*)&VSol)[0];           // Solar Panel Voltage
  payload[indice++] = ((uint8_t*)&VSol)[1];

    payload[indice++] = ((uint8_t*)&Masse)[0];        // Hive1 Weight
    payload[indice++] = ((uint8_t*)&Masse)[1];

    payload[indice++] = ((uint8_t*)&Masse)[0];        // Hive2 Weight
    payload[indice++] = ((uint8_t*)&Masse)[1];
  
    payload[indice++] = ((uint8_t*)&Masse)[0];        // Hive3 Weight
    payload[indice++] = ((uint8_t*)&Masse)[1];
   
    payload[indice++] = ((uint8_t*)&Masse)[0];        // Hive4 Weight
    payload[indice++] = ((uint8_t*)&Masse)[1];

soit: 
0b be05 c012 0000 3a01 0000 fce7 30ca 6cc6 b875
0B 2206 C012 0000 3901 2A00 04C8 1CF7 2468 B86 =>37 car HEX ?????
*/

// En LoRaWAN, la convention par défaut est little-endian.  
// ---------------------------------------------------------------------------*
void buildLoraPayload(void)
{ int indice = 0, i;

//LOG_DEBUG("buildLoraPayload, datas:");
  
  payload[indice++] = config.applicatif.RucherID;
//debugSerial.println(config.applicatif.RucherID);

  int temperature = (int)(HiveSensor_Data.DHT_Temp * 100); // We multiple the values below because we then divide them on AllThingsTalk and don't need to send floats which use more data
  payload[indice++] = ((uint8_t*)&temperature)[0];   // Temperature: Least significant byte first, little endian
  payload[indice++] = ((uint8_t*)&temperature)[1];
//debugSerial.println(temperature);
  
  int humidity = (int)HiveSensor_Data.DHT_Hum * 100;
  payload[indice++] = ((uint8_t*)&humidity)[0];       // Humidity
  payload[indice++] = ((uint8_t*)&humidity)[1];
//debugSerial.println(humidity);

  sprintf(serialbuf,"Rucher: %d  [RucherName: %s] temp: %d, Hum.: %d",config.applicatif.RucherID, config.applicatif.RucherName, temperature, humidity);
//  debugSerial.println(serialbuf);
  
  int Brightness = (int)HiveSensor_Data.Brightness;
  payload[indice++] = ((uint8_t*)&Brightness)[0];     // Brightness
  payload[indice++] = ((uint8_t*)&Brightness)[1];
//debugSerial.println(Brightness);
    
  // tension moyenne des 10 dernières lectures
  int VBat = (int)(HiveSensor_Data.Bat_Voltage * 100); // We multiple the values below because we then divide them on AllThingsTalk and don't need to send floats which use more data
  payload[indice++] = ((uint8_t*)&VBat)[0];           // Battery Voltage
  payload[indice++] = ((uint8_t*)&VBat)[1];
//debugSerial.print(VBat);
  
  // tension moyenne des 10 dernières lectures
  int VSol = (int)(HiveSensor_Data.Solar_Voltage * 100); // multiple then divide: don't need to send floats
  payload[indice++] = ((uint8_t*)&VSol)[0];           // Solar Panel Voltage
  payload[indice++] = ((uint8_t*)&VSol)[1];
//debugSerial.println(VSol);

  sprintf(serialbuf,"Lum:    %d Vbat: %d,  Vsol: %d",Brightness, VBat, VSol);
//  LOG_DEBUG(serialbuf);

// constructioon PayLoad des 4 pesées

 int Masse=0;

  for (i = 0; i < 4; i++) 
  {
    Masse = (int)roundf(HiveSensor_Data.HX711Weight[i] * 100.0f); 
    memcpy(&payload[indice], &Masse, sizeof(Masse));
    indice+=2;

//  sprintf(serialbuf,"Masse%d: %f / ",i,HiveSensor_Data.HX711Weight[i] *100); //
//  debugSerial.print(serialbuf);
//debugSerialDisplayScaledSensorState(i);   // num 0 .. 3
  
  }
//debugSerial.println(" => Build HEX Payload");
  
//debugSerial.print("final i/payload[indice]"); debugSerial.print(i);  debugSerial.print("/");
//debugSerial.println(indice);

// c'est quoi la suite?
// rappel : char hexPayload[]

  for (i = 0; i <= payloadSize; i++) // PAYLOADSIZE  19 / HEXPAYLOADSIZE  38
  {
    sprintf(&hexPayload[i * 2], "%02X", payload[i]);  
  }  
// remplissage Payload de '7'                 // 7 car visible, mettre 0 quand validé
      for (i = payloadSize * 2; i <= hexPayloadSize; i++) // complete si size HEXPAYLOAD > size Payload*2
      {
        hexPayload[i] = '7'; // Pad with leading zeros to reach the required nn bytes
      }
  hexPayload[hexPayloadSize] = '\0';          // Null terminate
  
//debugSerial.print("final hexpayload[i]"); debugSerial.println(i); 
//debugSerial.print("hexPayload: "); 
//debugSerial.println(hexPayload);                 // chaine de caractère HEXA de Payload
}

                                                                                                            // verifier vs orange



// ---------------------------------------------------------------------------*
// envoi toutes les IRQ2
// ---------------------------------------------------------------------------*
uint8_t RN2483AsendLoRaPayload(uint8_t *Datas,uint8_t len)
{ 

debugSerialPrintLoraPayload(Datas,len);

uint8_t rc = LoRaBee.send(1,Datas,len);

  switch (rc)
  {
    case NoError:
      LOG_INFO("Successful transmission in Send_LoRa_Mess().");
      break;
    case NoResponse:
      LOG_INFO("There was no response from the device in Send_LoRa_Mess().");
      break;
    case Timeout:
      LOG_INFO("Connection timed-out in Send_LoRa_Mess(). Check your serial connection to the device! Sleeping for 20sec.");
      delay(20000);
      break;
    case PayloadSizeError:
      LOG_INFO("The size of the payload is greater than allowed in Send_LoRa_Mess(). Transmission failed!");
    break;
    case InternalError:
      LOG_INFO("Oh No! This shouldn't happen in Send_LoRa_Mess(). Something is really wrong! The program will reset the RN module...");
      SETUPsetupLoRa();
      // renvoyer MESSAGE
   LoRaBee.send(1,Datas, len);
    break;
    case Busy:
      LOG_INFO("The device is busy in Send_LoRa_Mess(). Sleeping for 10 extra seconds.");
      delay(10000);
      break;
    case NetworkFatalError:
      LOG_INFO("There is a non-recoverable error with the network connection in Send_LoRa_Mess(). The program will reset the RN module...");
      SETUPsetupLoRa();
       // renvoyer MESSAGE
   LoRaBee.send(1,Datas, len);
       break;
    case NotConnected:
      LOG_INFO("The device is not connected to the network in Send_LoRa_Mess(). The program will reset the RN module...");
      SETUPsetupLoRa();
      // renvoyer MESSAGE
  LoRaBee.send(1,Datas, len);
      break;
    case NoAcknowledgment:
      LOG_INFO("There was no acknowledgment sent back! in Send_LoRa_Mess()");
      break;
    default:
      break;
  }  // Fin Switch
  return(rc);
}


/*

// ---------------------------------------------------------------------------*
// Lit la réponse du modem avec timeout
// Paramètres :
//   - timeoutMs (int) : Timeout en millisecondes
// Retour : String - Réponse complète du modem
// ---------------------------------------------------------------------------*
String non_appelle_readLoRaResponse(int timeoutMs = 1000)
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
uint8_t non_appelle_RN2483Version(void)   
{ uint8_t len;

  RN2483AclearLoRaBuffer();
  loraSerial.println("sys get ver"); // RN2483 1.0.5 Oct 31 2018 15:06:52
   delay(100);
  len = loraSerial.readBytesUntil(0x20, serialbuf, 33); //sizeof(Module_ID));
// envoi sur moniteur RS résultat RN2483AgetHWEUI()
debugSerial.print("Version: ");  debugSerial.print(len);
debugSerial.print(" car. lus: "); debugSerial.println(serialbuf);
//  RN2483AclearLoRaBuffer(); // normalement buffer vide.
// Fin test communic ation avec RN2483A
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
    sprintf(serialbuf, "%5.1f",HiveSensor_Data.DHT_Temp);
    dataGrafana +=serialbuf;  

// température Peson
//    sprintf(serialbuf,"%5.1f",Temp_Peson(2));        // DHT_Hum); sp
//    dataGrafana +=serialbuf; 

// Luminance LDR
    sprintf(serialbuf, "%5.1f",HiveSensor_Data.Brightness);        // DHT_Hum);sp
    dataGrafana +=serialbuf;//HiveSensor_Data.Brightness;  
 
// Tension BAT
   sprintf(serialbuf, "%5.2f",HiveSensor_Data.Bat_Voltage);  // 2 fois???
    dataGrafana +=serialbuf; 
// MASSE A
   sprintf(serialbuf, "%5.2f",poidsBal_g(4));   // au lieu de 1
    dataGrafana +=serialbuf; 


// MASSE B
   sprintf(serialbuf, "%5.2f",poidsBal_g(2));   
     dataGrafana +=serialbuf;  
     
// MASSE C
   sprintf(serialbuf, "%5.2f",poidsBal_g(3));  
   dataGrafana +=serialbuf; 
   
// MASSE D   peson 2 brut
   sprintf(serialbuf,"%5.2f",Contrainte_List [1]); //HiveSensor_Data.HX711Weight_4);   

   sprintf(serialbuf,"%5.2f",HiveSensor_Data.Bat_Voltage); // 2 fois???
    dataGrafana +=serialbuf;  

// tension panneau solaire
   sprintf(serialbuf, "%5.2f",HiveSensor_Data.Solar_Voltage);
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
    sprintf(serialbuf,"%5.2f;",HiveSensor_Data.DHT_Temp);
    dataString +=serialbuf; 
    
// température Peson
//    sprintf(serialbuf,"%5.2f;",Temp_Peson(2));        // DHT_Hum); sp
//    dataString +=serialbuf;

    
// Luminance LDR
    sprintf(serialbuf, "%5.1f;",HiveSensor_Data.Brightness);        // DHT_Hum);sp
    dataString +=serialbuf; //HiveSensor_Data.Brightness; 
 
// Tension BAT
   sprintf(serialbuf, "%5.2f;",HiveSensor_Data.Bat_Voltage);  // 2 fois???
   dataString +=serialbuf;

// MASSE A
   sprintf(serialbuf, "%5.2f;",poidsBal_g(1));   
   dataString +=serialbuf;

// MASSE B
   sprintf(serialbuf, "%5.2f;",poidsBal_g(2));   
   dataString +=serialbuf;

// MASSE C
   sprintf(serialbuf, "%5.2f;",poidsBal_g(3));  
   dataString +=serialbuf;  

// MASSE D   peson 2 brut
   sprintf(serialbuf, "%5.2f;",Contrainte_List [1]); //Poids(4));   
   dataString +=serialbuf;

// tension panneau solaire
   sprintf(serialbuf,"%5.2f;",HiveSensor_Data.Solar_Voltage);
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
void no_Send_DATA_LoRa()
{
  static char Num_Mess=0;
  uint8_t lum=0;
  String readingL ,readingT; 
//String Local_LoRa_Buffer="";
  String dataString = "";
  String dataGrafana = "";

dataGrafana=Build_Lora_String(dataGrafana);
dataString=Build_CSV_String(dataString);

char chardata[256]="";

 dataGrafana.toCharArray(chardata,dataGrafana.length()); //48); //sizeof(dataGrafana));
 
 debugSerial.println("-------------------------------------------------------------------------------------------------------------------------------------------------");
 debugSerial.println(dataString);
 debugSerial.println(dataGrafana);
 debugSerial.println(chardata); 
 debugSerial.println("-------------------------------------------------------------------------------------------------------------------------------------------------");
  RN2483AsendLoRaPayload((uint8_t *)chardata,47);
}


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
  RN2483AclearLoRaBuffer();
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
