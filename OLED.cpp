//       1         2         3         4         5         6         7        7
//34567890123456789012345678901234567890123456789012345678901234567890123456789
// IMPRESSION 79 COLONES EN TAILLE 12
//
// ---------------------------------------------------------------------------*
//       ____  _      ______ _____                  
//      / __ \| |    |  ____|  __ \                 
//     | |  | | |    | |__  | |  | | ___ _ __  _ __ 
//     | |  | | |    |  __| | |  | |/ __| '_ \| '_ \
//     | |__| | |____| |____| |__| | (__| |_) | |_) |
//      \____/|______|______|_____(_)___| .__/| .__/
//                                      | |   | |   
//                                      |_|   |_|    
// ---------------------------------------------------------------------------*
#define __INIT_DONE
#include "define.h"


// ---------------------------------------------------------------------------*
// @brief Initialise l'écran OLED selon le type sélectionné
// @param Aucun
// @return void
// ---------------------------------------------------------------------------*
void OLEDInit(void) 
{
#ifdef OLED096
    OLEDInit096();
#endif
#ifdef OLED130
    OLEDInit130();
#endif
#ifdef OLED154
    OLEDInit154();
#endif
    debugSerial.print("LOGO ");
    delay(500);
    display.clearDisplay();
    debugSerial.println(" done");
}

// ---------------------------------------------------------------------------*
// @brief Initialise l'écran OLED 0.96"
// @param Aucun
// @return void
// ---------------------------------------------------------------------------*
void OLEDInit096(void) 
{
  debugSerial.println("INIT OLED096");
#define SSD1306_SWITCHCAPVCC 0x02
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.display();
}

// ---------------------------------------------------------------------------*
// @brief Initialise l'écran OLED 1.30"
// @param Aucun
// @return void
// ---------------------------------------------------------------------------*
void OLEDInit130(void) 
{
  debugSerial.println("INIT OLED130");
  display.begin(OLED_ADDRESS, true);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.display();
}

// ---------------------------------------------------------------------------*
// @brief Initialise l'écran OLED 1.54"
// @param Aucun
// @return void
// ---------------------------------------------------------------------------*
void OLEDInit154(void) 
{
  debugSerial.println("INIT OLED154");
  display.begin(OLED_ADDRESS, true);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.display();
}

// ---------------------------------------------------------------------------*
// @brief Efface tout le contenu visuel de l'écran OLED
// @param Aucun
// @return void
// ---------------------------------------------------------------------------*
void OLEDClear(void) 
{
  display.clearDisplay();
  display.display();
//  OLEDDebugDisplay("OLEDClear()");
}


// ---------------------------------------------------------------------------*
// @brief Efface du texte à une position donnée
// @param col Colonne de départ
// @param lig Ligne de départ
// @param Ncar Nombre de caractères à effacer
// @return void
// ---------------------------------------------------------------------------*
void OLEDEraseText(int16_t colonne, int16_t lig, int16_t Ncar)
{
  char TL_ColPixel = (colonne*OLED_Col);
  char TL_LigPixel = lig * OLED_L1;
    
  display.fillRect(TL_ColPixel, TL_LigPixel, OLED_Col * Ncar, OLED_L1, 0);
  display.display(); // Ajoutez ceci pour forcer l'affichage
}

// ---------------------------------------------------------------------------*
// @brief Affiche l'heure du prochain Payload sur l'écran OLED
// @param col Colonne de départ
// @param lig Ligne de départ
// @return void
// ---------------------------------------------------------------------------*
void OLEDDrawScreenNextPayload(uint8_t ligne, uint8_t colonne, DateTime nextPayload )
{ char localOLEDbuf[21] = "12345678901234567890"; 

  sprintf(localOLEDbuf, "PayLoad: %02d:%02d:%02d", 
                nextPayload.hour(), nextPayload.minute(), nextPayload.second());
  OLEDDrawText(1, ligne, colonne, localOLEDbuf);
}

// ---------------------------------------------------------------------------*
// @brief Affiche l'heure et la date sur l'écran OLED
// @param col Colonne de départ
// @param lig Ligne de départ
// @return void
// ---------------------------------------------------------------------------*
void OLEDDrawScreenTime(uint8_t ligne, uint8_t colonne)
{ static DateTime oldSystemTime(1692712245);  //1692712245);  // timestamp Unix
  char localOLEDbuf[21] = "00:00:00    00/00/00";  

  DateTime systemTime = rtc.now();
  snprintf(localOLEDbuf, 21, "%02d:%02d:%02d    %02d/%02d/%02d", 
          systemTime.hour(), systemTime.minute(), systemTime.second(),
          systemTime.day(), systemTime.month(), systemTime.year()-2000);

debugSerial.println(localOLEDbuf);
             
  if (systemTime != oldSystemTime)
  { 
    OLEDDrawText(1, ligne, colonne, localOLEDbuf);
    oldSystemTime = systemTime;
  }
}

