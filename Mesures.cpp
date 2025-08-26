//       1         2         3         4         5         6         7        7
//34567890123456789012345678901234567890123456789012345678901234567890123456789
// IMPRESSION 79 COLONES EN TAILLE 12
//
//  M   M  EEEEE   SSS   U   U  RRRR   EEEEE  SSS    
//  MM MM  E      S      U   U  R   R  E     S      
//  M M M  EEE     SSS   U   U  RRRR   EEE    SSS   
//  M   M  E          S  U   U  R  R   E         S   
//  M   M  EEEEE   SSS    UUU   R   R  EEEEE  SSS  
//
// Projet Ruche PhLe
//
//
// /////////////////////////////////////////////////////////////////////////////

#define __INIT_DONE
#include "define.h"

// ILS avec anti rebond : 47k en Pull Up et 100nf // ILS

// Copie de l'exemple:  powerDownWakeExternalInterrupt.ino

/*
   typedef struct
  {
  uint8_t rucher_ID;        // ID Rucher           xx  uint8_t
  float   DHT_Temp;         // Temp DHT en °C    xx,x  Float
  float   DHT_Hum;          // Hum DHT en %      xx,x  Float
  float   Brightness;       // %Lum en LUX      xxxxx  uint16_t
  float   Bat_Voltage;      // Tension BAT en V     xx,xx  Float (uint16_t)
  float   Solar_Voltage;    // Tension BAT en V     xx,xx  Float (uint16_t)
  float   HX711Weight_1;    // masse Ruche 1 en kg xxx,xx (précision affich. 10g)
  float   temp_1;           // température peson 1 en °C xx,xx  Float
  float   HX711Weight_2;    // masse Ruche 2 en kg xx,xx
  float   temp_2;           // masse Ruche 1 en kg xx,xx  Float
  float   HX711Weight_3;    // masse Ruche 3 en kg xx,xx
  float   temp_3;           // masse Ruche 1 en kg xx,xx  Float
  float   HX711Weight_4;    // masse Ruche 4 en kg xx,xx
  float   temp_4;           // masse Ruche 1 en kg xx,xx  Float
//  float   VSol[11];
//  float   VBat[11];
  float   Lux;              // temp µC, ne sera pas conservé
  float   ProcessorTemp;    // temp µC, ne sera pas conservé
  } LoRa_Var;


*/

