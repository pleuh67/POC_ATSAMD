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

// Hailege 2,42 "SSD1309 128x64 Module d’affichage OLED 2,42 pouces 4 broches Module d’affichage LCD (IIC blanc) 
// https://www.amazon.fr/Hailege-SSD1309-128x64-Display-Module/dp/B0CJY3VS12/ref=sr_1_1?__mk_fr_FR=%C3%85M%C3%85%C5%BD%C3%95%C3%91&crid=3DDGR7BUX1IVB&dib=eyJ2IjoiMSJ9.N1OyjiI5D-TWOkCwTrXSXsfjKXhkJ7zKc7sc43fwx0zGjHj071QN20LucGBJIEps.JjrFKTpqg3E94FFfvZ1CGXbK2NocWfFiRkYYlshZmpU&dib_tag=se&keywords=hailege%2B2.42&qid=1768289462&sprefix=hailege%2B2.42%2Caps%2C225&sr=8-1&th=1
// https://fr.aliexpress.com/item/1005003091769556.html?spm=a2g0o.order_list.order_list_main.5.43d35e5b8FWOPk&gatewayAdapt=glo2fra

// Module OLED 1.54 pouces 12864 128x64 SSD1309, pilote IIC I2C, Interface série, panneau d'affichage auto-lumineux pour Raspberry Pi
// https://fr.aliexpress.com/item/1005009313931934.html?spm=a2g0o.order_list.order_list_main.15.43d35e5b8FWOPk&gatewayAdapt=glo2fra

// Fonctionne
// Module OLED 1.3 pouces, Module d'affichage blanc/bleu 128x64 I2C SSD1306 12864, panneau d'écran LCD 1.3 pouces IIC OLED LED pour Arduino
// https://fr.aliexpress.com/item/1005007176955358.html?spm=a2g0o.order_list.order_list_main.10.43d35e5b8FWOPk&gatewayAdapt=glo2fra

// Fonctionne
// Carte d'écran LCD pour Ardu37, technologie d'affichage OLED, 0.96 pouces, série IIC éventuelles I, 7 broches, 4 broches, blanc, bleu, jaune, bleu, jaune, SSD1306 12864
// https://fr.aliexpress.com/item/32638662748.html?spm=a2g0o.order_list.order_list_main.31.43d35e5b8FWOPk&gatewayAdapt=glo2fra

// https://electroniqueamateur.blogspot.com/2019/01/ecran-oled-sh1106-i2c-et-arduino.html  image BITMAP


#define __INIT_DONE
#include "define.h"


// ---------------------------------------------------------------------------*
// @brief Initialise l'écran OLED selon le type sélectionné
// @param Aucun
// @return void
// ---------------------------------------------------------------------------*
void OLEDInit(void) 
{
#ifdef OLED096      // SSD1306
    OLEDInit096();
#endif
#ifdef OLED130      // SSD1306
    OLEDInit130();
#endif
#ifdef OLED154      // SSD1309
    OLEDInit154();
#endif
#ifdef OLED242      // SSD1309
    //OLEDInit242();
#endif
  delay(500);
  display.clearDisplay();
}

