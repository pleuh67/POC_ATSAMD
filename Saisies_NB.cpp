#define __INIT_DONE
#include "define.h"

// s'assurer que stringInputCtx.title[31] et autres ne sont pas pour OLED!!!!!


// ===== FONCTIONS DE SÉLECTION DANS UNE LISTE ===== 12
// ===== FONCTIONS DE SAISIE NUMÉRIQUE ===== 250
#define __SerialDebugSaisie   // decommenter pour afficher messages debug
// ===== FONCTIONS DE SAISIE ALPHANUMÉRIQUE ===== 637
// suite HEXA
// suite HEURE
// suite DATE



// -------------------------------------------------------------------------------------
// ===== FONCTIONS DE SÉLECTION DANS UNE LISTE =====
// -------------------------------------------------------------------------------------

/**
 * @brief Traite la sélection dans la liste (à appeler dans loop)
 * @param void
 * @return État actuel de la sélection
 */
listInputState_t processListInput(void)
{
    if (listInputCtx.state != LIST_INPUT_ACTIVE)
    {
        return listInputCtx.state;
    }
    
    // Gestion du clignotement du curseur
    updateListInputCursorBlink();
    
    // Traitement des touches
    if (touche != KEY_NONE)
    {
        switch (touche)
        {
            case LEFT:
                       break;
            case UP:
                // Remonter dans la liste
                if (listInputCtx.selectedIndex > 0)
                {
                    listInputCtx.selectedIndex--;
                }
                else
                {
                    listInputCtx.selectedIndex = listInputCtx.maxItems - 1; // Boucler à la fin
                }
                listInputCtx.displayRefresh = true;
                debugSerial.print("Selection index: ");
                debugSerial.print(listInputCtx.selectedIndex);
                debugSerial.print(" - ");
                debugSerial.println(listInputCtx.itemList[listInputCtx.selectedIndex]);
                break;
                
            case RIGHT:
                       break;
            case DOWN:
                // Descendre dans la liste
                if (listInputCtx.selectedIndex < listInputCtx.maxItems - 1)
                {
                    listInputCtx.selectedIndex++;
                }
                else
                {
                    listInputCtx.selectedIndex = 0; // Boucler au début
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
                OLEDDisplayMessageL8("Selection acceptee", false, false);
                break;
                
            default:
                break;
        }
        
        // Reset de la touche après traitement
        touche = KEY_NONE;
    }
    
    // Rafraîchissement de l'affichage si nécessaire
    if (listInputCtx.displayRefresh || (millis() - listInputCtx.lastUpdate > 100))
    {
        refreshListDisplay();
        listInputCtx.displayRefresh = false;
        listInputCtx.lastUpdate = millis();
    }
    
    return listInputCtx.state;
}

/**
 * @brief Démarre la sélection dans une liste de valeurs
 * @param title Titre de la sélection à afficher
 * @param itemList Tableau de pointeurs vers les chaînes de caractères
 * @param numItems Nombre d'éléments dans la liste (maximum 6)
 * @param initialIndex Index initial sélectionné
 * @return void
 */
void startListInput(const char* title, const char** itemList, uint8_t numItems, uint8_t initialIndex)
{
debugSerial.print("Param : ");debugSerial.println(itemList[2]);
  
    if (listInputCtx.state != LIST_INPUT_IDLE)
    {
        return; // Sélection déjà en cours
    }
    
    // Validation des paramètres
    if (numItems > 6) numItems = 6; // Limitation à 6 éléments max
    if (initialIndex >= numItems) initialIndex = 0;
    
    // Initialisation du contexte
    listInputCtx.state = LIST_INPUT_ACTIVE;
    listInputCtx.selectedIndex = initialIndex;
    listInputCtx.maxItems = numItems;
    listInputCtx.itemList = itemList;
debugSerial.print("struct: ");debugSerial.println(listInputCtx.itemList[2]);
    strncpy(listInputCtx.title, title, 21);
    listInputCtx.title[20] = '\0';
    
    listInputCtx.displayRefresh = true;
    listInputCtx.lastUpdate = millis();
    listInputCtx.cursorBlink = true;
    listInputCtx.lastBlink = millis();
    
    debugSerial.println("Selection dans liste demarree: ");
//    debugSerial.println(title);

debugSerialListStruct();

    OLEDDisplayMessageL8("Choisissez une valeur", false, false);
}


/**
 * @brief Finalise la sélection et récupère l'index choisi
 * @param void
 * @return Index de l'élément sélectionné
 */
uint8_t finalizeListInput(void)
{
    uint8_t selectedIndex = 0;
    
    if (listInputCtx.state == LIST_INPUT_COMPLETED)
    {
        selectedIndex = listInputCtx.selectedIndex;
        
        // Reset du contexte
        listInputCtx.state = LIST_INPUT_IDLE;
        listInputCtx.selectedIndex = 0;
        listInputCtx.maxItems = 0;
        listInputCtx.itemList = NULL;
        
        debugSerial.print("Index final selectionne: ");
        debugSerial.println(selectedIndex);
    }
    
    return selectedIndex;
}

/**
 * @brief Annule la sélection dans la liste
 * @param void
 * @return void
 */
void cancelListInput(void)
{
    listInputCtx.state = LIST_INPUT_CANCELLED;
    debugSerial.println("Selection dans liste annulee");
    OLEDDisplayMessageL8("Selection annulee", false, false);
    
    // Reset du contexte après un délai
    listInputCtx.state = LIST_INPUT_IDLE;
}

/**
 * @brief Vérifie si une sélection dans une liste est en cours
 * @param void
 * @return true si sélection active
 */
bool isListInputActive(void)
{
    return (listInputCtx.state == LIST_INPUT_ACTIVE);
}

/**
 * @brief Rafraîchit l'affichage de la liste avec curseur
 * @param void
 * @return void
 */
void refreshListDisplay(void)
{
    // Afficher le titre
    OLEDDrawText(1, 0, 0, listInputCtx.title);
    
    // Afficher les éléments de la liste
    for (uint8_t i = 0; i < listInputCtx.maxItems && i < 6; i++) // Maximum 6 lignes affichables
    {
  //      char lineBuffer[32];
        
        // Préparer la ligne avec indicateur de sélection
        if (i == listInputCtx.selectedIndex && listInputCtx.cursorBlink)
        {
            sprintf(OLEDbuf/*lineBuffer*/, "> %s", listInputCtx.itemList[i]);
        }
        else if (i == listInputCtx.selectedIndex)
        {
            sprintf(OLEDbuf/*lineBuffer*/, "  %s", listInputCtx.itemList[i]);
        }
        else
        {
            sprintf(OLEDbuf/*lineBuffer*/, "  %s", listInputCtx.itemList[i]);
        }
//debugSerial.println(listInputCtx.itemList[i]);
//debugSerial.println(OLEDbuf);        
        // Afficher la ligne
        OLEDDrawText(1, i+1, 0, OLEDbuf/*lineBuffer*/);
    }
//displayListDebug = true;
  if (displayListDebug)                       // Afficher infos debug
  {       
    // Si plus de 4 éléments, afficher un indicateur de scroll
    if (listInputCtx.maxItems > 4)
    {
        char scrollInfo[16];
        sprintf(scrollInfo, "(%d/%d)", listInputCtx.selectedIndex + 1, listInputCtx.maxItems);
        OLEDDrawText(1, 6, 0, scrollInfo);
    }
  }   
// displayListDebug = false;

    // Afficher les instructions en bas
    if (listInputCtx.cursorBlink)
    {
        OLEDDrawText(1, 7, 0, "+/- : Nav  VALIDE : OK");
    }
}

/**
 * @brief Gère le clignotement du curseur pour la sélection de liste
 * @param void
 * @return void
 */
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


/**
 * @brief Démarre la saisie numérique non-bloquante, initialise MAE
 * @param title Titre à afficher
 * @param initialNumber Nombre initial
 * @param maxLength Longueur maximum (max 10)
 * @param allowNegative Autoriser les nombres négatifs
 * @return void
 */
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


/**
 * @brief Obtient le caractère numérique suivant/précédent
 * @param current Caractère actuel
 * @param delta Direction (+1 ou -1)
 * @param allowNegative Autoriser le signe moins
 * @param position Position du caractère (0 = premier caractère)
 * @return Nouveau caractère numérique
 */
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

/**
 * @brief Valide qu'une chaîne représente un nombre valide
 * @param number Chaîne à valider
 * @return true si nombre valide
 */
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


/**
 * @brief Traite la saisie numérique (à appeler dans loop)
 * @param void
 * @return État actuel de la saisie
 */
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

/**
 * @brief Finalise la saisie et récupère le nombre
 * @param outputNumber Buffer pour stocker le nombre final
 * @return void
 */
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

/**
 * @brief Annule la saisie numérique
 * @param void
 * @return void
 */
void cancelNumberInput(void)
{
  numberInputCtx.state = NUMBER_INPUT_CANCELLED;
  debugSerial.println("Saisie numerique annulee");
  OLEDDisplayMessageL8("Saisie annulee", false, false);
  
  // Reset du contexte après un délai
  numberInputCtx.state = NUMBER_INPUT_IDLE;
}

/**
 * @brief Vérifie si une saisie numérique est en cours
 * @param void
 * @return true si saisie active
 */
bool isNumberInputActive(void)
{
  return (numberInputCtx.state == NUMBER_INPUT_ACTIVE);
}

/**
 * @brief Rafraîchit l'affichage du nombre avec curseur
 * @param void
 * @return void
 */
void refreshNumberDisplay(void)
{
  // Afficher le titre (ligne 0)
  OLEDDrawText(1, 0, 0, numberInputCtx.title);
  
  // Afficher le nombre en cours (ligne 1, tronqué à 20 caractères)
//  char displayText[21];
  strncpy(OLEDbuf/*displayText*/, numberInputCtx.workingNumber, 20);
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

/**
 * @brief Gère le clignotement du curseur pour la saisie numérique
 * @param void
 * @return void
 */
void updateNumberInputCursorBlink(void)
{
  if (millis() - numberInputCtx.lastBlink > 500) // Clignotement toutes les 500ms
  {
    numberInputCtx.cursorBlink = !numberInputCtx.cursorBlink;
    numberInputCtx.lastBlink = millis();
    numberInputCtx.displayRefresh = true;
  }
}

/**
 * @brief Modifie un caractère du nombre
 * @param str Chaîne de caractères représentant le nombre
 * @param pos Position du caractère à modifier
 * @param delta Valeur à ajouter (+1 ou -1)
 * @return void
 */
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


// -------------------------------------------------------------------------------------
// ===== FONCTIONS DE SAISIE ALPHANUMÉRIQUE =====
// -------------------------------------------------------------------------------------
/**
 * @brief Démarre la saisie alphanumérique non-bloquante
 * @param title Titre à afficher
 * @param initialString Chaîne initiale
 * @param maxLength Longueur maximum (max 20)
 * @return void
 */
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



/**
 * @brief Rafraîchit l'affichage de la chaîne avec curseur
 * @param void
 * @return void
 */
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



/**
 * @brief Obtient le caractère alphanumérique suivant/précédent
 * @param current Caractère actuel
 * @param delta Direction (+1 ou -1)
 * @return Nouveau caractère alphanumérique
 */
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


/**
 * @brief Traite la saisie alphanumérique (à appeler dans loop)
 * @param void
 * @return État actuel de la saisie
 */
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

/**
 * @brief Finalise la saisie et récupère la chaîne
 * @param outputString Buffer pour stocker la chaîne finale
 * @return void
 */
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

/**
 * @brief Annule la saisie alphanumérique
 * @param void
 * @return void
 */
void cancelStringInput(void)
{
  stringInputCtx.state = STRING_INPUT_CANCELLED;
  debugSerial.println("Saisie alphanumérique annulee");
  OLEDDisplayMessageL8("Saisie annulee", false, false);
  
  // Reset du contexte après un délai
  stringInputCtx.state = STRING_INPUT_IDLE;
}

/**
 * @brief Vérifie si une saisie alphanumérique est en cours
 * @param void
 * @return true si saisie active
 */
bool isStringInputActive(void)
{
  return (stringInputCtx.state == STRING_INPUT_ACTIVE);
}

/**
 * @brief Gère le clignotement du curseur pour la saisie alphanumérique
 * @param void
 * @return void
 */
void updateStringInputCursorBlink(void)
{
  if (millis() - stringInputCtx.lastBlink > 500) // Clignotement toutes les 500ms
  {
    stringInputCtx.cursorBlink = !stringInputCtx.cursorBlink;
    stringInputCtx.lastBlink = millis();
    stringInputCtx.displayRefresh = true;
  }
}

/**
 * @brief Modifie un caractère de la chaîne
 * @param str Chaîne de caractères
 * @param pos Position du caractère à modifier
 * @param delta Valeur à ajouter (+1 ou -1)
 * @return void
 */
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