// ---------------------------------------------------------------------------*
//                    indices des macros de 0 à 3 ou de 1 à 4????? manque de cohérence!           
// ---------------------------------------------------------------------------*
void take_All_Measure()  // durée de la proc?
{
  read_DHT(dht); // initialise : Data_LoRa.DHT_Temp et Data_LoRa.DHT_Hum
  readingT=getTemperature();
  Data_LoRa.Brightness = getLuminance();
//  Data_LoRa.Lux = ???();
//  Data_LoRa.ProcessorTemp = getTemperature(); // lecture Temp en String
  Data_LoRa.Solar_Voltage=getVSolMoy();
  Data_LoRa.Bat_Voltage=getVBatMoy();
  Temp_Peson(2)= get_DS(DS18B20[0]);
//#ifdef WEIGHT_YES

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// poids peson doit être de 0 à 3 et GetPoids de 1..4 confirmé 06/06/2025
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// rappel : #define Poids_Peson(num)      Data_LoRa.HX711Weight[num] 
// rappel : #define Temp_Peson(num)       Data_LoRa.Temp_Peson[num]
// rappel : #define Tare_Peson(num)       Jauge[Peson[Ruche.Num_Carte][num]][0]
// rappel : #define Echelle_Peson(num)    Jauge[Peson[Ruche.Num_Carte][num]][1]
//#define BalPoids(num)  (Contrainte_List[num]-Jauge[Peson[Ruche.Num_Carte][num]][0])/Jauge[Peson[Ruche.Num_Carte][num]][1]/1000) //retourne float
// rappel : #define BalPoids(num) (Contrainte_List[num]-Tare_Peson(num))/Echelle_Peson(num)/1000 //retourne float
// ?? attention step(num) entre TareTemp et la correespondance
//         1..4                    0..3
// rappel : #define TareTemp(num)   Jauge[Peson[Ruche.Num_Carte][num]][2]  // Ruche de type HW_equipement (ligne 21)
// rappel : #define CompTemp(num)   Jauge[Peson[Ruche.Num_Carte][num]][3]

  Poids_Peson(0) = GetPoids(1); // à ranger dans Structure   // getpoids paramètre decrementé
  Poids_Peson(1) = GetPoids(2); // à ranger dans Structure
  Poids_Peson(2) = GetPoids(3); // à ranger dans Structure
  Poids_Peson(3) = GetPoids(4); // à ranger dans Structure
/* 
// compensation de 2 vers 4 pour validation règle de correction
static float oldpoids2=0;

Poids_Peson(2) = Poids_Peson(2)-0.5 - (Temp_Peson(2)-TareTemp(2))*CompTemp(2); // decalé de 500g
if (oldpoids2 ==0)
  oldpoids2 = Poids_Peson(2);
  float delta = abs(Poids_Peson(2)-oldpoids2);
  snprintf(serialbuf, BUFF_MAX,"%6.3f",Poids_Peson(2) );
debugSerial.println(serialbuf); 
  snprintf(serialbuf, BUFF_MAX,"%6.3f",oldpoids2);
debugSerial.println(serialbuf); 
  snprintf(serialbuf, BUFF_MAX,"%6.3f",delta);
debugSerial.println(serialbuf); 
  if ( delta < 0.1) // moyennage si variation inf à 100g
    Poids_Peson(2)=(oldpoids2 + Poids_Peson(2)) / 2;  // moyenné sur moyenne(2points) + nlle mesure.
  snprintf(serialbuf, BUFF_MAX,"%6.3f",Poids_Peson(2) );
debugSerial.println(serialbuf); 
  oldpoids2 = Poids_Peson(2);  // memorise poids
  Poids_Peson(3) = Poids_Peson(1) - (Temp_Peson(1)-TareTemp(1))*CompTemp(1); 
//#endif // WEIGHT_YES
*/
debugSerial.println("Take_All_Measure");
debugSerial.println(" Temp  Hum   Vs   Vb   ds18    P1   P2    P3    P4    tare1 comp1");
snprintf(serialbuf, BUFF_MAX,"%04.2f %04.2f %04.2f %04.2f %04.2f %04.2f %04.2f %04.2f %04.2f %04.2f %04.2f ",
                 Data_LoRa.DHT_Temp,Data_LoRa.DHT_Hum,Data_LoRa.Solar_Voltage,Data_LoRa.Bat_Voltage,
                 Temp_Peson(1),Poids_Peson(0),Poids_Peson(1),Poids_Peson(2),Poids_Peson(3),TareTemp(1),CompTemp(1));
debugSerial.println(serialbuf); 
}


// jauges de contrainte de J01 à J15
// 1: float valini = 180116;
// valini = scale.read_average(20);  // sans ruche sur balance
// 2: float Echelle = 108.5;   // V
// Echelle = Val20kg - Valini / 20 (en g ou 20000 en KG)
// debugSerial.print("poids calculé : ");
// debugSerial.println((scale.read_average(20)- valini )/Echelle
 
