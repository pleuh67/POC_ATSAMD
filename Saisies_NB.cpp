//       1         2         3         4         5         6         7        7
//34567890123456789012345678901234567890123456789012345678901234567890123456789
// IMPRESSION 79 COLONES EN TAILLE 12
//
// ---------------------------------------------------------------------------*
//       _____       _     _             _   _ ____                  
//      / ____|     (_)   (_)           | \ | |  _ \                 
//     | (___   __ _ _ ___ _  ___  ___  |  \| | |_) | ___ _ __  _ __ 
//      \___ \ / _` | / __| |/ _ \/ __| | . ` |  _ < / __| '_ \| '_ \
//      ____) | (_| | \__ \ |  __/\__ \ | |\  | |_) | (__| |_) | |_) |
//     |_____/ \__,_|_|___/_|\___||___/ |_| \_|____(_)___| .__/| .__/
//                                  ______               | |   | |   
//                                 |______|              |_|   |_|    
// ---------------------------------------------------------------------------*
#define __INIT_DONE
#include "define.h"

// ---------------------------------------------------------------------------*
// initStartupList() : Menu de démarrage
// 
// startListInputWithTimeout()
// startNumberInput()
// startStringInput()
// startHexInput()
// startTimeInput()
// startDateInput()
// startIPInput()
// startEmailInput()
// 
// ---------------------------------------------------------------------------*






// toutes les machines à état de saisie retournent le résultat de la saisie dans 
// char stringSaisie[OLEDBUFLEN]="azerty";   // 128/retour de toutes les saisies
// initialisé quand XXXXXX_INPUT_COMPLETED = true

// voir pour tenter d'utilise LISTE comme MENU
// a la sélection d'une option:
//    - soit execution fonction
//    - soit propose Selection d'une nlle Liste (menus déroulants)
//    - mémorisation du menu actif pour naviguer


// ===== FONCTIONS DE SÉLECTION DANS UNE LISTE ===== 12
// ===== FONCTIONS DE SAISIE NUMÉRIQUE ===== 250
#define __SerialDebugSaisie   // decommenter pour afficher messages debug
// ===== FONCTIONS DE SAISIE ALPHANUMÉRIQUE ===== 
// suite HEXA
// suite HEURE
// suite DATE



// ---------------------------------------------------------------------------*
// ===== FONCTIONS DE SÉLECTION DANS UNE LISTE =====
// ---------------------------------------------------------------------------*
// @brief Initialise l'affichage de la liste au démarrage
// @param void
// @return void
// ---------------------------------------------------------------------------*

void initStartupList(void)
{
  // Activer la liste menu000Demarrage au démarrage
  pushMenu("-- MENU PRINCIPAL --", m0_Demarrage, M0_ITEM, 0);

  startupListActivated = true;
  debugSerial.println("Menu principal active");
  debugSerial.print("currentMenuDepth apres init: ");
  debugSerial.println(currentMenuDepth);

// debugSerialPrintMenuStruct(currentMenu);
//      menuLevel_t* currentMenu = &menuStack[currentMenuDepth - 1];
//      startListInput(currentMenu->title, currentMenu->menuList, currentMenu->menuSize, currentMenu->selectedIndex);
}

// ---------------------------------------------------------------------------*
// ===== FONCTIONS DE SAISIE DANS LISTE =====
// ---------------------------------------------------------------------------*
// @brief Démarre la sélection dans une liste de valeurs avec timeout personnalisé
// @param title Titre de la sélection à afficher
// @param itemList Tableau de pointeurs vers les chaînes de caractères
// @param numItems Nombre d'éléments dans la liste
// @param initialIndex Index initial sélectionné
// @param timeoutMs Timeout en millisecondes (0 = pas de timeout)
// @return void
// ---------------------------------------------------------------------------*
void startListInputWithTimeout(const char* title, const char** itemList, uint8_t numItems, uint8_t initialIndex, unsigned long timeoutMs)
{

//   ici inseré: void startListInput(const char* title, const char** itemList, uint8_t numItems, uint8_t initialIndex)
//debugSerial.print("Param : ");debugSerial.println(itemList[2]);
  if (listInputCtx.state != LIST_INPUT_IDLE)
  {
    return; // Sélection déjà en cours
  }
  
  // Validation des paramètres
  if (initialIndex >= numItems) initialIndex = 0;
  
  // Initialisation du contexte
  listInputCtx.state = LIST_INPUT_ACTIVE;
  listInputCtx.selectedIndex = initialIndex;
  listInputCtx.maxItems = numItems;
  listInputCtx.itemList = itemList;
//debugSerial.print("struct: ");debugSerial.println(listInputCtx.itemList[2]);
  strncpy(listInputCtx.title, title, 20);  
  listInputCtx.title[20] = '\0';
  
  // Calcul du décalage initial pour centrer l'élément sélectionné
  if (initialIndex < 3)
  {
    listInputCtx.scrollOffset = 0;
  }
  else if (initialIndex >= numItems - 3)
  {
    listInputCtx.scrollOffset = (numItems > 6) ? numItems - 6 : 0;
  }
  else
  {
    listInputCtx.scrollOffset = initialIndex - 3;
  }
  
  listInputCtx.displayRefresh = true;
  listInputCtx.lastUpdate = millis();
  listInputCtx.cursorBlink = true;
  listInputCtx.lastBlink = millis();
  listInputCtx.lastActivity = millis();      // Initialiser le timeout
  listInputCtx.timeoutDuration = 0;          // 0 = pas de timeout par défaut
  listInputCtx.lastScrollOffset = 0xFF;      // Forcer le rafraîchissement initial
  listInputCtx.lastSelectedIndex = 0xFF;     // Forcer le rafraîchissement initial
  listInputCtx.lastCursorBlink = false;
    
  debugSerial.print("Selection dans liste demarree: ");
  debugSerial.println(title);
  OLEDDisplayMessageL8("Choix d'une valeur", false, false);
 
// ---------------  
 // startListInput(title, itemList, numItems, initialIndex);
  if (timeoutMs > 0)
  {
    listInputCtx.timeoutDuration = timeoutMs;
  }
    else
  {
    listInputCtx.timeoutDuration = 0; // 0 = désactivé
  }
}


// ---------------------------------------------------------------------------*
// @brief Traite la sélection dans la liste (à appeler dans loop)
// @param void
// @return État actuel de la sélection
// ---------------------------------------------------------------------------*
listInputState_t processListInput(void)
{
  if (listInputCtx.state != LIST_INPUT_ACTIVE)
  {
    return listInputCtx.state;
  }
  
  // Gestion du clignotement du curseur
  updateListInputCursorBlink();

 // Vérification du timeout
  if (listInputCtx.timeoutDuration > 0 && (millis() - listInputCtx.lastActivity > listInputCtx.timeoutDuration))
  {
    listInputCtx.state = LIST_INPUT_CANCELLED;
    debugSerial.println("Selection annulee par timeout");
    OLEDDisplayMessageL8("Timeout", false, false);
    return listInputCtx.state;
  }
  
  // Traitement des touches
  if (touche != KEY_NONE)
  {
    switch (touche)
    {
//      case LEFT:
      case UP:
        // Remonter dans la liste
        if (listInputCtx.selectedIndex > 0)
        {
          listInputCtx.selectedIndex--;
          
          // Gestion du défilement vers le haut
          if (listInputCtx.selectedIndex < listInputCtx.scrollOffset)
          {
            listInputCtx.scrollOffset = listInputCtx.selectedIndex;
          }
        }
        else
        {
          listInputCtx.selectedIndex = listInputCtx.maxItems - 1; // Boucler à la fin
          
          // Ajuster le décalage pour afficher la fin de la liste
          if (listInputCtx.maxItems > 6)
          {
            listInputCtx.scrollOffset = listInputCtx.maxItems - 6;
          }
        }
        listInputCtx.displayRefresh = true;
        debugSerial.print("Selection index: ");
        debugSerial.print(listInputCtx.selectedIndex);
        debugSerial.print(" - ");
        debugSerial.println(listInputCtx.itemList[listInputCtx.selectedIndex]);
        break;
        
//      case RIGHT:
      case DOWN:
        // Descendre dans la liste

debugSerial.println("Down de processListInput"); 
        
        if (listInputCtx.selectedIndex < listInputCtx.maxItems - 1)
        {
          listInputCtx.selectedIndex++;
          
          // Gestion du défilement vers le bas
          if (listInputCtx.selectedIndex >= listInputCtx.scrollOffset + 6)
          {
            listInputCtx.scrollOffset = listInputCtx.selectedIndex - 5;
          }
        }
        else
        {
          listInputCtx.selectedIndex = 0; // Boucler au début
          listInputCtx.scrollOffset = 0;  // Remettre au début de l'affichage
        }
        listInputCtx.displayRefresh = true;
        debugSerial.print("Selection index: ");
        debugSerial.print(listInputCtx.selectedIndex);
        debugSerial.print(" - ");
        debugSerial.println(listInputCtx.itemList[listInputCtx.selectedIndex]);
        break;
        
      case VALIDE:
        listInputCtx.state = LIST_INPUT_COMPLETED;
        debugSerial.print("Selection validee: ");
        debugSerial.print(listInputCtx.selectedIndex);
        debugSerial.print(" - ");
        debugSerial.println(listInputCtx.itemList[listInputCtx.selectedIndex]);
        OLEDDisplayMessageL8("Selection validee", false, false);
        break;
        
      default:
        break;
    }
    
    // Reset de la touche après traitement
    touche = KEY_NONE;
  }

  // Rafraîchissement de l'affichage si nécessaire (clignotement, scrolling liste, ///)
  if (listInputCtx.displayRefresh || (millis() - listInputCtx.lastUpdate > 100))
  {
    refreshListDisplay();
    listInputCtx.displayRefresh = false;
    listInputCtx.lastUpdate = millis();
  }   
  return listInputCtx.state;
}

// ---------------------------------------------------------------------------*
// @brief Finalise la sélection et récupère l'index choisi
// @param void
// @return Index de l'élément sélectionné
// ---------------------------------------------------------------------------*
uint8_t finalizeListInput(void)
{
  uint8_t selectedIndex = 0;
  
  if (listInputCtx.state == LIST_INPUT_COMPLETED)
  {
    selectedIndex = listInputCtx.selectedIndex;
    
    // Reset du contexte
    listInputCtx.state = LIST_INPUT_IDLE;
    listInputCtx.selectedIndex = 0;
    listInputCtx.scrollOffset = 0;
    listInputCtx.maxItems = 0;
    listInputCtx.itemList = NULL;
    
    debugSerial.print("Index final selectionne: ");
    debugSerial.println(selectedIndex);
  }
  return selectedIndex;
}

// ---------------------------------------------------------------------------*
// @brief Annule la sélection dans la liste
// @param void
// @return void
// ---------------------------------------------------------------------------*
void cancelListInput(void)
{
  listInputCtx.state = LIST_INPUT_CANCELLED;
  debugSerial.println("Selection dans liste annulee");
  OLEDDisplayMessageL8("Selection annulee", false, false);
  
  // Reset du contexte après un délai
  listInputCtx.state = LIST_INPUT_IDLE;
}

// ---------------------------------------------------------------------------*
// @brief Vérifie si une sélection dans une liste est en cours
// @param void
// @return true si sélection active
// ---------------------------------------------------------------------------*
bool isListInputActive(void)
{
  return (listInputCtx.state == LIST_INPUT_ACTIVE);
}

// ---------------------------------------------------------------------------*
// @brief Rafraîchit l'affichage de la liste avec curseur et défilement
// @param void
// @return void
// ---------------------------------------------------------------------------*
void refreshListDisplay(void)
{ char timeoutMsg[21];
  bool scrollChanged = (listInputCtx.scrollOffset != listInputCtx.lastScrollOffset);
  bool selectionChanged = (listInputCtx.selectedIndex != listInputCtx.lastSelectedIndex);
  bool cursorBlinkChanged = (listInputCtx.cursorBlink != listInputCtx.lastCursorBlink);
  
  // Afficher le titre seulement au premier affichage
  if (listInputCtx.lastScrollOffset == 0xFF)
  {
    OLEDDrawText(1, 0, 0, listInputCtx.title);
  }
  
  // Calcul du nombre d'éléments à afficher (maximum 6 lignes disponibles)
  uint8_t displayCount = (listInputCtx.maxItems < 6) ? listInputCtx.maxItems : 6;
  
  // Si le scroll a changé, rafraîchir toutes les lignes
  if (scrollChanged)
  {
    for (uint8_t i = 0; i < 6; i++) // Toujours effacer 6 lignes
    {
      if (i < displayCount)
      {
        uint8_t itemIndex = listInputCtx.scrollOffset + i;
        if (itemIndex < listInputCtx.maxItems)
        {
          char lineBuffer[21];
          
          // Préparer la ligne avec indicateur de sélection (limité à 20 caractères)
          if (itemIndex == listInputCtx.selectedIndex && listInputCtx.cursorBlink)
          {
            snprintf(lineBuffer, 21, "> %-18.18s", listInputCtx.itemList[itemIndex]);
          }
          else
          {
            snprintf(lineBuffer, 21, "  %-18.18s", listInputCtx.itemList[itemIndex]);
          }
          
          // Afficher la ligne (lignes 1 à 6)
          OLEDDrawText(1, i + 1, 0, lineBuffer);
        }
        else
        {
          // Effacer les lignes vides
          OLEDDrawText(1, i + 1, 0, "--------------------");
        }
      }
      else
      {
        // Effacer les lignes non utilisées (passage à menu plus court par ex.)
        OLEDDrawText(1, i + 1, 0, "++++++++++++++++++++");
      }
    }
  }
  // Sinon, rafraîchir seulement les lignes affectées par la sélection ou le clignotement
  else if (selectionChanged  || cursorBlinkChanged )
  {
    // Rafraîchir l'ancienne ligne sélectionnée (si visible)
    if (listInputCtx.lastSelectedIndex != 0xFF && 
        listInputCtx.lastSelectedIndex >= listInputCtx.scrollOffset && 
        listInputCtx.lastSelectedIndex < listInputCtx.scrollOffset + displayCount &&
        !cursorBlinkChanged ) // moi pour derniere condition
    {
      uint8_t lineIndex = listInputCtx.lastSelectedIndex - listInputCtx.scrollOffset;
      char lineBuffer[21];
      snprintf(lineBuffer, 21, "  "); //%-18.18s", listInputCtx.itemList[listInputCtx.lastSelectedIndex]); // juste efface "> "
      OLEDDrawText(1, lineIndex + 1, 0, lineBuffer);
    }
    
    // Rafraîchir la nouvelle ligne sélectionnée (si visible)
    if (listInputCtx.selectedIndex >= listInputCtx.scrollOffset && 
        listInputCtx.selectedIndex < listInputCtx.scrollOffset + displayCount)
    {
      uint8_t lineIndex = listInputCtx.selectedIndex - listInputCtx.scrollOffset;
      char lineBuffer[21];
      
      if (listInputCtx.cursorBlink)
      {
        snprintf(lineBuffer, 21, "> "); //%-18.18s", listInputCtx.itemList[listInputCtx.selectedIndex]);
      }
      else
      {
        snprintf(lineBuffer, 21, "  "); //%-18.18s", listInputCtx.itemList[listInputCtx.selectedIndex]);
      }
//debugSerial.println(lineBuffer);     
      OLEDDrawText(1, lineIndex + 1, 0, lineBuffer);
    }
  }

/*   
  // Afficher l'indicateur de position si plus de 6 éléments (toujours rafraîchir si sélection a changé)
  if (listInputCtx.maxItems > 6 && (selectionChanged || listInputCtx.lastScrollOffset == 0xFF))
  {
    char scrollInfo[6];
    snprintf(scrollInfo, 6, "(%d/%d)", listInputCtx.selectedIndex + 1, listInputCtx.maxItems);
    OLEDDrawText(1, 6, 15, scrollInfo); // Colonne 15 pour alignement à droite
  }
*/  
  // Afficher les instructions en bas (ligne 7) avec indication du timeout si activé
  // Rafraîchir seulement si le clignotement change ou au premier affichage
  if (/* cursorBlinkChanged ||*/ listInputCtx.lastScrollOffset == 0xFF)
  {
   // if (listInputCtx.cursorBlink)
   // {
      if (listInputCtx.timeoutDuration > 0) // Timeout activé
      { 
        unsigned long remainingTime = (listInputCtx.timeoutDuration - (millis() - listInputCtx.lastActivity)) / 1000;
        if (remainingTime <= 5) // Afficher le countdown les 5 dernières secondes
        {
          snprintf(timeoutMsg, 21, "+/-:Nav VALIDE:OK %lds", remainingTime);
          OLEDDrawText(1, 7, 0, timeoutMsg);
        }
        else
        {
          snprintf(timeoutMsg, 21, "+/-: Nav  VALIDE: OK");
          OLEDDrawText(1, 7, 0, timeoutMsg);
//          OLEDDrawText(1, 7, 0, "+/-: Nav  VALIDE: OK");
        }
      }
      else // Pas de timeout
      {
          snprintf(timeoutMsg, 21, "+/-: Nav  VALIDE: OK");
          OLEDDrawText(1, 7, 0, timeoutMsg);        
//        OLEDDrawText(1, 7, 0, "+/-: Nav  VALIDE: OK");
      }
//    }
   // else
   // {
   //   OLEDDrawText(1, 7, 0, "********************"); // Effacer pendant le non-clignotement
   // }
//debugSerial.println(timeoutMsg);
  }
  
  // Sauvegarder les états pour la prochaine comparaison
  listInputCtx.lastScrollOffset = listInputCtx.scrollOffset;
  listInputCtx.lastSelectedIndex = listInputCtx.selectedIndex;
  listInputCtx.lastCursorBlink = listInputCtx.cursorBlink;
}

