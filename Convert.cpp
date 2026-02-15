//       1         2         3         4         5         6         7        7
//34567890123456789012345678901234567890123456789012345678901234567890123456789
// IMPRESSION 79 COLONES EN TAILLE 12
//
// ---------------------------------------------------------------------------*
//       _____                          _                     
//      / ____|                        | |                    
//     | |     ___  _ ____   _____ _ __| |_   ___ _ __  _ __  
//     | |    / _ \| '_ \ \ / / _ \ '__| __| / __| '_ \| '_ \ 
//     | |___| (_) | | | \ V /  __/ |  | |_ | (__| |_) | |_) |
//      \_____\___/|_| |_|\_/ \___|_|   \__(_)___| .__/| .__/ 
//                                               | |   | |    
//                                               |_|   |_|    
// ---------------------------------------------------------------------------*
#define __INIT_DONE
#include "define.h"
// ---------------------------------------------------------------------------*

/*
// Fonctions de conversions
void TestConvert(void);
// Conversions hexadécimales
uint8_t hexCharToNibble(char c);
char nibbleToHexChar(uint8_t nibble);
bool hexStringToByteArray(const char* hexString, uint8_t* byteArray, uint8_t maxBytes);
bool byteArrayToHexString(const uint8_t* byteArray, uint8_t numBytes, char* hexString, uint8_t maxChars);

// Conversions décimales uint8_t ↔ char buffer
bool uint8ToDecimalString(uint8_t value, char* buffer, uint8_t maxChars);
bool decimalStringToUint8(const char* buffer, uint8_t* value);
bool isValidLoRaWanSF(uint8_t sf);
bool validateLoRaWanSF(const char* sfString, uint8_t* sfValue);

// Affichage
void printByteArray(const uint8_t* byteArray, uint8_t length);
void printHexString(const char* hexString);
*/

// ---------------------------------------------------------------------------*
// @brief Convertit un caractère hexadécimal en valeur numérique (nibble)
// @param c Caractère hexadécimal ('0'-'9', 'A'-'F', 'a'-'f')
// @return uint8_t Valeur numérique (0-15), ou 0xFF si invalide
// ---------------------------------------------------------------------------*
uint8_t hexCharToNibble(char c)
{
  if (c >= '0' && c <= '9')
  {
    return (uint8_t)(c - '0');
  }
  else if (c >= 'A' && c <= 'F')
  {
    return (uint8_t)(c - 'A' + 10);
  }
  else if (c >= 'a' && c <= 'f')
  {
    return (uint8_t)(c - 'a' + 10);
  }
  else
  {
    return 0xFF; // Caractère invalide
  }
}

// ---------------------------------------------------------------------------*
// @brief Convertit une valeur numérique (nibble) en caractère hexadécimal
// @param nibble Valeur numérique (0-15)
// @return char Caractère hexadécimal ('0'-'9', 'A'-'F'), ou '?' si invalide
// ---------------------------------------------------------------------------*
char nibbleToHexChar(uint8_t nibble)
{
  if (nibble <= 9)
  {
    return (char)('0' + nibble);
  }
  else if (nibble <= 15)
  {
    return (char)('A' + nibble - 10);
  }
  else
  {
    return '?'; // Valeur invalide
  }
}

// ---------------------------------------------------------------------------*
// @brief Convertit une chaîne hexadécimale en tableau d'octets
// @param source Chaîne de caractères contenant les valeurs hexadécimales
// @param destination Tableau d'octets où stocker le résultat
// @param len Nombre d'octets à convertir (longueur du tableau destination)
// @return void
// ---------------------------------------------------------------------------*
void CONVERTfconvertByteArray(const char *source, uint8_t *destination, uint8_t len)
{
  for (uint8_t i = 0; i < len; i++)
  {
    // Extraire deux caractères hexadécimaux de la source
    char hexByte[3] = {source[i * 2], source[i * 2 + 1], '\0'};
    
    // Convertir en valeur numérique et stocker dans destination
    destination[i] = (uint8_t)strtol(hexByte, NULL, 16);
  }

   // Print the Hardware EUI
  debugSerial.print("CONVERTfconvertByteArray()/destination => DevEUI: ");
  for (uint8_t i = 0; i < len; i++) 
  {
      debugSerial.print((char)NIBBLE_TO_HEX_CHAR(HIGH_NIBBLE(destination[i])));
      debugSerial.print((char)NIBBLE_TO_HEX_CHAR(LOW_NIBBLE(destination[i])));
  }
debugSerial.println(" de CONVERTfconvertByteArray()");
}

// ---------------------------------------------------------------------------*
// @brief Convertit une chaîne hexadécimale en tableau d'octets
// @param hexString Chaîne hexadécimale source (ex: "5048494C...")
// @param byteArray Tableau destination pour les octets
// @param maxBytes Taille maximale du tableau destination
// @return bool True si conversion réussie, false sinon
// @note La chaîne doit avoir un nombre pair de caractères
// @note Le tableau sera terminé par 0x00 si possible
// ---------------------------------------------------------------------------*
bool hexStringToByteArray(const char* hexString, uint8_t* byteArray, uint8_t maxBytes)
{
  if (!hexString || !byteArray || maxBytes == 0)
  {
    debugSerial.println("Erreur: parametres invalides");
    return false;
  }
  
  uint8_t hexLength = strlen(hexString);
  
  // Vérifier que la longueur est paire
  if (hexLength % 2 != 0)
  {
    debugSerial.print("Erreur: longueur impaire (");
    debugSerial.print(hexLength);
    debugSerial.println(" caracteres)");
    return false;
  }
  
  // Calculer le nombre d'octets à générer
  uint8_t numBytes = hexLength / 2;
  
  // Vérifier que le tableau destination est assez grand
  // (+1 pour le terminateur 0x00)
  if (numBytes >= maxBytes)
  {
    debugSerial.print("Erreur: tableau trop petit (besoin de ");
    debugSerial.print(numBytes + 1);
    debugSerial.print(" octets, disponible: ");
    debugSerial.print(maxBytes);
    debugSerial.println(")");
    return false;
  }
  
  // Conversion
  for (uint8_t i = 0; i < numBytes; i++)
  {
    // Convertir les 2 caractères hexa en 1 octet
    uint8_t highNibble = hexCharToNibble(hexString[i * 2]);
    uint8_t lowNibble = hexCharToNibble(hexString[i * 2 + 1]);
    
    // Vérifier la validité
    if (highNibble == 0xFF || lowNibble == 0xFF)
    {
      debugSerial.print("Erreur: caractere hexa invalide a la position ");
      debugSerial.println(i * 2);
      return false;
    }
    
    // Combiner les nibbles en un octet
    byteArray[i] = (highNibble << 4) | lowNibble;
  }
  
  // Ajouter le terminateur 0x00
  byteArray[numBytes] = 0x00;
  
  debugSerial.print("Conversion reussie: ");
  debugSerial.print(hexLength);
  debugSerial.print(" caracteres hexa -> ");
  debugSerial.print(numBytes);
  debugSerial.println(" octets");
  
  return true;
}

// ---------------------------------------------------------------------------*
// @brief Convertit un tableau d'octets en chaîne hexadécimale
// @param byteArray Tableau source d'octets
// @param numBytes Nombre d'octets à convertir
// @param hexString Chaîne destination pour le résultat hexa
// @param maxChars Taille maximale de la chaîne destination
// @return bool True si conversion réussie, false sinon
// @note La chaîne sera terminée par '\0'
// @note La chaîne résultante aura (numBytes * 2) caractères + '\0'
// ---------------------------------------------------------------------------*
bool byteArrayToHexString(const uint8_t* byteArray, uint8_t numBytes, char* hexString, uint8_t maxChars)
{
  if (!byteArray || !hexString || maxChars == 0)
  {
    debugSerial.println("Erreur: parametres invalides");
    return false;
  }
  
  // Vérifier que la chaîne destination est assez grande
  // (numBytes * 2 caractères hexa + 1 pour '\0')
  uint8_t requiredChars = (numBytes * 2) + 1;
  if (requiredChars > maxChars)
  {
    debugSerial.print("Erreur: chaine trop petite (besoin de ");
    debugSerial.print(requiredChars);
    debugSerial.print(" caracteres, disponible: ");
    debugSerial.print(maxChars);
    debugSerial.println(")");
    return false;
  }
  
  // Conversion
  for (uint8_t i = 0; i < numBytes; i++)
  {
    uint8_t octet = byteArray[i];
    
    // Extraire les nibbles (4 bits hauts et 4 bits bas)
    uint8_t highNibble = (octet >> 4) & 0x0F;
    uint8_t lowNibble = octet & 0x0F;
    
    // Convertir en caractères hexa
    hexString[i * 2] = nibbleToHexChar(highNibble);
    hexString[i * 2 + 1] = nibbleToHexChar(lowNibble);
  }
  
  // Ajouter le terminateur '\0'
  hexString[numBytes * 2] = '\0';
/*  
  debugSerial.print("Conversion reussie: ");
  debugSerial.print(numBytes);
  debugSerial.print(" octets -> ");
  debugSerial.print(numBytes * 2);
  debugSerial.println(" caracteres hexa");
  */
  return true;
}