// ---------------------------------------------------------------------------*
// Calcule le coefficient de mise à l'echelle val peson => kg
// valeurs non sauvegardées, à noter en table des jauges.   
// in : num (de la balance 1..4), poids_en_grammes (sur la balance)
// out :  coefficient de mise à l'echelle                          
// ---------------------------------------------------------------------------*
float Set_Scale_Bal(char num, float poids_en_grammes)    // N° de jauges des balances 1 à 4
{ float pesee, valAVid, Echelle, valEnCharge;

 // num--;
  debugSerial.print("Bal. num : "); debugSerial.print((int)num);
  debugSerial.print(" Peson : J"); debugSerial.println(Peson[Ruche.Num_Carte][num]);

  // marche plus nouvelle version!
  scale.begin(balance[num][0], balance[num][1]); // DOUT, SCK
  valAVid = abs(scale.read_average(20));                   // sans charge
  debugSerial.print("Val a vide : "); debugSerial.println(valAVid);
  //valeur à saisir ou en //metre !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  debugSerial.println("poser poids xxxxxg (15s)");
  delay(15000);

  //  pesee = scale.get_units(HX711_NbLect);
  valEnCharge = abs(scale.read_average(20));              // en charge
  debugSerial.print("val en chargeue : "); debugSerial.println(valEnCharge);
  Echelle = (valEnCharge - valAVid) / poids_en_grammes;  //
  debugSerial.println("Mise à echelle faite");
  debugSerial.print("Echelle : "); debugSerial.println(Echelle);
  debugSerial.print("poids calculé : ");
  Contrainte_List [num] = (abs(scale.read_average(20)));
  debugSerial.println((Contrainte_List [num] - valAVid ) / Echelle);

  scale.power_down();              // put the ADC in sleep mode

  Jauge[Peson[Ruche.Num_Carte][num]][0] = valAVid;
  Jauge[Peson[Ruche.Num_Carte][num]][1] = Echelle;
  return ((Contrainte_List [num] - valAVid ) / Echelle);
}


// ---------------------------------------------------------------------------*
// Retourne val non compensée                             
// in  : Numéro de la jauge de contrainte (1..3)
// out : 0 = Pas de jauge enregistrée; valeur jauge, -1 = jauge enr/err HX711
//
// Formule de compensation
//  Wcomp=Wraw(1−c(Tcal−Tsc)) where :
//   Wraw : raw weight values from scale
//      c : compensation coefficient (I use 0.0002 for the same scale/ADC setup)
//   Tcal : Temperature at calibration time (slighly below 20°C)
//    Tsc : Temperature of scale (put the sensor close to the scale)
// ---------------------------------------------------------------------------*
float GetPoids(int num)    // N° de jauges des balances 1 à 4
{ float pesee;
  char param[10];

  num--;
//debugSerial.print(" carte. num : "); debugSerial.print((int)Ruche.Num_Carte);
//debugSerial.print(" Bal. num (0..3) : "); debugSerial.print((int)num); debugSerial.print(" Peson num : "); debugSerial.print((int)Peson[Ruche.Num_Carte][num]);
  if (!Peson[Ruche.Num_Carte][num])
  {
//debugSerial.println(" Peson : N/A");
    return (0); // pas de jauge déclarée
  }

  // Initialisation du HX711
  // si marche plus, contrôler version!
 // CS_File1("HX711.begin" , itoa(num, param, 10) , "" , "");
  scale.begin(balance[num][0], balance[num][1]); // DOUT, SCK
  if (scale.wait_ready_timeout(1000))
  {
    // Lecture brute de la jauge de contrainte
   // CS_File1("HX711.read" , itoa(num, param, 10) , "10" , "");
    pesee = abs(scale.read_average(10));
   // CS_File1("HX711.read.done" , itoa(num, param, 10) , "" , "");
debugSerial.print(" reading: ");
debugSerial.print(pesee);
  }
  else
  {
   // CS_File1("HX711.readA.fail" , itoa(num, param, 10) , "" , "");
debugSerial.print("HX711 not found.");
    return (-1); // jauge déclarée mais err lecture
  }
  float temp = ((analogRead(TEMP_SENSOR) * 3300.0 / 1023.0) - 500.0) / 10.0;
  scale.power_down();              // put the ADC in sleep mode
  //  Poids_List [num] = pesee*(1-Jauge[Peson[carte][num]][3]*(Jauge[Peson[carte][num]][2]/temp));
  Contrainte_List [num] = pesee; // *(1-Jauge[Peson[carte][num]][3]*(Jauge[Peson[carte][num]][2]/temp));
  // (peson-tare)/echelle
  pesee = BalPoids(num);   // ( pesee - Jauge[Peson[Ruche.Num_Carte][num]][0] ) / Jauge[Peson[Ruche.Num_Carte][num]][1] / 1000; // trouve pas , donne carte = 10 = err
debugSerial.print(" poids : "); debugSerial.println(pesee);
}