// ---------------------------------------------------------------------------*
// @brief Gère le clignotement du curseur pour la sélection de liste
// @param void
// @return void
// ---------------------------------------------------------------------------*
void updateListInputCursorBlink(void)
{
  if (millis() - listInputCtx.lastBlink > 500) // Clignotement toutes les 500ms
  {
    listInputCtx.cursorBlink = !listInputCtx.cursorBlink;
    listInputCtx.lastBlink = millis();
    listInputCtx.displayRefresh = true;
  }
}

// -------------------------------------------------------------------------------------
// ===== FONCTIONS DE SAISIE NUMÉRIQUE =====
// -------------------------------------------------------------------------------------
// exemple d'appel :
// Saisie numérique positive: 
//  static char currentNumber[11] = "12345";
//  startNumberInput("SAISIE NOMBRE:", currentNumber, 8, false);
// Saisie numérique avec négatifs:
//  static char currentNumber[11] = "-123";
//  startNumberInput("NOMBRE +/-:", currentNumber, 8, true);


// startNumberInput(initialise sans affichage)
// refreshNumberDisplay(affichage) & processNumberInput(traite touche et mets touche = KEY_NONE; + affichage)


// ---------------------------------------------------------------------------*
// @brief Démarre la saisie numérique non-bloquante, initialise MAE
// @param title Titre à afficher
// @param initialNumber Nombre initial
// @param maxLength Longueur maximum (max 10)
// @param allowNegative Autoriser les nombres négatifs
// @return void
// ---------------------------------------------------------------------------*
void startNumberInput(const char* title, const char* initialNumber, uint8_t maxLength, bool allowNegative)
{
  if (numberInputCtx.state != NUMBER_INPUT_IDLE)
  {
    return; // Saisie déjà en cours
  }
  
  // Validation des paramètres
  if (maxLength > 10) maxLength = 10;
  
  // Initialisation du contexte
  numberInputCtx.state = NUMBER_INPUT_ACTIVE;
  numberInputCtx.position = 0;
  numberInputCtx.maxLength = maxLength;
  numberInputCtx.allowNegative = allowNegative;
  
  // Copier le nombre initial
  if (initialNumber)
  {
    strncpy(numberInputCtx.workingNumber, initialNumber, maxLength);
    numberInputCtx.workingNumber[maxLength] = '\0';
  }
  else
  {
    // Nombre vide par défaut (espaces)
    for (int i = 0; i < maxLength; i++)
    {
      numberInputCtx.workingNumber[i] = ' ';
    }
    numberInputCtx.workingNumber[maxLength] = '\0';
  }
  
  // Titre
  strncpy(numberInputCtx.title, title, 21);
  numberInputCtx.title[20] = '\0';
  
  numberInputCtx.displayRefresh = true;
  numberInputCtx.lastUpdate = millis();
  numberInputCtx.cursorBlink = true;
  numberInputCtx.lastBlink = millis();
  
#ifdef __SerialDebugSaisie
  debugSerial.println("\nLancement saisie numerique: ");
//  debugSerial.println(title);
  debugSerialPrintNumberStruct();   // cas d'une saisie numérique:
#endif
  OLEDDisplayMessageL8("Modifiez le nombre", false, false);
}


// ---------------------------------------------------------------------------*
// @brief Obtient le caractère numérique suivant/précédent
// @param current Caractère actuel
// @param delta Direction (+1 ou -1)
// @param allowNegative Autoriser le signe moins
// @param position Position du caractère (0 = premier caractère)
// @return Nouveau caractère numérique
// ---------------------------------------------------------------------------*
char getNextNumericChar(char current, int delta, bool allowNegative, uint8_t position)
{
  // Pour la première position, inclure le signe moins si autorisé
  if (position == 0 && allowNegative)
  {
    const char numericChars[] = " 0123456789-";
    const int numChars = sizeof(numericChars) - 1; // -1 pour le '\0'
    
    // Trouver l'index du caractère actuel
    int currentIndex = -1;
    for (int i = 0; i < numChars; i++)
    {
      if (numericChars[i] == current)
      {
        currentIndex = i;
        break;
      }
    }
    
    if (currentIndex == -1) currentIndex = 0; // Caractère invalide, utiliser espace
    
    // Calculer le nouvel index avec bouclage
    int newIndex = currentIndex + delta;
    if (newIndex < 0) newIndex = numChars - 1;
    if (newIndex >= numChars) newIndex = 0;
    
    return numericChars[newIndex];
  }
  else
  {
    // Pour les autres positions, seulement les chiffres et l'espace
    const char numericChars[] = " 0123456789";
    const int numChars = sizeof(numericChars) - 1; // -1 pour le '\0'
    
    // Trouver l'index du caractère actuel
    int currentIndex = -1;
    for (int i = 0; i < numChars; i++)
    {
      if (numericChars[i] == current)
      {
        currentIndex = i;
        break;
      }
    }
    
    if (currentIndex == -1) currentIndex = 0; // Caractère invalide, utiliser espace
    
    // Calculer le nouvel index avec bouclage
    int newIndex = currentIndex + delta;
    if (newIndex < 0) newIndex = numChars - 1;
    if (newIndex >= numChars) newIndex = 0;
    
    return numericChars[newIndex];
  }
}

// ---------------------------------------------------------------------------*
// @brief Valide qu'une chaîne représente un nombre valide
// @param number Chaîne à valider
// @return true si nombre valide
// ---------------------------------------------------------------------------*
bool isValidNumber(const char* number)
{
  if (!number || strlen(number) == 0) return false;
  
  bool hasDigit = false;
  bool hasSign = false;
  
  for (int i = 0; number[i] != '\0'; i++)
  {
    char c = number[i];
    
    if (c == ' ') continue; // Ignorer les espaces
    
    if (c == '-')
    {
      if (hasSign || hasDigit || i != 0) return false; // Signe seulement en première position
      hasSign = true;
    }
    else if (c >= '0' && c <= '9')
    {
      hasDigit = true;
    }
    else
    {
      return false; // Caractère invalide
    }
  }
  
  return hasDigit; // Au moins un chiffre requis
}


// ---------------------------------------------------------------------------*
// @brief Traite la saisie numérique (à appeler dans loop)
// @param void
// @return État actuel de la saisie
// ---------------------------------------------------------------------------*
numberInputState_t processNumberInput(void)
{
  if (numberInputCtx.state != NUMBER_INPUT_ACTIVE)
  {
    return numberInputCtx.state;
  }
  
  // Gestion du clignotement du curseur
  updateNumberInputCursorBlink();
  
  // Traitement des touches
  if (touche != KEY_NONE)
  {
sprintf(serialbuf, "processNumberInput: %s ", keyToString(touche)); 
debugSerial.println(serialbuf);
    switch (touche)
    {
      case LEFT:
        if (numberInputCtx.position > 0)
        {
          numberInputCtx.position--;
          numberInputCtx.displayRefresh = true;
          debugSerial.print("Position curseur nombre: ");
          debugSerial.println(numberInputCtx.position);
        }
        break;
        
      case RIGHT:
        if (numberInputCtx.position < numberInputCtx.maxLength - 1)
        {
          numberInputCtx.position++;
          numberInputCtx.displayRefresh = true;
          debugSerial.print("Position curseur nombre: ");
          debugSerial.println(numberInputCtx.position);
        }
        break;
        
      case PLUS:
        modifyNumberChar(numberInputCtx.workingNumber, numberInputCtx.position, +1);
        numberInputCtx.displayRefresh = true;
        break;
        
      case MOINS:
        modifyNumberChar(numberInputCtx.workingNumber, numberInputCtx.position, -1);
        numberInputCtx.displayRefresh = true;
        break;
        
      case VALIDE:
        // Vérifier que le nombre est valide avant de valider
        if (isValidNumber(numberInputCtx.workingNumber))
        {
          numberInputCtx.state = NUMBER_INPUT_COMPLETED;
          debugSerial.print("Nombre valide: ");
          debugSerial.println(numberInputCtx.workingNumber);
          OLEDDisplayMessageL8("Nombre accepte", false, false);
        }
        else
        {
          debugSerial.println("Nombre invalide - saisie continuee");
          OLEDDisplayMessageL8("Nombre invalide", false, false);
          delay(1000); // Pause pour afficher le message
        }
        break;
        
      default:
        break;
    }
    // Reset de la touche après traitement
    touche = KEY_NONE;
  }
  
  // Rafraîchissement de l'affichage si nécessaire
  if (numberInputCtx.displayRefresh || (millis() - numberInputCtx.lastUpdate > 100))
  {
    refreshNumberDisplay();
    numberInputCtx.displayRefresh = false;
    numberInputCtx.lastUpdate = millis();
  }
  return numberInputCtx.state;
}

// ---------------------------------------------------------------------------*
// @brief Finalise la saisie et récupère le nombre
// @param outputNumber Buffer pour stocker le nombre final
// @return void
// ---------------------------------------------------------------------------*
void finalizeNumberInput(char* outputNumber)
{
  if (numberInputCtx.state == NUMBER_INPUT_COMPLETED)
  {
    strncpy(outputNumber, numberInputCtx.workingNumber, numberInputCtx.maxLength);
    outputNumber[numberInputCtx.maxLength] = '\0';
    
    // Supprimer les espaces en début et fin de chaîne
    int len = strlen(outputNumber);
    
    // Supprimer les espaces à la fin
    while (len > 0 && outputNumber[len - 1] == ' ')
    {
      outputNumber[--len] = '\0';
    }
    
    // Supprimer les espaces au début (sauf si c'est un signe moins)
    int start = 0;
    while (outputNumber[start] == ' ' && outputNumber[start + 1] != '-')
    {
      start++;
    }
    
    if (start > 0)
    {
      memmove(outputNumber, outputNumber + start, len - start + 1);
    }
    
    // Reset du contexte
    numberInputCtx.state = NUMBER_INPUT_IDLE;
    numberInputCtx.position = 0;
    
    debugSerial.print("Nombre final: ");
    debugSerial.println(outputNumber);
  }
}

// ---------------------------------------------------------------------------*
// @brief Annule la saisie numérique
// @param void
// @return void
// ---------------------------------------------------------------------------*
void cancelNumberInput(void)
{
  numberInputCtx.state = NUMBER_INPUT_CANCELLED;
  debugSerial.println("Saisie numerique annulee");
  OLEDDisplayMessageL8("Saisie annulee", false, false);
  
  // Reset du contexte après un délai
  numberInputCtx.state = NUMBER_INPUT_IDLE;
}

// ---------------------------------------------------------------------------*
// @brief Vérifie si une saisie numérique est en cours
// @param void
// @return true si saisie active
// ---------------------------------------------------------------------------*
bool isNumberInputActive(void)
{
  return (numberInputCtx.state == NUMBER_INPUT_ACTIVE);
}

// ---------------------------------------------------------------------------*
// @brief Rafraîchit l'affichage du nombre avec curseur
// @param void
// @return void
// ---------------------------------------------------------------------------*
void refreshNumberDisplay(void)
{
  // Afficher le titre (ligne 0)
  OLEDDrawText(1, 0, 0, numberInputCtx.title);

  // Afficher le nombre en cours (ligne 1, tronqué à 20 caractères)
//  char displayText[21];
  strncpy(/*displayText*/OLEDbuf, numberInputCtx.workingNumber, 20);
  OLEDbuf/*displayText*/[20] = '\0';
  OLEDDrawText(1, 1, 0, OLEDbuf/*displayText*/);
  
  // Afficher l'indicateur de position si curseur visible
  if (numberInputCtx.cursorBlink)
  {
    // Créer l'indicateur de position (ligne 2, tronqué à 20 caractères)
//    char underline[21]
    sprintf(OLEDbuf,"                    "); // 20 espaces
    if (numberInputCtx.position < 20)
    {
      OLEDbuf/*underline*/[numberInputCtx.position] = '^';
    }
    OLEDbuf/*underline*/[20] = '\0';
    OLEDDrawText(1, 2, 0, OLEDbuf/*underline*/);
    

 //displayNumberDebug = true;
  if (displayNumberDebug)                       // Afficher infos debug
  {        
    // Afficher la position et le caractère actuel (ligne 3)
//    char posCharInfo[21];
    snprintf(OLEDbuf/*posCharInfo*/, 21, "Pos:%02d/%02d Car:'%c'", 
             numberInputCtx.position + 1, numberInputCtx.maxLength, 
             numberInputCtx.workingNumber[numberInputCtx.position]);
    OLEDDrawText(1, 3, 0, OLEDbuf/*posCharInfo*/);
    
    // Afficher l'état de validation (ligne 4)
    if (isValidNumber(numberInputCtx.workingNumber))
    {
      OLEDDrawText(1, 4, 0, "Nombre: VALIDE");
    }
    else
    {
      OLEDDrawText(1, 4, 0, "Nombre: INVALIDE");
    }
  }  
// displayNumberDebug = false;
    // Afficher les instructions (lignes 5 et 6)
    OLEDDrawText(1, 5, 0, "+/- : Modifier nb");
    OLEDDrawText(1, 6, 0, "</> : Pos  V : OK");
  }
  else
  {
    // Effacer l'indicateur (lignes 2, 3, 4, 5, 6)
    OLEDDrawText(1, 2, 0, " E                  ");
    OLEDDrawText(1, 3, 0, "    F               ");
    OLEDDrawText(1, 4, 0, "       F            ");
    OLEDDrawText(1, 5, 0, "          A         ");
    OLEDDrawText(1, 6, 0, "             C  E   ");
  }
}