// ---------------------------------------------------------------------------*
// @brief Rafraichi l'heure /* et la date */ sur l'écran OLED 
//        après appel OLEDDrawScreenTime()
// @param col Colonne de départ
// @param lig Ligne de départ
// @return void
// ---------------------------------------------------------------------------*
void OLEDDrawScreenRefreshTime(uint8_t ligne, uint8_t colonne)
{ static DateTime oldSystemTime(1692712245);  //1692712245);  // timestamp Unix
  DateTime systemTime = rtc.now();
  int val;
  
  if (oldSystemTime.hour() != systemTime.hour())
  { val = systemTime.hour();
    OLEDPrintVar(ligne, 0, &val, 'i');
  }
  if (oldSystemTime.minute() != systemTime.minute())
  { val = systemTime.minute();
    OLEDPrintVar(ligne, 3, &val, 'i');
  }
  if (oldSystemTime.second() != systemTime.second())
  { val = systemTime.second();
    OLEDPrintVar(ligne, 6, &val, 'i');
  }       
/*               
               if (oldSystemTime.day() != systemTime.day())
               { val = systemTime.day();
   //              OLEDPrintVar(ligne, 11, &val, 'i');
               }
               if (oldSystemTime.month() != systemTime.month())
               { val = systemTime.month();
        //         OLEDPrintVar(ligne, 14, &val, 'i');
               }               
               if (oldSystemTime.year() != systemTime.year())
               { val = systemTime.year();
     //            OLEDPrintVar(ligne, 17, &val, 'i');
               }               
*/
// ---------------------------------------------------------------------------*              
  if (systemTime != oldSystemTime)
    oldSystemTime = systemTime;
}


// ---------------------------------------------------------------------------*
// @brief Ajoute un message avec scrolling si besoin
// @param message Message à afficher
// @return void
// ---------------------------------------------------------------------------*
void OLEDDebugDisplay(char* message) 
{
  if (!OLEDModeDebugActif)
  {
    debugSerial.println("OLEDModeDebugActif:non");
    return;
  }
// remplissage du bas vers le haut    
  for (int i = 0; i < MAX_LIGNES - 1; i++) // rappel MAX_LIGNES = 8
  {
    strcpy(lignesDebug[i], lignesDebug[i+1]);
  }
  strcpy(lignesDebug[MAX_LIGNES - 1], message);
// réaffiche tout l'écran    
  display.clearDisplay();
  for (uint8_t i = 1; i < MAX_LIGNES; i++) // uint8_t i = 0 => reserve L0 pour Time/Date
  {
//    display.setCursor(0, i// TAILLE_LIGNE);
    OLEDDrawText(1,i, 0, lignesDebug[i]);
  }
  display.display();
  delay(delaiAffichage);
//  debugSerial.print("OLEDDebugDisplay => "); debugSerial.println(message);
}


// ---------------------------------------------------------------------------*
// @brief efface les messages
// @param message Message à afficher
// @return void
// ---------------------------------------------------------------------------*
void OLEDDebugDisplayReset(void) 
{    
  if (!OLEDModeDebugActif) return;
    
  for (int i = 0; i < MAX_LIGNES; i++) // int i = 0, on garde time/date dispo
  { 
 //           sprintf(OLEDbuf, " --- %02d --- ",i);     
    strcpy(lignesDebug[i], "                    ");
  }
OLEDClear(); 
//  display.clearDisplay();
/* 
  for (uint8_t i = 0; i < MAX_LIGNES; i++) 
  {
  //  display.setCursor(0, i// TAILLE_LIGNE);
    OLEDDrawText(1,i, 0, lignesDebug[i]);
  }
  display.display();
  delay(delaiAffichage);
*/  
}


// ---------------------------------------------------------------------------*
// @brief Efface une ligne entière en la remplissant d'espaces
// @param ligne Ligne à effacer (0 à 7)
// @return void
// ---------------------------------------------------------------------------*
void non_OLEDClearLine(uint8_t ligne) 
{
    OLEDDrawText(1,ligne, 0, "                    "); // 20 espaces
delay(1000);
////    
}

// ---------------------------------------------------------------------------*
// @brief Affiche un seul caractère à une position donnée
// @param ligne Ligne (0 à 7)
// @param colonne Colonne (0 à ~21)
// @param c Caractère à afficher
// @return void
// ---------------------------------------------------------------------------*
void OLEDPrintChar(uint8_t ligne, uint8_t colonne, char c) 
{
    display.setCursor(colonne * 6, ligne * 8);
    display.write(c);
    display.display();
}

// ---------------------------------------------------------------------------*
// @brief Affiche une variable formatée sur l'écran OLED à une position donnée
// @param ligne Ligne sur l'écran OLED où afficher la variable
// @param col Colonne sur l'écran OLED où commencer l'affichage
// @param valeur Pointeur vers la valeur à afficher
// @param type Type de la variable ('c', 's', 'i', 'I', 'f', 'F')
// @return void
// ---------------------------------------------------------------------------*
void OLEDPrintVar(uint8_t ligne, uint8_t col, const void *valeur, char type) 
{ char localOLEDbuf[21] = "12345678901234567890";  
  switch (type) 
  {
    case 'c':
              sprintf(localOLEDbuf, "%c", *((char *)valeur));
              break;
    case 's':
              sprintf(localOLEDbuf, "%s", (char *)valeur);
              break;
    case 'i':
    case 'I':
              sprintf(localOLEDbuf, "%02d", *((int *)valeur));
 // %02d arrangement temporaire           
              break;
    case 'f':
    case 'F':
              sprintf(localOLEDbuf, "%.2f", *((float *)valeur));
              break;
    default:
              sprintf(localOLEDbuf, "<inconnu>");
              break;
  }
  OLEDDrawText(1,ligne, col, localOLEDbuf);
}