//
//
//                 TEMP AUTRES
//


// température interne SAMD21G18 + MCP9700AT + SF + RN2483

// String getTemperature(void);

// ---------------------------------------------------------------------------*
//                               
// ---------------------------------------------------------------------------*
float getTemperature()
{
#ifdef EXPLORER
  //10mV per C, 0C is 500mV
  float mVolts = (float)analogRead(TEMP_SENSOR) * 3300.0 / 1023.0;
  float temp = (mVolts - 500.0) / 10.0;
  sprintf(OLEDbuf, "%4.1f %%", temp);
  debugSerial.print(" TEMP_SENSOR : "); debugSerial.println(OLEDbuf);
  return(temp);
#endif
  return(TEMP_ERR);
}

// /////////////////////////////////////////////////////////////////////////////
//   L       DDD     RRRR
//   L       D   D   R   R
//   L       D   D   RRRR
//   L       D   D   R  R
//   LLLLL   DDDD    R   R
// /////////////////////////////////////////////////////////////////////////////

// ---------------------------------------------------------------------------*
//                               
// ---------------------------------------------------------------------------*
float getLuminance(void) //
{
  //10mV per C, 0C is 500mV
 // CS_File1("LDR.start" , "" , "" , "");
  float mVolts = (float)analogRead(LUM_SENSOR) / 10.23; // echelle 0 à 100
  float lum = (mVolts * VLumScale_List[Ruche.Num_Carte]); // - 500.0) / 10.0;
  sprintf(OLEDbuf, "%4.1f %%", lum);
//  debugSerial.print(" LDR : "); debugSerial.println(OLEDbuf);
 // CS_File1("LDR.end" , "" , "" , "");
  return (lum);
}


// ---------------------------------------------------------------------------*
//                               
// ---------------------------------------------------------------------------*
void toggle(int port)  // ??????????????????????????????????????????????????????????????????????????????????????????????
{ bool state = 0;

  state = ! state;
  digitalWrite(port, state);
}

// /////////////////////////////////////////////////////////////////////////////
//   TTTTT  EEEEE  N   N   SSSS  III   OOO   N    N   SSSS
//     T    E      NN  N  S       I   O   O  NN   N  S
//     T    EEE    N N N   SSS    I   O   O  N  N N   SSS
//     T    E      N  NN      S   I   O   O  N   NN      S
//     T    EEEEE  N   N  SSSS   III   OOO   N    N  SSSS
// /////////////////////////////////////////////////////////////////////////////
// ---------------------------------------------------------------------------*
//                               
// ---------------------------------------------------------------------------*
float getVBatMoy(void)
{ char i;
  static char v = 0;

 // CS_File1("VBat.start" , "" , "" , "");

  // pilote FET@RD_VBAT à 1
  digitalWrite(RD_VBAT, HIGH);       // Blocage FET



  VBat[v] = analogRead(VBAT_MEASURE);   // on fait une mesure
  sprintf(OLEDbuf, "%d ", VBat[v]);
  //  debug
  //  loraSerial.print(" ANA : ");loraSerial.println(VBat[v]); //OLEDbuf);

  VBat[v] = VBat[v] * 43 / 33; //43k/10k+33k aux tolerances des R près... et VGS
  VBat[v++] *= VBatScale_List[Ruche.Num_Carte];  // on applique la résolution

  // Pilote FET@RD_VBAT à 0
  digitalWrite(RD_VBAT, LOW);       // Blocage FET

  for (i = 0; i < 10; i++)    // on recalcule la moyenne des 10 mesures
  {
    VBat[10] += VBat[i];
  }
  VBat[10] /= 11;
/*          Affichage de controle
  sprintf(OLEDbuf, " VBat : %4.3f V", VBat[v - 1]);
  debugSerial.print(OLEDbuf);
  sprintf(OLEDbuf, " VBatMoy : %4.3f V", VBat[10]);
  debugSerial.println(OLEDbuf);
*/

  if (v > 9) v = 0;
//  CS_File1("VBat.end" , "" , "" , "");
  return (VBat[10]);

}