// ---------------------------------------------------------------------------*
// @brief Gère le clignotement du curseur pour la saisie numérique
// @param void
// @return void
// ---------------------------------------------------------------------------*
void updateNumberInputCursorBlink(void)
{
  if (millis() - numberInputCtx.lastBlink > 500) // Clignotement toutes les 500ms
  {
    numberInputCtx.cursorBlink = !numberInputCtx.cursorBlink;
    numberInputCtx.lastBlink = millis();
    numberInputCtx.displayRefresh = true;
  }
}

// ---------------------------------------------------------------------------*
// @brief Modifie un caractère du nombre
// @param str Chaîne de caractères représentant le nombre
// @param pos Position du caractère à modifier
// @param delta Valeur à ajouter (+1 ou -1)
// @return void
// ---------------------------------------------------------------------------*
void modifyNumberChar(char* str, uint8_t pos, int delta)
{
  if (pos >= numberInputCtx.maxLength)
  {
    return;
  }
  
  str[pos] = getNextNumericChar(str[pos], delta, numberInputCtx.allowNegative, pos);
  
  debugSerial.print("Position nombre ");
  debugSerial.print(pos);
  debugSerial.print(" modifiee: '");
  debugSerial.print(str[pos]);
  debugSerial.println("'");
}


// ---------------------------------------------------------------------------*
// ===== FONCTIONS DE SAISIE ALPHANUMÉRIQUE =====
// ---------------------------------------------------------------------------*
// @brief Démarre la saisie alphanumérique non-bloquante
// @param title Titre à afficher
// @param initialString Chaîne initiale
// @param maxLength Longueur maximum (max 20)
// @return void
// ---------------------------------------------------------------------------*
void startStringInput(const char* title, const char* initialString, uint8_t maxLength)
{
  if (stringInputCtx.state != STRING_INPUT_IDLE)
  {
    return; // Saisie déjà en cours
  }
  
  // Validation des paramètres
  if (maxLength > 20) maxLength = 20;
  
  // Initialisation du contexte
  stringInputCtx.state = STRING_INPUT_ACTIVE;
  stringInputCtx.position = 0;
  stringInputCtx.maxLength = maxLength;
  
  // Copier la chaîne initiale
  if (initialString)
  {
    strncpy(stringInputCtx.workingString, initialString, maxLength);
    stringInputCtx.workingString[maxLength] = '\0';
  }
  else
  {
    // Chaîne vide par défaut
    for (int i = 0; i < maxLength; i++)
    {
      stringInputCtx.workingString[i] = ' ';
    }
    stringInputCtx.workingString[maxLength] = '\0';
  }
  
  // Titre
  strncpy(stringInputCtx.title, title, 21);
  stringInputCtx.title[20] = '\0';
  
  stringInputCtx.displayRefresh = true;
  stringInputCtx.lastUpdate = millis();
  stringInputCtx.cursorBlink = true;
  stringInputCtx.lastBlink = millis();
  
  debugSerial.print("Saisie alphanumérique demarree: ");
  debugSerial.println(title);

#ifdef __SerialDebugSaisie
  debugSerial.println("\nLancement saisie alphaNumerique: ");
  debugSerialPrintStringStruct();   // cas d'une saisie numérique:
#endif
  OLEDDisplayMessageL8("Modifiez le texte", false, false);
}



// ---------------------------------------------------------------------------*
// @brief Rafraîchit l'affichage de la chaîne avec curseur
// @param void
// @return void
// ---------------------------------------------------------------------------*
void refreshStringDisplay(void)
{
  // Afficher le titre (ligne 0)
  OLEDDrawText(1, 0, 0, stringInputCtx.title);
  
  // Afficher la chaîne en cours (ligne 2, tronquée à 20 caractères)
  char displayText[21];
  strncpy(displayText, stringInputCtx.workingString, 20);
  displayText[20] = '\0';
  OLEDDrawText(1, 2, 0, displayText);
  
  // Afficher l'indicateur de position si curseur visible
  if (stringInputCtx.cursorBlink)
  {
    // Créer l'indicateur de position (ligne 3, tronqué à 20 caractères)
    char underline[21] = "                    "; // 20 espaces
    if (stringInputCtx.position < 20)
    {
      underline[stringInputCtx.position] = '^';
    }
    underline[20] = '\0';
    OLEDDrawText(1, 3, 0, underline);
  }
  else
  {
    // Effacer l'indicateur (lignes 3, 4, 5)
    OLEDDrawText(1, 3, 0, "                    ");
    OLEDDrawText(1, 4, 0, "                    ");
    OLEDDrawText(1, 5, 0, "                    ");
  }

 //displayStringDebug = true;
  if (displayStringDebug)                       // Afficher infos debug
  {    
    char posInfo[21];
    snprintf(posInfo, 21, "Pos:%02d/%02d", stringInputCtx.position + 1, stringInputCtx.maxLength);
    OLEDDrawText(1, 4, 0, posInfo);
    
    // Afficher le caractère actuel (ligne 5)
    char currentCharInfo[21];
    snprintf(currentCharInfo, 21, "Car:'%c'", stringInputCtx.workingString[stringInputCtx.position]);
    OLEDDrawText(1, 5, 0, currentCharInfo);
  }
// displayStringDebug = false;


  
  // Afficher les instructions (lignes 6 et 7)
  if (stringInputCtx.cursorBlink)
  {
    OLEDDrawText(1, 6, 0, "+/-:Car </>:Pos");
    OLEDDrawText(1, 7, 0, "VALIDE:OK");
  }
}



// ---------------------------------------------------------------------------*
// @brief Obtient le caractère alphanumérique suivant/précédent
// @param current Caractère actuel
// @param delta Direction (+1 ou -1)
// @return Nouveau caractère alphanumérique
// ---------------------------------------------------------------------------*
char getNextAlphaNumChar(char current, int delta)
{
  // Caractères disponibles : 0-9, A-Z, espace, tiret, underscore
  const char alphaNumChars[] = " 0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ-_";
  const int numChars = sizeof(alphaNumChars) - 1; // -1 pour le '\0'
  
  // Trouver l'index du caractère actuel
  int currentIndex = -1;
  char upperCurrent = toupper(current);
  
  for (int i = 0; i < numChars; i++)
  {
    if (alphaNumChars[i] == upperCurrent)
    {
      currentIndex = i;
      break;
    }
  }
  
  if (currentIndex == -1) currentIndex = 0; // Caractère invalide, utiliser espace
  
  // Calculer le nouvel index avec bouclage
  int newIndex = currentIndex + delta;
  if (newIndex < 0) newIndex = numChars - 1;
  if (newIndex >= numChars) newIndex = 0;
  
  return alphaNumChars[newIndex];
}


// ---------------------------------------------------------------------------*
// @brief Traite la saisie alphanumérique (à appeler dans loop)
// @param void
// @return État actuel de la saisie
// ---------------------------------------------------------------------------*
stringInputState_t processStringInput(void)
{
  if (stringInputCtx.state != STRING_INPUT_ACTIVE)
  {
    return stringInputCtx.state;
  }
  
  // Gestion du clignotement du curseur
  updateStringInputCursorBlink();
  
  // Traitement des touches
  if (touche != KEY_NONE)
  {
    switch (touche)
    {
      case LEFT:
        if (stringInputCtx.position > 0)
        {
          stringInputCtx.position--;
          stringInputCtx.displayRefresh = true;
          debugSerial.print("Position curseur string: ");
          debugSerial.println(stringInputCtx.position);
        }
        break;
        
      case RIGHT:
        if (stringInputCtx.position < stringInputCtx.maxLength - 1)
        {
          stringInputCtx.position++;
          stringInputCtx.displayRefresh = true;
          debugSerial.print("Position curseur string: ");
          debugSerial.println(stringInputCtx.position);
        }
        break;
        
      case PLUS:
        modifyStringChar(stringInputCtx.workingString, stringInputCtx.position, +1);
        stringInputCtx.displayRefresh = true;
        break;
        
      case MOINS:
        modifyStringChar(stringInputCtx.workingString, stringInputCtx.position, -1);
        stringInputCtx.displayRefresh = true;
        break;
        
      case VALIDE:
        stringInputCtx.state = STRING_INPUT_COMPLETED;
        debugSerial.print("Chaine alphanumérique validee: ");
        debugSerial.println(stringInputCtx.workingString);
        OLEDDisplayMessageL8("Texte accepte", false, false);
        break;
        
      default:
        break;
    }
    
    // Reset de la touche après traitement
    touche = KEY_NONE;
  }
  
  // Rafraîchissement de l'affichage si nécessaire
  if (stringInputCtx.displayRefresh || (millis() - stringInputCtx.lastUpdate > 100))
  {
    refreshStringDisplay();
    stringInputCtx.displayRefresh = false;
    stringInputCtx.lastUpdate = millis();
  }
  
  return stringInputCtx.state;
}

// ---------------------------------------------------------------------------*
// @brief Finalise la saisie et récupère la chaîne
// @param outputString Buffer pour stocker la chaîne finale
// @return void
// ---------------------------------------------------------------------------*
void finalizeStringInput(char* outputString)
{
  if (stringInputCtx.state == STRING_INPUT_COMPLETED)
  {
    strncpy(outputString, stringInputCtx.workingString, stringInputCtx.maxLength);
    outputString[stringInputCtx.maxLength] = '\0';
    
    // Supprimer les espaces en fin de chaîne
    int len = strlen(outputString);
    while (len > 0 && outputString[len - 1] == ' ')
    {
      outputString[--len] = '\0';
    }
    
    // Reset du contexte
    stringInputCtx.state = STRING_INPUT_IDLE;
    stringInputCtx.position = 0;
    
    debugSerial.print("Chaine finale: ");
    debugSerial.println(outputString);
  }
}

// ---------------------------------------------------------------------------*
// @brief Annule la saisie alphanumérique
// @param void
// @return void
// ---------------------------------------------------------------------------*
void cancelStringInput(void)
{
  stringInputCtx.state = STRING_INPUT_CANCELLED;
  debugSerial.println("Saisie alphanumérique annulee");
  OLEDDisplayMessageL8("Saisie annulee", false, false);
  
  // Reset du contexte après un délai
  stringInputCtx.state = STRING_INPUT_IDLE;
}

// ---------------------------------------------------------------------------*
// @brief Vérifie si une saisie alphanumérique est en cours
// @param void
// @return true si saisie active
// ---------------------------------------------------------------------------*
bool isStringInputActive(void)
{
  return (stringInputCtx.state == STRING_INPUT_ACTIVE);
}

// ---------------------------------------------------------------------------*
// @brief Gère le clignotement du curseur pour la saisie alphanumérique
// @param void
// @return void
// ---------------------------------------------------------------------------*
void updateStringInputCursorBlink(void)
{
  if (millis() - stringInputCtx.lastBlink > 500) // Clignotement toutes les 500ms
  {
    stringInputCtx.cursorBlink = !stringInputCtx.cursorBlink;
    stringInputCtx.lastBlink = millis();
    stringInputCtx.displayRefresh = true;
  }
}

// ---------------------------------------------------------------------------*
// @brief Modifie un caractère de la chaîne
// @param str Chaîne de caractères
// @param pos Position du caractère à modifier
// @param delta Valeur à ajouter (+1 ou -1)
// @return void
// ---------------------------------------------------------------------------*
void modifyStringChar(char* str, uint8_t pos, int delta)
{
  if (pos >= stringInputCtx.maxLength)
  {
    return;
  }
  
  str[pos] = getNextAlphaNumChar(str[pos], delta);
  
  debugSerial.print("Position string ");
  debugSerial.print(pos);
  debugSerial.print(" modifiee: '");
  debugSerial.print(str[pos]);
  debugSerial.println("'");
}