// ===== CONVERSIONS DÉCIMALES (uint8_t ↔ char buffer) =====

// ---------------------------------------------------------------------------*
// @brief Convertit un uint8_t en chaîne décimale
// @param value Valeur numérique à convertir (0-255)
// @param buffer Chaîne destination pour le résultat décimal
// @param maxChars Taille maximale de la chaîne destination (min 4)
// @return bool True si conversion réussie, false sinon
// @note La chaîne sera terminée par '\0'
// @note Exemples : 7 → "7", 12 → "12", 255 → "255"
// ---------------------------------------------------------------------------*
bool uint8ToDecimalString(uint8_t value, char* buffer, uint8_t maxChars)
{
  if (!buffer || maxChars < 4)
  {
    debugSerial.println("Erreur: buffer trop petit (min 4 caracteres)");
    return false;
  }
  
  // Conversion avec sprintf
  snprintf(buffer, maxChars, "%u", value);
  
  debugSerial.print("Conversion uint8 -> string: ");
  debugSerial.print(value);
  debugSerial.print(" -> \"");
  debugSerial.print(buffer);
  debugSerial.println("\"");
  
  return true;
}

// ---------------------------------------------------------------------------*
// @brief Convertit une chaîne décimale en uint8_t
// @param buffer Chaîne décimale source (ex: "12", "255")
// @param value Pointeur vers la variable destination uint8_t
// @return bool True si conversion réussie, false sinon
// @note La chaîne doit contenir uniquement des chiffres 0-9
// @note La valeur doit être entre 0 et 255
// @note Les espaces en début/fin sont ignorés
// ---------------------------------------------------------------------------*
bool decimalStringToUint8(const char* buffer, uint8_t* value)
{
  if (!buffer || !value)
  {
    debugSerial.println("Erreur: parametres invalides");
    return false;
  }
  
  // Ignorer les espaces en début de chaîne
  while (*buffer == ' ')
  {
    buffer++;
  }
  
  // Vérifier que la chaîne n'est pas vide
  if (*buffer == '\0')
  {
    debugSerial.println("Erreur: chaine vide");
    return false;
  }
  
  // Vérifier que tous les caractères sont des chiffres
  const char* ptr = buffer;
  while (*ptr != '\0' && *ptr != ' ')
  {
    if (*ptr < '0' || *ptr > '9')
    {
      debugSerial.print("Erreur: caractere invalide '");
      debugSerial.print(*ptr);
      debugSerial.println("' (chiffres 0-9 uniquement)");
      return false;
    }
    ptr++;
  }
  
  // Conversion avec atoi
  long temp = atol(buffer);
  
  // Vérifier la plage (0-255)
  if (temp < 0 || temp > 255)
  {
    debugSerial.print("Erreur: valeur hors plage (");
    debugSerial.print(temp);
    debugSerial.println("), plage valide: 0-255");
    return false;
  }
  
  *value = (uint8_t)temp;
  
  debugSerial.print("Conversion string -> uint8: \"");
  debugSerial.print(buffer);
  debugSerial.print("\" -> ");
  debugSerial.println(*value);
  
  return true;
}

// ===== VALIDATION SPREADING FACTOR LoRaWAN =====

// ---------------------------------------------------------------------------*
// @brief Vérifie si un Spreading Factor est valide pour LoRaWAN
// @param sf Valeur du Spreading Factor à vérifier
// @return bool True si SF est valide (7, 9 ou 12), false sinon
// @note LoRaWAN utilise uniquement SF7, SF9 et SF12
// ---------------------------------------------------------------------------*
bool isValidLoRaWanSF(uint8_t sf)
{
  return (sf == 7 || sf == 9 || sf == 12);
}

// ---------------------------------------------------------------------------*
// @brief Convertit et valide un Spreading Factor LoRaWAN depuis une chaîne
// @param sfString Chaîne contenant le SF (ex: "7", "9", "12")
// @param sfValue Pointeur vers la variable destination uint8_t
// @return bool True si conversion et validation réussies, false sinon
// @note Accepte uniquement les valeurs 7, 9 et 12
// ---------------------------------------------------------------------------*
bool validateLoRaWanSF(const char* sfString, uint8_t* sfValue)
{
  if (!sfString || !sfValue)
  {
    debugSerial.println("Erreur: parametres invalides");
    return false;
  }
  
  uint8_t tempSF = 0;
  
  // Convertir la chaîne en uint8_t
  if (!decimalStringToUint8(sfString, &tempSF))
  {
    debugSerial.println("Erreur: conversion SF impossible");
    return false;
  }
  
  // Vérifier que c'est un SF LoRaWAN valide
  if (!isValidLoRaWanSF(tempSF))
  {
    debugSerial.print("Erreur: SF");
    debugSerial.print(tempSF);
    debugSerial.println(" invalide. Valeurs autorisees: 7, 9, 12");
    return false;
  }
  
  *sfValue = tempSF;
  
  debugSerial.print("SF LoRaWAN valide: SF");
  debugSerial.println(*sfValue);
  
  return true;
}

// ---------------------------------------------------------------------------*
// @brief Convertit une chaîne hexadécimale en tableau d'octets
// @param source Chaîne de caractères contenant les valeurs hexadécimales
// @param destination Tableau d'octets où stocker le résultat
// @param len Nombre d'octets à convertir (longueur du tableau destination)
// @return void
// ---------------------------------------------------------------------------*
void convertByteArray(const char *source, uint8_t *destination, uint8_t len)
{
  for (uint8_t i = 0; i < len; i++)
  {
    // Extraire deux caractères hexadécimaux de la source
    char hexByte[3] = {source[i * 2], source[i * 2 + 1], '\0'};
    
    // Convertir en valeur numérique et stocker dans destination
    destination[i] = (uint8_t)strtol(hexByte, NULL, 16);
  }
}

// ---------------------------------------------------------------------------*
// @brief Convertit un tableau d'octets en chaîne hexadécimale
// @param source Tableau d'octets à convertir
// @param destination Chaîne de caractères où stocker le résultat 
//        (doit contenir 2 * len + 1 caractères)
// @param len Nombre d'octets à convertir
// @return void
// ---------------------------------------------------------------------------*
void convertToHexString(const uint8_t *source, char *destination, uint8_t len)
{
  for (uint8_t i = 0; i < len; i++)
  {
    // Convertir chaque octet en deux caractères hexadécimaux
    sprintf(&destination[i * 2], "%02X", source[i]);
  }
  // Terminer la chaîne avec un caractère nul
  destination[len * 2] = '\0';
}




// ---------------------------------------------------------------------------*
// @brief Affiche un tableau d'octets en hexadécimal sur le port série
// @param byteArray Tableau d'octets à afficher
// @param length Nombre d'octets à afficher
// @return void
// ---------------------------------------------------------------------------*
void printByteArray(const uint8_t* byteArray, uint8_t length)
{
  debugSerial.print("uint8_t array[");
  debugSerial.print(length);
  debugSerial.print("] = { ");
  
  for (uint8_t i = 0; i < length; i++)
  {
    debugSerial.print("0x");
    if (byteArray[i] < 0x10)
    {
      debugSerial.print("0");
    }
    debugSerial.print(byteArray[i], HEX);
    
    if (i < length - 1)
    {
      debugSerial.print(", ");
    }
  }
  
  debugSerial.println(" };");
}

// ---------------------------------------------------------------------------*
// @brief Affiche une chaîne hexadécimale sur le port série
// @param hexString Chaîne hexadécimale à afficher
// @return void
// ---------------------------------------------------------------------------*
void printHexString(const char* hexString)
{
  uint8_t len = strlen(hexString);
  
  debugSerial.print("char hexString[");
  debugSerial.print(len + 1);
  debugSerial.print("] = \"");
  debugSerial.print(hexString);
  debugSerial.println("\";");
}

// ===== FONCTION TestConvert() =====

/**
 * @brief Initialisation et test de conversion
 * @param void
 * @return void
 */