// ---------------------------------------------------------------------------*
// @brief Initialise l'écran OLED 0.96"
// @param Aucun
// @return void
// ---------------------------------------------------------------------------*
void OLEDInit096(void) 
{
  LOG_INFO("INIT OLED096");
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
  LOG_INFO("INIT OLED130");
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
  LOG_INFO("INIT OLED154");
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
// Avec col = 6; ligne = 4 ;  nbcar = 14
// display.fillRect( x, y, w, h, color );
// x      int16_t   Colonne de départ (pixel gauche)    32
// y      int16_t   Ligne de départ (pixel haut)        24
// w      int16_t   Largeur en pixels                   84  
// h      int16_t   Hauteur en pixels                    8
// color  uint16_t  Couleur : 0 = noir, 1 = blanc
//
// @return void
// ---------------------------------------------------------------------------*
void OLEDEraseText(int16_t colonne, int16_t lig, int16_t Ncar)
{
  char TL_ColPixel = (OLED_Col_Offset + colonne*OLED_Col);
  char TL_LigPixel = lig * OLED_L1;

    
  if (debugOLEDDrawText)
  {
    debugSerial.print(" efface zone display.fillRect (pixels) TL_ColPixel : "); debugSerial.print(TL_ColPixel,DEC);
    debugSerial.print(" TL_LigPixel: "); debugSerial.print(TL_LigPixel,DEC);
    debugSerial.print(" OLED_Col * Ncar: "); debugSerial.print(OLED_Col * Ncar);
    debugSerial.print("  OLED_L1: "); debugSerial.println( OLED_L1);
  }

  display.fillRect( TL_ColPixel, TL_LigPixel, OLED_Col * Ncar, OLED_L1, 0);
  display.display();                    // Ajoutez ceci pour forcer l'affichage     
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
//    setCursorAdjusted(0, i// TAILLE_LIGNE);
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
  //  setCursorAdjusted(0, i// TAILLE_LIGNE);
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
  setCursorAdjusted(colonne * 6, ligne * 8);
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
      setCursorAdjusted(-offset, pixelLine);   // OLED_Col_Offset +
      display.print(message);
      display.display();
      delay(30);
    }
    delay(500);
  } 
  else 
  {
   setCursorAdjusted((SCREEN_WIDTH - len// 6) / 2, pixelLine);
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
    LOG_ERROR("OLEDDrawText: Txt dépasse colonne 20");
  }
  
  display.setTextSize(Txt_Size);
  display.setTextColor(WHITE); 

//debugOLEDDrawText = true;    
// Efface zone à réecrire
//10:24:43.663 ->  efface zone display.fillRect (pixels) TL_ColPixel :  TL_LigPixel:  OLED_Col * Ncar: 84  OLED_L1: 8

    OLEDEraseText(colonne, ligne, strlen(text));  

//10:24:43.709 -> txt: <z:     0.01 ko> NB Car: 14 Ligne: 3 Colonne: 5/30
  if (debugOLEDDrawText)
  {
    debugSerial.print("txt: <"); debugSerial.print(text);debugSerial.print(">");
    debugSerial.print(" NB Car: "); debugSerial.print(strlen(text));
    debugSerial.print(" Ligne: "); debugSerial.print(ligne); 
    debugSerial.print(" Colonne: "); debugSerial.print(colonne); 
    debugSerial.print("/"); debugSerial.println((colonne*OLED_Col)); // OLED_Col vaut nb pix 1 col (6)
    }

 // positionne le curseur 
//10:24:43.709 ->  positionne curseur (pixels) x :32 y: 24 
  setCursorAdjusted((colonne*OLED_Col), (ligne*OLED_L1)); // OLED_ColOffset
debugOLEDDrawText = false; 

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
// ---------------------------------------------------------------------------*
void OLEDDisplayHivesDatas(void)
{ char localOLEDbuf[21] = "12345678901234567890";

  OLEDClear();
  OLEDDrawScreenTime(0, 0); // Affiche Time/Date au complet

    //OLEDDrawText(1,0, 0, );  // conserver rafraichissement Heure/Date

  OLEDDrawText(1,1, 0, "=== INFOS Ruches ===");
// faire Fonction()
  snprintf(localOLEDbuf, 21, "Tem: %4.1f - Hum: %4.1f", HiveSensor_Data.DHT_Temp , HiveSensor_Data.DHT_Hum);
  OLEDDrawText(1,2, 0, localOLEDbuf );
        
  snprintf(localOLEDbuf, 21, "LDR: %4.1f -  VB: %4.1f", HiveSensor_Data.Brightness, HiveSensor_Data.Bat_Voltage);
  OLEDDrawText(1,3, 0, localOLEDbuf);
        
  snprintf(localOLEDbuf, 21,"VSo: %4.1f -  mT: %4.1f", HiveSensor_Data.Solar_Voltage, readingT);
  OLEDDrawText(1,4, 0, localOLEDbuf);

////    sprintf(OLEDbuf, "Ba1: %4.1f - Ba2: %4.1f", poidsBal_g(0), poidsBal_g(1));


// WTF poids
       
  snprintf(localOLEDbuf, 21,"Ba1: %4.1f - Ba2: %4.1f", HiveSensor_Data.HX711Weight[0]/1000, 15.22);
  OLEDDrawText(1,5, 0, localOLEDbuf);

  snprintf(localOLEDbuf, 21, "Ba3: %4.1f - Ba4: %4.1f", poidsBal_g(2), poidsBal_g(3));
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
void OLEDdisplayInfoScreenSyst(void)
{ char localOLEDbuf[21] = "12345678901234567890";
  infoScreenState = INFO_SCREEN_ACTIVE;   // pour eviter KKKKKKKK

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
    snprintf(localOLEDbuf, 21,"T: %.2f C H: %.f %%",HiveSensor_Data.DHT_Temp ,HiveSensor_Data.DHT_Hum);
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
void OLEDdisplayInfoScreenLoRa(void)
{ char localOLEDbuf[21] = "12345678901234567890";
  infoScreenState = INFO_SCREEN_ACTIVE;   // pour eviter KKKKKKKK
  
  debugSerial.print("OLEDdisplayInfoScreenSyst - currentMenuDepth: ");
  debugSerial.println(currentMenuDepth);

  OLEDClear();
  OLEDDrawText(1, 0, 0, "==== INFOS LoRa ====");
  OLEDDrawScreenTime(1, 0); // Affiche Time/Date au complet
  snprintf(localOLEDbuf, 21,"#%2d %15s",config.applicatif.RucherID,config.applicatif.RucherName);  // config.applicatif_t =>   uint8_t Balance_ID + char    RucherName [20]; 
  OLEDDrawText(1, 2, 0,localOLEDbuf);
// Affichage SF
  snprintf(localOLEDbuf, 21,"SF: %d  Tx: %d min.",config.applicatif.SpreadingFactor,config.applicatif.SendingPeriod);  // SF + Tx Interval (min)  
  OLEDDrawText(1, 3, 0,localOLEDbuf);
// Affichage DevEUI
char Module_ID[17];
  convertToHexString(config.materiel.DevEUI, Module_ID, 8);
  snprintf(localOLEDbuf, 21,"SN: %16s",Module_ID);  // DevEUI (N° RN 16 car) 
  OLEDDrawText(1, 4, 0, localOLEDbuf);
  
// AK => AppKey = (uint8_t *)AppKey_List[Ruche.Num_Carte];  
// 5048494C49505045 4C4F56454C414B4F != PHILIPPELOVEBEES
  snprintf(localOLEDbuf, 21,"AK: %s",config.applicatif.AppKey); 
  OLEDDrawText(1, 5, 0,localOLEDbuf);

//  snprintf(localOLEDbuf, 21,"+ %s",&AppKey_List[Ruche.Num_Carte][16]);  // AppKey 1/2 => "5048494C49505045"
//  OLEDDrawText(1, 5, 0,localOLEDbuf);

// AE => AppEUI => 414245494C4C4533  (16 char vers 8 uint8_t )
  snprintf(localOLEDbuf, 13,"AE: %s",config.applicatif.AppEUI);  // "AE: XXXXXXXX\0" = 13 ou modifier memcpy en incluant \0
  OLEDDrawText(1, 6, 0,localOLEDbuf);
  OLEDDrawText(1, 7, 0, "VALIDE pour retour");
debugSerial.println("Ecran infos affiche");
}


// ---------------------------------------------------------------------------*
// @brief Affiche l'écran d'informations des Balances (non-bloquant)
// @param void
// @return void
// ---------------------------------------------------------------------------*
void OLEDdisplayInfoBal(void)
{ char localOLEDbuf[21] = "12345678901234567890";
//  int num = 1;
  
  infoScreenState = INFO_SCREEN_ACTIVE;   // pour eviter KKKKKKKK

  debugSerial.print("OLEDdisplayInfoBal - currentMenuDepth: ");
  debugSerial.println(currentMenuDepth);
  OLEDClear();

snprintf(localOLEDbuf, 21,"== INFOS BALANCES ==");
  OLEDDrawText(1, 0, 0, localOLEDbuf);
  OLEDDrawScreenTime(1, 0); // Affiche Time/Date au complet

// faire Fonction()
  if (Peson[config.materiel.Num_Carte][0])
    snprintf(localOLEDbuf, 21,"Bal. A: %8.2f kg", calculePoids(0) );
  else
    snprintf(localOLEDbuf, 21,"Bal. A: N/A         ");
  OLEDDrawText(1, 3, 0, localOLEDbuf);
  if (Peson[config.materiel.Num_Carte][1])
    snprintf(localOLEDbuf, 21,"Bal. B: %8.2f kg", calculePoids(1) );
  else
    snprintf(localOLEDbuf, 21,"Bal. B: N/A         ");
  OLEDDrawText(1, 4, 0, localOLEDbuf);

  if (Peson[config.materiel.Num_Carte][2])
    snprintf(localOLEDbuf, 21,"Bal. C: %8.2f kg", calculePoids(2) );
  else
    snprintf(localOLEDbuf, 21,"Bal. C: N/A         ");
  OLEDDrawText(1, 5, 0, localOLEDbuf);
  
   if (Peson[config.materiel.Num_Carte][3])
    snprintf(localOLEDbuf, 21,"Bal. D: %8.2f kg", calculePoids(3) );
  else
    snprintf(localOLEDbuf, 21,"Bal. D: N/A         ");
  OLEDDrawText(1, 6, 0, localOLEDbuf);
  OLEDDrawText(1, 7, 0, "VALIDE pour retour");
}



// pas reellement du INFO_SCREEN

void OLEDdisplayCalibBal(void)
{char localOLEDbuf[21] = "12345678901234567890";
  infoScreenState = INFO_SCREEN_ACTIVE;   // pour eviter KKKKKKKK
  
  debugSerial.print("OLEDdisplayCalibBal - currentMenuDepth: ");
  debugSerial.println(currentMenuDepth);
  OLEDClear();

snprintf(localOLEDbuf, 21,"=== CALIB. BAL:%d ===", bal);
  OLEDDrawText(1, 0, 0, localOLEDbuf);

// faire Fonction()
  snprintf(localOLEDbuf, 21,"Bal%d : %4.2d kg ", bal, pesonNum(bal));
  OLEDDrawText(1, 2, 0, localOLEDbuf);
  snprintf(localOLEDbuf, 21,"Scale%d : %4.2d kg ", bal, pesonScale(bal));
  OLEDDrawText(1, 3, 0, localOLEDbuf);
  snprintf(localOLEDbuf, 21,"Temp%d : %4.2d kg ", bal, TareTemp(bal)); 
  OLEDDrawText(1, 4, 0, localOLEDbuf);
  snprintf(localOLEDbuf, 21,"Tare%D : %4.2d kg ", bal, pesonTare(bal));
  OLEDDrawText(1, 5, 0, localOLEDbuf);
// fin Fonction()
  OLEDDrawText(1, 7, 0, "VALIDE pour retour");
}


// ---------------------------------------------------------------------------*
// Gestion rafraichissement écrans (System\INFOS, )
// ---------------------------------------------------------------------------*
void OLEDRefreshDisplay(void)
{
 // debugSerial.println("OLEDRefreshDisplay()");

  if (InfoScreenRefreshTime)             // rien à rafraichir
    OLEDDrawScreenRefreshTime(1, 0);    // refresh Time/Date every second      
// LoRa INFOS   
  if (LoRaScreenRefreshTime)        // rafraichir Heure
     OLEDDrawScreenRefreshTime(1, 0);    // refresh Time/Date every second 
// Balances INFOS
  if (InfoBalScreenRefreshTime)        // rafraichir Heure
     OLEDDrawScreenRefreshTime(1, 0);    // refresh Time/Date every second 
  if (InfoVSolScreenRefreshTime)        // rafraichir VSol
  {
debugSerial.println("InfoVSolScreenRefreshTime()");
 //   OLEDDrawScreenRefreshTime(1, 0);    // refresh Time/Date every second 
  }

// fin des rafraichissement d'heures

// Weight INFOS
char localOLEDbuf[21] = "";
  if (InfoBalScreenRefreshBal_1)   // rafraichir les poids
  { 
 
// debugOLEDDrawText = true;     
     
     snprintf(localOLEDbuf,15 ," %8.2f",calculePoids(0));
     OLEDDrawText(1, 3, 7, localOLEDbuf);
  } 

  if (InfoBalScreenRefreshBal_2)   // rafraichir les poids
  { 
     snprintf(localOLEDbuf, 21," %8.2f",calculePoids(1));
     OLEDDrawText(1, 4, 7, localOLEDbuf);
  } 
  if (InfoBalScreenRefreshBal_3)   // rafraichir les poids
  {
     snprintf(localOLEDbuf, 21," %8.2f",calculePoids(2));
     OLEDDrawText(1, 5, 7, localOLEDbuf);
  } 
  if (InfoBalScreenRefreshBal_4)   // rafraichir les poids
  {
     snprintf(localOLEDbuf, 21," %8.2f",calculePoids(3));
     OLEDDrawText(1, 6, 7, localOLEDbuf);
  } 
// FIN Weight INFOS
//  
  if (InfoVBatScreenRefreshTime)        // rafraichir VBat
  {
// appel à la fonction dédiée
debugSerial.println("InfoVBatScreenRefreshTime()");
    OLEDDrawScreenRefreshTime(1, 0);    // refresh Time/Date every second 
  }
  if (InfoVLumScreenRefresh)        // rafraichir VLum                            // tester en premier
  {
debugSerial.println("InfoVLumScreenRefresh()");
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



// correction OLED 2.42 pouces

// Fonction wrapper pour setCursor
void setCursorAdjusted(int16_t x, int16_t y) 
{

  if (debugOLEDDrawText)
  {
    debugSerial.print(" positionne curseur (pixels) x :"); debugSerial.print(x + OLED_Col_Offset);
    debugSerial.print(" y: "); debugSerial.println(y);
  }
  display.setCursor(x + OLED_Col_Offset, y);
}

// Fonction wrapper pour drawPixel
void drawPixelAdjusted(int16_t x, int16_t y, uint16_t color) {
  display.drawPixel(x + OLED_Col_Offset, y, color);
}

// Fonction wrapper pour drawRect
void drawRectAdjusted(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
  display.drawRect(x + OLED_Col_Offset, y, w, h, color);
}


void OLEDDisabelAllRefresh()
{
// écran InfoSysteme
          InfoScreenRefreshTime = false;   
// écran InfoLoRa
          LoRaScreenRefreshTime = false;
          LoraScreenRefreshNextPayload = false;
// écran InFoBal
  InfoBalScreenRefreshTime = false; // desactive rafraichissement time/date  
  InfoBalScreenRefreshBal_1 = false; // desactive rafraichissement Balance 1
  InfoBalScreenRefreshBal_2 = false; // desactive rafraichissement Balance 2
  InfoBalScreenRefreshBal_3 = false; // desactive rafraichissement Balance 3
  InfoBalScreenRefreshBal_4 = false; // desactive rafraichissement Balance 4
// CalibVBat() 
 InfoVBatScreenRefresh = false; // desactive rafraichissement
 InfoVBatScreenRefreshTime = false; // desactive rafraichissement time/date  
// CalibVSol()
  InfoVSolScreenRefresh = false; // desactive rafraichissement 
  InfoVSolScreenRefreshTime = false; // desactive rafraichissement time/date  
// CalibVLum()
  InfoVLumScreenRefresh = false; // desactive rafraichissement 
  InfoVLumScreenRefreshTime = false; // desactive rafraichissement time/date
}
