//       1         2         3         4         5         6         7        7
//34567890123456789012345678901234567890123456789012345678901234567890123456789
// IMPRESSION 79 COLONES EN TAILLE 12
//
// ---------------------------------------------------------------------------*

// ---------------------------------------------------------------------------*
#define __INIT_DONE
#include "define.h"

// ---------------------------------------------------------------------------*
// @brief Traite le clavier en continu (à appeler dans loop)
// @param Aucun
// @return void
// @description Gère l'anti-rebond et la détection des touches de manière non-bloquante
// ---------------------------------------------------------------------------*
void processContinuousKeyboard(void)
{
    unsigned long currentTime = millis();

// debugSerial.println("C");
    
    // Vérifier l'intervalle minimum entre lectures
    if (currentTime - clavierContext.derniereLecture < DEBOUNCE_DELAY_MS)
       return; // Trop tôt pour une nouvelle lecture

    clavierContext.derniereLecture = currentTime;
    
    // Lire la touche actuelle
    key_code_t toucheActuelle = readKeyOnce();
    
    // Vérifier la stabilité
    if (toucheActuelle == clavierContext.derniereTouche)
    {
        clavierContext.compteStable++;
        
        // Si suffisamment stable et différente de la précédente
        if (clavierContext.compteStable >= DEBOUNCE_COUNT)
        {
            if (toucheActuelle != clavierContext.toucheStable)
            {
                clavierContext.toucheStable = toucheActuelle;
                
                // Nouvelle touche disponible (pas KEY_NONE)
                if (toucheActuelle != KEY_NONE)
                {
                    clavierContext.toucheDisponible = true;
/*                    
                    // Debug optionnel
                    static int toucheCount = 0;
                    toucheCount++;
                    if (toucheCount % 5 == 0)
                    {
                        debugSerial.print("Touche détectée #");
                        debugSerial.print(toucheCount);
                        debugSerial.print(": ");
                        debugSerial.println(keyToString(toucheActuelle));
                    }
*/                    
                }
            }
        }
    }
    else
    {
        // Touche différente, recommencer le comptage
        clavierContext.derniereTouche = toucheActuelle;
        clavierContext.compteStable = 0;
    }
}

// ---------------------------------------------------------------------------*
// @brief Lecture instantanée d'une touche du clavier analogique
// @param Aucun
// @return key_code_t Code de la touche ou état d'erreur
// ---------------------------------------------------------------------------*
key_code_t readKeyOnce(void) 
{ static const int levels[NB_KEYS] = {10, 149, 332, 501, 735};
  static const key_code_t keycodes[NB_KEYS] = {KEY_1, KEY_2, KEY_3, KEY_4, KEY_5};
  int val = analogRead(KBD_ANA);

  if (val > 1000) return KEY_NONE;                        // pas de touche appuyée
  for (int i = 0; i < NB_KEYS; i++) 
  {
    if (val >= levels[i] - TOL && val <= levels[i] + TOL) // teste les 5 levels
      return keycodes[i];                                 // renvoie KEY_n identifiée
  }
  return KEY_INVALID;                                     // Val non identifiable
}

// ---------------------------------------------------------------------------*
// @brief Lecture avec anti-rebond du clavier analogique
// @param Aucun
// @return key_code_t Code de touche stable après confirmation
// ---------------------------------------------------------------------------*
/* 
key_code_t readKey(void) 
{ key_code_t key, key2;
  int stable_count = 0;
    
  key = readKeyOnce();
  do 
  {
    delay(DEBOUNCE_DELAY_MS);
    key2 = readKeyOnce();
   
    if (key == key2) 
    {
      stable_count++;
    } 
    else 
    {
      stable_count = 0;
      key = key2;
    }
  } while (stable_count < DEBOUNCE_COUNT);
  return key;
}
*/

// ---------------------------------------------------------------------------*
// @brief Convertit un code de touche en chaîne de caractères
// @param key Code de la touche
// @return const char* Nom de la touche
// ---------------------------------------------------------------------------*
const char* keyToString(key_code_t key) 
{ switch (key) 
  {
    case KEY_NONE: return "NONE";
    case KEY_1: return "KEY_1";
    case KEY_2: return "KEY_2";
    case KEY_3: return "KEY_3";
    case KEY_4: return "KEY_4";
    case KEY_5: return "KEY_5";
    case KEY_INVALID: return "INVALID";
    default: return "UNKNOWN";
  }
}


// ---------------------------------------------------------------------------*
// @brief Version non-bloquante de readKey() - Retourne immédiatement
// @param Aucun
// @return key_code_t Touche disponible ou KEY_NONE si aucune touche
// @description Récupère une touche stabilisée sans attendre
// ---------------------------------------------------------------------------*
key_code_t readKeyNonBlocking(void)
{
    if (clavierContext.toucheDisponible)
    {
        clavierContext.toucheDisponible = false; // Consommer la touche
        return clavierContext.toucheStable;
    }
    
    return KEY_NONE; // Pas de touche disponible
}
