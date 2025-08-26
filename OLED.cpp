
#define __INIT_DONE
#include "define.h"


/**
 * @brief Initialise l'écran OLED selon le type sélectionné
 * @param Aucun
 * @return void
 */
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

/**
 * @brief Initialise l'écran OLED 0.96"
 * @param Aucun
 * @return void
 */
void OLEDInit096(void) 
{
    if (OLED) 
    {
        debugSerial.println("INIT OLED096");
#define SSD1306_SWITCHCAPVCC 0x02
        display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS);

        display.setTextSize(1);
        display.setTextColor(WHITE);
        
        display.display();
    }
}

/**
 * @brief Initialise l'écran OLED 1.30"
 * @param Aucun
 * @return void
 */
void OLEDInit130(void) 
{
    if (OLED) 
    {
        debugSerial.println("INIT OLED130");
        display.begin(OLED_ADDRESS, true);

        display.setTextSize(1);
        display.setTextColor(WHITE);
       
        display.display();
    }
}

/**
 * @brief Initialise l'écran OLED 1.54"
 * @param Aucun
 * @return void
 */
void OLEDInit154(void) 
{
    if (OLED) 
    {
        debugSerial.println("INIT OLED154");
        display.begin(OLED_ADDRESS, true);

        display.setTextSize(1);
        display.setTextColor(WHITE);
        
        display.display();
    }
}

/**
 * @brief Efface tout le contenu visuel de l'écran OLED
 * @param Aucun
 * @return void
 */
void OLEDClear(void) 
{
  display.clearDisplay();
  display.display();
//  OLEDDebugDisplay("OLEDClear()");
}


/**
 * @brief Efface du texte à une position donnée
 * @param col Colonne de départ
 * @param lig Ligne de départ
 * @param Ncar Nombre de caractères à effacer
 * @return void
 */
void OLEDEraseText(int16_t colonne, int16_t lig, int16_t Ncar)
{
    char TL_ColPixel = (colonne*OLED_Col)-(colonne?0:0); // 1:0
    char TL_LigPixel = lig * OLED_L1;
    
    if (OLED)
    {
      display.fillRect(TL_ColPixel, TL_LigPixel, OLED_Col * Ncar, OLED_L1, 0);
      display.display(); // Ajoutez ceci pour forcer l'affichage
    }
}

/**
 * @brief Affiche l'heure du prochain Payload sur l'écran OLED
 * @param col Colonne de départ
 * @param lig Ligne de départ
 * @return void
 */
void OLEDDrawScreenNextPayload(uint8_t ligne, uint8_t colonne, DateTime nextPayload )
{
    if (OLED) 
    {  
        sprintf(OLEDbuf, "PayLoad: %02d:%02d:%02d", 
                nextPayload.hour(), nextPayload.minute(), nextPayload.second());
        OLEDDrawText(1, ligne, colonne, OLEDbuf);
                
    }
}

/**
 * @brief Affiche l'heure et la date sur l'écran OLED
 * @param col Colonne de départ
 * @param lig Ligne de départ
 * @return void
 */
void OLEDDrawScreenTime(uint8_t ligne, uint8_t colonne)
{ static DateTime oldSystemTime(1692712245);  //1692712245);  // timestamp Unix

    if (OLED) 
    {  
        DateTime systemTime = getSystemTime();
        sprintf(OLEDbuf, "%02d:%02d:%02d - %02d/%02d/%d", 
                systemTime.hour(), systemTime.minute(), systemTime.second(),
                systemTime.day(), systemTime.month(), systemTime.year());
              
         if (systemTime != oldSystemTime)
         { 
            OLEDDrawText(1, ligne, colonne, OLEDbuf);
            oldSystemTime = systemTime;
         }
    }
}

/**
 * @brief Affiche l'heure et la date sur l'écran OLED
 * @param col Colonne de départ
 * @param lig Ligne de départ
 * @return void
 */