// ---------------------------------------------------------------------------*
//                               
// ---------------------------------------------------------------------------*
float getVSolMoy(void)
{ char i;
  static char v = 0;

  VSol[v] = analogRead(VSOL_MEASURE) * VSolScale_List[Ruche.Num_Carte]; // on fait une mesure
  VSol[10] = 0;
  for (i = 0; i < 10; i++)    // on recalcule la moyenne des 10 mesures
    VSol[10] += VSol[i]; 
  VSol[10] /= 10;

/*          Affichage de controle
  sprintf(OLEDbuf, " VSol : %4.2f V %d", VSol[v], v); // valeur lue
  debugSerial.print(OLEDbuf);
  sprintf(OLEDbuf, " VSolMoy : %4.2f V", VSol[10]); // moyenne courante
  debugSerial.println(OLEDbuf);
*/
  if (++v > 9) 
    v = 0;
  return (VSol[10]);   // moyenne courante
}

// exemples struct et union


struct nombre
{
  unsigned entier : 1;
  unsigned flottant : 1;
  union
  {
    int e;
    double f;
  } u;
};


// ---------------------------------------------------------------------------*
//                               
// ---------------------------------------------------------------------------*
static void affiche_nombre(struct nombre n)  // ????????????????????????????????????????????????????????????????????
{
  if (n.entier)
    printf("%d\n", n.u.e);
  else if (n.flottant)
    printf("%f\n", n.u.f);
}


// ---------------------------------------------------------------------------*
//                               
// ---------------------------------------------------------------------------*
int uniontest(void)  // ????????????????????????????????????????????????????????????????????????????????????????????
{
  struct nombre a = { 0 };
  struct nombre b = { 0 };

  a.entier = 1;
  a.u.e = 10;
  b.flottant = 1;
  b.u.f = 10.56;

  affiche_nombre(a);
  affiche_nombre(b);
  return 0;
}



