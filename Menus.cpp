//       1         2         3         4         5         6         7        7
//34567890123456789012345678901234567890123456789012345678901234567890123456789
// IMPRESSION 79 COLONES EN TAILLE 12
//
// ---------------------------------------------------------------------------*

// ---------------------------------------------------------------------------*
#define __INIT_DONE
#include "define.h"


// Navigation entre le 5 niveaux de menus : pushMenu() et popMenu()




// PROGRAMMATION MODE
// ---------------------------------------------------------------------------*
//  MENUS
// ---------------------------------------------------------------------------*
// ===== FONCTIONS DE GESTION DES MENUS =====
// ---------------------------------------------------------------------------*
// @brief Empile un nouveau menu dans la pile
// @param title Titre du menu
// @param menuList Pointeur vers la liste du menu
// @param menuSize Nombre d'éléments
// @param initialIndex Index initial sélectionné
// @return void
// ---------------------------------------------------------------------------*
// exemple appel: pushMenu("Menu Niv 3:", menu033Reserve040, 5, 0);
void pushMenu(const char* title, const char** menuList, uint8_t menuSize, uint8_t initialIndex)
{
sprintf(serialbuf, "pushMenu() title: %s",title); 
debugSerial.println(serialbuf);
  if (currentMenuDepth < MAX_MENU_DEPTH)
  {
    menuStack[currentMenuDepth].menuList = menuList;
    menuStack[currentMenuDepth].menuSize = menuSize;
    menuStack[currentMenuDepth].selectedIndex = initialIndex;
    strncpy(menuStack[currentMenuDepth].title, title, 20);
    menuStack[currentMenuDepth].title[20] = '\0';    
    currentMenuDepth++;
    
    // Démarrer l'affichage du nouveau menu
    startListInputWithTimeout(title, menuList, menuSize, initialIndex, 0);
    
sprintf(serialbuf,"Menu empile: %s  currentMenuDepth: %d" ,title, currentMenuDepth); 
debugSerial.println(serialbuf);
  }
  else
    OLEDDisplayMessageL8(">5 SOUS MENUS !!!", false, false);  
}

// ---------------------------------------------------------------------------*
// @brief Dépile le menu actuel et revient au précédent
// @param void
// @return void
// ---------------------------------------------------------------------------*
void popMenu(void)
{
sprintf(serialbuf, "popMenu()"); 
debugSerial.println(serialbuf);  
  if (currentMenuDepth > 1)
  {
    currentMenuDepth--;
    
    // Récupérer le menu précédent
    menuLevel_t* prevMenu = &menuStack[currentMenuDepth - 1];
    
    // Redémarrer l'affichage du menu précédent
    startListInputWithTimeout(prevMenu->title, prevMenu->menuList, prevMenu->menuSize, prevMenu->selectedIndex, 0);
    
    debugSerial.print("Retour menu precedent: ");
    debugSerial.print(prevMenu->title);
    debugSerial.print(" Profondeur: ");
    debugSerial.println(currentMenuDepth);
  }
// Apres 
  debugSerial.print(" menu apres popMenu(): ");
  debugSerialPrintMenuStruct(&menuStack[currentMenuDepth - 1]);  
}


// isListInputActive()FALSE
// n'est plus vrai dans handle donc teste plus comme si reponse à liste
// Trouver ou il se devalide (en passant par Fonction())
//
//    case VALIDE:
//        listInputCtx.state = LIST_INPUT_COMPLETED;

// voir aussi apres uint8_t finalizeListInput(void)
    // Reset du contexte
//    listInputCtx.state = LIST_INPUT_IDLE;
//    listInputCtx.selectedIndex = 0;
//    listInputCtx.scrollOffset = 0;
//    listInputCtx.maxItems = 0;
//    listInputCtx.itemList = NULL;