// ---------------------------------------------------------------------------*
// @brief Affiche une variable formatée avec unité et alignement sur l'écran OLED
// @param ligne Ligne sur l'écran OLED
// @param col Colonne sur l'écran OLED
// @param valeur Pointeur vers la donnée à afficher
// @param type Type de la variable
// @param unite Chaîne représentant l'unité
// @param precision Nombre de chiffres après la virgule pour les float
// @param align Mode d'alignement ('L', 'R', 'C')
// @return void
// ---------------------------------------------------------------------------*
void OLEDPrintFormatted(uint8_t ligne, uint8_t col, const void *valeur, char type,
                        const char *unite, int precision, char align) 
{ char valeurStr[16] = "";
    
    switch (type) 
    {
        case 'c':
            sprintf(valeurStr, "%c", *((char *)valeur));
            break;
        case 's':
            sprintf(valeurStr, "%s", (char *)valeur);
            break;
        case 'i':
        case 'I':
            sprintf(valeurStr, "%d", *((int *)valeur));
            break;
        case 'f':
        case 'F': 
        {
            char format[8];
            sprintf(format, "%%.%df", precision);
            sprintf(valeurStr, format, *((float *)valeur));
            break;
        }
        default:
            sprintf(valeurStr, "<invalide>");
            break;
    }
    
    sprintf(OLEDbuf, "%s %s", valeurStr, unite ? unite : "");
    
    int len = strlen(OLEDbuf);
    char ligneFinale[22] = "";
    
    switch (align) 
    {
        case 'R':
            sprintf(ligneFinale, "%*s", 20, OLEDbuf);
            break;
        case 'C':
            sprintf(ligneFinale, "%*s", (20 + len)/2, OLEDbuf);
            break;
        default:
            sprintf(ligneFinale, "%-20s", OLEDbuf);
            break;
    }   
  OLEDDrawText(1,ligne, col, ligneFinale);
}

// ---------------------------------------------------------------------------*
// @brief Affiche un message pendant 1s sur la ligne 8
// @param message Message à afficher
// @param defilant True pour faire défiler le message si trop long
// @param inverse True pour affichage inversé
// @return void
// ---------------------------------------------------------------------------*
void OLEDDisplayMessageL8(const char* message, bool defilant, bool inverse) 
{
  debugSerial.print("OLEDDisplayMessageL8() => "); 
  debugSerial.println(message);
    
//  display.clearDisplay();
//  display.setTextSize(1);
    
  if (inverse)
    display.setTextColor(BLACK, WHITE);
  else 
    display.setTextColor(WHITE);
   
  OLEDDrawText(1, 7, 0, message);
/*  
  int len = strlen(message);
    
  if (defilant && len > (SCREEN_WIDTH / 6)) 
  {
    for (int offset = 0; offset <= (len// 6 - SCREEN_WIDTH); offset += 2) 
    {
      display.clearDisplay();
      display.setCursor(-offset, pixelLine);
      display.print(message);
      display.display();
      delay(30);
    }
    delay(500);
  } 
  else 
  {
    display.setCursor((SCREEN_WIDTH - len// 6) / 2, pixelLine);
    display.print(message);
    display.display();
    delay(1000);
  }
  display.setTextColor(WHITE);
*/
    delay(500);  // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
}


// ---------------------------------------------------------------------------*
// @brief Affiche du texte avec taille et position spécifiées
// @param Txt_Size Taille du texte
// @param col Colonne de départ
// @param lig Ligne de départ
// @param text Texte à afficher
// @return void
// ---------------------------------------------------------------------------*
void OLEDDrawText(int8_t Txt_Size, uint8_t ligne, uint8_t colonne, const char *text)
{
  if (strlen(text) > 20-colonne )   // controler la longueur du texte à afficher  
  {
    debugSerial.println("OLEDDrawText: Txt dépasse colonne 20");
// erreur, texte dépasse colonne 20
  }
  
  display.setTextSize(Txt_Size);
  display.setTextColor(WHITE); 

  
  OLEDEraseText(colonne, ligne, strlen(text));
//debugOLEDDrawText = true;     
  if (debugOLEDDrawText)
  {
    debugSerial.print("txt: <"); debugSerial.print(text);debugSerial.print(">");
    debugSerial.print(" NB Car: "); debugSerial.print(strlen(text));
    debugSerial.print(" Ligne: "); debugSerial.print(ligne); 
    debugSerial.print(" Colonne: "); debugSerial.print(colonne); 
    debugSerial.print("/"); debugSerial.println((colonne*OLED_Col)); 
  }
//debugOLEDDrawText = false; 

/* rappel
//    #define OLED_Col      6
  #define OLED_Max_Col  20
  #define OLED_L1       8
*/
// coordonnées en PIXELS
  display.setCursor((colonne*OLED_Col), (ligne*OLED_L1));
        
  for (uint8_t i = 0; i < strlen(text); i++)
  {
    display.write(text[i]);
  }      
  display.display();
}