void OLEDDrawScreenRefreshTime(uint8_t ligne, uint8_t colonne)
{ static DateTime oldSystemTime(1692712245);  //1692712245);  // timestamp Unix

    if (OLED) 
    {  
        DateTime systemTime = getSystemTime();
/*
        sprintf(OLEDbuf, "%02d:%02d:%02d - %02d/%02d/%d", 
                systemTime.hour(), systemTime.minute(), systemTime.second(),
                systemTime.day(), systemTime.month(), systemTime.year());
*/
// Check changes and refresh with:
// OLEDPrintVar(uint8_t ligne, uint8_t col, const void *valeur, char type)
/* @param ligne Ligne sur l'écran OLED où afficher la variable
 * @param col Colonne sur l'écran OLED où commencer l'affichage
 * @param valeur Pointeur vers la valeur à afficher
 * @param type Type de la variable ('c', 's', 'i', 'I', 'f', 'F')
*/

/*
Police par défaut (built-in font)
Taille fixe : 6x8 pixels par caractère (incluant 1 pixel d'espacement)
Monospace : chaque caractère occupe exactement la même largeur
Positionnement prévisible : le curseur avance de 6 pixels horizontalement pour chaque caractère
*/

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
         if (systemTime != oldSystemTime)
         { 
  //          OLEDDrawText(1, ligne, colonne, OLEDbuf);
            oldSystemTime = systemTime;
         }
    }
}


/**
 * @brief Ajoute un message avec scrolling si besoin
 * @param message Message à afficher
 * @return void
 */
void OLEDDebugDisplay(char* message) 
{
    if (!modeDebugActif) return;
    
    for (int i = 0; i < MAX_LIGNES - 1; i++) 
    {
        strcpy(lignesDebug[i], lignesDebug[i+1]);
    }
    strcpy(lignesDebug[MAX_LIGNES - 1], message);
    
    display.clearDisplay();
    for (uint8_t i = 1; i < MAX_LIGNES; i++) // uint8_t i = 0 => reserve L0 pour Time/Date
    {
        display.setCursor(0, i * TAILLE_LIGNE);
        OLEDDrawText(1,i, 0, lignesDebug[i]);
    }
    display.display();
    delay(delaiAffichage);
}


/**
 * @brief efface les messages
 * @param message Message à afficher
 * @return void
 */
void OLEDDebugDisplayReset(void) 
{ char message[21];
   
    if (!modeDebugActif) return;
    
    for (int i = 1; i < MAX_LIGNES; i++) // int i = 0, on garde time/date dispo
    {
 
 //           sprintf(message, " --- %02d --- ",i);     
        strcpy(lignesDebug[i], "                    ");
    }
    
    display.clearDisplay();
    for (uint8_t i = 0; i < MAX_LIGNES; i++) 
    {
        display.setCursor(0, i * TAILLE_LIGNE);
        OLEDDrawText(1,i, 0, lignesDebug[i]);
    }
    display.display();
    delay(delaiAffichage);
}


/**
 * @brief Efface une ligne entière en la remplissant d'espaces
 * @param ligne Ligne à effacer (0 à 7)
 * @return void
 */
void non_OLEDClearLine(uint8_t ligne) 
{
    OLEDDrawText(1,ligne, 0, "                    "); // 20 espaces
delay(1000);
////    
}

/**
 * @brief Affiche un seul caractère à une position donnée
 * @param ligne Ligne (0 à 7)
 * @param colonne Colonne (0 à ~21)
 * @param c Caractère à afficher
 * @return void
 */
void OLEDPrintChar(uint8_t ligne, uint8_t colonne, char c) 
{
    display.setCursor(colonne * 6, ligne * 8);
    display.write(c);
    display.display();
}

/**
 * @brief Affiche une variable formatée sur l'écran OLED à une position donnée
 * @param ligne Ligne sur l'écran OLED où afficher la variable
 * @param col Colonne sur l'écran OLED où commencer l'affichage
 * @param valeur Pointeur vers la valeur à afficher
 * @param type Type de la variable ('c', 's', 'i', 'I', 'f', 'F')
 * @return void
 */
void OLEDPrintVar(uint8_t ligne, uint8_t col, const void *valeur, char type) 
{   char serialbuf[21];
    switch (type) 
    {
        case 'c':
            sprintf(OLEDbuf, "%c", *((char *)valeur));
  
            break;
        case 's':
            sprintf(OLEDbuf, "%s", (char *)valeur);
            break;
        case 'i':
        case 'I':
            sprintf(OLEDbuf, "%02d", *((int *)valeur));
 // %02d arrangement temporaire           
            break;
        case 'f':
        case 'F':
            sprintf(OLEDbuf, "%.2f", *((float *)valeur));
            break;
        default:
            sprintf(OLEDbuf, "<inconnu>");
            break;
    }
    OLEDDrawText(1,ligne, col, OLEDbuf);
}