void TestConvert(void)
{
  debugSerial.println("========================================");
  debugSerial.println("Test conversion Hexa String -> Byte Array");
  debugSerial.println("========================================\n");
  
  // ===== TEST 1 : Conversion standard =====
  debugSerial.println("--- TEST 1 : Conversion standard ---");
  const char* hexString1 = "5048494C495050454C4F56454C4F56454C414F4F";
  uint8_t AppKey[17] = {0}; // 16 octets + 1 terminateur
  
  debugSerial.print("Chaine hexa source: ");
  debugSerial.println(hexString1);
  debugSerial.print("Longueur: ");
  debugSerial.print(strlen(hexString1));
  debugSerial.println(" caracteres\n");
  
  if (hexStringToByteArray(hexString1, AppKey, 17))
  {
    debugSerial.println("Resultat de la conversion:");
    printByteArray(AppKey, 16);
  }
  else
  {
    debugSerial.println("Echec de la conversion !");
  }
  
  // ===== TEST 2 : Clé AES-128 (32 caractères hexa = 16 octets) =====
  debugSerial.println("\n--- TEST 2 : Cle AES-128 ---");
  const char* hexString2 = "0123456789ABCDEF0123456789ABCDEF";
  uint8_t aesKey[17] = {0};
  
  debugSerial.print("Chaine hexa source: ");
  debugSerial.println(hexString2);
  
  if (hexStringToByteArray(hexString2, aesKey, 17))
  {
    debugSerial.println("Resultat de la conversion:");
    printByteArray(aesKey, 16);
  }
  
  // ===== TEST 3 : DevEUI LoRaWAN (16 caractères hexa = 8 octets) =====
  debugSerial.println("\n--- TEST 3 : DevEUI LoRaWAN ---");
  const char* hexString3 = "0004A30B001A2B3C";
  // uint8_t devEUI[9] = {0};
  
  debugSerial.print("Chaine hexa source: ");
  debugSerial.println(hexString3);
  
  if (hexStringToByteArray(hexString3, config.materiel.DevEUI, 9))
  {
    debugSerial.println("Resultat de la conversion:");
    printByteArray(config.materiel.DevEUI, 8);
  }
  
  // ===== TEST 4 : Adresse MAC (12 caractères hexa = 6 octets) =====
  debugSerial.println("\n--- TEST 4 : Adresse MAC ---");
  const char* hexString4 = "AABBCCDDEEFF";
  uint8_t macAddr[7] = {0};
  
  debugSerial.print("Chaine hexa source: ");
  debugSerial.println(hexString4);
  
  if (hexStringToByteArray(hexString4, macAddr, 7))
  {
    debugSerial.println("Resultat de la conversion:");
    printByteArray(macAddr, 6);
  }
  
  // ===== TEST 5 : Valeur courte (4 caractères hexa = 2 octets) =====
  debugSerial.println("\n--- TEST 5 : Valeur courte ---");
  const char* hexString5 = "BEEF";
  uint8_t shortVal[3] = {0};
  
  debugSerial.print("Chaine hexa source: ");
  debugSerial.println(hexString5);
  
  if (hexStringToByteArray(hexString5, shortVal, 3))
  {
    debugSerial.println("Resultat de la conversion:");
    printByteArray(shortVal, 2);
  }
  
  // ===== TEST 6 : Test d'erreur - longueur impaire =====
  debugSerial.println("\n--- TEST 6 : Erreur longueur impaire ---");
  const char* hexString6 = "ABC"; // 3 caractères (impair)
  uint8_t errorTest1[2] = {0};
  
  debugSerial.print("Chaine hexa source: ");
  debugSerial.println(hexString6);
  
  if (!hexStringToByteArray(hexString6, errorTest1, 2))
  {
    debugSerial.println("Erreur detectee comme prevu !\n");
  }
  
  // ===== TEST 7 : Test d'erreur - caractère invalide =====
  debugSerial.println("--- TEST 7 : Erreur caractere invalide ---");
  const char* hexString7 = "12G4"; // 'G' n'est pas hexa
  uint8_t errorTest2[3] = {0};
  
  debugSerial.print("Chaine hexa source: ");
  debugSerial.println(hexString7);
  
  if (!hexStringToByteArray(hexString7, errorTest2, 3))
  {
    debugSerial.println("Erreur detectee comme prevu !\n");
  }
  
  // ===== TEST 8 : Utilisation pratique avec saisie hexa =====
  debugSerial.println("--- TEST 8 : Integration avec saisie hexa ---");
  debugSerial.println("Simulation d'une saisie utilisateur:");
  
  // Simuler une saisie utilisateur
  char userHexInput[41] = "0123456789ABCDEF0123456789ABCDEF"; // 32 caractères
  uint8_t userKey[17] = {0};
  
  debugSerial.print("Utilisateur a saisi: ");
  debugSerial.println(userHexInput);
  
  if (hexStringToByteArray(userHexInput, userKey, 17))
  {
    debugSerial.println("Cle prete a etre utilisee:");
    printByteArray(userKey, 16);
    
    debugSerial.println("\nUtilisation dans le code:");
    debugSerial.println("lorawan.setAppKey(userKey); // 16 octets sans le 0x00");
  }
  
  debugSerial.println("\n========================================");
  debugSerial.println("Tests termines !");
  debugSerial.println("========================================");
  
  // ===== TESTS DE CONVERSION INVERSE : Byte Array → Hex String =====
  debugSerial.println("\n\n========================================");
  debugSerial.println("CONVERSION INVERSE: Byte Array -> Hex String");
  debugSerial.println("========================================\n");
  
  // ===== TEST 9 : Conversion inverse standard =====
  debugSerial.println("--- TEST 9 : Conversion inverse AppKey ---");
  uint8_t testAppKey[16] = {0x50, 0x48, 0x49, 0x4C, 0x49, 0x50, 0x50, 0x45, 
                            0x4C, 0x4F, 0x56, 0x45, 0x4C, 0x41, 0x4F, 0x4F};
  char hexOutput1[41] = {0}; // 40 caractères + '\0'
  
  debugSerial.println("Tableau source:");
  printByteArray(testAppKey, 16);
  debugSerial.println();
  
  if (byteArrayToHexString(testAppKey, 16, hexOutput1, 41))
  {
    debugSerial.println("Resultat de la conversion:");
    printHexString(hexOutput1);
    debugSerial.print("Chaine obtenue: ");
    debugSerial.println(hexOutput1);
  }
  else
  {
    debugSerial.println("Echec de la conversion !");
  }
  
  // ===== TEST 10 : Conversion inverse clé AES-128 =====
  debugSerial.println("\n--- TEST 10 : Conversion inverse cle AES-128 ---");
  uint8_t testAesKey[16] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF,
                            0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF};
  char hexOutput2[41] = {0};
  
  debugSerial.println("Tableau source:");
  printByteArray(testAesKey, 16);
  debugSerial.println();
  
  if (byteArrayToHexString(testAesKey, 16, hexOutput2, 41))
  {
    debugSerial.println("Resultat de la conversion:");
    printHexString(hexOutput2);
  }
  
  // ===== TEST 11 : Conversion inverse DevEUI =====
  debugSerial.println("\n--- TEST 11 : Conversion inverse DevEUI ---");
  uint8_t testDevEUI[8] = {0x00, 0x04, 0xA3, 0x0B, 0x00, 0x1A, 0x2B, 0x3C};
  char hexOutput3[17] = {0}; // 16 caractères + '\0'
  
  debugSerial.println("Tableau source:");
  printByteArray(testDevEUI, 8);
  debugSerial.println();
  
  if (byteArrayToHexString(testDevEUI, 8, hexOutput3, 17))
  {
    debugSerial.println("Resultat de la conversion:");
    printHexString(hexOutput3);
  }
  
  // ===== TEST 12 : Conversion inverse adresse MAC =====
  debugSerial.println("\n--- TEST 12 : Conversion inverse MAC address ---");
  uint8_t testMac[6] = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};
  char hexOutput4[13] = {0}; // 12 caractères + '\0'
  
  debugSerial.println("Tableau source:");
  printByteArray(testMac, 6);
  debugSerial.println();
  
  if (byteArrayToHexString(testMac, 6, hexOutput4, 13))
  {
    debugSerial.println("Resultat de la conversion:");
    printHexString(hexOutput4);
  }
  
  // ===== TEST 13 : Conversion inverse valeur courte =====
  debugSerial.println("\n--- TEST 13 : Conversion inverse valeur courte ---");
  uint8_t testShort[2] = {0xBE, 0xEF};
  char hexOutput5[5] = {0}; // 4 caractères + '\0'
  
  debugSerial.println("Tableau source:");
  printByteArray(testShort, 2);
  debugSerial.println();
  
  if (byteArrayToHexString(testShort, 2, hexOutput5, 5))
  {
    debugSerial.println("Resultat de la conversion:");
    printHexString(hexOutput5);
  }
  
  // ===== TEST 14 : Test d'erreur - buffer trop petit =====
  debugSerial.println("\n--- TEST 14 : Erreur buffer trop petit ---");
  uint8_t testBigArray[16] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                              0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
  char tooSmallBuffer[10] = {0}; // Trop petit pour 16 octets (besoin de 33)
  
  debugSerial.println("Tentative de conversion de 16 octets dans un buffer de 10 caracteres:");
  
  if (!byteArrayToHexString(testBigArray, 16, tooSmallBuffer, 10))
  {
    debugSerial.println("Erreur detectee comme prevu !\n");
  }
  
  // ===== TEST 15 : Conversion aller-retour (round-trip) =====
  debugSerial.println("--- TEST 15 : Conversion aller-retour ---");
  debugSerial.println("Test de coherence: Hexa -> Bytes -> Hexa");
  
  const char* originalHex = "DEADBEEFCAFEBABE";
  uint8_t intermediateBytes[9] = {0}; // 8 octets + '\0'
  char finalHex[17] = {0};
  
  debugSerial.print("Chaine hexa originale: ");
  debugSerial.println(originalHex);
  
  // Conversion 1 : Hex -> Bytes
  if (hexStringToByteArray(originalHex, intermediateBytes, 9))
  {
    debugSerial.println("\nApres conversion vers bytes:");
    printByteArray(intermediateBytes, 8);
    
    // Conversion 2 : Bytes -> Hex
    if (byteArrayToHexString(intermediateBytes, 8, finalHex, 17))
    {
      debugSerial.println("\nApres conversion retour vers hexa:");
      printHexString(finalHex);
      
      // Vérification
      if (strcmp(originalHex, finalHex) == 0)
      {
        debugSerial.println("\n*** TEST REUSSI: Les chaines sont identiques ! ***");
      }
      else
      {
        debugSerial.println("\n*** ERREUR: Les chaines different ! ***");
        debugSerial.print("Originale: ");
        debugSerial.println(originalHex);
        debugSerial.print("Finale:    ");
        debugSerial.println(finalHex);
      }
    }
  }
  
  debugSerial.println("\n========================================");
  debugSerial.println("Tous les tests termines !");
  debugSerial.println("========================================");
  
  // ===== TESTS DE CONVERSION DÉCIMALE : uint8_t ↔ char buffer =====
  debugSerial.println("\n\n========================================");
  debugSerial.println("CONVERSIONS DECIMALES: uint8_t <-> char buffer");
  debugSerial.println("========================================\n");
  
  // ===== TEST 16 : uint8_t → string (cas typiques) =====
  debugSerial.println("--- TEST 16 : uint8_t -> string (cas typiques) ---");
  char decBuffer[10] = {0};
  
  // Spreading Factor 7
  uint8_t sf7 = 7;
  if (uint8ToDecimalString(sf7, decBuffer, 10))
  {
    debugSerial.print("Resultat: SF");
    debugSerial.print(sf7);
    debugSerial.print(" = \"");
    debugSerial.print(decBuffer);
    debugSerial.println("\"");
  }
  
  // Spreading Factor 12
  uint8_t sf12 = 12;
  if (uint8ToDecimalString(sf12, decBuffer, 10))
  {
    debugSerial.print("Resultat: SF");
    debugSerial.print(sf12);
    debugSerial.print(" = \"");
    debugSerial.print(decBuffer);
    debugSerial.println("\"");
  }
  
  // Valeur maximale
  uint8_t maxVal = 255;
  if (uint8ToDecimalString(maxVal, decBuffer, 10))
  {
    debugSerial.print("Resultat: ");
    debugSerial.print(maxVal);
    debugSerial.print(" = \"");
    debugSerial.print(decBuffer);
    debugSerial.println("\"");
  }
  
  // Valeur minimale
  uint8_t minVal = 0;
  if (uint8ToDecimalString(minVal, decBuffer, 10))
  {
    debugSerial.print("Resultat: ");
    debugSerial.print(minVal);
    debugSerial.print(" = \"");
    debugSerial.print(decBuffer);
    debugSerial.println("\"");
  }
  
  // ===== TEST 17 : string → uint8_t (cas valides) =====
  debugSerial.println("\n--- TEST 17 : string -> uint8_t (cas valides) ---");
  uint8_t resultValue = 0;
  
  // "7" → 7
  if (decimalStringToUint8("7", &resultValue))
  {
    debugSerial.print("Spreading Factor = ");
    debugSerial.println(resultValue);
  }
  
  // "12" → 12
  if (decimalStringToUint8("12", &resultValue))
  {
    debugSerial.print("Spreading Factor = ");
    debugSerial.println(resultValue);
  }
  
  // "255" → 255
  if (decimalStringToUint8("255", &resultValue))
  {
    debugSerial.print("Valeur maximale = ");
    debugSerial.println(resultValue);
  }
  
  // "0" → 0
  if (decimalStringToUint8("0", &resultValue))
  {
    debugSerial.print("Valeur minimale = ");
    debugSerial.println(resultValue);
  }
  
  // Avec espaces " 42 "
  if (decimalStringToUint8(" 42 ", &resultValue))
  {
    debugSerial.print("Avec espaces = ");
    debugSerial.println(resultValue);
  }
  
  // ===== TEST 18 : Erreurs de conversion string → uint8_t =====
  debugSerial.println("\n--- TEST 18 : Erreurs de conversion ---");
  
  // Valeur trop grande
  debugSerial.println("Test: \"256\" (hors plage)");
  if (!decimalStringToUint8("256", &resultValue))
  {
    debugSerial.println("Erreur detectee comme prevu !\n");
  }
  
  // Valeur négative
  debugSerial.println("Test: \"-5\" (negatif)");
  if (!decimalStringToUint8("-5", &resultValue))
  {
    debugSerial.println("Erreur detectee comme prevu !\n");
  }
  
  // Caractère invalide
  debugSerial.println("Test: \"12A\" (caractere invalide)");
  if (!decimalStringToUint8("12A", &resultValue))
  {
    debugSerial.println("Erreur detectee comme prevu !\n");
  }
  
  // Chaîne vide
  debugSerial.println("Test: \"\" (chaine vide)");
  if (!decimalStringToUint8("", &resultValue))
  {
    debugSerial.println("Erreur detectee comme prevu !\n");
  }
  
  // ===== TEST 19 : Conversion aller-retour décimale =====
  debugSerial.println("--- TEST 19 : Conversion aller-retour decimale ---");
  debugSerial.println("Test de coherence: uint8 -> string -> uint8");
  
  uint8_t originalSF = 12;
  char sfBuffer[10] = {0};
  uint8_t finalSF = 0;
  
  debugSerial.print("Valeur originale: ");
  debugSerial.println(originalSF);
  
  // Conversion 1 : uint8 -> string
  if (uint8ToDecimalString(originalSF, sfBuffer, 10))
  {
    debugSerial.print("Apres conversion en string: \"");
    debugSerial.print(sfBuffer);
    debugSerial.println("\"");
    
    // Conversion 2 : string -> uint8
    if (decimalStringToUint8(sfBuffer, &finalSF))
    {
      debugSerial.print("Apres conversion retour: ");
      debugSerial.println(finalSF);
      
      // Vérification
      if (originalSF == finalSF)
      {
        debugSerial.println("\n*** TEST REUSSI: Les valeurs sont identiques ! ***");
      }
      else
      {
        debugSerial.println("\n*** ERREUR: Les valeurs different ! ***");
      }
    }
  }
  
  // ===== TEST 20 : Cas d'usage LoRaWAN Spreading Factor =====
  debugSerial.println("\n--- TEST 20 : Usage pratique LoRaWAN SF ---");
  debugSerial.println("Configuration Spreading Factor:\n");
  
  // Test des 3 valeurs valides
  const char* validSFs[] = {"7", "9", "12"};
  
  for (uint8_t i = 0; i < 3; i++)
  {
    uint8_t spreadingFactor = 0;
    debugSerial.print("Test SF: \"");
    debugSerial.print(validSFs[i]);
    debugSerial.println("\"");
    
    if (validateLoRaWanSF(validSFs[i], &spreadingFactor))
    {
      debugSerial.println("  => lorawan.setSpreadingFactor(spreadingFactor);\n");
    }
  }
  
  // Test des valeurs invalides
  debugSerial.println("Tests de valeurs invalides:");
  const char* invalidSFs[] = {"6", "8", "10", "11", "13"};
  
  for (uint8_t i = 0; i < 5; i++)
  {
    uint8_t spreadingFactor = 0;
    debugSerial.print("Test SF: \"");
    debugSerial.print(invalidSFs[i]);
    debugSerial.println("\"");
    
    if (!validateLoRaWanSF(invalidSFs[i], &spreadingFactor))
    {
      debugSerial.println("  => Rejete comme prevu\n");
    }
  }
  
  // ===== TEST 21 : Autres paramètres LoRaWAN =====
  debugSerial.println("\n--- TEST 21 : Autres parametres LoRaWAN ---");
  
  // Bandwidth
  uint8_t bandwidth = 125;
  char bwBuffer[10] = {0};
  uint8ToDecimalString(bandwidth, bwBuffer, 10);
  debugSerial.print("Bandwidth: ");
  debugSerial.print(bwBuffer);
  debugSerial.println(" kHz");
  
  // Coding Rate
  uint8_t codingRate = 5;
  char crBuffer[10] = {0};
  uint8ToDecimalString(codingRate, crBuffer, 10);
  debugSerial.print("Coding Rate: 4/");
  debugSerial.println(crBuffer);
  
  // TX Power
  uint8_t txPower = 14;
  char pwrBuffer[10] = {0};
  uint8ToDecimalString(txPower, pwrBuffer, 10);
  debugSerial.print("TX Power: ");
  debugSerial.print(pwrBuffer);
  debugSerial.println(" dBm");
  
  debugSerial.println("\n========================================");
  debugSerial.println("Tous les tests de conversion termines !");
  debugSerial.println("========================================");
}