// ---------------------------------------------------------------------------*
// @brief Affiche la date en cours de saisie sur l'écran
// @param d Chaîne de caractères de la date
// @param pos Position du curseur
// @return void
// ---------------------------------------------------------------------------*
void OLEDDisplayDate(char *d, uint8_t pos) 
{
  sprintf(serialbuf,"OLEDDisplayDate:%s",d);
  debugSerial.println(serialbuf); 

  if (millis() - lastBlink >= 500) 
  {
    lastBlink = millis();
    blinkState = !blinkState;
  }
    
  char ligne[11];
  for (int i = 0; i < 10; i++) 
    ligne[i] = (i == pos && blinkState) ? '_' : d[i];
  ligne[10] = '\0';
    
  OLEDClear();
  OLEDDrawText(1,0, 0, "Saisir la date :");
  OLEDDrawText(1,1, 0, ligne);
}

// ---------------------------------------------------------------------------*
// @brief Affiche l'heure en cours de saisie sur l'écran
// @param h Chaîne de caractères de l'heure
// @param pos Position du curseur
// @return void
// ---------------------------------------------------------------------------*
void OLEDDisplayTime(char *h, uint8_t pos) 
{
  if (millis() - lastBlink >= 500) 
  {
    lastBlink = millis();
    blinkState = !blinkState;
  }
    
  char ligne[9];
  for (int i = 0; i < 8; i++) 
    ligne[i] = (i == pos && blinkState) ? '_' : h[i];
  ligne[8] = '\0';
    
  OLEDClear();
  OLEDDrawText(1,0, 0, "Saisir l'heure :");
  OLEDDrawText(1,1, 0, ligne);
}

// ---------------------------------------------------------------------------*
// @brief Affiche les informations système sur écran
// 01 :  read_DHT(dht);  // Reading temperature or humidity takes about 250 milliseconds!
// 02 :  LDR, VBat, VSol   // lectures ANA (une seule lecture, moyenne calculée quand nécéssaire).
// 03 :  Poids Balance 1
// 04 :  Poids Balance 2
// 05 :  Poids Balance 3
// 06 :  Poids Balance 4
// 07 :  Température µC
// @param Aucun
// @return void
/* 
 typedef struct   
{
  uint8_t rucher_ID;  // 0:non affecté, 1: Fleurs, 2: Acacia, 3: Tilleul, 4: Chataignier
                      // 5: Sapin 6: Jachère 7: Forêt  8: Phacélie 9: Pommes 
  float   DHT_Temp;         // Temp DHT en °C    xx,x  Float
  float   DHT_Hum;          // Hum DHT en %      xx,x  Float
  float   Brightness;       // %Lum en LUX      xxxxx  uint16_t
  float   Bat_Voltage;      // Tension BAT en V     xx,xx  Float (uint16_t)
  float   Solar_Voltage;    // Tension BAT en V     xx,xx  Float (uint16_t)
  float   HX711Weight[4];    // masse Ruche 1 en kg xxx,xx (précision affich. 10g)
  float   VSol[11];
  float   VBat[11];   
//  float   _noLux;              // VLux, ne sera pas conservé 
  float   ProcessorTemp;    // temp µC, conservé en backup DHT22 
} LoRa_Var;
*/
// ---------------------------------------------------------------------------*

// Modifier pour avoir données dynamiques ++++ tout +++++++++++++++++++++++++++

void OLEDDisplayHivesDatas(void)
{ char localOLEDbuf[21] = "12345678901234567890";
  OLEDClear();
  OLEDDrawScreenTime(0, 0); // Affiche Time/Date au complet

    //OLEDDrawText(1,0, 0, );  // conserver rafraichissement Heure/Date

  OLEDDrawText(1,1, 0, "=== INFOS Ruches ===");
// faire Fonction()
  snprintf(localOLEDbuf, 21, "Tem: %4.1f - Hum: %4.1f", Data_LoRa.DHT_Temp , Data_LoRa.DHT_Hum);
  OLEDDrawText(1,2, 0, localOLEDbuf );
        
  snprintf(localOLEDbuf, 21, "LDR: %4.1f -  VB: %4.1f", Data_LoRa.Brightness, Data_LoRa.Bat_Voltage);
  OLEDDrawText(1,3, 0, localOLEDbuf);
        
  snprintf(localOLEDbuf, 21,"VSo: %4.1f -  mT: %4.1f", Data_LoRa.Solar_Voltage, readingT);
  OLEDDrawText(1,4, 0, localOLEDbuf);

//#define Poids_Peson(num)      Data_LoRa.HX711Weight[num]   //  Data_LoRa de type LoRa_Var (ligne 38)


////    sprintf(OLEDbuf, "Ba1: %4.1f - Ba2: %4.1f", Poids_Peson(0), Poids_Peson(1));


 float poids = (float)Poids_Peson(0);
       poids = poids/1000;
//Data_LoRa.HX711Weight[0]=123456;
       
  snprintf(localOLEDbuf, 21,"Ba1: %4.1f - Ba2: %4.1f", Data_LoRa.HX711Weight[0]/1000, 15.22); //Poids_Peson(1));
  OLEDDrawText(1,5, 0, localOLEDbuf);

  snprintf(localOLEDbuf, 21, "Ba3: %4.1f - Ba4: %4.1f", Poids_Peson(2), Poids_Peson(3));
  OLEDDrawText(1,6, 0, localOLEDbuf);

// Eviter, Status Line
//        sprintf(OLEDbuf, "Next PL:  %02d", , );
//        OLEDDrawText(1,7, 0, OLEDbuf);
  OLEDDrawScreenNextPayload(7, 0, nextPayload );
// Attendre une touche pour continuer
// fin Fonction()


/*     
    while (readKey() == KEY_NONE)
    {
        delay(100);
    }
    
    OLEDDisplayMessageL8("Retour menu principal", false, false);
*/
}


