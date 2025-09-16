#define __INIT_DONE
#include "define.h"

// /////////////////////////////////////////////////////////////////////////////
//  DDDD    H   H   TTTTT    222     222
//  D   D   H   H     T     2   2   2   2
//  D   D   HHHHH     T        2       2
//  D   D   H   H     T      2       2
//  DDDD    H   H     T     22222   22222
// /////////////////////////////////////////////////////////////////////////////
// ---------------------------------------------------------------------------*
// @brief Read temp and Humidity with DHT22                              
// @param DHT
// @return void
 #define debugSerialread_DHT  // decommenter pour les messages debugSerial
// ---------------------------------------------------------------------------*
void read_DHT(DHT dht)
{
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  Data_LoRa.DHT_Hum = dht.readHumidity(); // Read temperature as Celsius
  Data_LoRa.DHT_Temp = dht.readTemperature();  // Read temperature as Fahrenheit
  //  DHT_f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(Data_LoRa.DHT_Hum) || isnan(Data_LoRa.DHT_Temp))
  {
#ifdef debugSerialread_DHT
    debugSerial.println(F("Failed to read from DHT sensor!"));
#endif    
    Data_LoRa.DHT_Hum = DHT_H_ERR;
    Data_LoRa.DHT_Temp = DHT_T_ERR;
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  //  float hif = dht.computeHeatIndex(DHT_f, Data_LoRa.DHT_Hum);
  // Compute heat index in Celsius (isFahreheit = false)
 // float hic = dht.computeHeatIndex(Data_LoRa.DHT_Temp, Data_LoRa.DHT_Hum, false);
}