// ===== EXEMPLE D'UTILISATION AVEC SAISIE HEXA =====
//  Attention commentaires en doublons
/*
 * ========================================
 * CONVERSION HEXA STRING → BYTE ARRAY
 * ========================================
 * 
 * Intégration avec votre système de saisie hexadécimale:
 * 
 * void handleHexInputCompletion(void)
 * {
 *   static char hexBuffer[41] = "";
 *   static uint8_t keyBuffer[17] = {0};
 *   
 *   // Récupérer la chaîne hexa saisie par l'utilisateur
 *   finalizeHexInput(hexBuffer);
 *   
 *   // Convertir en tableau d'octets
 *   if (hexStringToByteArray(hexBuffer, keyBuffer, 17))
 *   {
 *     debugSerial.println("Conversion reussie !");
 *     
 *     // Utiliser le tableau (sans le dernier octet qui est 0x00)
 *     uint8_t numBytes = strlen(hexBuffer) / 2;
 *     
 *     // Exemple d'utilisation avec LoRaWAN
 *     lorawan.setAppKey(keyBuffer);      // 16 octets
 *     lorawan.setDevEUI(keyBuffer);      // 8 octets
 *     lorawan.setAppEUI(keyBuffer);      // 8 octets
 *     
 *     // Ou copier dans une variable globale
 *     memcpy(globalAppKey, keyBuffer, numBytes);
 *   }
 *   else
 *   {
 *     debugSerial.println("Erreur de conversion !");
 *     OLEDDisplayMessageL8("Erreur conversion", false, false);
 *   }
 * }
 * 
 * ========================================
 * CONVERSION INVERSE BYTE ARRAY → HEXA STRING
 * ========================================
 * 
 * Conversion d'un tableau d'octets vers une chaîne hexa pour affichage:
 * 
 * void displayStoredKey(void)
 * {
 *   // Récupérer la clé stockée en mémoire
 *   uint8_t storedAppKey[16] = {0x50, 0x48, 0x49, 0x4C, 0x49, 0x50, 0x50, 0x45,
 *                               0x4C, 0x4F, 0x56, 0x45, 0x4C, 0x41, 0x4F, 0x4F};
 *   
 *   char hexDisplay[41] = {0}; // 40 caractères + '\0'
 *   
 *   // Convertir en chaîne hexa pour l'affichage
 *   if (byteArrayToHexString(storedAppKey, 16, hexDisplay, 41))
 *   {
 *     debugSerial.print("AppKey actuelle: ");
 *     debugSerial.println(hexDisplay);
 *     
 *     // Afficher sur l'OLED (en plusieurs lignes si nécessaire)
 *     OLEDDrawText(1, 0, 0, "APPKEY:");
 *     
 *     // Afficher les 20 premiers caractères
 *     char line1[21];
 *     strncpy(line1, hexDisplay, 20);
 *     line1[20] = '\0';
 *     OLEDDrawText(1, 1, 0, line1);
 *     
 *     // Afficher les 20 derniers caractères
 *     char line2[21];
 *     strncpy(line2, &hexDisplay[20], 20);
 *     line2[20] = '\0';
 *     OLEDDrawText(1, 2, 0, line2);
 *     
 *     // Ou utiliser pour pré-remplir la saisie hexa
 *     startHexInput("MODIFIER APPKEY:", hexDisplay, 32);
 *   }
 * }
 * 
 * ========================================
 * EXEMPLE COMPLET : LECTURE/MODIFICATION/SAUVEGARDE
 * ========================================
 * 
 * Workflow complet de gestion d'une clé LoRaWAN:
 * 
 * // Variables globales
 * uint8_t globalAppKey[16] = {0};  // Clé stockée en bytes
 * char hexBuffer[41] = {0};        // Buffer pour affichage/saisie
 * 
 * // 1. Charger la clé depuis EEPROM/Flash au démarrage
 * void loadAppKeyFromStorage(void)
 * {
 *   // Lire depuis EEPROM (exemple)
 *   for (uint8_t i = 0; i < 16; i++)
 *   {
 *     globalAppKey[i] = EEPROM.read(APPKEY_EEPROM_ADDR + i);
 *   }
 *   
 *   debugSerial.println("AppKey chargee depuis EEPROM");
 * }
 * 
 * // 2. Afficher la clé actuelle
 * void displayCurrentAppKey(void)
 * {
 *   if (byteArrayToHexString(globalAppKey, 16, hexBuffer, 41))
 *   {
 *     OLEDDrawText(1, 0, 0, "APPKEY ACTUELLE:");
 *     OLEDDrawText(1, 1, 0, hexBuffer);  // Ligne 1
 *     OLEDDrawText(1, 2, 0, &hexBuffer[20]);  // Ligne 2
 *   }
 * }
 * 
 * // 3. Permettre la modification
 * void modifyAppKey(void)
 * {
 *   // Convertir la clé actuelle en hexa pour pré-remplissage
 *   byteArrayToHexString(globalAppKey, 16, hexBuffer, 41);
 *   
 *   // Lancer la saisie avec la valeur actuelle
 *   startHexInput("NOUVELLE APPKEY:", hexBuffer, 32);
 * }
 * 
 * // 4. Valider et sauvegarder la nouvelle clé
 * void saveNewAppKey(void)
 * {
 *   // hexBuffer contient la nouvelle clé saisie
 *   finalizeHexInput(hexBuffer);
 *   
 *   // Convertir en bytes
 *   uint8_t tempKey[17] = {0};
 *   if (hexStringToByteArray(hexBuffer, tempKey, 17))
 *   {
 *     // Copier dans la clé globale
 *     memcpy(globalAppKey, tempKey, 16);
 *     
 *     // Sauvegarder en EEPROM
 *     for (uint8_t i = 0; i < 16; i++)
 *     {
 *       EEPROM.write(APPKEY_EEPROM_ADDR + i, globalAppKey[i]);
 *     }
 *     EEPROM.commit();
 *     
 *     // Configurer le module LoRaWAN
 *     lorawan.setAppKey(globalAppKey);
 *     
 *     debugSerial.println("Nouvelle AppKey sauvegardee !");
 *     OLEDDisplayMessageL8("Cle sauvegardee", false, false);
 *   }
 * }
 * 
 * ========================================
 * UTILITAIRES SUPPLÉMENTAIRES
 * ========================================
 * 
 * // Comparer deux clés (bytes)
 * bool compareKeys(const uint8_t* key1, const uint8_t* key2, uint8_t length)
 * {
 *   return (memcmp(key1, key2, length) == 0);
 * }
 * 
 * // Effacer une clé (remplir de zéros)
 * void clearKey(uint8_t* key, uint8_t length)
 * {
 *   memset(key, 0x00, length);
 * }
 * 
 * // Générer une clé aléatoire (nécessite un générateur aléatoire)
 * void generateRandomKey(uint8_t* key, uint8_t length)
 * {
 *   for (uint8_t i = 0; i < length; i++)
 *   {
 *     key[i] = random(0, 256);
 *   }
 * }
 * 
 * ========================================
 * CONVERSIONS DÉCIMALES : uint8_t ↔ char buffer
 * ========================================
 * 
 * Utilisations typiques avec paramètres LoRaWAN :
 * 
 * // 1. Configuration Spreading Factor
 * void configureSpreadingFactor(void)
 * {
 *   char sfInput[10] = "12";  // Saisie utilisateur ("7", "9" ou "12")
 *   uint8_t spreadingFactor = 0;
 *   
 *   // Convertir et valider la saisie (uniquement SF7, SF9, SF12)
 *   if (validateLoRaWanSF(sfInput, &spreadingFactor))
 *   {
 *     lorawan.setSpreadingFactor(spreadingFactor);
 *     debugSerial.print("SF configure: SF");
 *     debugSerial.println(spreadingFactor);
 *   }
 *   else
 *   {
 *     debugSerial.println("Erreur: SF doit etre 7, 9 ou 12");
 *     OLEDDisplayMessageL8("SF invalide (7/9/12)", false, false);
 *   }
 * }
 * 
 * // 2. Affichage du Spreading Factor actuel
 * void displayCurrentSF(void)
 * {
 *   uint8_t currentSF = lorawan.getSpreadingFactor(); // Ex: 12
 *   char sfBuffer[10] = {0};
 *   
 *   // Convertir en chaîne pour affichage
 *   if (uint8ToDecimalString(currentSF, sfBuffer, 10))
 *   {
 *     char displayLine[21];
 *     snprintf(displayLine, 21, "SF actuel: %s", sfBuffer);
 *     OLEDDrawText(1, 0, 0, displayLine);
 *     
 *     // Ou plus simple sans conversion :
 *     // snprintf(displayLine, 21, "SF actuel: %u", currentSF);
 *   }
 * }
 * 
 * // 3. Configuration TX Power
 * void configureTxPower(void)
 * {
 *   char pwrInput[10] = "14";  // 14 dBm
 *   uint8_t txPower = 0;
 *   
 *   if (decimalStringToUint8(pwrInput, &txPower))
 *   {
 *     // Validation LoRaWAN (0 à 20 dBm en Europe)
 *     if (txPower <= 20)
 *     {
 *       lorawan.setTxPower(txPower);
 *       debugSerial.print("TX Power: ");
 *       debugSerial.print(txPower);
 *       debugSerial.println(" dBm");
 *     }
 *   }
 * }
 * 
 * // 4. Configuration Bandwidth
 * void configureBandwidth(void)
 * {
 *   // Note: pour bandwidth, on utilise souvent des valeurs > 255
 *   // Donc cette fonction est limitée. Pour BW, préférer uint16_t
 *   
 *   char bwInput[10] = "125";  // 125 kHz
 *   uint8_t bw = 0;
 *   
 *   if (decimalStringToUint8(bwInput, &bw))
 *   {
 *     // BW typiques : 125, 250 (attention 250 dépasse uint8_t !)
 *     lorawan.setBandwidth(bw);
 *   }
 * }
 * 
 * // 5. Saisie numérique et conversion avec validation SF LoRaWAN
 * void handleNumericParameterInput(void)
 * {
 *   static char numBuffer[21] = "12";
 *   
 *   // Démarrer saisie numérique (autorise 7-12 pour la saisie)
 *   // Note: la validation finale se fera avec validateLoRaWanSF
 *   startNumInput("SPREADING FACTOR:", numBuffer, 2, false, false, 7, 12);
 *   
 *   // Après validation
 *   if (processNumInput() == NUM_INPUT_COMPLETED)
 *   {
 *     finalizeNumInput(numBuffer);  // numBuffer = "7", "9" ou "12"
 *     
 *     uint8_t sf = 0;
 *     // Utiliser validateLoRaWanSF pour vérifier que c'est 7, 9 ou 12
 *     if (validateLoRaWanSF(numBuffer, &sf))
 *     {
 *       lorawan.setSpreadingFactor(sf);
 *       debugSerial.print("SF configure: ");
 *       debugSerial.println(sf);
 *     }
 *     else
 *     {
 *       OLEDDisplayMessageL8("SF invalide (7/9/12)", false, false);
 *     }
 *   }
 * }
 * 
 * // 6. Menu de sélection avec affichage
 * void displayLoRaWanParameters(void)
 * {
 *   uint8_t currentSF = 12;
 *   uint8_t currentPwr = 14;
 *   uint8_t currentCR = 5;  // Coding Rate 4/5
 *   
 *   char sfBuf[10], pwrBuf[10], crBuf[10];
 *   
 *   // Convertir tous les paramètres
 *   uint8ToDecimalString(currentSF, sfBuf, 10);
 *   uint8ToDecimalString(currentPwr, pwrBuf, 10);
 *   uint8ToDecimalString(currentCR, crBuf, 10);
 *   
 *   // Afficher sur OLED
 *   char line[21];
 *   
 *   snprintf(line, 21, "SF: %s", sfBuf);
 *   OLEDDrawText(1, 0, 0, line);
 *   
 *   snprintf(line, 21, "PWR: %s dBm", pwrBuf);
 *   OLEDDrawText(1, 1, 0, line);
 *   
 *   snprintf(line, 21, "CR: 4/%s", crBuf);
 *   OLEDDrawText(1, 2, 0, line);
 * }
 * 
 * // 7. Validation complète de paramètres
 * bool validateLoRaWanParameter(const char* paramName, const char* valueStr, 
 *                               uint8_t minVal, uint8_t maxVal, uint8_t* result)
 * {
 *   uint8_t value = 0;
 *   
 *   if (!decimalStringToUint8(valueStr, &value))
 *   {
 *     debugSerial.print("Erreur conversion ");
 *     debugSerial.println(paramName);
 *     return false;
 *   }
 *   
 *   if (value < minVal || value > maxVal)
 *   {
 *     debugSerial.print(paramName);
 *     debugSerial.print(" hors plage: ");
 *     debugSerial.print(minVal);
 *     debugSerial.print("-");
 *     debugSerial.println(maxVal);
 *     return false;
 *   }
 *   
 *   *result = value;
 *   return true;
 * }
 * 
 * // Utilisation pour TX Power (0-20):
 * uint8_t txPower = 0;
 * if (validateLoRaWanParameter("TX Power", "14", 0, 20, &txPower))
 * {
 *   lorawan.setTxPower(txPower);
 * }
 * 
 * // Pour le Spreading Factor, utiliser validateLoRaWanSF:
 * uint8_t sf = 0;
 * if (validateLoRaWanSF("12", &sf))  // Accepte uniquement 7, 9, 12
 * {
 *   lorawan.setSpreadingFactor(sf);
 * }
 * 
 * ========================================
 * MENU DE SÉLECTION SF AVEC 3 CHOIX
 * ========================================
 * 
 * // Menu de sélection du Spreading Factor
 * void selectSpreadingFactor(void)
 * {
 *   // Valeurs valides uniquement
 *   const uint8_t validSFs[] = {7, 9, 12};
 *   const char* sfNames[] = {"SF7", "SF9", "SF12"};
 *   uint8_t currentSelection = 1; // SF9 par défaut
 *   
 *   OLEDDrawText(1, 0, 0, "CHOISIR SF:");
 *   OLEDDrawText(1, 2, 0, "1. SF7  (Rapide)");
 *   OLEDDrawText(1, 3, 0, "2. SF9  (Normal)");
 *   OLEDDrawText(1, 4, 0, "3. SF12 (Longue portee)");
 *   
 *   // Navigation avec touches
 *   if (touche == UP && currentSelection > 0)
 *   {
 *     currentSelection--;
 *   }
 *   else if (touche == DOWN && currentSelection < 2)
 *   {
 *     currentSelection++;
 *   }
 *   else if (touche == VALIDE)
 *   {
 *     uint8_t selectedSF = validSFs[currentSelection];
 *     lorawan.setSpreadingFactor(selectedSF);
 *     
 *     debugSerial.print("SF selectionne: ");
 *     debugSerial.println(sfNames[currentSelection]);
 *   }
 * }
 */