// ---------------------------------------------------------------------------*
// @brief Affiche les informations système sur écran
// @param Aucun
// @return void
// ---------------------------------------------------------------------------*
/*
void nonOLEDDisplaySystemInfo(void)
{
  OLEDClear();
  OLEDDrawText(1,0, 0, "=== INFOS SYSTEME ==");
  // Affichage date/heure rafraichie le L1
  OLEDDrawScreenTime(1, 0); // Affiche Time/Date au complet
  OLEDDrawText(1,2, 0, "     POC ATSAMD     ");
  OLEDDrawText(1,3, 0, "Version: " VERSION);
        
  sprintf(OLEDbuf, "Mode: %s", modeExploitation ? "EXPLOIT" : "PROGRAM");
  OLEDDrawText(1,4, 0, OLEDbuf);
        
 // sprintf(OLEDbuf, "Config v: %d.%02d", config.materiel.version/100, config.materiel.version%100);  // verifier longueur
 // OLEDDrawText(1,5, 0, OLEDbuf);
        
  OLEDDrawText(1,6, 0, "Appuyer => continuer");
*/  
/*
     // Attendre une touche pour continuer
  while (readKey() == KEY_NONE)
  {
    delay(100);
  }
*/    
//  OLEDDisplayMessageL8("Retour menu princ.", false, false);
//}



/*
      // Revenir au menu principal
      menuLevel_t* currentMenu = &menuStack[currentMenuDepth - 1];
      startListInput(currentMenu->title, currentMenu->menuList, currentMenu->menuSize, currentMenu->selectedIndex);
*/
// ---------------------------------------------------------------------------*
// @brief Affiche l'écran d'informations du  Projet (non-bloquant)
// @param void
// @return void
// ---------------------------------------------------------------------------*

// Modifier pour avoir données dynamiques ++++ DHT ou TµC +++++++++++++++++++++

void OLEDdisplayInfoScreenSyst(void)
{ char localOLEDbuf[21] = "12345678901234567890";
  infoScreenState = INFO_SCREEN_ACTIVE;   // pour eviter KKKKKKKK

  ScreenRefreshed = true;         // active rafraichissement en général
  InfoScreenRefreshTime = true;   // active rafraichissement date/heure
    
  debugSerial.print("OLEDdisplayInfoScreenSyst - currentMenuDepth: ");
  debugSerial.println(currentMenuDepth);

  OLEDClear();
  OLEDDrawText(1, 0, 0, "=== INFOS SYSTEME ==");
  OLEDDrawScreenTime(1, 0); // Affiche Time/Date au complet
  OLEDDrawText(1, 2, 0, "     POC ATSAMD     ");
  OLEDDrawText(1, 3, 0, "Version: " VERSION);
  OLEDDrawText(1, 4, 0, "Build: 20250924");

// faire Fonction()  
  if  (!read_DHT(dht))  // temp et hum. DHT22
  {
    snprintf(localOLEDbuf, 21,"T: %.2f C H: %.f %%",Data_LoRa.DHT_Temp ,Data_LoRa.DHT_Hum);
//debugSerial.println("OLEDdisplayInfoScreenSyst() DHT OK");
  }
  else    // /!\   Si pas DHT22, T:µC et H:N/A
  {
    float temp = ((analogRead(TEMP_SENSOR) * 3300.0 / 1023.0) - 500.0) / 10.0; // Lecture temp µC  
    snprintf(localOLEDbuf, 21,"T: %.2f C H: N/A",temp);
//debugSerial.println("OLEDdisplayInfoScreenSyst() DHT KO");
  } 
  OLEDDrawText(1, 5, 0, localOLEDbuf);
// fin Fonction()        

  OLEDDrawText(1, 7, 0, "VALIDE pour retour");
  
 // infoScreenState = INFO_SCREEN_ACTIVE;   // pour eviter KKKKKKKK
  debugSerial.println("Ecran infos affiche");
}


// ---------------------------------------------------------------------------*
// @brief Affiche l'écran d'informations du  Projet (non-bloquant)
// @param void
// @return void
// ---------------------------------------------------------------------------*
/*

// Config LoRa
typedef struct  
{
  uint8_t HWEUI [20];       // ID RN2483: "0004A30B00EEEE01"
  uint8_t AppEUI [10];      // AppEUI: {0x41, 0x42, 0x45, 0x49, 0x4C, 0x4C, 0x45, 0x31, 0x00}
  uint8_t AppKey [18];      // AppKEY: // 5048494C495050454C4F564542454553 - PHILIPPELOVEBEES
// {0x50, 0x48, 0x49, 0x4C, 0x49, 0x50, 0x50, 0x45, 0x4C, 0x4F, 0x56, 0x45, 0x42, 0x45, 0x45, 0x53, 0x00} 
  uint8_t SpreadingFactor;  // 7, 9 et 12 echec freudeneck
  uint8_t SendingPeriod;    // 15 minutes = 500 sans IT
} LoRa_configuration;  // LoRa_Config.  
 
*/