// ---------------------------------------------------------------------------*
// ===== FONCTIONS DE SAISIE HEXA =====
// ---------------------------------------------------------------------------*
// @brief Vérifie si une chaîne hexadécimale est valide
// @param hex Chaîne de caractères hexadécimale (40 caractères)
// @return bool True si la chaîne est valide
// ---------------------------------------------------------------------------*
bool isHexStringValid(const char *hex) 
{
  if (strlen(hex) != 40) return false;
  
  for (int i = 0; i < 40; i++)
  {
    char c = hex[i];
    if (!((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f')))
    {
      return false;
    }
  }
  return true;
}

// ---------------------------------------------------------------------------*
// @brief Obtient le caractère hexadécimal suivant/précédent
// @param current Caractère actuel
// @param delta Direction (+1 ou -1)
// @return Nouveau caractère hexadécimal
// ---------------------------------------------------------------------------*
char getNextHexChar(char current, int delta)
{
  const char hexChars[] = "0123456789ABCDEF";
  const int numChars = 16;
  
  // Trouver l'index du caractère actuel
  int currentIndex = -1;
  char upperCurrent = toupper(current);
  
  for (int i = 0; i < numChars; i++)
  {
    if (hexChars[i] == upperCurrent)
    {
      currentIndex = i;
      break;
    }
  }
  
  if (currentIndex == -1) return '0'; // Caractère invalide, retourner '0'
  
  // Calculer le nouvel index avec bouclage
  int newIndex = currentIndex + delta;
  if (newIndex < 0) newIndex = numChars - 1;
  if (newIndex >= numChars) newIndex = 0;
  
  return hexChars[newIndex];
}

// ---------------------------------------------------------------------------*
// @brief Démarre la saisie hexadécimale non-bloquante
// @param initialHex Chaîne hexadécimale initiale (40 caractères)
// @return void
// ---------------------------------------------------------------------------*
void startHexInput(const char* initialHex)
{
  if (hexInputCtx.state != HEX_INPUT_IDLE)
  {
  debugSerial.println("Saisie hexadecimale en cours????");
    return; // Saisie déjà en cours
  }
  
  // Initialisation du contexte
  hexInputCtx.state = HEX_INPUT_ACTIVE;
  hexInputCtx.position = 0;
  hexInputCtx.lastPosition = 0xFF;
  hexInputCtx.displayOffset = 0;
  hexInputCtx.lastDisplayOffset = 0xFF;
  hexInputCtx.lastCursorOffset = 0xFF;
  hexInputCtx.displayWidth = 16; // 16 caractères visibles sur l'écran
  
  // Copier la chaîne initiale ou créer une chaîne par défaut
  if (initialHex && strlen(initialHex) == 40)
  {
    strncpy(hexInputCtx.workingHex, initialHex, 40);
  }
  else
  {
    // Chaîne par défaut (tous des '0')
    for (int i = 0; i < 40; i++)
    {
      hexInputCtx.workingHex[i] = '0';
    }
  }
  hexInputCtx.workingHex[40] = '\0';
  strcpy(hexInputCtx.lastDisplayedHex, "");
  
  hexInputCtx.displayRefresh = true;
  hexInputCtx.lastUpdate = millis();
  hexInputCtx.cursorBlink = true;
  hexInputCtx.lastCursorBlink = false;
  hexInputCtx.lastBlink = millis();
  hexInputCtx.lastActivity = millis();      // Initialiser le timer d'activité
  hexInputCtx.timeoutDuration = 30000;      // 30 secondes de timeout
  hexInputCtx.lastValidity = false;
  hexInputCtx.firstDisplay = true;
  
  debugSerial.println("Saisie hexadecimale demarree");
  OLEDDisplayMessageL8("Modifiez la cle hexa", false, false);
}

// ---------------------------------------------------------------------------*
// @brief Met à jour le décalage d'affichage selon la position du curseur
// @param void
// @return void
// ---------------------------------------------------------------------------*
void updateHexDisplayOffset(void)
{
  // Centrer l'affichage sur la position du curseur
  int idealOffset = hexInputCtx.position - (hexInputCtx.displayWidth / 2);
  
  // Limiter les bornes
  if (idealOffset < 0) idealOffset = 0;
  if (idealOffset > (40 - hexInputCtx.displayWidth)) idealOffset = 40 - hexInputCtx.displayWidth;
  
  if (hexInputCtx.displayOffset != idealOffset)
  {
    hexInputCtx.displayOffset = idealOffset;
    hexInputCtx.displayRefresh = true;
  }
}

// ---------------------------------------------------------------------------*
// @brief Traite la saisie hexadécimale (à appeler dans loop)
// @param void
// @return État actuel de la saisie
// ---------------------------------------------------------------------------*
hexInputState_t processHexInput(void)
{
  if (hexInputCtx.state != HEX_INPUT_ACTIVE)
  {
    return hexInputCtx.state;
  }
  
  // Gestion du clignotement du curseur
  updateHexInputCursorBlink();
  
  // Vérification du timeout
  if (hexInputCtx.timeoutDuration > 0 && 
      (millis() - hexInputCtx.lastActivity > hexInputCtx.timeoutDuration))
  {
    hexInputCtx.state = HEX_INPUT_CANCELLED;
    debugSerial.println("Saisie hexadecimale annulee par timeout");
    OLEDDisplayMessageL8("Timeout", false, false);
    return hexInputCtx.state;
  }
  
  // Traitement des touches
  if (touche != KEY_NONE)
  {
    // Réinitialiser le timer d'activité à chaque action utilisateur
    hexInputCtx.lastActivity = millis();
    
    switch (touche)
    {
      case LEFT:
        if (hexInputCtx.position > 0)
        {
          hexInputCtx.position--;
          updateHexDisplayOffset();
          hexInputCtx.displayRefresh = true;
          debugSerial.print("Position curseur hex: ");
          debugSerial.println(hexInputCtx.position);
        }
        break;
        
      case RIGHT:
        if (hexInputCtx.position < 39)
        {
          hexInputCtx.position++;
          updateHexDisplayOffset();
          hexInputCtx.displayRefresh = true;
          debugSerial.print("Position curseur hex: ");
          debugSerial.println(hexInputCtx.position);
        }
        break;
        
      case PLUS:
        modifyHexDigit(hexInputCtx.workingHex, hexInputCtx.position, +1);
        hexInputCtx.displayRefresh = true;
        break;
        
      case MOINS:
        modifyHexDigit(hexInputCtx.workingHex, hexInputCtx.position, -1);
        hexInputCtx.displayRefresh = true;
        break;
        
      case VALIDE:
        if (isHexStringValid(hexInputCtx.workingHex))
        {
          hexInputCtx.state = HEX_INPUT_COMPLETED;
          debugSerial.println("Chaine hexadecimale validee");
          OLEDDisplayMessageL8("Cle hexa acceptee", false, false);
        }
        else
        {
          debugSerial.println("Chaine hexadecimale invalide");
          OLEDDisplayMessageL8("Cle hexa invalide !", false, false);
        }
        break;
        
      default:
        break;
    }
    
    // Reset de la touche après traitement
    touche = KEY_NONE;
  }
  
  // Rafraîchissement de l'affichage UNIQUEMENT si nécessaire
  if (hexInputCtx.displayRefresh)
  {
    refreshHexDisplay();
    hexInputCtx.displayRefresh = false;
    hexInputCtx.lastUpdate = millis();
  }
  
  return hexInputCtx.state;
}

// ---------------------------------------------------------------------------*
// @brief Finalise la saisie et récupère la chaîne hexadécimale
// @param outputHex Buffer pour stocker la chaîne finale (41 chars minimum)
// @return void
// ---------------------------------------------------------------------------*
void finalizeHexInput(char* outputHex)
{
  if (hexInputCtx.state == HEX_INPUT_COMPLETED)
  {
    strncpy(outputHex, hexInputCtx.workingHex, 40);
    outputHex[40] = '\0';
    
    // Reset du contexte
    hexInputCtx.state = HEX_INPUT_IDLE;
    hexInputCtx.position = 0;
    hexInputCtx.displayOffset = 0;
    hexInputCtx.firstDisplay = true;
    
    debugSerial.print("Chaine hexadecimale finale: ");
    debugSerial.println(outputHex);
  }
}

// ---------------------------------------------------------------------------*
// @brief Annule la saisie hexadécimale
// @param void
// @return void
// ---------------------------------------------------------------------------*
void cancelHexInput(void)
{
  hexInputCtx.state = HEX_INPUT_CANCELLED;
  debugSerial.println("Saisie hexadecimale annulee");
  OLEDDisplayMessageL8("Saisie annulee", false, false);
  
  // Reset du contexte après un délai
  hexInputCtx.state = HEX_INPUT_IDLE;
  hexInputCtx.firstDisplay = true;
}

// ---------------------------------------------------------------------------*
// @brief Vérifie si une saisie hexadécimale est en cours
// @param void
// @return true si saisie active
// ---------------------------------------------------------------------------*
bool isHexInputActive(void)
{
  return (hexInputCtx.state == HEX_INPUT_ACTIVE);
}


// ---------------------------------------------------------------------------*
// @brief Rafraîchit l'affichage de la chaîne hexadécimale avec curseur (optimisé)
// @param void
// @return void
// ---------------------------------------------------------------------------*
void refreshHexDisplay(void)
{
  // Préparer la chaîne d'affichage (portion visible)
  char displayBuffer[17]; // 16 chars + '\0'
  strncpy(displayBuffer, &hexInputCtx.workingHex[hexInputCtx.displayOffset], 16);
  displayBuffer[16] = '\0';
  
  bool hexChanged = (strcmp(displayBuffer, hexInputCtx.lastDisplayedHex) != 0);
  bool offsetChanged = (hexInputCtx.displayOffset != hexInputCtx.lastDisplayOffset);
  bool positionChanged = (hexInputCtx.position != hexInputCtx.lastPosition);
  bool cursorBlinkChanged = (hexInputCtx.cursorBlink != hexInputCtx.lastCursorBlink);
  bool currentValidity = isHexStringValid(hexInputCtx.workingHex);
  bool validityChanged = (currentValidity != hexInputCtx.lastValidity);
  
  // Afficher le titre seulement au premier affichage (ligne 0)
  if (hexInputCtx.firstDisplay)
  {
    OLEDDrawText(1, 0, 0, "CLE HEXADECIMALE:");
    OLEDDrawText(1, 7, 0, "+/- Modif VALIDE OK"); // Instructions fixes
    hexInputCtx.firstDisplay = false;
  }
  
  // Afficher la portion visible seulement si elle a changé (ligne 2)
  if (hexChanged || offsetChanged)
  {
    OLEDDrawText(1, 2, 0, displayBuffer);
    strcpy(hexInputCtx.lastDisplayedHex, displayBuffer);
  }
  
  // Gestion du curseur (ligne 3)
  if (positionChanged || cursorBlinkChanged || offsetChanged)
  {
    // Si l'offset a changé, effacer toute la ligne 3 pour supprimer tous les curseurs fantômes
    if (offsetChanged)
    {
      OLEDDrawText(1, 3, 0, "                "); // 16 espaces
      hexInputCtx.lastCursorOffset = 0xFF; // Invalider l'ancien offset
    }
    else
    {
      // Sinon, effacer uniquement l'ancienne position du curseur
      if (hexInputCtx.lastPosition != 0xFF && hexInputCtx.lastCursorOffset != 0xFF)
      {
        int lastRelativePos = hexInputCtx.lastPosition - hexInputCtx.lastCursorOffset;
        if (lastRelativePos >= 0 && lastRelativePos < 16)
        {
          OLEDDrawText(1, 3, lastRelativePos, " ");
        }
      }
    }
    
    // Calculer la position relative du curseur dans l'affichage actuel
    int relativePos = hexInputCtx.position - hexInputCtx.displayOffset;
    
    // Afficher le curseur ^ si visible et dans la zone d'affichage
    if (hexInputCtx.cursorBlink && relativePos >= 0 && relativePos < 16)
    {
      OLEDDrawText(1, 3, relativePos, "^");
    }
    
    // TOUJOURS mémoriser l'offset actuel (même si curseur invisible)
    hexInputCtx.lastCursorOffset = hexInputCtx.displayOffset;
    hexInputCtx.lastPosition = hexInputCtx.position;
  }
  
  // Afficher la position et scroll info seulement si changé (ligne 4)
  if (positionChanged || offsetChanged)
  {
    char posInfo[21];
    sprintf(posInfo, "Pos: %02d/40", hexInputCtx.position + 1);
    
    // Afficher les informations de scroll si nécessaire
    if (hexInputCtx.displayOffset > 0 || hexInputCtx.displayOffset < (40 - 16))
    {
      char scrollInfo[9];
      sprintf(scrollInfo, " <%02d-%02d>", 
              hexInputCtx.displayOffset + 1, 
              hexInputCtx.displayOffset + 16);
      strcat(posInfo, scrollInfo);
    }
    OLEDDrawText(1, 4, 0, posInfo);
    
    hexInputCtx.lastDisplayOffset = hexInputCtx.displayOffset;
  }
  
  // Afficher le statut de validité seulement s'il a changé (ligne 5)
  if (validityChanged)
  {
    if (currentValidity)
    {
      OLEDDrawText(1, 5, 0, "Cle valide          ");
    }
    else
    {
      OLEDDrawText(1, 5, 0, "Cle invalide        ");
    }
    hexInputCtx.lastValidity = currentValidity;
  }
  
  // Afficher le countdown du timeout seulement si < 5s (ligne 7)
  if (hexInputCtx.timeoutDuration > 0)
  {
    unsigned long remainingTime = (hexInputCtx.timeoutDuration - (millis() - hexInputCtx.lastActivity)) / 1000;
    
    // N'afficher le timeout que dans les 5 dernières secondes
    if (remainingTime <= 5 && remainingTime != hexInputCtx.lastTimeoutValue)
    {
      char timeoutMsg[21];
      snprintf(timeoutMsg, 21, "Timeout: %lds       ", remainingTime);
      OLEDDrawText(1, 7, 0, timeoutMsg);
      hexInputCtx.lastTimeoutValue = remainingTime;
    }
    else if (remainingTime > 5 && hexInputCtx.lastTimeoutValue <= 5)
    {
      // Restaurer les instructions si on sort du mode timeout
      OLEDDrawText(1, 7, 0, "+/- Modif VALIDE OK");
      hexInputCtx.lastTimeoutValue = 255;
    }
  }
  
  // Sauvegarder les états pour la prochaine comparaison
  hexInputCtx.lastCursorBlink = hexInputCtx.cursorBlink;
}

// ---------------------------------------------------------------------------*
// @brief Gère le clignotement du curseur pour l'hexadécimal
// @param void
// @return void
// ---------------------------------------------------------------------------*
void updateHexInputCursorBlink(void)
{
  if (millis() - hexInputCtx.lastBlink > 500) // Clignotement toutes les 500ms
  {
    hexInputCtx.cursorBlink = !hexInputCtx.cursorBlink;
    hexInputCtx.lastBlink = millis();
    hexInputCtx.displayRefresh = true;
  }
}

// ---------------------------------------------------------------------------*
// @brief Modifie un caractère hexadécimal
// @param hex Chaîne de caractères hexadécimale
// @param pos Position du caractère à modifier (0-39)
// @param delta Valeur à ajouter (+1 ou -1)
// @return void
// ---------------------------------------------------------------------------*
void modifyHexDigit(char *hex, uint8_t pos, int delta) 
{
  if (pos >= 40)
  {
    return;
  }
  
  hex[pos] = getNextHexChar(hex[pos], delta);
  
  debugSerial.print("Position hex ");
  debugSerial.print(pos);
  debugSerial.print(" modifiee: ");
  debugSerial.println(hex[pos]);
}


// ---------------------------------------------------------------------------*
// ===== FONCTIONS DE SAISIE TIME =====
// ---------------------------------------------------------------------------*
// @brief Vérifie si une heure est valide
// @param t Chaîne de caractères représentant l'heure (format HH:MM:SS)
// @return bool True si l'heure est valide
// ---------------------------------------------------------------------------*
bool isTimeValid(const char *t) 
{
  if (strlen(t) != 8) return false;
  if (t[2] != ':' || t[5] != ':') return false;
  
  int hh = (t[0]-'0')*10 + (t[1]-'0');
  int mm = (t[3]-'0')*10 + (t[4]-'0');
  int ss = (t[6]-'0')*10 + (t[7]-'0');
  
  return (hh >= 0 && hh <= 23 && mm >= 0 && mm <= 59 && ss >= 0 && ss <= 59);
}

// ---------------------------------------------------------------------------*
// @brief Démarre la saisie d'heure non-bloquante avec timeout
// @param initialTime Heure initiale au format "HH:MM:SS"
// @return void
// ---------------------------------------------------------------------------*
void startTimeInput(const char* initialTime)
{
  if (timeInputCtx.state != TIME_INPUT_IDLE)
  {
    return; // Saisie déjà en cours
  }
  
  // Initialisation du contexte
  timeInputCtx.state = TIME_INPUT_ACTIVE;
  timeInputCtx.position = 0;
  timeInputCtx.lastPosition = 0xFF;
  strncpy(timeInputCtx.workingTime, initialTime, 8);
  timeInputCtx.workingTime[8] = '\0';
  strcpy(timeInputCtx.lastDisplayedTime, "");
  timeInputCtx.displayRefresh = true;
  timeInputCtx.lastUpdate = millis();
  timeInputCtx.cursorBlink = true;
  timeInputCtx.lastCursorBlink = false;
  timeInputCtx.lastBlink = millis();
  timeInputCtx.lastActivity = millis();
  timeInputCtx.timeoutDuration = 30000; // 30 secondes
  timeInputCtx.lastValidity = false;
  timeInputCtx.firstDisplay = true;
  
  debugSerial.println("Saisie d'heure demarree");
  OLEDDisplayMessageL8("Modifiez l'heure", false, false);
}

// ---------------------------------------------------------------------------*
// @brief Traite la saisie d'heure (à appeler dans loop)
// @param void
// @return État actuel de la saisie
// ---------------------------------------------------------------------------*
timeInputState_t processTimeInput(void)
{
  if (timeInputCtx.state != TIME_INPUT_ACTIVE)
  {
    return timeInputCtx.state;
  }
  
  // Gestion du clignotement du curseur
  updateTimeInputCursorBlink();
  
  // Vérification du timeout
  if (timeInputCtx.timeoutDuration > 0 && (millis() - timeInputCtx.lastActivity > timeInputCtx.timeoutDuration))
  {
    timeInputCtx.state = TIME_INPUT_CANCELLED;
    debugSerial.println("Saisie heure annulee par timeout");
    OLEDDisplayMessageL8("Timeout", false, false);
    return timeInputCtx.state;
  }
  
  // Traitement des touches
  if (touche != KEY_NONE)
  {
    // Réinitialiser le timer d'activité à chaque action utilisateur
    timeInputCtx.lastActivity = millis();
    
    switch (touche)
    {
      case LEFT:
        {
          uint8_t newPos = getNextValidTimePosition(timeInputCtx.position, false);
          if (newPos != timeInputCtx.position)
          {
            timeInputCtx.position = newPos;
            timeInputCtx.displayRefresh = true;
            debugSerial.print("Position curseur heure: ");
            debugSerial.println(timeInputCtx.position);
          }
        }
        break;
        
      case RIGHT:
        {
          uint8_t newPos = getNextValidTimePosition(timeInputCtx.position, true);
          if (newPos != timeInputCtx.position)
          {
            timeInputCtx.position = newPos;
            timeInputCtx.displayRefresh = true;
            debugSerial.print("Position curseur heure: ");
            debugSerial.println(timeInputCtx.position);
          }
        }
        break;
        
      case PLUS:
        modifyTimeDigit(timeInputCtx.workingTime, timeInputCtx.position, +1);
        timeInputCtx.displayRefresh = true;
        break;
        
      case MOINS:
        modifyTimeDigit(timeInputCtx.workingTime, timeInputCtx.position, -1);
        timeInputCtx.displayRefresh = true;
        break;
        
      case VALIDE:
        if (isTimeValid(timeInputCtx.workingTime))
        {
          timeInputCtx.state = TIME_INPUT_COMPLETED;
          debugSerial.println("Heure validee");
          OLEDDisplayMessageL8("Heure OK", false, false);
        }
        else
        {
          debugSerial.println("Heure invalide, validation refusee");
          OLEDDisplayMessageL8("Heure invalide !", false, false);
        }
        break;
        
      default:
        break;
    }
    
    // Reset de la touche après traitement
    touche = KEY_NONE;
  }
  
  // Rafraîchissement de l'affichage si nécessaire
  if (timeInputCtx.displayRefresh || (millis() - timeInputCtx.lastUpdate > 100))
  {
    refreshTimeDisplay();
    timeInputCtx.displayRefresh = false;
    timeInputCtx.lastUpdate = millis();
  }
  
  return timeInputCtx.state;
}

// ---------------------------------------------------------------------------*
// @brief Finalise la saisie et récupère l'heure
// @param outputTime Buffer pour stocker l'heure finale
// @return void
// ---------------------------------------------------------------------------*
void finalizeTimeInput(char* outputTime)
{
  if (timeInputCtx.state == TIME_INPUT_COMPLETED)
  {
    strncpy(outputTime, timeInputCtx.workingTime, 8);
    outputTime[8] = '\0';
    
    // Reset du contexte
    timeInputCtx.state = TIME_INPUT_IDLE;
    timeInputCtx.position = 0;
    
    debugSerial.print("Heure finale: ");
    debugSerial.println(outputTime);
  }
}

// ---------------------------------------------------------------------------*
// @brief Annule la saisie d'heure
// @param void
// @return void
// ---------------------------------------------------------------------------*
void cancelTimeInput(void)
{
  timeInputCtx.state = TIME_INPUT_CANCELLED;
  debugSerial.println("cancelTimeInput() Saisie d'heure annulee");
  OLEDDisplayMessageL8("Saisie annulee", false, false);  
  // Reset du contexte
  timeInputCtx.state = TIME_INPUT_IDLE;

// retour dans l'affichage et traitement du menu "Père"              
  if (currentMenuDepth > 0)           // Revenir au menu
  {
    menuLevel_t* currentMenu = &menuStack[currentMenuDepth - 1];
    startListInputWithTimeout(currentMenu->title, currentMenu->menuList, currentMenu->menuSize, currentMenu->selectedIndex, 0);
  }
}

// ---------------------------------------------------------------------------*
// @brief Vérifie si une saisie d'heure est en cours
// @param void
// @return true si saisie active
// ---------------------------------------------------------------------------*
bool isTimeInputActive(void)
{
  return (timeInputCtx.state == TIME_INPUT_ACTIVE);
}

// ---------------------------------------------------------------------------*
// @brief Rafraîchit l'affichage de l'heure avec curseur (optimisé)
// @param void
// @return void
// ---------------------------------------------------------------------------*
void refreshTimeDisplay(void)
{
  bool timeChanged = (strcmp(timeInputCtx.workingTime, timeInputCtx.lastDisplayedTime) != 0);
  bool positionChanged = (timeInputCtx.position != timeInputCtx.lastPosition);
  bool cursorBlinkChanged = (timeInputCtx.cursorBlink != timeInputCtx.lastCursorBlink);
  bool currentValidity = isTimeValid(timeInputCtx.workingTime);
  bool validityChanged = (currentValidity != timeInputCtx.lastValidity);
  
  // Afficher le titre seulement au premier affichage
  if (timeInputCtx.firstDisplay)
  {
    OLEDDrawText(1, 0, 0, "HEURE:");
    OLEDDrawText(1, 7, 0, "+/- Modif  VALIDE OK");
    timeInputCtx.firstDisplay = false;
  }
  
  // Afficher l'heure seulement si elle a changé
  if (timeChanged || timeInputCtx.firstDisplay)
  {
    char displayBuffer[10];
    strncpy(displayBuffer, timeInputCtx.workingTime, 9);
    OLEDDrawText(1, 2, 7, displayBuffer);
    strcpy(timeInputCtx.lastDisplayedTime, timeInputCtx.workingTime);
  }
  
  // Afficher l'indicateur de curseur seulement si position ou clignotement a changé
  if (positionChanged || cursorBlinkChanged || timeInputCtx.firstDisplay)
  {
    // Effacer l'ancienne position si elle a changé
    if (positionChanged && timeInputCtx.lastPosition != 0xFF)
    {
      char clearUnderline[10] = "        ";
      OLEDDrawText(1, 3, 7, clearUnderline);
    }
    
    // Afficher le nouveau curseur si visible
    if (timeInputCtx.cursorBlink)
    {
      char underline[10] = "        ";
      underline[timeInputCtx.position] = '^';
      underline[8] = '\0';
      OLEDDrawText(1, 3, 7, underline);
    }
    else if (!positionChanged) // Seulement si la position n'a pas changé
    {
      char clearUnderline[10] = "        ";
      OLEDDrawText(1, 3, 7, clearUnderline);
    }
    
    timeInputCtx.lastPosition = timeInputCtx.position;
  }
  
  // Afficher le statut de validité seulement s'il a changé
  if (validityChanged || timeInputCtx.firstDisplay)
  {
    if (currentValidity)
    {
      OLEDDrawText(1, 5, 0, "Heure valide        ");
    }
    else
    {
      OLEDDrawText(1, 5, 0, "Heure invalide      ");
    }
    timeInputCtx.lastValidity = currentValidity;
  }
  
  // Afficher le countdown ou instructions (ligne 7)
  // Rafraîchir seulement si clignotement change ou timeout proche
  if (cursorBlinkChanged || timeInputCtx.firstDisplay)
  {
    if (timeInputCtx.timeoutDuration > 0)
    {
      unsigned long remainingTime = (timeInputCtx.timeoutDuration - (millis() - timeInputCtx.lastActivity)) / 1000;
      if (remainingTime <= 5)
      {
        char timeoutMsg[21];
        snprintf(timeoutMsg, 21, "Timeout: %lds       ", remainingTime);
        OLEDDrawText(1, 7, 0, timeoutMsg);
      }

// 25 ne me plait pas !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
      
      else if (remainingTime >= 29) //timeInputCtx.timeoutDuration-1) // timeInputCtx.timeoutDuration
        OLEDDrawText(1, 7, 0, "+/- Modif  VALIDE OK");
/*      
      else if (timeInputCtx.cursorBlink)
      {
        OLEDDrawText(1, 7, 0, "+/- Modif  VALIDE OK");
      }
      else
      {
        OLEDDrawText(1, 7, 0, "                    ");
      }
*/      
    }
    else
    {
      if (timeInputCtx.cursorBlink)
      {
        OLEDDrawText(1, 7, 0, "+/- Modif  VALIDE OK");
      }
      else
      {
        OLEDDrawText(1, 7, 0, "                    ");
      }
    }
  }  
  // Sauvegarder les états pour la prochaine comparaison
  timeInputCtx.lastCursorBlink = timeInputCtx.cursorBlink;
}

// ---------------------------------------------------------------------------*
// @brief Gère le clignotement du curseur pour l'heure
// @param void
// @return void
// ---------------------------------------------------------------------------*
void updateTimeInputCursorBlink(void)
{
  if (millis() - timeInputCtx.lastBlink > 500) // Clignotement toutes les 500ms
  {
    timeInputCtx.cursorBlink = !timeInputCtx.cursorBlink;
    timeInputCtx.lastBlink = millis();
    timeInputCtx.displayRefresh = true;
  }
}

// ---------------------------------------------------------------------------*
// @brief Calcule la prochaine position valide du curseur pour l'heure
// @param currentPos Position actuelle
// @param forward true pour avancer, false pour reculer
// @return Nouvelle position valide
// ---------------------------------------------------------------------------*
uint8_t getNextValidTimePosition(uint8_t currentPos, bool forward)
{
  uint8_t validPositions[] = {0, 1, 3, 4, 6, 7}; // Positions des chiffres (pas les ':')
  uint8_t numPositions = sizeof(validPositions) / sizeof(validPositions[0]);
  
  // Trouver l'index de la position actuelle
  uint8_t currentIndex = 0;
  for (uint8_t i = 0; i < numPositions; i++)
  {
    if (validPositions[i] == currentPos)
    {
      currentIndex = i;
      break;
    }
  }
  
  // Calculer la nouvelle position
  if (forward)
  {
    if (currentIndex < numPositions - 1)
    {
      return validPositions[currentIndex + 1];
    }
    else
    {
      return validPositions[0]; // Boucler au début
    }
  }
  else
  {
    if (currentIndex > 0)
    {
      return validPositions[currentIndex - 1];
    }
    else
    {
      return validPositions[numPositions - 1]; // Boucler à la fin
    }
  }
}

// ---------------------------------------------------------------------------*
// @brief Modifie un chiffre d'une heure avec gestion des limites
// @param t Chaîne de caractères de l'heure
// @param pos Position du chiffre à modifier
// @param delta Valeur à ajouter (+1 ou -1)
// @return void
// ---------------------------------------------------------------------------*
void modifyTimeDigit(char *t, uint8_t pos, int delta) 
{
  if (pos >= 8 || t[pos] < '0' || t[pos] > '9') 
  {
    return;
  }
  
  int currentVal = t[pos] - '0';
  int newVal = currentVal + delta;
  
  // Gestion des limites selon la position
  switch (pos)
  {
    case 0: // Premier chiffre des heures (0-2)
      if (newVal < 0) newVal = 2;
      if (newVal > 2) newVal = 0;
      break;
      
    case 1: // Deuxième chiffre des heures (0-9, mais limité selon le premier)
      {
        int firstDigit = t[0] - '0';
        int maxVal = (firstDigit == 2) ? 3 : 9; // Si premier chiffre = 2, max = 3 (23h)
        if (newVal < 0) newVal = maxVal;
        if (newVal > maxVal) newVal = 0;
      }
      break;
      
    case 3: // Premier chiffre des minutes (0-5)
      if (newVal < 0) newVal = 5;
      if (newVal > 5) newVal = 0;
      break;
      
    case 4: // Deuxième chiffre des minutes (0-9)
      if (newVal < 0) newVal = 9;
      if (newVal > 9) newVal = 0;
      break;
      
    case 6: // Premier chiffre des secondes (0-5)
      if (newVal < 0) newVal = 5;
      if (newVal > 5) newVal = 0;
      break;
      
    case 7: // Deuxième chiffre des secondes (0-9)
      if (newVal < 0) newVal = 9;
      if (newVal > 9) newVal = 0;
      break;
      
    default:
      return;
  }
  
  t[pos] = newVal + '0';
  
  debugSerial.print("Position heure ");
  debugSerial.print(pos);
  debugSerial.print(" modifiee: ");
  debugSerial.println(t[pos]);
}

// ---------------------------------------------------------------------------*
// ===== FONCTIONS DE SAISIE DATE =====
// ---------------------------------------------------------------------------*
// @brief Vérifie si une date est valide
// @param d Chaîne de caractères représentant la date (format JJ/MM/AAAA)
// @return bool True si la date est valide
// ---------------------------------------------------------------------------*
bool isDateValid(const char *d) 
{
  if (strlen(d) != 10) return false;
  if (d[2] != '/' || d[5] != '/') return false;
  
  int jj = (d[0]-'0')*10 + (d[1]-'0');
  int mm = (d[3]-'0')*10 + (d[4]-'0');
  int yyyy = (d[6]-'0')*1000 + (d[7]-'0')*100 + (d[8]-'0')*10 + (d[9]-'0');
  
  if (mm < 1 || mm > 12 || yyyy < 1900 || yyyy > 2099) 
    return false;
  
  int joursMax[] = {31,28,31,30,31,30,31,31,30,31,30,31};
  
  if ((yyyy % 4 == 0 && yyyy % 100 != 0) || (yyyy % 400 == 0)) 
    joursMax[1] = 29;
  
  return jj >= 1 && jj <= joursMax[mm - 1];
}

// ---------------------------------------------------------------------------*
// @brief Démarre la saisie de date non-bloquante avec timeout
// @param initialDate Date initiale au format "JJ/MM/AAAA"
// @return void
// ---------------------------------------------------------------------------*
void startDateInput(const char* initialDate)
{
  if (dateInputCtx.state != DATE_INPUT_IDLE)
  {
    return; // Saisie déjà en cours
  }
  
  // Initialisation du contexte
  dateInputCtx.state = DATE_INPUT_ACTIVE;
  dateInputCtx.position = 0;
  dateInputCtx.lastPosition = 0xFF;
  strncpy(dateInputCtx.workingDate, initialDate, 10);
  dateInputCtx.workingDate[10] = '\0';
  strcpy(dateInputCtx.lastDisplayedDate, "");
  dateInputCtx.displayRefresh = true;
  dateInputCtx.lastUpdate = millis();
  dateInputCtx.cursorBlink = true;
  dateInputCtx.lastCursorBlink = false;
  dateInputCtx.lastBlink = millis();
  dateInputCtx.lastActivity = millis();
  dateInputCtx.timeoutDuration = 30000; // 30 secondes
  dateInputCtx.lastValidity = false;
  dateInputCtx.firstDisplay = true;
  
  debugSerial.println("Saisie de date demarree");
  OLEDDisplayMessageL8("Modifiez la date", false, false);
}

// ---------------------------------------------------------------------------*
// @brief Traite la saisie de date (à appeler dans loop)
// @param void
// @return État actuel de la saisie
// ---------------------------------------------------------------------------*
dateInputState_t processDateInput(void)
{
  if (dateInputCtx.state != DATE_INPUT_ACTIVE)
  {
    return dateInputCtx.state;
  }
  
  // Gestion du clignotement du curseur
  updateDateInputCursorBlink();
  
  // Vérification du timeout
  if (dateInputCtx.timeoutDuration > 0 && (millis() - dateInputCtx.lastActivity > dateInputCtx.timeoutDuration))
  {
    dateInputCtx.state = DATE_INPUT_CANCELLED;
    debugSerial.println("Saisie date annulee par timeout");
    OLEDDisplayMessageL8("Timeout", false, false);
    return dateInputCtx.state;
  }
  
  // Traitement des touches
  if (touche != KEY_NONE)
  {
    // Réinitialiser le timer d'activité à chaque action utilisateur
    dateInputCtx.lastActivity = millis();
    
    switch (touche)
    {
      case LEFT:
        {
          uint8_t newPos = getNextValidDatePosition(dateInputCtx.position, false);
          if (newPos != dateInputCtx.position)
          {
            dateInputCtx.position = newPos;
            dateInputCtx.displayRefresh = true;
            debugSerial.print("Position curseur date: ");
            debugSerial.println(dateInputCtx.position);
          }
        }
        break;
        
      case RIGHT:
        {
          uint8_t newPos = getNextValidDatePosition(dateInputCtx.position, true);
          if (newPos != dateInputCtx.position)
          {
            dateInputCtx.position = newPos;
            dateInputCtx.displayRefresh = true;
            debugSerial.print("Position curseur date: ");
            debugSerial.println(dateInputCtx.position);
          }
        }
        break;
        
      case PLUS:
        modifyDateDigit(dateInputCtx.workingDate, dateInputCtx.position, +1);
        dateInputCtx.displayRefresh = true;
        break;
        
      case MOINS:
        modifyDateDigit(dateInputCtx.workingDate, dateInputCtx.position, -1);
        dateInputCtx.displayRefresh = true;
        break;
        
      case VALIDE:
        if (isDateValid(dateInputCtx.workingDate))
        {
          dateInputCtx.state = DATE_INPUT_COMPLETED;
          debugSerial.println("Date validee");
          OLEDDisplayMessageL8("Date OK", false, false);
        }
        else
        {
          debugSerial.println("Date invalide, validation refusee");
          OLEDDisplayMessageL8("Date invalide !     ", false, false);
        }
        break;
        
      default:
        break;
    }
    
    // Reset de la touche après traitement
    touche = KEY_NONE;
  }
  
  // Rafraîchissement de l'affichage si nécessaire
  if (dateInputCtx.displayRefresh || (millis() - dateInputCtx.lastUpdate > 100))
  {
    refreshDateDisplay();
    dateInputCtx.displayRefresh = false;
    dateInputCtx.lastUpdate = millis();
  }
  
  return dateInputCtx.state;
}

// ---------------------------------------------------------------------------*
// @brief Finalise la saisie et récupère la date
// @param outputDate Buffer pour stocker la date finale
// @return void
// ---------------------------------------------------------------------------*
void finalizeDateInput(char* outputDate)
{
  if (dateInputCtx.state == DATE_INPUT_COMPLETED)
  {
    strncpy(outputDate, dateInputCtx.workingDate, 10);
    outputDate[10] = '\0';
    
    // Reset du contexte
    dateInputCtx.state = DATE_INPUT_IDLE;
    dateInputCtx.position = 0;
    
    debugSerial.print("Date finale: ");
    debugSerial.println(outputDate);
  }
}

// ---------------------------------------------------------------------------*
// @brief Annule la saisie de date
// @param void
// @return void
// ---------------------------------------------------------------------------*
void cancelDateInput(void)
{
  dateInputCtx.state = DATE_INPUT_CANCELLED;
  debugSerial.println("Saisie de date annulee");
  OLEDDisplayMessageL8("Saisie annulee      ", false, false);
  
  // Reset du contexte
  dateInputCtx.state = DATE_INPUT_IDLE;
  
// Revenir au menu, déplacé de Handle.cpp, après cancelDateInput();
  if (currentMenuDepth > 0)
  {
    menuLevel_t* currentMenu = &menuStack[currentMenuDepth - 1];
    startListInputWithTimeout(currentMenu->title, currentMenu->menuList, currentMenu->menuSize, currentMenu->selectedIndex, 0);
  }
}

// ---------------------------------------------------------------------------*
// @brief Vérifie si une saisie de date est en cours
// @param void
// @return true si saisie active
// ---------------------------------------------------------------------------*
bool isDateInputActive(void)
{
  return (dateInputCtx.state == DATE_INPUT_ACTIVE);
}

// ---------------------------------------------------------------------------*
// @brief Rafraîchit l'affichage de la date avec curseur (optimisé)
// @param void
// @return void
// ---------------------------------------------------------------------------*
void refreshDateDisplay(void)
{
  bool dateChanged = (strcmp(dateInputCtx.workingDate, dateInputCtx.lastDisplayedDate) != 0);
  bool positionChanged = (dateInputCtx.position != dateInputCtx.lastPosition);
  bool cursorBlinkChanged = (dateInputCtx.cursorBlink != dateInputCtx.lastCursorBlink);
  bool currentValidity = isDateValid(dateInputCtx.workingDate);
  bool validityChanged = (currentValidity != dateInputCtx.lastValidity);
  
  // Afficher le titre seulement au premier affichage
  if (dateInputCtx.firstDisplay)
  {
    OLEDDrawText(1, 0, 0, "DATE:");
    OLEDDrawText(1, 7, 0, "+/- Modif  VALIDE OK");
    dateInputCtx.firstDisplay = false;
  }
  
  // Afficher la date seulement si elle a changé
  if (dateChanged || dateInputCtx.firstDisplay)
  {
    char displayBuffer[12];
    strncpy(displayBuffer, dateInputCtx.workingDate, 11);
    OLEDDrawText(1, 2, 5, displayBuffer);
    strcpy(dateInputCtx.lastDisplayedDate, dateInputCtx.workingDate);
  }
  
  // Afficher l'indicateur de curseur seulement si position ou clignotement a changé
  if (positionChanged || cursorBlinkChanged || dateInputCtx.firstDisplay)
  {
    // Effacer l'ancienne position si elle a changé
    if (positionChanged && dateInputCtx.lastPosition != 0xFF)
    {
      char clearUnderline[11] = "          ";
      OLEDDrawText(1, 3, 5, clearUnderline);
    }
    
    // Afficher le nouveau curseur si visible
    if (dateInputCtx.cursorBlink)
    {
      char underline[11] = "          ";
      underline[dateInputCtx.position] = '^';
      underline[10] = '\0';
      OLEDDrawText(1, 3, 5, underline);
    }
    else if (!positionChanged) // Seulement si la position n'a pas changé
    {
      char clearUnderline[11] = "          ";
      OLEDDrawText(1, 3, 5, clearUnderline);
    }
    
    dateInputCtx.lastPosition = dateInputCtx.position;
  }
  
  // Afficher le statut de validité seulement s'il a changé
  if (validityChanged || dateInputCtx.firstDisplay)
  {
    if (currentValidity)
    {
      OLEDDrawText(1, 5, 0, "Date valide         ");
    }
    else
    {
      OLEDDrawText(1, 5, 0, "Date invalide       ");
    }
    dateInputCtx.lastValidity = currentValidity;
  }
  
  // Afficher le countdown ou instructions (ligne 7)
  // Rafraîchir seulement si clignotement change ou timeout proche
  if (cursorBlinkChanged || dateInputCtx.firstDisplay)
  {
    if (dateInputCtx.timeoutDuration > 0)
    {
      unsigned long remainingTime = (dateInputCtx.timeoutDuration - (millis() - dateInputCtx.lastActivity)) / 1000;
      if (remainingTime <= 5)
      {
        char timeoutMsg[21];
        snprintf(timeoutMsg, 21, "Timeout: %lds       ", remainingTime);
        OLEDDrawText(1, 7, 0, timeoutMsg);
      }
/* //     Les instructions ne clignottent pas
      else if (dateInputCtx.cursorBlink)
      {
        OLEDDrawText(1, 7, 0, "+/- Modif  VALIDE OK");
      }
      else
      {
        OLEDDrawText(1, 7, 0, "                    ");
      }
*/
    }
/* //     Les instructions ne clignottent pas
    else
    {
      if (dateInputCtx.cursorBlink)
      {
        OLEDDrawText(1, 7, 0, "+/- Modif  VALIDE OK");
      }
      else
      {
        OLEDDrawText(1, 7, 0, "                    ");
      }
    }
*/
  }
  
  // Sauvegarder les états pour la prochaine comparaison
  dateInputCtx.lastCursorBlink = dateInputCtx.cursorBlink;
}

// ---------------------------------------------------------------------------*
// @brief Gère le clignotement du curseur pour la date
// @param void
// @return void
// ---------------------------------------------------------------------------*
void updateDateInputCursorBlink(void)
{
  if (millis() - dateInputCtx.lastBlink > 500) // Clignotement toutes les 500ms
  {
    dateInputCtx.cursorBlink = !dateInputCtx.cursorBlink;
    dateInputCtx.lastBlink = millis();
    dateInputCtx.displayRefresh = true;
  }
}

// ---------------------------------------------------------------------------*
// @brief Calcule la prochaine position valide du curseur pour la date
// @param currentPos Position actuelle
// @param forward true pour avancer, false pour reculer
// @return Nouvelle position valide
// ---------------------------------------------------------------------------*
uint8_t getNextValidDatePosition(uint8_t currentPos, bool forward)
{
  uint8_t validPositions[] = {0, 1, 3, 4, 6, 7, 8, 9}; // Positions des chiffres (pas les '/')
  uint8_t numPositions = sizeof(validPositions) / sizeof(validPositions[0]);
  
  // Trouver l'index de la position actuelle
  uint8_t currentIndex = 0;
  for (uint8_t i = 0; i < numPositions; i++)
  {
    if (validPositions[i] == currentPos)
    {
      currentIndex = i;
      break;
    }
  }
  
  // Calculer la nouvelle position
  if (forward)
  {
    if (currentIndex < numPositions - 1)
    {
      return validPositions[currentIndex + 1];
    }
    else
    {
      return validPositions[0]; // Boucler au début
    }
  }
  else
  {
    if (currentIndex > 0)
    {
      return validPositions[currentIndex - 1];
    }
    else
    {
      return validPositions[numPositions - 1]; // Boucler à la fin
    }
  }
}

// ---------------------------------------------------------------------------*
// @brief Modifie un chiffre d'une date avec gestion des limites
// @param d Chaîne de caractères de la date
// @param pos Position du chiffre à modifier
// @param delta Valeur à ajouter (+1 ou -1)
// @return void
// ---------------------------------------------------------------------------*
void modifyDateDigit(char *d, uint8_t pos, int delta) 
{
  if (pos >= 10 || d[pos] < '0' || d[pos] > '9') 
  {
    return;
  }
  
  int currentVal = d[pos] - '0';
  int newVal = currentVal + delta;
  
  // Gestion des limites selon la position
  switch (pos)
  {
    case 0: // Premier chiffre du jour (0-3)
      if (newVal < 0) newVal = 3;
      if (newVal > 3) newVal = 0;
      break;
      
    case 1: // Deuxième chiffre du jour (0-9, mais limité selon le premier)
      {
        int firstDigit = d[0] - '0';
        int maxVal = (firstDigit == 3) ? 1 : 9; // Si premier chiffre = 3, max = 1 (31)
        if (newVal < 0) newVal = maxVal;
        if (newVal > maxVal) newVal = 0;
      }
      break;
      
    case 3: // Premier chiffre du mois (0-1)
      if (newVal < 0) newVal = 1;
      if (newVal > 1) newVal = 0;
      break;
      
    case 4: // Deuxième chiffre du mois (0-9, mais limité selon le premier)
      {
        int firstDigit = d[3] - '0';
        int maxVal = (firstDigit == 1) ? 2 : 9; // Si premier chiffre = 1, max = 2 (12)
        if (newVal < 0) newVal = maxVal;
        if (newVal > maxVal) newVal = 0;
      }
      break;
      
    case 6: // Premier chiffre de l'année (1-2 pour 1900-2099)
      if (newVal < 1) newVal = 2;
      if (newVal > 2) newVal = 1;
      break;
      
    default: // Autres chiffres (0-9)
      if (newVal < 0) newVal = 9;
      if (newVal > 9) newVal = 0;
      break;
  }
  
  d[pos] = newVal + '0';
  
  debugSerial.print("Position date ");
  debugSerial.print(pos);
  debugSerial.print(" modifiee: ");
  debugSerial.println(d[pos]);
}

// ---------------------------------------------------------------------------*
// ===== FONCTIONS DE SAISIE IP =====
// ---------------------------------------------------------------------------*
// @brief Vérifie si une adresse IP est valide
// @param ip Chaîne de caractères IP (format XXX.XXX.XXX.XXX)
// @return bool True si l'IP est valide
// ---------------------------------------------------------------------------*
bool isIPValid(const char *ip) 
{
  if (strlen(ip) != 15) return false;
  
  // Vérifier le format XXX.XXX.XXX.XXX
  if (ip[3] != '.' || ip[7] != '.' || ip[11] != '.') return false;
  
  // Vérifier chaque octet
  for (int octet = 0; octet < 4; octet++)
  {
    int offset = octet * 4;
    
    // Vérifier que ce sont bien des chiffres
    for (int i = 0; i < 3; i++)
    {
      if (ip[offset + i] < '0' || ip[offset + i] > '9')
        return false;
    }
    
    // Calculer la valeur de l'octet
    int value = (ip[offset] - '0') * 100 + (ip[offset + 1] - '0') * 10 + (ip[offset + 2] - '0');
    
    // Vérifier que la valeur est entre 0 et 255
    if (value > 255) return false;
  }
  
  return true;
}

// ---------------------------------------------------------------------------*
// @brief Calcule la prochaine position valide du curseur pour l'IP
// @param currentPos Position actuelle
// @param forward true pour avancer, false pour reculer
// @return Nouvelle position valide
// ---------------------------------------------------------------------------*
uint8_t getNextValidIPPosition(uint8_t currentPos, bool forward)
{
  // Positions valides : 0,1,2, 4,5,6, 8,9,10, 12,13,14 (pas les points)
  uint8_t validPositions[] = {0, 1, 2, 4, 5, 6, 8, 9, 10, 12, 13, 14};
  uint8_t numPositions = 12;
  
  // Trouver l'index de la position actuelle
  uint8_t currentIndex = 0;
  for (uint8_t i = 0; i < numPositions; i++)
  {
    if (validPositions[i] == currentPos)
    {
      currentIndex = i;
      break;
    }
  }
  
  // Calculer la nouvelle position avec bouclage
  if (forward)
  {
    if (currentIndex < numPositions - 1)
    {
      return validPositions[currentIndex + 1];
    }
    else
    {
      return validPositions[0]; // Boucler au début
    }
  }
  else
  {
    if (currentIndex > 0)
    {
      return validPositions[currentIndex - 1];
    }
    else
    {
      return validPositions[numPositions - 1]; // Boucler à la fin
    }
  }
}

// ---------------------------------------------------------------------------*
// @brief Modifie un chiffre d'une adresse IP avec gestion des limites
// @param ip Chaîne de caractères de l'IP
// @param pos Position du chiffre à modifier
// @param delta Valeur à ajouter (+1 ou -1)
// @return void
// ---------------------------------------------------------------------------*
void modifyIPDigit(char *ip, uint8_t pos, int delta) 
{
  if (pos >= 15 || ip[pos] < '0' || ip[pos] > '9') 
  {
    return;
  }
  
  // Déterminer quel octet on modifie (0-3)
  uint8_t octet = pos / 4;
  uint8_t digitInOctet = pos % 4; // 0, 1, 2 (pas 3 qui est le point)
  
  int currentVal = ip[pos] - '0';
  int newVal = currentVal + delta;
  
  // Calculer l'offset de début de l'octet
  int offset = octet * 4;
  
  // Récupérer la valeur complète de l'octet
  int octetValue = (ip[offset] - '0') * 100 + (ip[offset + 1] - '0') * 10 + (ip[offset + 2] - '0');
  
  // Selon la position du chiffre, calculer la nouvelle valeur de l'octet
  if (digitInOctet == 0) // Centaines
  {
    int hundreds = (newVal < 0) ? 2 : ((newVal > 2) ? 0 : newVal);
    octetValue = hundreds * 100 + (ip[offset + 1] - '0') * 10 + (ip[offset + 2] - '0');
    
    // Si centaines = 2, limiter le reste
    if (hundreds == 2 && octetValue > 255)
    {
      octetValue = 255;
    }
  }
  else if (digitInOctet == 1) // Dizaines
  {
    int hundreds = ip[offset] - '0';
    int tens = newVal;
    int units = ip[offset + 2] - '0';
    
    if (hundreds == 2)
    {
      // Si centaines = 2, dizaines max = 5
      if (tens < 0) tens = 5;
      if (tens > 5) tens = 0;
    }
    else
    {
      if (tens < 0) tens = 9;
      if (tens > 9) tens = 0;
    }
    
    octetValue = hundreds * 100 + tens * 10 + units;
    
    if (octetValue > 255) octetValue = 255;
  }
  else // Unités (digitInOctet == 2)
  {
    int hundreds = ip[offset] - '0';
    int tens = ip[offset + 1] - '0';
    int units = newVal;
    
    if (hundreds == 2 && tens == 5)
    {
      // Si 25X, unités max = 5
      if (units < 0) units = 5;
      if (units > 5) units = 0;
    }
    else
    {
      if (units < 0) units = 9;
      if (units > 9) units = 0;
    }
    
    octetValue = hundreds * 100 + tens * 10 + units;
    
    if (octetValue > 255) octetValue = 255;
  }
  
  // Réécrire l'octet complet avec padding zéros
  ip[offset] = '0' + (octetValue / 100);
  ip[offset + 1] = '0' + ((octetValue / 10) % 10);
  ip[offset + 2] = '0' + (octetValue % 10);
  
  debugSerial.print("Position IP ");
  debugSerial.print(pos);
  debugSerial.print(" modifiee, octet: ");
  debugSerial.println(octetValue);
}

// ---------------------------------------------------------------------------*
// @brief Démarre la saisie IP non-bloquante
// @param initialIP Adresse IP initiale (format XXX.XXX.XXX.XXX)
// @return void
// ---------------------------------------------------------------------------*
void startIPInput(const char* initialIP)
{
  if (ipInputCtx.state != IP_INPUT_IDLE)
  {
    return; // Saisie déjà en cours
  }
  
  // Initialisation du contexte
  ipInputCtx.state = IP_INPUT_ACTIVE;
  ipInputCtx.position = 0;
  ipInputCtx.lastPosition = 0xFF;
  
  // Copier l'IP initiale ou utiliser valeur par défaut
  if (initialIP && strlen(initialIP) == 15)
  {
    strncpy(ipInputCtx.workingIP, initialIP, 15);
  }
  else
  {
    strcpy(ipInputCtx.workingIP, "192.168.001.001");
  }
  ipInputCtx.workingIP[15] = '\0';
  strcpy(ipInputCtx.lastDisplayedIP, "");
  
  ipInputCtx.displayRefresh = true;
  ipInputCtx.lastUpdate = millis();
  ipInputCtx.cursorBlink = true;
  ipInputCtx.lastCursorBlink = false;
  ipInputCtx.lastBlink = millis();
  ipInputCtx.lastActivity = millis();
  ipInputCtx.timeoutDuration = 30000; // 30 secondes
  ipInputCtx.lastValidity = false;
  ipInputCtx.firstDisplay = true;
  
  debugSerial.println("Saisie IP demarree");
  OLEDDisplayMessageL8("Modifiez l'IP", false, false);
}

// ---------------------------------------------------------------------------*
// @brief Traite la saisie IP (à appeler dans loop)
// @param void
// @return État actuel de la saisie
// ---------------------------------------------------------------------------*
ipInputState_t processIPInput(void)
{
  if (ipInputCtx.state != IP_INPUT_ACTIVE)
  {
    return ipInputCtx.state;
  }
  
  // Gestion du clignotement du curseur
  updateIPInputCursorBlink();
  
  // Vérification du timeout
  if (ipInputCtx.timeoutDuration > 0 && (millis() - ipInputCtx.lastActivity > ipInputCtx.timeoutDuration))
  {
    ipInputCtx.state = IP_INPUT_CANCELLED;
    debugSerial.println("Saisie IP annulee par timeout");
    OLEDDisplayMessageL8("Timeout", false, false);
    return ipInputCtx.state;
  }
  
  // Traitement des touches
  if (touche != KEY_NONE)
  {
    // Réinitialiser le timer d'activité
    ipInputCtx.lastActivity = millis();
    
    switch (touche)
    {
      case LEFT:
        {
          uint8_t newPos = getNextValidIPPosition(ipInputCtx.position, false);
          if (newPos != ipInputCtx.position)
          {
            ipInputCtx.position = newPos;
            ipInputCtx.displayRefresh = true;
            debugSerial.print("Position curseur IP: ");
            debugSerial.println(ipInputCtx.position);
          }
        }
        break;
        
      case RIGHT:
        {
          uint8_t newPos = getNextValidIPPosition(ipInputCtx.position, true);
          if (newPos != ipInputCtx.position)
          {
            ipInputCtx.position = newPos;
            ipInputCtx.displayRefresh = true;
            debugSerial.print("Position curseur IP: ");
            debugSerial.println(ipInputCtx.position);
          }
        }
        break;
        
      case PLUS:
        modifyIPDigit(ipInputCtx.workingIP, ipInputCtx.position, +1);
        ipInputCtx.displayRefresh = true;
        break;
        
      case MOINS:
        modifyIPDigit(ipInputCtx.workingIP, ipInputCtx.position, -1);
        ipInputCtx.displayRefresh = true;
        break;
        
      case VALIDE:
        if (isIPValid(ipInputCtx.workingIP))
        {
          ipInputCtx.state = IP_INPUT_COMPLETED;
          debugSerial.println("IP validee");
          OLEDDisplayMessageL8("IP acceptee", false, false);
        }
        else
        {
          debugSerial.println("IP invalide");
          OLEDDisplayMessageL8("IP invalide !", false, false);
        }
        break;
        
      default:
        break;
    }
    
    // Reset de la touche après traitement
    touche = KEY_NONE;
  }
  
  // Rafraîchissement de l'affichage si nécessaire
  if (ipInputCtx.displayRefresh || (millis() - ipInputCtx.lastUpdate > 100))
  {
    refreshIPDisplay();
    ipInputCtx.displayRefresh = false;
    ipInputCtx.lastUpdate = millis();
  }
  
  return ipInputCtx.state;
}

// ---------------------------------------------------------------------------*
// @brief Finalise la saisie et récupère l'adresse IP
// @param outputIP Buffer pour stocker l'IP finale (16 chars minimum)
// @return void
// ---------------------------------------------------------------------------*
void finalizeIPInput(char* outputIP)
{
  if (ipInputCtx.state == IP_INPUT_COMPLETED)
  {
    strncpy(outputIP, ipInputCtx.workingIP, 15);
    outputIP[15] = '\0';
    
    // Reset du contexte
    ipInputCtx.state = IP_INPUT_IDLE;
    ipInputCtx.position = 0;
    
    debugSerial.print("IP finale: ");
    debugSerial.println(outputIP);
  }
}

// ---------------------------------------------------------------------------*
// @brief Annule la saisie IP
// @param void
// @return void
// ---------------------------------------------------------------------------*
void cancelIPInput(void)
{
  ipInputCtx.state = IP_INPUT_CANCELLED;
  debugSerial.println("Saisie IP annulee");
  OLEDDisplayMessageL8("Saisie annulee", false, false);
  
  // Reset du contexte
  ipInputCtx.state = IP_INPUT_IDLE;
}

// ---------------------------------------------------------------------------*
// @brief Vérifie si une saisie IP est en cours
// @param void
// @return true si saisie active
// ---------------------------------------------------------------------------*
bool isIPInputActive(void)
{
  return (ipInputCtx.state == IP_INPUT_ACTIVE);
}

// ---------------------------------------------------------------------------*
// @brief Rafraîchit l'affichage de l'IP avec curseur (optimisé)
// @param void
// @return void
// ---------------------------------------------------------------------------*
void refreshIPDisplay(void)
{
  bool ipChanged = (strcmp(ipInputCtx.workingIP, ipInputCtx.lastDisplayedIP) != 0);
  bool positionChanged = (ipInputCtx.position != ipInputCtx.lastPosition);
  bool cursorBlinkChanged = (ipInputCtx.cursorBlink != ipInputCtx.lastCursorBlink);
  bool currentValidity = isIPValid(ipInputCtx.workingIP);
  bool validityChanged = (currentValidity != ipInputCtx.lastValidity);
  
  // Afficher le titre seulement au premier affichage
  if (ipInputCtx.firstDisplay)
  {
    OLEDDrawText(1, 0, 0, "ADRESSE IP:");
    ipInputCtx.firstDisplay = false;
  }
  
  // Afficher l'IP seulement si elle a changé
  if (ipChanged)
  {
    OLEDDrawText(1, 2, 0, ipInputCtx.workingIP);
    strcpy(ipInputCtx.lastDisplayedIP, ipInputCtx.workingIP);
  }
  
  // Afficher l'indicateur de curseur seulement si position ou clignotement a changé
  if (positionChanged || cursorBlinkChanged)
  {
    // Effacer l'ancienne position si elle a changé
    if (positionChanged && ipInputCtx.lastPosition != 0xFF)
    {
      char clearUnderline[16] = "               ";
      OLEDDrawText(1, 3, 0, clearUnderline);
    }
    
    // Afficher le nouveau curseur si visible
    if (ipInputCtx.cursorBlink)
    {
      char underline[16] = "               ";
      underline[ipInputCtx.position] = '^';
      underline[15] = '\0';
      OLEDDrawText(1, 3, 0, underline);
    }
    else if (!positionChanged)
    {
      char clearUnderline[16] = "               ";
      OLEDDrawText(1, 3, 0, clearUnderline);
    }
    
    ipInputCtx.lastPosition = ipInputCtx.position;
  }
  
  // Afficher le statut de validité seulement s'il a changé
  if (validityChanged)
  {
    if (currentValidity)
    {
      OLEDDrawText(1, 5, 0, "IP valide           ");
    }
    else
    {
      OLEDDrawText(1, 5, 0, "IP invalide         ");
    }
    ipInputCtx.lastValidity = currentValidity;
  }
  
  // Afficher les instructions et timeout seulement si clignotement change
  if (cursorBlinkChanged)
  {
    if (ipInputCtx.timeoutDuration > 0)
    {
      unsigned long remainingTime = (ipInputCtx.timeoutDuration - (millis() - ipInputCtx.lastActivity)) / 1000;
      if (remainingTime <= 5)
      {
        char timeoutMsg[21];
        snprintf(timeoutMsg, 21, "Timeout: %lds       ", remainingTime);
        OLEDDrawText(1, 7, 0, timeoutMsg);
      }
      else if (ipInputCtx.cursorBlink)
      {
        OLEDDrawText(1, 7, 0, "+/- Modif  VALIDE OK");
      }
      else
      {
        OLEDDrawText(1, 7, 0, "                    ");
      }
    }
    else
    {
      if (ipInputCtx.cursorBlink)
      {
        OLEDDrawText(1, 7, 0, "+/- Modif  VALIDE OK");
      }
      else
      {
        OLEDDrawText(1, 7, 0, "                    ");
      }
    }
  }
  
  // Sauvegarder l'état du clignotement
  ipInputCtx.lastCursorBlink = ipInputCtx.cursorBlink;
}

// ---------------------------------------------------------------------------*
// @brief Gère le clignotement du curseur pour l'IP
// @param void
// @return void
// ---------------------------------------------------------------------------*
void updateIPInputCursorBlink(void)
{
  if (millis() - ipInputCtx.lastBlink > 500)
  {
    ipInputCtx.cursorBlink = !ipInputCtx.cursorBlink;
    ipInputCtx.lastBlink = millis();
    ipInputCtx.displayRefresh = true;
  }
}

// ---------------------------------------------------------------------------*
// ===== FONCTIONS DE SAISIE MAIL =====
// ---------------------------------------------------------------------------*
// @brief Vérifie si une adresse email est valide (basique)
// @param email Chaîne de caractères email
// @return bool True si l'email semble valide
// ---------------------------------------------------------------------------*
bool isEmailValid(const char *email) 
{
  uint8_t len = strlen(email);
  if (len < 3 || len > 40) return false;
  
  // Doit contenir un @
  char* atPos = strchr(email, '@');
  if (atPos == NULL) return false;
  
  // @ ne doit pas être au début ou à la fin
  if (atPos == email || atPos == email + len - 1) return false;
  
  // Doit contenir un . après le @
  char* dotPos = strchr(atPos, '.');
  if (dotPos == NULL) return false;
  
  // Le . ne doit pas être juste après @ ni à la fin
  if (dotPos == atPos + 1 || dotPos == email + len - 1) return false;
  
  return true;
}

// ---------------------------------------------------------------------------*
// @brief Obtient le caractère email suivant/précédent dans le jeu de caractères
// @param current Caractère actuel
// @param delta Direction (+1 ou -1)
// @return Nouveau caractère
// ---------------------------------------------------------------------------*
char getNextEmailChar(char current, int delta)
{
// Trouver l'index du caractère actuel
  int currentIndex = -1;
  
  for (int i = 0; i < emailCharSetSize; i++)
  {
    if (emailCharSet[i] == current)
    {
      currentIndex = i;
      break;
    }
  }
  
  if (currentIndex == -1) currentIndex = 0; // Si non trouvé, commencer à 'a'
  
  // Calculer le nouvel index avec bouclage
  int newIndex = currentIndex + delta;
  if (newIndex < 0) newIndex = emailCharSetSize - 1;
  if (newIndex >= emailCharSetSize) newIndex = 0;
  
  return emailCharSet[newIndex];
}

// ---------------------------------------------------------------------------*
// @brief Insère un caractère à la position donnée
// @param email Chaîne email
// @param length Pointeur vers la longueur actuelle
// @param pos Position d'insertion
// @param c Caractère à insérer
// @return void
// ---------------------------------------------------------------------------*
void insertCharAtPosition(char *email, uint8_t *length, uint8_t pos, char c)
{
  if (*length >= 40 || pos > *length) return;
  
  // Décaler les caractères vers la droite
  for (int i = *length; i > pos; i--)
  {
    email[i] = email[i-1];
  }
  
  email[pos] = c;
  (*length)++;
  email[*length] = '\0';
}

// ---------------------------------------------------------------------------*
// @brief Supprime un caractère à la position donnée
// @param email Chaîne email
// @param length Pointeur vers la longueur actuelle
// @param pos Position de suppression
// @return void
// ---------------------------------------------------------------------------*
void deleteCharAtPosition(char *email, uint8_t *length, uint8_t pos)
{
  if (*length == 0 || pos >= *length) return;
  
  // Décaler les caractères vers la gauche
  for (int i = pos; i < *length - 1; i++)
  {
    email[i] = email[i+1];
  }
  
  (*length)--;
  email[*length] = '\0';
}

// ---------------------------------------------------------------------------*
// @brief Démarre la saisie email non-bloquante
// @param initialEmail Adresse email initiale
// @return void
// ---------------------------------------------------------------------------*
void startEmailInput(const char* initialEmail)
{
  if (emailInputCtx.state != EMAIL_INPUT_IDLE)
  {
    return; // Saisie déjà en cours
  }
  
  // Initialisation du contexte
  emailInputCtx.state = EMAIL_INPUT_ACTIVE;
  emailInputCtx.position = 0;
  emailInputCtx.lastPosition = 0xFF;
  emailInputCtx.displayOffset = 0;
  emailInputCtx.lastDisplayOffset = 0xFF;
  emailInputCtx.displayWidth = 16; // 16 caractères visibles sur l'écran
  emailInputCtx.charSetIndex = 0;
  
  // Copier l'email initial ou créer une chaîne vide
  if (initialEmail && strlen(initialEmail) > 0 && strlen(initialEmail) <= 40)
  {
    strncpy(emailInputCtx.workingEmail, initialEmail, 40);
    emailInputCtx.length = strlen(initialEmail);
  }
  else
  {
    emailInputCtx.workingEmail[0] = '\0';
    emailInputCtx.length = 0;
  }
  emailInputCtx.workingEmail[40] = '\0';
  emailInputCtx.lastLength = 0xFF;
  strcpy(emailInputCtx.lastDisplayedEmail, "");
  
  emailInputCtx.displayRefresh = true;
  emailInputCtx.lastUpdate = millis();
  emailInputCtx.cursorBlink = true;
  emailInputCtx.lastCursorBlink = false;
  emailInputCtx.lastBlink = millis();
  emailInputCtx.lastActivity = millis();
  emailInputCtx.timeoutDuration = 30000; // 30 secondes
  emailInputCtx.lastValidity = false;
  emailInputCtx.lastValidityState = 0xFF;
  emailInputCtx.firstDisplay = true;
  
  debugSerial.println("Saisie email demarree");
  OLEDDisplayMessageL8("Saisissez email", false, false);
}

// ---------------------------------------------------------------------------*
// @brief Met à jour le décalage d'affichage selon la position du curseur
// @param void
// @return void
// ---------------------------------------------------------------------------*
void updateEmailDisplayOffset(void)
{
  // Centrer l'affichage sur la position du curseur
  int idealOffset = emailInputCtx.position - (emailInputCtx.displayWidth / 2);
  
  // Limiter les bornes
  if (idealOffset < 0) idealOffset = 0;
  
  int maxOffset = emailInputCtx.length - emailInputCtx.displayWidth;
  if (maxOffset < 0) maxOffset = 0;
  if (idealOffset > maxOffset) idealOffset = maxOffset;
  
  if (emailInputCtx.displayOffset != idealOffset)
  {
    emailInputCtx.displayOffset = idealOffset;
    emailInputCtx.displayRefresh = true;
  }
}

// ---------------------------------------------------------------------------*
// @brief Traite la saisie email (à appeler dans loop)
// @param void
// @return État actuel de la saisie
// ---------------------------------------------------------------------------*
emailInputState_t processEmailInput(void)
{
  if (emailInputCtx.state != EMAIL_INPUT_ACTIVE)
  {
    return emailInputCtx.state;
  }
  
  // Gestion du clignotement du curseur
  updateEmailInputCursorBlink();
  
  // Vérification du timeout
  if (emailInputCtx.timeoutDuration > 0 && (millis() - emailInputCtx.lastActivity > emailInputCtx.timeoutDuration))
  {
    emailInputCtx.state = EMAIL_INPUT_CANCELLED;
    debugSerial.println("Saisie email annulee par timeout");
    OLEDDisplayMessageL8("Timeout", false, false);
    return emailInputCtx.state;
  }
  
  // Traitement des touches
  if (touche != KEY_NONE)
  {
    // Réinitialiser le timer d'activité
    emailInputCtx.lastActivity = millis();
    
    switch (touche)
    {
      case LEFT:
        // Déplacer curseur vers la gauche
        if (emailInputCtx.position > 0)
        {
          emailInputCtx.position--;
          updateEmailDisplayOffset();
          emailInputCtx.displayRefresh = true;
          debugSerial.print("Position curseur email: ");
          debugSerial.println(emailInputCtx.position);
        }
        break;
        
      case RIGHT:
        // Déplacer curseur vers la droite
        if (emailInputCtx.position < emailInputCtx.length)
        {
          emailInputCtx.position++;
          updateEmailDisplayOffset();
          emailInputCtx.displayRefresh = true;
          debugSerial.print("Position curseur email: ");
          debugSerial.println(emailInputCtx.position);
        }
        break;
        
      case PLUS:
        // Modifier le caractère sous le curseur ou insérer
        if (emailInputCtx.position < emailInputCtx.length)
        {
          // Modifier le caractère existant
          emailInputCtx.workingEmail[emailInputCtx.position] = 
            getNextEmailChar(emailInputCtx.workingEmail[emailInputCtx.position], +1);
        }
        else if (emailInputCtx.length < 40)
        {
          // Insérer un nouveau caractère
          // insertCharAtPosition(char *email, uint8_t *length, uint8_t pos, char c);
          insertCharAtPosition(emailInputCtx.workingEmail, &emailInputCtx.length, 
                              emailInputCtx.position, emailCharSet[0]);
        }
        emailInputCtx.displayRefresh = true;
        break;
        
      case MOINS:
        // Modifier le caractère sous le curseur ou supprimer
        if (emailInputCtx.position < emailInputCtx.length)
        {
          // Modifier le caractère existant
          emailInputCtx.workingEmail[emailInputCtx.position] = 
            getNextEmailChar(emailInputCtx.workingEmail[emailInputCtx.position], -1);
          emailInputCtx.displayRefresh = true;
        }
        break;
/*        
      case KEY_DELETE: // Si vous avez une touche DELETE
        // Supprimer le caractère sous le curseur
        if (emailInputCtx.position < emailInputCtx.length)
        {
          deleteCharAtPosition(emailInputCtx.workingEmail, &emailInputCtx.length, 
                              emailInputCtx.position);
          updateEmailDisplayOffset();
          emailInputCtx.displayRefresh = true;
        }
        break;
*/        
      case VALIDE:
        if (isEmailValid(emailInputCtx.workingEmail))
        {
          emailInputCtx.state = EMAIL_INPUT_COMPLETED;
          debugSerial.println("Email valide");
          OLEDDisplayMessageL8("Email accepte", false, false);
        }
        else
        {
          debugSerial.println("Email invalide");
          OLEDDisplayMessageL8("Email invalide !", false, false);
        }
        break;
        
      default:
        break;
    }
    
    // Reset de la touche après traitement
    touche = KEY_NONE;
  }
  
  // Rafraîchissement de l'affichage si nécessaire
  if (emailInputCtx.displayRefresh || (millis() - emailInputCtx.lastUpdate > 100))
  {
    refreshEmailDisplay();
    emailInputCtx.displayRefresh = false;
    emailInputCtx.lastUpdate = millis();
  }
  
  return emailInputCtx.state;
}

// ---------------------------------------------------------------------------*
// @brief Finalise la saisie et récupère l'adresse email
// @param outputEmail Buffer pour stocker l'email final (41 chars minimum)
// @return void
// ---------------------------------------------------------------------------*
void finalizeEmailInput(char* outputEmail)
{
  if (emailInputCtx.state == EMAIL_INPUT_COMPLETED)
  {
    strncpy(outputEmail, emailInputCtx.workingEmail, 40);
    outputEmail[40] = '\0';
    
    // Reset du contexte
    emailInputCtx.state = EMAIL_INPUT_IDLE;
    emailInputCtx.position = 0;
    emailInputCtx.length = 0;
    emailInputCtx.displayOffset = 0;
    
    debugSerial.print("Email final: ");
    debugSerial.println(outputEmail);
  }
}

// ---------------------------------------------------------------------------*
// @brief Annule la saisie email
// @param void
// @return void
// ---------------------------------------------------------------------------*
void cancelEmailInput(void)
{
  emailInputCtx.state = EMAIL_INPUT_CANCELLED;
  debugSerial.println("Saisie email annulee");
  OLEDDisplayMessageL8("Saisie annulee", false, false);
  
  // Reset du contexte
  emailInputCtx.state = EMAIL_INPUT_IDLE;
}

// ---------------------------------------------------------------------------*
// @brief Vérifie si une saisie email est en cours
// @param void
// @return true si saisie active
// ---------------------------------------------------------------------------*
bool isEmailInputActive(void)
{
  return (emailInputCtx.state == EMAIL_INPUT_ACTIVE);
}

// ---------------------------------------------------------------------------*
// @brief Rafraîchit l'affichage de l'email avec curseur (optimisé)
// @param void
// @return void
// ---------------------------------------------------------------------------*
void refreshEmailDisplay(void)
{
  // Préparer la chaîne d'affichage (portion visible)
  char displayBuffer[17]; // 16 chars + '\0'
  uint8_t displayLen = (emailInputCtx.length < 16) ? emailInputCtx.length : 16;
  
  if (emailInputCtx.length > 0)
  {
    strncpy(displayBuffer, &emailInputCtx.workingEmail[emailInputCtx.displayOffset], displayLen);
    displayBuffer[displayLen] = '\0';
  }
  else
  {
    strcpy(displayBuffer, "_"); // Afficher underscore si vide
  }
  
  bool emailChanged = (strcmp(displayBuffer, emailInputCtx.lastDisplayedEmail) != 0);
  bool offsetChanged = (emailInputCtx.displayOffset != emailInputCtx.lastDisplayOffset);
  bool positionChanged = (emailInputCtx.position != emailInputCtx.lastPosition);
  bool lengthChanged = (emailInputCtx.length != emailInputCtx.lastLength);
  bool cursorBlinkChanged = (emailInputCtx.cursorBlink != emailInputCtx.lastCursorBlink);
  bool currentValidity = isEmailValid(emailInputCtx.workingEmail);
  bool validityChanged = (currentValidity != emailInputCtx.lastValidity);
  
  // Déterminer l'état de validité actuel : 0=vide, 1=valide, 2=invalide
  uint8_t currentValidityState;
  if (emailInputCtx.length == 0)
    currentValidityState = 0;
  else if (currentValidity)
    currentValidityState = 1;
  else
    currentValidityState = 2;
  
  bool validityStateChanged = (currentValidityState != emailInputCtx.lastValidityState);
  
  // Afficher le titre seulement au premier affichage
  if (emailInputCtx.firstDisplay)
  {
    OLEDDrawText(1, 0, 0, "ADRESSE EMAIL:");
    emailInputCtx.firstDisplay = false;
  }
  
  // Afficher la portion visible seulement si elle a changé
  if (emailChanged || offsetChanged)
  {
    char padded[21];
    snprintf(padded, 21, "%-16s", displayBuffer); // Padding pour effacer anciens chars
    OLEDDrawText(1, 1, 0, padded);
    strcpy(emailInputCtx.lastDisplayedEmail, displayBuffer);
  }
  
  // Afficher l'indicateur de curseur seulement si position ou clignotement a changé
  if (positionChanged || cursorBlinkChanged || offsetChanged)
  {
    // Effacer l'ancienne position si nécessaire
    if ((positionChanged || offsetChanged) && emailInputCtx.lastPosition != 0xFF)
    {
      char clearUnderline[17] = "                ";
      OLEDDrawText(1, 2, 0, clearUnderline);
    }
    
    // Calculer la position relative du curseur dans l'affichage
    int relativePos = emailInputCtx.position - emailInputCtx.displayOffset;
    
    // Afficher le nouveau curseur si visible
    if (emailInputCtx.cursorBlink && relativePos >= 0 && relativePos <= displayLen)
    {
      char underline[17] = "                ";
      underline[relativePos] = '^';
      underline[16] = '\0';
      OLEDDrawText(1, 2, 0, underline);
    }
    else if (!cursorBlinkChanged) // Seulement si pas juste un clignotement
    {
      char clearUnderline[17] = "                ";
      OLEDDrawText(1, 2, 0, clearUnderline);
    }
    
    emailInputCtx.lastPosition = emailInputCtx.position;
  }
  
  // Afficher la position et longueur seulement si changées
  if (positionChanged || lengthChanged || offsetChanged)
  {
    char posInfo[21];
    sprintf(posInfo, "Pos:%02d Len:%02d/40   ", 
            emailInputCtx.position + 1, 
            emailInputCtx.length);
    OLEDDrawText(1, 3, 0, posInfo);
    
    emailInputCtx.lastDisplayOffset = emailInputCtx.displayOffset;
    emailInputCtx.lastLength = emailInputCtx.length;
  }
  
  // Afficher le statut de validité seulement s'il a changé
  if (validityStateChanged)
  {
    switch (currentValidityState)
    {
      case 0: // Vide
        OLEDDrawText(1, 4, 0, "Saisie en cours     ");
        break;
      case 1: // Valide
        OLEDDrawText(1, 4, 0, "Email valide        ");
        break;
      case 2: // Invalide
        OLEDDrawText(1, 4, 0, "Email invalide      ");
        break;
    }
    emailInputCtx.lastValidity = currentValidity;
    emailInputCtx.lastValidityState = currentValidityState;
  }
  
  // Afficher les instructions et timeout seulement si clignotement change
  if (cursorBlinkChanged)
  {
    if (emailInputCtx.timeoutDuration > 0)
    {
      unsigned long remainingTime = (emailInputCtx.timeoutDuration - (millis() - emailInputCtx.lastActivity)) / 1000;
      if (remainingTime <= 5)
      {
        char timeoutMsg[21];
        snprintf(timeoutMsg, 21, "Timeout: %lds       ", remainingTime);
        OLEDDrawText(1, 5, 0, timeoutMsg);
      }
      else if (emailInputCtx.cursorBlink)
      {
        OLEDDrawText(1, 5, 0, "+/- Char VALIDE: OK ");
      }
      else
      {
        OLEDDrawText(1, 5, 0, "                    ");
      }
    }
    else
    {
      if (emailInputCtx.cursorBlink)
      {
        OLEDDrawText(1, 5, 0, "+/- Char VALIDE: OK ");
      }
      else
      {
        OLEDDrawText(1, 5, 0, "                    ");
      }
    }
  }
  
  // Sauvegarder l'état du clignotement
  emailInputCtx.lastCursorBlink = emailInputCtx.cursorBlink;
}

// ---------------------------------------------------------------------------*
// @brief Gère le clignotement du curseur pour l'email
// @param void
// @return void
// ---------------------------------------------------------------------------*
void updateEmailInputCursorBlink(void)
{
  if (millis() - emailInputCtx.lastBlink > 500)
  {
    emailInputCtx.cursorBlink = !emailInputCtx.cursorBlink;
    emailInputCtx.lastBlink = millis();
    emailInputCtx.displayRefresh = true;
  }
}


// DNS ????
// Password solide