/*
 * ========================================
 * CONVERSION HEXA STRING → BYTE ARRAY
 * ========================================
 * 
 * Intégration avec votre système de saisie hexadécimale:
 * 
 * void handleHexInputCompletion(void)
 * {
 *   static char hexBuffer[41] = "";
 *   static uint8_t keyBuffer[17] = {0};
 *   
 *   // Récupérer la chaîne hexa saisie par l'utilisateur
 *   finalizeHexInput(hexBuffer);
 *   
 *   // Convertir en tableau d'octets
 *   if (hexStringToByteArray(hexBuffer, keyBuffer, 17))
 *   {
 *     debugSerial.println("Conversion reussie !");
 *     
 *     // Utiliser le tableau (sans le dernier octet qui est 0x00)
 *     uint8_t numBytes = strlen(hexBuffer) / 2;
 *     
 *     // Exemple d'utilisation avec LoRaWAN
 *     lorawan.setAppKey(keyBuffer);      // 16 octets
 *     lorawan.setDevEUI(keyBuffer);      // 8 octets
 *     lorawan.setAppEUI(keyBuffer);      // 8 octets
 *     
 *     // Ou copier dans une variable globale
 *     memcpy(globalAppKey, keyBuffer, numBytes);
 *   }
 *   else
 *   {
 *     debugSerial.println("Erreur de conversion !");
 *     OLEDDisplayMessageL8("Erreur conversion", false, false);
 *   }
 * }
 * 
 * ========================================
 * CONVERSION INVERSE BYTE ARRAY → HEXA STRING
 * ========================================
 * 
 * Conversion d'un tableau d'octets vers une chaîne hexa pour affichage:
 * 
 * void displayStoredKey(void)
 * {
 *   // Récupérer la clé stockée en mémoire
 *   uint8_t storedAppKey[16] = {0x50, 0x48, 0x49, 0x4C, 0x49, 0x50, 0x50, 0x45,
 *                               0x4C, 0x4F, 0x56, 0x45, 0x4C, 0x41, 0x4F, 0x4F};
 *   
 *   char hexDisplay[41] = {0}; // 40 caractères + '\0'
 *   
 *   // Convertir en chaîne hexa pour l'affichage
 *   if (byteArrayToHexString(storedAppKey, 16, hexDisplay, 41))
 *   {
 *     debugSerial.print("AppKey actuelle: ");
 *     debugSerial.println(hexDisplay);
 *     
 *     // Afficher sur l'OLED (en plusieurs lignes si nécessaire)
 *     OLEDDrawText(1, 0, 0, "APPKEY:");
 *     
 *     // Afficher les 20 premiers caractères
 *     char line1[21];
 *     strncpy(line1, hexDisplay, 20);
 *     line1[20] = '\0';
 *     OLEDDrawText(1, 1, 0, line1);
 *     
 *     // Afficher les 20 derniers caractères
 *     char line2[21];
 *     strncpy(line2, &hexDisplay[20], 20);
 *     line2[20] = '\0';
 *     OLEDDrawText(1, 2, 0, line2);
 *     
 *     // Ou utiliser pour pré-remplir la saisie hexa
 *     startHexInput("MODIFIER APPKEY:", hexDisplay, 32);
 *   }
 * }
 * 
 * ========================================
 * EXEMPLE COMPLET : LECTURE/MODIFICATION/SAUVEGARDE
 * ========================================
 * 
 * Workflow complet de gestion d'une clé LoRaWAN:
 * 
 * // Variables globales
 * uint8_t globalAppKey[16] = {0};  // Clé stockée en bytes
 * char hexBuffer[41] = {0};        // Buffer pour affichage/saisie
 * 
 * // 1. Charger la clé depuis EEPROM/Flash au démarrage
 * void loadAppKeyFromStorage(void)
 * {
 *   // Lire depuis EEPROM (exemple)
 *   for (uint8_t i = 0; i < 16; i++)
 *   {
 *     globalAppKey[i] = EEPROM.read(APPKEY_EEPROM_ADDR + i);
 *   }
 *   
 *   debugSerial.println("AppKey chargee depuis EEPROM");
 * }
 * 
 * // 2. Afficher la clé actuelle
 * void displayCurrentAppKey(void)
 * {
 *   if (byteArrayToHexString(globalAppKey, 16, hexBuffer, 41))
 *   {
 *     OLEDDrawText(1, 0, 0, "APPKEY ACTUELLE:");
 *     OLEDDrawText(1, 1, 0, hexBuffer);  // Ligne 1
 *     OLEDDrawText(1, 2, 0, &hexBuffer[20]);  // Ligne 2
 *   }
 * }
 * 
 * // 3. Permettre la modification
 * void modifyAppKey(void)
 * {
 *   // Convertir la clé actuelle en hexa pour pré-remplissage
 *   byteArrayToHexString(globalAppKey, 16, hexBuffer, 41);
 *   
 *   // Lancer la saisie avec la valeur actuelle
 *   startHexInput("NOUVELLE APPKEY:", hexBuffer, 32);
 * }
 * 
 * // 4. Valider et sauvegarder la nouvelle clé
 * void saveNewAppKey(void)
 * {
 *   // hexBuffer contient la nouvelle clé saisie
 *   finalizeHexInput(hexBuffer);
 *   
 *   // Convertir en bytes
 *   uint8_t tempKey[17] = {0};
 *   if (hexStringToByteArray(hexBuffer, tempKey, 17))
 *   {
 *     // Copier dans la clé globale
 *     memcpy(globalAppKey, tempKey, 16);
 *     
 *     // Sauvegarder en EEPROM
 *     for (uint8_t i = 0; i < 16; i++)
 *     {
 *       EEPROM.write(APPKEY_EEPROM_ADDR + i, globalAppKey[i]);
 *     }
 *     EEPROM.commit();
 *     
 *     // Configurer le module LoRaWAN
 *     lorawan.setAppKey(globalAppKey);
 *     
 *     debugSerial.println("Nouvelle AppKey sauvegardee !");
 *     OLEDDisplayMessageL8("Cle sauvegardee", false, false);
 *   }
 * }
 * 
 * ========================================
 * UTILITAIRES SUPPLÉMENTAIRES
 * ========================================
 * 
 * // Comparer deux clés (bytes)
 * bool compareKeys(const uint8_t* key1, const uint8_t* key2, uint8_t length)
 * {
 *   return (memcmp(key1, key2, length) == 0);
 * }
 * 
 * // Effacer une clé (remplir de zéros)
 * void clearKey(uint8_t* key, uint8_t length)
 * {
 *   memset(key, 0x00, length);
 * }
 * 
 * // Générer une clé aléatoire (nécessite un générateur aléatoire)
 * void generateRandomKey(uint8_t* key, uint8_t length)
 * {
 *   for (uint8_t i = 0; i < length; i++)
 *   {
 *     key[i] = random(0, 256);
 *   }
 * }
 * 
 * ========================================
 * CONVERSIONS DÉCIMALES : uint8_t ↔ char buffer
 * ========================================
 * 
 * Utilisations typiques avec paramètres LoRaWAN :
 * 
 * // 1. Configuration Spreading Factor
 * void configureSpreadingFactor(void)
 * {
 *   char sfInput[10] = "12";  // Saisie utilisateur
 *   uint8_t spreadingFactor = 0;
 *   
 *   // Convertir la saisie en uint8_t
 *   if (decimalStringToUint8(sfInput, &spreadingFactor))
 *   {
 *     // Validation LoRaWAN (SF7 à SF12)
 *     if (spreadingFactor >= 7 && spreadingFactor <= 12)
 *     {
 *       lorawan.setSpreadingFactor(spreadingFactor);
 *       debugSerial.print("SF configure: SF");
 *       debugSerial.println(spreadingFactor);
 *     }
 *     else
 *     {
 *       debugSerial.println("Erreur: SF doit etre entre 7 et 12");
 *       OLEDDisplayMessageL8("SF invalide", false, false);
 *     }
 *   }
 * }
 * 
 * // 2. Affichage du Spreading Factor actuel
 * void displayCurrentSF(void)
 * {
 *   uint8_t currentSF = lorawan.getSpreadingFactor(); // Ex: 12
 *   char sfBuffer[10] = {0};
 *   
 *   // Convertir en chaîne pour affichage
 *   if (uint8ToDecimalString(currentSF, sfBuffer, 10))
 *   {
 *     char displayLine[21];
 *     snprintf(displayLine, 21, "SF actuel: %s", sfBuffer);
 *     OLEDDrawText(1, 0, 0, displayLine);
 *     
 *     // Ou plus simple sans conversion :
 *     // snprintf(displayLine, 21, "SF actuel: %u", currentSF);
 *   }
 * }
 * 
 * // 3. Configuration TX Power
 * void configureTxPower(void)
 * {
 *   char pwrInput[10] = "14";  // 14 dBm
 *   uint8_t txPower = 0;
 *   
 *   if (decimalStringToUint8(pwrInput, &txPower))
 *   {
 *     // Validation LoRaWAN (0 à 20 dBm en Europe)
 *     if (txPower <= 20)
 *     {
 *       lorawan.setTxPower(txPower);
 *       debugSerial.print("TX Power: ");
 *       debugSerial.print(txPower);
 *       debugSerial.println(" dBm");
 *     }
 *   }
 * }
 * 
 * // 4. Configuration Bandwidth
 * void configureBandwidth(void)
 * {
 *   // Note: pour bandwidth, on utilise souvent des valeurs > 255
 *   // Donc cette fonction est limitée. Pour BW, préférer uint16_t
 *   
 *   char bwInput[10] = "125";  // 125 kHz
 *   uint8_t bw = 0;
 *   
 *   if (decimalStringToUint8(bwInput, &bw))
 *   {
 *     // BW typiques : 125, 250 (attention 250 dépasse uint8_t !)
 *     lorawan.setBandwidth(bw);
 *   }
 * }
 * 
 * // 5. Saisie numérique et conversion
 * void handleNumericParameterInput(void)
 * {
 *   static char numBuffer[21] = "12";
 *   
 *   // Démarrer saisie numérique
 *   startNumInput("SPREADING FACTOR:", numBuffer, 2, false, false, 7, 12);
 *   
 *   // Après validation
 *   if (processNumInput() == NUM_INPUT_COMPLETED)
 *   {
 *     finalizeNumInput(numBuffer);  // numBuffer = "12"
 *     
 *     uint8_t sf = 0;
 *     if (decimalStringToUint8(numBuffer, &sf))
 *     {
 *       lorawan.setSpreadingFactor(sf);
 *       debugSerial.print("SF configure: ");
 *       debugSerial.println(sf);
 *     }
 *   }
 * }
 * 
 * // 6. Menu de sélection avec affichage
 * void displayLoRaWanParameters(void)
 * {
 *   uint8_t currentSF = 12;
 *   uint8_t currentPwr = 14;
 *   uint8_t currentCR = 5;  // Coding Rate 4/5
 *   
 *   char sfBuf[10], pwrBuf[10], crBuf[10];
 *   
 *   // Convertir tous les paramètres
 *   uint8ToDecimalString(currentSF, sfBuf, 10);
 *   uint8ToDecimalString(currentPwr, pwrBuf, 10);
 *   uint8ToDecimalString(currentCR, crBuf, 10);
 *   
 *   // Afficher sur OLED
 *   char line[21];
 *   
 *   snprintf(line, 21, "SF: %s", sfBuf);
 *   OLEDDrawText(1, 0, 0, line);
 *   
 *   snprintf(line, 21, "PWR: %s dBm", pwrBuf);
 *   OLEDDrawText(1, 1, 0, line);
 *   
 *   snprintf(line, 21, "CR: 4/%s", crBuf);
 *   OLEDDrawText(1, 2, 0, line);
 * }
 * 
 * // 7. Validation complète de paramètres
 * bool validateLoRaWanParameter(const char* paramName, const char* valueStr, 
 *                               uint8_t minVal, uint8_t maxVal, uint8_t* result)
 * {
 *   uint8_t value = 0;
 *   
 *   if (!decimalStringToUint8(valueStr, &value))
 *   {
 *     debugSerial.print("Erreur conversion ");
 *     debugSerial.println(paramName);
 *     return false;
 *   }
 *   
 *   if (value < minVal || value > maxVal)
 *   {
 *     debugSerial.print(paramName);
 *     debugSerial.print(" hors plage: ");
 *     debugSerial.print(minVal);
 *     debugSerial.print("-");
 *     debugSerial.println(maxVal);
 *     return false;
 *   }
 *   
 *   *result = value;
 *   return true;
 * }
 * 
 * // Utilisation :
 * uint8_t sf = 0;
 * if (validateLoRaWanParameter("SF", "12", 7, 12, &sf))
 * {
 *   lorawan.setSpreadingFactor(sf);
 * }
 */