// AppKey = (uint8_t *)AppKey_List[Ruche.Num_Carte];
void OLEDdisplayInfoScreenLoRa(void)
{ char localOLEDbuf[21] = "12345678901234567890";
  infoScreenState = INFO_SCREEN_ACTIVE;   // pour eviter KKKKKKKK

  ScreenRefreshed = true;         // active rafraichissement en général
  LoRaScreenRefreshTime = true;   // active rafraichissement date/heure
  LoraScreenRefreshNextPayload = true;  // active rafraichissement heure payload
  
  
  debugSerial.print("OLEDdisplayInfoScreenSyst - currentMenuDepth: ");
  debugSerial.println(currentMenuDepth);

  OLEDClear();
  OLEDDrawText(1, 0, 0, "==== INFOS LoRa ====");
  OLEDDrawScreenTime(1, 0); // Affiche Time/Date au complet
  snprintf(localOLEDbuf, 21,"#%2d %15s",ConfigApplicatif.RucherID,ConfigApplicatif.RucherName);  // ConfigApplicatif_t =>   uint8_t Balance_ID + char    RucherName [20]; 
  OLEDDrawText(1, 2, 0,localOLEDbuf);
// Affichage SF
  snprintf(localOLEDbuf, 21,"SF: %d  Tx: %d min.",config.applicatif.SpreadingFactor,config.applicatif.SendingPeriod);  // SF + Tx Interval (min)  
  OLEDDrawText(1, 3, 0,localOLEDbuf);
// Affichage DevEUI
char Module_ID[17];
  convertToHexString(ConfigMateriel.DevEUI, Module_ID, 8);
  snprintf(localOLEDbuf, 21,"SN: %16s",Module_ID);  // DevEUI (N° RN 16 car) 
  OLEDDrawText(1, 4, 0, localOLEDbuf);
  
// AK => AppKey = (uint8_t *)AppKey_List[Ruche.Num_Carte];  
// 5048494C49505045 4C4F56454C414B4F != PHILIPPELOVEBEES
  snprintf(localOLEDbuf, 21,"AK: %s",ConfigApplicatif.AppKey); 
  OLEDDrawText(1, 5, 0,localOLEDbuf);

//  snprintf(localOLEDbuf, 21,"+ %s",&AppKey_List[Ruche.Num_Carte][16]);  // AppKey 1/2 => "5048494C49505045"
//  OLEDDrawText(1, 5, 0,localOLEDbuf);

// AE => AppEUI => 414245494C4C4533  (16 char vers 8 uint8_t )
  snprintf(localOLEDbuf, 21,"AE: %s",ConfigApplicatif.AppEUI);  
  OLEDDrawText(1, 6, 0,localOLEDbuf);
  OLEDDrawText(1, 7, 0, "VALIDE pour retour");
debugSerial.println("Ecran infos affiche");
}


// ---------------------------------------------------------------------------*
// @brief Affiche l'écran d'informations des Balances (non-bloquant)
// @param void
// @return void
// ---------------------------------------------------------------------------*

// Modifier pour avoir données dynamiques ++++ Poids, Tare, Echelle, CompTemp +

void OLEDdisplayInfoBal(void)
{ char localOLEDbuf[21] = "12345678901234567890";
  int num = 1;
  
  infoScreenState = INFO_SCREEN_ACTIVE;   // pour eviter KKKKKKKK

  debugSerial.print("OLEDdisplayInfoBal - currentMenuDepth: ");
  debugSerial.println(currentMenuDepth);
  OLEDClear();

snprintf(localOLEDbuf, 21,"= INFOS BALANCE %d ==", bal);
  OLEDDrawText(1, 0, 0, localOLEDbuf);
  OLEDDrawScreenTime(1, 0); // Affiche Time/Date au complet
/*
// N° des jauge montée sur le dispositif de pesée A,B,C,D
int Peson [10][4] = {
      {0,0,0,0},    // Module LoRa pas Lu; pas de Peson
      {0,0,0,17},    // 0004A30B0020300A carte 1 HS; sur Carte PROTO2 en service le 05/03/2021
      {13,8,9,0}, //15},    // 0004A30B0024BF45 carte 2; en service le 10/05/2020
 
float Jauge[21][4] = {                // Tare , Echelle , TareTemp , CompTemp
      {0,0,0,0},     // J00 => pas de peson connecté
      {178666,108.5,20,0},    // J01 20kg

Tare : Jauge[Peson [Ruche.Num_Carte  ][Bal-1]][0] 

*/
// faire Fonction()
  snprintf(localOLEDbuf, 21,"Poids   : %d ", BalPoids(num-1) );
  OLEDDrawText(1, 2, 0, localOLEDbuf);
  snprintf(localOLEDbuf, 21,"Tare    : %d ", Jauge[Peson[ConfigMateriel.Num_Carte][bal-1]][0]);
  OLEDDrawText(1, 3, 0, localOLEDbuf);
  snprintf(localOLEDbuf, 21,"Echelle : %d ", Jauge[Peson[ConfigMateriel.Num_Carte][bal-1]][1]);
  OLEDDrawText(1, 4, 0, localOLEDbuf);
  snprintf(localOLEDbuf, 21,"Comp. T : %d ", Jauge[Peson[ConfigMateriel .Num_Carte][bal-1]][3]);
  OLEDDrawText(1, 5, 0, localOLEDbuf);

  snprintf(localOLEDbuf, 21, "#%1d - Num Pes:%2d", ConfigMateriel.Num_Carte,Peson[ConfigMateriel.Num_Carte][bal-1]);
  OLEDDrawText(1, 6, 0, localOLEDbuf);
// fin Fonction()

  OLEDDrawText(1, 7, 0, "VALIDE pour retour");
}