// ---------------------------------------------------------------------------*
// @brief Traite la sélection d'un élément de menu
// @param selectedIndex Index de l'élément sélectionné
// @return void
// ---------------------------------------------------------------------------*
void processMenuSelection(uint8_t selectedIndex)
{
  if (currentMenuDepth == 0)
  {
    debugSerial.println("Erreur: Aucun menu actif");
    return;
  }
  
  menuLevel_t* currentMenu = &menuStack[currentMenuDepth - 1];
debugSerialPrintMenuStruct(currentMenu);
 // =>   currentMenuDepth 1 / title  MENU PRINCIPAL: / menuSize 3 /selectedIndex 536879296
  // Sauvegarder la sélection dans le menu actuel
  currentMenu->selectedIndex = selectedIndex;

debugSerial.print("processMenuSelection ");
debugSerial.println(selectedIndex);  
// Traitement selon le menu actuel
// ---------------------------------------------------------------------------*
  if (currentMenu->menuList == m0_Demarrage)
  {
// Menu principal
    switch (selectedIndex)
    {
      case 0: // "INFOS"
      {
debugSerial.print("Appel d'un ecran: ");        
        m0_0E_PageInfos();   // APPEL D'UN AFFICHAGE D'ECRAN 
debugSerial.println("CONFIG. SYSTEME - Ecran INFOS demandé");   
        break;
      }
      case 1: // "CONFIG. SYSTEME"
      {
        m0_1M_ConfigSysteme();       // APPEL D'UN MENU
        break;
      }
      case 2: // "CONNEX. RESEAU"
      {  
        m0_2M_ConnexLoRa();
        break;
      }  
      case 3: // "CALIB. TENSIONS"
      {
        m0_3M_CalibTensions();
        break;
      }
      case 4: // "CALIB. BALANCES"
      {  
        m0_4M_CalibBalances();    
        break;
      }
      case 5: // "SAISIE DATE"
      {
        m0_5F_GetDate();   // APPEL D'UNE SAISIE, ici DATE
        break;
      }
      case 6: // "SAISIE HEURE"
      {
        m0_6F_GetTime();   // APPEL D'UNE SAISIE, ici HEURE
        break;
      }
      case 7: 
      {
        m0_7F_GetHex();    // "SAISIE HEXA "
        break;
      }
      default:
      {
        break;
      }  
    }
  }
// ---------------------------------------------------------------------------*
  else if (currentMenu->menuList == m01_ConfigSysteme)
  {
    switch (selectedIndex)
    {
      case 0:  
      {
debugSerial.println("m01_0 demandé");   
        break;
      }
      case 1: 
      {
debugSerial.println("m01_1 demandé");   
        break;
      }
      case 2:
      {
debugSerial.println("m01_2 demandé");   
        break;
      }
      case 3:
      {
debugSerial.println("m01_3 demandé");   
        break;
      }
      case 4:
      {  
debugSerial.println("m01_4 demandé");   
        break;
      }
      case 5:
      {  
debugSerial.println("m01_5 demandé");   
        break;
      }
      case 6:
      {  
debugSerial.println("m01_6 demandé");   
        break;
      }
      default:
      {
        break;
      }  
    }
  }
// ---------------------------------------------------------------------------*
  else if (currentMenu->menuList == m02_ConfigLoRa)
  {
    switch (selectedIndex)
    {
      case 0:  
      {
debugSerial.println("m02_0 demandé");   
        break;
      }
      case 1: 
      {
debugSerial.println("m02_1 demandé");   
        break;
      }
      case 2:
      {
debugSerial.println("m02_2 demandé");   

        break;
      }
      case 3:
      {
debugSerial.println("m02_3 demandé");   
        break;
      }
      case 4:
      {  
debugSerial.println("m02_4 demandé");   
        break;
      }
      default:
      {
        break;
      }  
    }
  }
// ---------------------------------------------------------------------------*
  else if (currentMenu->menuList == m03_CalibTensions)
  {
    switch (selectedIndex)
    {
      case 0: // "Calib. VBAT"
      {
        debugSerial.println("Calib. VBAT - Fonction a implementer");
//        popMenu(); // Retour au menu principal
        break;
      }  
      case 1: // "Calib. VSOL"
      {
        debugSerial.println("Calib. VSOL - Saisie numerique a implementer");
// startNumericInput("CALIB VSOL:", &variable_vsol, min, max);
//        popMenu(); // Retour au menu principal
        break;
      }  
      case 2: // "Calib. LUM"
      {
        debugSerial.println("Calib. LUM - Fonction a implementer");
//        popMenu(); // Retour au menu principal
        break;
      } 
      case 3: // "Reserve"
      {
        debugSerial.println("Reserve - Fonction libre");
        pushMenu("Menu Niv 3:", m033_Reserve040, 5, 0);
//        popMenu(); // Retour au menu principal
        break;
      } 
      case 4: // "RETOUR"
      {
        popMenu(); // Retour au menu principal
        break;
      }  
      default:
        break;
    }
  }  
// ---------------------------------------------------------------------------*
  else if (currentMenu->menuList == m033_Reserve040)
  {
    switch (selectedIndex)
    {
      case 0: // menu033-0
      {
        debugSerial.println("0");
//        popMenu(); // Retour au menu principal
        break;
      } 
      case 1: // menu033-1
      {
        debugSerial.println("menu033-1");
//        popMenu(); // Retour au menu principal
        break;
      }  
      case 2: // menu033-2
      {
        debugSerial.println("menu033-2");
//        popMenu(); // Retour au menu principal
        break;
      }  
      case 3: // menu033-3
      {
        debugSerial.println("menu033-3");
//        popMenu(); // Retour au menu principal
        break;
      }  
      case 4: // RET   popMenu(M030)
      {
        popMenu(); // Retour au menu: menu040CaliTensions
        break;
      }  
      default:
        break;
    }
  }
// ---------------------------------------------------------------------------*
  else if (currentMenu->menuList == m04_CalibBalances)
  {
    switch (selectedIndex)
    {
      case 0:  
      {
debugSerial.println("m04_0 demandé");   
        break;
      }
      case 1: 
      {
debugSerial.println("m04_1 demandé");   
        break;
      }
      case 2:
      {
debugSerial.println("m04_2 demandé");   
        break;
      }
      case 3:
      {
debugSerial.println("m04_3 demandé");   
        break;
      }
      case 4:
      {  
debugSerial.println("m04_4 demandé");   
        break;
      }
      default:
      {
        break;
      }  
    }
  }  
// ---------------------------------------------------------------------------*
  else if (currentMenu->menuList == m04x_CalibBal)
  {
    switch (selectedIndex)
    {
      case 0: // m04x-0
        debugSerial.println("menu04x-0  demandé");
//        popMenu(); // Retour au menu principal
        break;
      case 1: // m04x-1
        debugSerial.println("menu04x-1  demandé");
//        popMenu(); // Retour au menu principal
        break;
      case 2: // m04x-2
        debugSerial.println("menu04x-2  demandé");
//        popMenu(); // Retour au menu principal
        break;
      case 3: // m04x-3
        debugSerial.println("menu04x-3  demandé");
//        popMenu(); // Retour au menu principal
        break;
      case 4: // RET   popMenu(M04)
        popMenu(); // Retour au menu: m04_CalibBalances
        break;
      default:
        break;
    }
  }
// ---------------------------------------------------------------------------*
/*
// A decliner selon les menus
// ---------------------------------------------------------------------------*
  else if (currentMenu->menuList == AutreMenu)
  {
    switch (selectedIndex)
    {
      case 1:  
              {
 
                break;
              }
      case 2: 
              {
 
                break;
              }
      case 3:
              {
 
                break;
              }
      case 4:
              {

                break;
              }
      case 5:
              {  

                break;
              }
     case 6:
              {
 
                break;
              }
      default:
              {
                break;
              }  
    }
  }  
*/
}