//// DS18B20
// OneWire DS18S20, DS18B20, DS1822 Temperature Example
//
// http://www.pjrc.com/teensy/td_libs_OneWire.html
//
// The DallasTemperature library can do all this work for you!
// https://github.com/milesburton/Arduino-Temperature-Control-Library
// retourne Temp ou -1000 si erreur
// ---------------------------------------------------------------------------*
//                               
// ---------------------------------------------------------------------------*
float get_DS(byte *DS18B20) 
{
  byte i;
  byte present = 0;
  byte type_s;
  byte data[12];
//  byte addr[8];
  float celsius; //, fahrenheit;

/*

  if ( !ds.search(addr))
  {

  debugSerial.print("ROMrien =");
  for ( i = 0; i < 8; i++) 
  {
    debugSerial.write(' ');
    debugSerial.print(addr[i], HEX);
  }
    
    //    debugSerial.println("No more addresses.");
    //    debugSerial.println();
//    CS_File1("DS.search" , "" , "" , "");   // cause possible erreur DS
    ds.reset_search();
    delay(250);
    return (DS18A20_ERR);
  }

  debugSerial.print("ROM =");
  for ( i = 0; i < 8; i++) 
  {
    debugSerial.write(' ');
    debugSerial.print(addr[i], HEX);
  }
*/
/* extrait de ruche_test_DS18x20.ino
11:54:16.179 -> ROM = 28 FF F4 AA 64 15 3 89  Chip = DS18B20
11:54:17.215 ->   Data = 1 1 1 4B 46 7F FF C 10 B6  CRC=B6
11:54:17.215 ->   Temperature = 16.06 Celsius, 60.91 Fahrenheit
11:54:17.215 -> ROM = 28 FF B9 BD 64 15 3 32  Chip = DS18B20
11:54:18.255 ->   Data = 1 FB 0 4B 46 7F FF C 10 0  CRC=0
11:54:18.255 ->   Temperature = 15.69 Celsius, 60.24 Fahrenheit
11:54:18.255 -> ROM = 28 FF 2D F6 64 15 3 EE  Chip = DS18B20
11:54:19.284 ->   Data = 1 FC 0 4B 46 7F FF C 10 D0  CRC=D0
11:54:19.284 ->   Temperature = 15.75 Celsius, 60.35 Fahrenheit
11:54:19.329 -> ROM = 28 FF DF E4 64 15 1 48  Chip = DS18B20
11:54:20.313 ->   Data = 1 24 1 4B 46 7F FF C 10 48  CRC=48
11:54:20.313 ->   Temperature = 18.25 Celsius, 64.85 Fahrenheit
11:54:20.313 -> No more addresses.

// 10:23:14.649 -> ROM = 28 FF DF E4 64 15 1 -48- 
// 10:36:06.875 -> ROM = 28 FF F4 AA 64 15 3 -89- Read data : 9B 1 4B 46 7F FF C 10 DC   Temperature = 25.69 Celsius, 
// 10:37:20.277 -> ROM = 28 FF 2D F6 64 15 3 -EE- Read data : FD 0 4B 46 7F FF C 10 93   Temperature = 15.81 Celsius, 
// 10:38:23.520 -> ROM = 28 FF B9 BD 64 15 3 -32- 1Read data : F9 0 4B 46 7F FF C 10 86   Temperature = 15.56 Celsius, 

10:43:51.326 -> ROM = 28 FF B9 BD 64 15 3 32 Read data : 0 1 4B 46 7F FF C 10 F5   Temperature = 16.00 Celsius, 
10:44:10.219 -> ROM = 28 FF 2D F6 64 15 3 EE Read data : FB 0 4B 46 7F FF C 10 0   Temperature = 15.69 Celsius,
10:44:28.784 -> ROM = 28 FF DF E4 64 15 1 48 Read data : CF 0 4B 46 7F FF C 10 E9   Temperature = 12.94 Celsius,
10:55:34.810 -> ROM = A4 22 0 20 58 3 0 20 => rien, ne retourne pas 28????
10:45:05.740 -> ROM = 28 FF F4 AA 64 15 3 89 Read data : A 1 4B 46 7F FF C 10 59   Temperature = 16.62 Celsius, 
10:45:24.652 -> ROM = 28 FF B9 BD 64 15 3 32 Read data : FF 0 4B 46 7F FF C 10 15   Temperature = 15.94 Celsius, 
10:45:43.209 -> ROM = 28 FF 2D F6 64 15 3 EE ead data : F9 0 4B 46 7F FF C 10 86   Temperature = 15.56 Celsius, 
10:46:02.213 -> ROM = 28 FF DF E4 64 15 1 48 Read data : D2 0 4B 46 7F FF C 10 C1   Temperature = 13.13 Celsius,
10:57:07.942 -> ROM = A4 22 0 20 58 3 0 20 => rien, ne retourne pas 28????
10:46:38.673 -> ROM = 28 FF F4 AA 64 15 3 89 Read data : 6 1 4B 46 7F FF C 10 66   Temperature = 16.37 Celsius, 
10:46:57.693 -> ROM = 28 FF B9 BD 64 15 3 32 Read data : FD 0 4B 46 7F FF C 10 93   Temperature = 15.81 Celsius, 
etc

exemple ROM du dernier: 28 FF B9 BD 64 15 3 32
28 = code famille
FF B9 BD 64 15 03 = octets propre au circuit
32 = CRC
exemple DATA du dernier: FD 0 4B 46 7F FF C 10 93
FD 0  = temp LSB/MSB

    byte cfg = (data[4] & 0x60); => 0111 1111 & 0110 0000 = 0x60
    // at lower res, the low bits are undefined, so let's zero them
    if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
    else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
    else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
    //// default is 12 bit resolution, 750 ms conversion time


4B
46
7F
FF = Reserve
0C = Reserve
10 = Reserve
93 = CRC
*/


// byte DS18B20[8] = addr


  if (OneWire::crc8(DS18B20, 7) != DS18B20[7]) // => CRC error
  {
    //      debugSerial.println("CRC is not valid!");
    return (DS18A20_ERR-1);
  }
  //debugSerial.println();

  // the first ROM byte indicates which chip
  switch (DS18B20[0])
  {
    case 0x10:
      //      debugSerial.println("  Chip = DS18S20");  // or old DS1820
      type_s = 1;
      break;
    case 0x28:
      //      debugSerial.println("  Chip = DS18B20"); // notre retour du capteur => OK
      type_s = 0;
      break;
    case 0x22:
      //      debugSerial.println("  Chip = DS1822");
      type_s = 0;
      break;
    default:
      //     debugSerial.println("Device is not a DS18x20 family device.");
      return (DS18A20_ERR-2);
  }

//  CS_File1("DS.reset" , "" , "" , "");
  ds.reset();
//  CS_File1("DS.select" , "" , "" , "");
  ds.select(DS18B20);
// CS_File1("DS.write" , "" , "" , "");
  ds.write(0x44, 1);        // start conversion, with parasite power on at the end

  delay(800);     // maybe 750ms is enough, maybe not
  // we might do a ds.depower() here, but the reset will take care of it.

  present = ds.reset();
  ds.select(DS18B20);
  ds.write(0xBE);         // Read Scratchpad

  //  debugSerial.print("  Data = ");
  //  debugSerial.print(present, HEX);
  //  debugSerial.print(" ");

debugSerial.print("Read data : ");
  for ( i = 0; i < 9; i++)
  { // we need 9 bytes

    data[i] = ds.read();
       debugSerial.print(data[i], HEX);
       debugSerial.print(" ");
  }

//Read data : C3 0 4B 46 7F FF C 10 D6   Temperature = 12.19 Celsius,  
  
  //  debugSerial.print(" CRC=");
  //  debugSerial.print(OneWire::crc8(data, 8), HEX);
  //  debugSerial.println();

//  char param[10];
//  CS_File1("DS.read" , itoa(i, param, 10) , itoa(data[i], param, 16) , "");

// température contenue dans DATA[ 1 et 2 ] 

  // Convert the data to actual temperature
  // because the result is a 16 bit signed integer, it should
  // be stored to an "int16_t" type, which is always 16 bits
  // even when compiled on a 32 bit processor.
  int16_t raw = (data[1] << 8) | data[0];
  if (type_s)                                       // Chip = DS18S20
  {
    raw = raw << 3; // 9 bit resolution default
    if (data[7] == 0x10)
    {
      // "count remain" gives full 12 bit resolution
      raw = (raw & 0xFFF0) + 12 - data[6];
    }
  } 
  else                                         // Chip = DS18B20 ou DS1822
  {
    byte cfg = (data[4] & 0x60);
    // at lower res, the low bits are undefined, so let's zero them
    if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
    else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
    else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
    //// default is 12 bit resolution, 750 ms conversion time
  }

  celsius = (float)raw / 16.0;
  //  fahrenheit = celsius * 1.8 + 32.0;
  debugSerial.print("  Temperature = ");
  debugSerial.print(celsius);
  debugSerial.println(" Celsius, ");
  //  debugSerial.print(fahrenheit);
  //  debugSerial.println(" Fahrenheit");
  return (celsius);
}