// ===== EXEMPLE D'UTILISATION AVEC SAISIE HEXA =====

/*
 * ========================================
 * CONVERSION HEXA STRING → BYTE ARRAY
 * ========================================
 * 
 * Intégration avec votre système de saisie hexadécimale:
 * 
 * void handleHexInputCompletion(void)
 * {
 *   static char hexBuffer[41] = "";
 *   static uint8_t keyBuffer[17] = {0};
 *   
 *   // Récupérer la chaîne hexa saisie par l'utilisateur
 *   finalizeHexInput(hexBuffer);
 *   
 *   // Convertir en tableau d'octets
 *   if (hexStringToByteArray(hexBuffer, keyBuffer, 17))
 *   {
 *     debugSerial.println("Conversion reussie !");
 *     
 *     // Utiliser le tableau (sans le dernier octet qui est 0x00)
 *     uint8_t numBytes = strlen(hexBuffer) / 2;
 *     
 *     // Exemple d'utilisation avec LoRaWAN
 *     lorawan.setAppKey(keyBuffer);      // 16 octets
 *     lorawan.setDevEUI(keyBuffer);      // 8 octets
 *     lorawan.setAppEUI(keyBuffer);      // 8 octets
 *     
 *     // Ou copier dans une variable globale
 *     memcpy(globalAppKey, keyBuffer, numBytes);
 *   }
 *   else
 *   {
 *     debugSerial.println("Erreur de conversion !");
 *     OLEDDisplayMessageL8("Erreur conversion", false, false);
 *   }
 * }
 * 
 * ========================================
 * CONVERSION INVERSE BYTE ARRAY → HEXA STRING
 * ========================================
 * 
 * Conversion d'un tableau d'octets vers une chaîne hexa pour affichage:
 * 
 * void displayStoredKey(void)
 * {
 *   // Récupérer la clé stockée en mémoire
 *   uint8_t storedAppKey[16] = {0x50, 0x48, 0x49, 0x4C, 0x49, 0x50, 0x50, 0x45,
 *                               0x4C, 0x4F, 0x56, 0x45, 0x4C, 0x41, 0x4F, 0x4F};
 *   
 *   char hexDisplay[41] = {0}; // 40 caractères + '\0'
 *   
 *   // Convertir en chaîne hexa pour l'affichage
 *   if (byteArrayToHexString(storedAppKey, 16, hexDisplay, 41))
 *   {
 *     debugSerial.print("AppKey actuelle: ");
 *     debugSerial.println(hexDisplay);
 *     
 *     // Afficher sur l'OLED (en plusieurs lignes si nécessaire)
 *     OLEDDrawText(1, 0, 0, "APPKEY:");
 *     
 *     // Afficher les 20 premiers caractères
 *     char line1[21];
 *     strncpy(line1, hexDisplay, 20);
 *     line1[20] = '\0';
 *     OLEDDrawText(1, 1, 0, line1);
 *     
 *     // Afficher les 20 derniers caractères
 *     char line2[21];
 *     strncpy(line2, &hexDisplay[20], 20);
 *     line2[20] = '\0';
 *     OLEDDrawText(1, 2, 0, line2);
 *     
 *     // Ou utiliser pour pré-remplir la saisie hexa
 *     startHexInput("MODIFIER APPKEY:", hexDisplay, 32);
 *   }
 * }
 * 
 * ========================================
 * EXEMPLE COMPLET : LECTURE/MODIFICATION/SAUVEGARDE
 * ========================================
 * 
 * Workflow complet de gestion d'une clé LoRaWAN:
 * 
 * // Variables globales
 * uint8_t globalAppKey[16] = {0};  // Clé stockée en bytes
 * char hexBuffer[41] = {0};        // Buffer pour affichage/saisie
 * 
 * // 1. Charger la clé depuis EEPROM/Flash au démarrage
 * void loadAppKeyFromStorage(void)
 * {
 *   // Lire depuis EEPROM (exemple)
 *   for (uint8_t i = 0; i < 16; i++)
 *   {
 *     globalAppKey[i] = EEPROM.read(APPKEY_EEPROM_ADDR + i);
 *   }
 *   
 *   debugSerial.println("AppKey chargee depuis EEPROM");
 * }
 * 
 * // 2. Afficher la clé actuelle
 * void displayCurrentAppKey(void)
 * {
 *   if (byteArrayToHexString(globalAppKey, 16, hexBuffer, 41))
 *   {
 *     OLEDDrawText(1, 0, 0, "APPKEY ACTUELLE:");
 *     OLEDDrawText(1, 1, 0, hexBuffer);  // Ligne 1
 *     OLEDDrawText(1, 2, 0, &hexBuffer[20]);  // Ligne 2
 *   }
 * }
 * 
 * // 3. Permettre la modification
 * void modifyAppKey(void)
 * {
 *   // Convertir la clé actuelle en hexa pour pré-remplissage
 *   byteArrayToHexString(globalAppKey, 16, hexBuffer, 41);
 *   
 *   // Lancer la saisie avec la valeur actuelle
 *   startHexInput("NOUVELLE APPKEY:", hexBuffer, 32);
 * }
 * 
 * // 4. Valider et sauvegarder la nouvelle clé
 * void saveNewAppKey(void)
 * {
 *   // hexBuffer contient la nouvelle clé saisie
 *   finalizeHexInput(hexBuffer);
 *   
 *   // Convertir en bytes
 *   uint8_t tempKey[17] = {0};
 *   if (hexStringToByteArray(hexBuffer, tempKey, 17))
 *   {
 *     // Copier dans la clé globale
 *     memcpy(globalAppKey, tempKey, 16);
 *     
 *     // Sauvegarder en EEPROM
 *     for (uint8_t i = 0; i < 16; i++)
 *     {
 *       EEPROM.write(APPKEY_EEPROM_ADDR + i, globalAppKey[i]);
 *     }
 *     EEPROM.commit();
 *     
 *     // Configurer le module LoRaWAN
 *     lorawan.setAppKey(globalAppKey);
 *     
 *     debugSerial.println("Nouvelle AppKey sauvegardee !");
 *     OLEDDisplayMessageL8("Cle sauvegardee", false, false);
 *   }
 * }
 * 
 * ========================================
 * UTILITAIRES SUPPLÉMENTAIRES
 * ========================================
 * 
 * // Comparer deux clés (bytes)
 * bool compareKeys(const uint8_t* key1, const uint8_t* key2, uint8_t length)
 * {
 *   return (memcmp(key1, key2, length) == 0);
 * }
 * 
 * // Effacer une clé (remplir de zéros)
 * void clearKey(uint8_t* key, uint8_t length)
 * {
 *   memset(key, 0x00, length);
 * }
 * 
 * // Générer une clé aléatoire (nécessite un générateur aléatoire)
 * void generateRandomKey(uint8_t* key, uint8_t length)
 * {
 *   for (uint8_t i = 0; i < length; i++)
 *   {
 *     key[i] = random(0, 256);
 *   }
 * }
 */