/**
 * @brief Affiche une variable formatée avec unité et alignement sur l'écran OLED
 * @param ligne Ligne sur l'écran OLED
 * @param col Colonne sur l'écran OLED
 * @param valeur Pointeur vers la donnée à afficher
 * @param type Type de la variable
 * @param unite Chaîne représentant l'unité
 * @param precision Nombre de chiffres après la virgule pour les float
 * @param align Mode d'alignement ('L', 'R', 'C')
 * @return void
 */
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

/**
 * @brief Affiche un message pendant 1s sur la ligne 8
 * @param message Message à afficher
 * @param defilant True pour faire défiler le message si trop long
 * @param inverse True pour affichage inversé
 * @return void
 */
void OLEDDisplayMessageL8(const char* message, bool defilant, bool inverse) 
{
    debugSerial.print("OLEDDisplayMessageL8() => "); 
    debugSerial.println(message);
    
    display.clearDisplay();
    display.setTextSize(1);
    
    if (inverse) 
    {
        display.setTextColor(BLACK, WHITE);
    }
    else 
    {
        display.setTextColor(WHITE);
    }
    
    int len = strlen(message);
    
    if (defilant && len > (SCREEN_WIDTH / 6)) 
    {
        for (int offset = 0; offset <= (len * 6 - SCREEN_WIDTH); offset += 2) 
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
        display.setCursor((SCREEN_WIDTH - len * 6) / 2, pixelLine);
        display.print(message);
        display.display();
        delay(1000);
    }
    
    display.setTextColor(WHITE);
}


/**
 * @brief Affiche du texte avec taille et position spécifiées
 * @param Txt_Size Taille du texte
 * @param col Colonne de départ
 * @param lig Ligne de départ
 * @param text Texte à afficher
 * @return void
 */
void OLEDDrawText(int8_t Txt_Size, uint8_t ligne, uint8_t colonne, const char *text)
{
    if (OLED)
    {
        display.setTextSize(Txt_Size);
        OLEDEraseText(colonne, ligne, strlen(text));
//   debugOLEDDrawText =1;     
        if (debugOLEDDrawText)
        {
            debugSerial.print("txt: "); debugSerial.println(text);
            debugSerial.print("NB Car: "); debugSerial.print(strlen(text));
            debugSerial.print("dColonne: "); debugSerial.print(colonne); 
            debugSerial.print("/"); debugSerial.println((colonne*OLED_Col)-(colonne?0:0));
        }
//   debugOLEDDrawText=0;     
        display.setTextColor(WHITE); 
        display.setCursor((colonne*OLED_Col)-(colonne?0:0), (ligne*OLED_L1));
        
        for (uint8_t i = 0; i < strlen(text); i++)
        {
            display.write(text[i]);
        }      
        display.display();
    }
}



/**
 * @brief Affiche la date en cours de saisie sur l'écran
 * @param d Chaîne de caractères de la date
 * @param pos Position du curseur
 * @return void
 */
void OLEDDisplayDate(char *d, uint8_t pos) 
{
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



/**
 * @brief Affiche l'heure en cours de saisie sur l'écran
 * @param h Chaîne de caractères de l'heure
 * @param pos Position du curseur
 * @return void
 */
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



/**
 * @brief Affiche les informations système sur écran
 * @param Aucun
 * @return void
 */
void OLEDDisplaySystemInfo(void)
{
    if (OLED)
    {
        OLEDClear();
        OLEDDrawText(1,0, 0, "=== INFOS SYSTEME ===");
        OLEDDrawText(1,1, 0, PROJECT_NAME);
        OLEDDrawText(1,2, 0, "Version: " VERSION);
        
        sprintf(OLEDbuf, "Mode: %s", modeExploitation ? "EXPLOIT" : "PROGRAM");
        OLEDDrawText(1,3, 0, OLEDbuf);
        
        sprintf(OLEDbuf, "Config v: %d.%02d", config.materiel.version/100, config.materiel.version%100);
        OLEDDrawText(1,4, 0, OLEDbuf);
//      OLEDDrawText(1,6, 0, "Appuyer pour continuer");
    }
/*
     // Attendre une touche pour continuer
    while (readKey() == KEY_NONE)
    {
        delay(100);
    }
    
    OLEDDisplayMessageL8("Retour menu principal", false, false);
*/
}



/**
 * @brief Active ou désactive l'affichage au boot
 * @param actif True pour activer, false pour désactiver
 * @return void
 */
void OLEDSetDebug(bool actif) 
{
    modeDebugActif = actif;
}