// ---------------------------------------------------------------------------*
// @brief Affiche l'écran d'informations des Balances (non-bloquant)
// @param void
// @return void
// ---------------------------------------------------------------------------*

// Modifier pour avoir données dynamiques ++++ 4 x Poids +++++++++++++++++++++

void OLEDdisplayWeightBal(void)
{ char localOLEDbuf[21] = "12345678901234567890";
  infoScreenState = INFO_SCREEN_ACTIVE;   // pour eviter KKKKKKKK
  
  debugSerial.print("OLEDdisplayWeightBal - currentMenuDepth: ");
  debugSerial.println(currentMenuDepth);
  OLEDClear();

snprintf(localOLEDbuf, 21,"== POIDS BALANCES ==", bal);
  OLEDDrawText(1, 0, 0, localOLEDbuf);
  OLEDDrawScreenTime(1, 0); // Affiche Time/Date au complet

// faire Fonction()  
  snprintf(localOLEDbuf, 21,"Bal1 : %4.2d kg ", BalPoids(0) );
  OLEDDrawText(1, 2, 0, localOLEDbuf);
  snprintf(localOLEDbuf, 21,"Bal1 : %4.2d kg ", BalPoids(1) );
  OLEDDrawText(1, 3, 0, localOLEDbuf);
  snprintf(localOLEDbuf, 21,"Bal1 : %4.2d kg ", BalPoids(2) );
  OLEDDrawText(1, 4, 0, localOLEDbuf);
  snprintf(localOLEDbuf, 21,"Bal1 : %4.2d kg ", BalPoids(3) );
  OLEDDrawText(1, 5, 0, localOLEDbuf);
// fin Fonction()

  OLEDDrawText(1, 7, 0, "VALIDE pour retour");
}



// pas reelement du INFO_SCREEN

void OLEDdisplayCalibBal(void)
{char localOLEDbuf[21] = "12345678901234567890";
  infoScreenState = INFO_SCREEN_ACTIVE;   // pour eviter KKKKKKKK
  
  debugSerial.print("OLEDdisplayCalibBal - currentMenuDepth: ");
  debugSerial.println(currentMenuDepth);
  OLEDClear();

snprintf(localOLEDbuf, 21,"=== CALIB. BAL:%d ===", bal);
  OLEDDrawText(1, 0, 0, localOLEDbuf);

// faire Fonction()
  snprintf(localOLEDbuf, 21,"Bal%d : %4.2d kg ", bal, Peson[ConfigMateriel.Num_Carte][bal] );
  OLEDDrawText(1, 2, 0, localOLEDbuf);
  snprintf(localOLEDbuf, 21,"Scale%d : %4.2d kg ", bal, Jauge[Peson[ConfigMateriel.Num_Carte][bal]][1] );
  OLEDDrawText(1, 3, 0, localOLEDbuf);
  snprintf(localOLEDbuf, 21,"Temp%d : %4.2d kg ", bal, BalPoids(2) );
  OLEDDrawText(1, 4, 0, localOLEDbuf);
  snprintf(localOLEDbuf, 21,"Tare%D : %4.2d kg ", bal, Jauge[Peson[ConfigMateriel.Num_Carte][bal]][0] );
  OLEDDrawText(1, 5, 0, localOLEDbuf);
// fin Fonction()

/*
 * Peson[Ruche.Num_Carte][num] // num. peson
 *  Jauge[Peson[Ruche.Num_Carte][num]][0] = valAVid; // tare
  Jauge[Peson[Ruche.Num_Carte][num]][1] = Echelle; // scale 

 // noms courts et explicites, parametrer (num: 0..3) par Macro
#define Poids_Peson(num)      Data_LoRa.HX711Weight[num]   //  Data_LoRa de type LoRa_Var (ligne 38)
//#define Temp_Peson(num)       Data_LoRa.Temp_Peson[num]
#define Tare_Peson(num)       Jauge[Peson[Ruche.Num_Carte][num]][0]
#define Echelle_Peson(num)    Jauge[Peson[Ruche.Num_Carte][num]][1]
//#define BalPoids(num)  (Contrainte_List[num]-Jauge[Peson[Ruche.Num_Carte][num]][0])/Jauge[Peson[Ruche.Num_Carte][num]][1]/1000) //retourne float
#define BalPoids(num) (Contrainte_List[num]-Tare_Peson(num))/Echelle_Peson(num)/1000 //retourne float

#define TareTemp(num)   Jauge[Peson[Ruche.Num_Carte][num]][2]  // Ruche de type HW_equipement (ligne 21)
#define CompTemp(num)   Jauge[Peson[Ruche.Num_Carte][num]][3]

 * 
 * 
 */
  OLEDDrawText(1, 7, 0, "VALIDE pour retour");
  
}


// ---------------------------------------------------------------------------*
// Gestion rafraichissement écrans (System\INFOS, )
// ---------------------------------------------------------------------------*
void OLEDRefreshDisplay(void)
{
  if (InfoScreenRefreshTime)             // rien à rafraichir
  {
//debugSerial.println("OLEDDrawScreenRefreshTime dans ISR 1 ttes sec \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\");  
    OLEDDrawScreenRefreshTime(1, 0);    // refresh Time/Date every second      
  }
// LoRa INFOS   
  else if (LoRaScreenRefreshTime)        // rafraichir Heure
  {
    OLEDDrawScreenRefreshTime(1, 0);    // refresh Time/Date every second 
  }
// Balances INFOS
  else if (InfoBalScreenRefreshTime)        // rafraichir Heure
  {
    OLEDDrawScreenRefreshTime(1, 0);    // refresh Time/Date every second 
  }
    else if (InfoVSolScreenRefreshTime)        // rafraichir VSol
  {
 //   OLEDDrawScreenRefreshTime(1, 0);    // refresh Time/Date every second 
  }

// fin des rafraichissement d'heures

  
// Weight INFOS
  else if (WeightScreenRefreshWeights)   // rafraichir les poids temp, hum
  { char localOLEDbuf[21] = "";
    int num = 0;
    do
    {
      if (Peson[ConfigMateriel.Num_Carte][0])
      { 
        snprintf(localOLEDbuf, 21,"Bal1 : %4.2d kg ",Poids_Peson(0)); // BalPoids(0) );
        OLEDDrawText(1, 2, 0, localOLEDbuf);
      }
    }
    while (num < NB_RUCHES);  // 4
  } 
  else if (InfoVBatScreenRefreshTime)        // rafraichir VBat
  {
// appel à la fonction dédiée
    OLEDDrawScreenRefreshTime(1, 0);    // refresh Time/Date every second 
  }
  else if (InfoVLumScreenRefresh)        // rafraichir VLum                            // tester en premier
  {
// appel à la fonction dédiée

// ligne 0: Title
// ligne 1: Reserver Date / Time ???  => message type saisie : Saisir mise a Ech.
// ligne 2: Num en cours de modification
// ligne 3: Gestion du curseur
// ligne 4: Variable dépendante du Num en cours de saisie à rafraichir
// ligne 5: Afficher le statut de validité => Masqué > passer à 6 ????
// ligne 6: 
// ligne 7: OLEDDrawText(1, 7, 0, "+/- Char VALIDE: OK"); ou timeout // Instructions fixes

    OLEDRefreshVlum(4,8);   // Affiche Vlum formaté 1,23V (%3.2fV) en pos Ligne 4, colonne 10
    OLEDRefreshlum(5,8);        // readingL * echelle = luminosité de 0 à 99%
  }

  
// FIN Gestion rafraichissement écrans (System\INFOS, )
}


//azerty entrer et sortir des ecrans infos balances

// ---------------------------------------------------------------------------*
// @brief Active ou désactive l'affichage au boot
// @param actif True pour activer, false pour désactiver
// @return void
// ---------------------------------------------------------------------------*
void OLEDSetDebug(bool actif) 
{
  OLEDModeDebugActif = actif;
}


/*
Le rafraichissement n'est pas executé.
pas appelé dans loop()=> OLEDRefreshDisplay();
*/

// ---------------------------------------------------------------------------*
// @brief Rafraichi l'affichage de Lum 0..99%
// @param actif True pour activer, false pour désactiver
// @param ligne Ligne de départ
// @param colonne Colonne de départ
// @return void
// ---------------------------------------------------------------------------*
void OLEDRefreshlum(uint8_t ligne, uint8_t colonne)        // readingL * numInputCtx.workingNum
{ char localOLEDbuf[81] = "12345678901234567890";
  int echelle = atoi(numInputCtx.workingNum);
  int brightness = getLuminance();
char outputNum[21];

    strcpy(outputNum, numInputCtx.workingNum);
    
    debugSerial.print("Nombre en cours: ");
    debugSerial.println(outputNum);
 
//getLuminance

int ana;
ana = analogRead(LUM_SENSOR);

sprintf(localOLEDbuf,"ANA: *%d* getLum: *%4.1f*  ECH : *%f*  fin", ana, (float)4/*getLuminance */,  atof(outputNum));
debugSerial.println(localOLEDbuf);

  sprintf(localOLEDbuf,"%02d %%", brightness * atoi(outputNum));
  OLEDDrawText(1, ligne, colonne, localOLEDbuf);
}


// ---------------------------------------------------------------------------*
// @brief Rafraichi l'affichage de VLum ("%3.2fV")
// @param actif True pour activer, false pour désactiver
// @param ligne Ligne de départ
// @param colonne Colonne de départ
// @return void
// ---------------------------------------------------------------------------*
void OLEDRefreshVlum(uint8_t ligne, uint8_t colonne)
{ char localOLEDbuf[21] = "12345678901234567890";

  sprintf(localOLEDbuf,"%3.2f", analogRead(LUM_SENSOR) / 10.23);
  OLEDDrawText(1, ligne, colonne, localOLEDbuf);
}
