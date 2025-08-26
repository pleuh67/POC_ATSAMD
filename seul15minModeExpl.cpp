// https://claude.ai/chat/716f1252-7f6a-4d0c-8c4b-59607fdf42f3

/*
#include <Wire.h>
#include <RTClib.h>

RTC_DS3231 rtc;



// Solution avec Alarme DS3231 - 15 minutes
volatile bool fifteenMinFlag = false;

void setup() {
    Serial.begin(9600);
    
    if (!rtc.begin()) {
        Serial.println("RTC non trouvé");
        while(1);
    }
    
    // Configuration alarme pour 15 minutes
    setupAlarm15Min();
    
    // IRQ1 (pin 3) pour alarme 15 min
    pinMode(3, INPUT_PULLUP);
    attachInterrupt(1, fifteenMinISR, FALLING);
    
    Serial.println("Alarme 15 minutes configurée sur IRQ1");
}

void setupAlarm15Min() {
    // Calculer la prochaine alarme (maintenant + 15 min)
    DateTime now = rtc.now();
    DateTime nextAlarm = now + TimeSpan(0, 0, 15, 0);
    
    // Configuration Alarme 1 (plus précise que Alarme 2)
    Wire.beginTransmission(0x68);
    Wire.write(0x07); // A1 Seconds
    Wire.write(0x80); // A1M1=1 (ignore seconds)
    Wire.endTransmission();
    
    Wire.beginTransmission(0x68);
    Wire.write(0x08); // A1 Minutes
    Wire.write(decToBcd(nextAlarm.minute()));
    Wire.endTransmission();
    
    Wire.beginTransmission(0x68);
    Wire.write(0x09); // A1 Hours
    Wire.write(decToBcd(nextAlarm.hour()));
    Wire.endTransmission();
    
    Wire.beginTransmission(0x68);
    Wire.write(0x0A); // A1 Day
    Wire.write(0x80); // A1M4=1 (ignore day)
    Wire.endTransmission();
    
    // Activer l'alarme dans le registre de contrôle
    Wire.beginTransmission(0x68);
    Wire.write(0x0E); // Control register
    Wire.write(0x05); // INTCN=1, A1IE=1 (alarme 1 activée)
    Wire.endTransmission();
    
    // Clear le flag d'alarme
    clearAlarmFlag();
    
    Serial.print("Prochaine alarme: ");
    Serial.print(nextAlarm.hour());
    Serial.print(":");
    Serial.println(nextAlarm.minute());
}

void fifteenMinISR() {
    fifteenMinFlag = true;
}

void loop() {
    if (fifteenMinFlag) {
        fifteenMinFlag = false;
        
        // Clear le flag d'alarme
        clearAlarmFlag();
        
        // Votre action toutes les 15 minutes
        handle15MinTick();
        
        // Programmer la prochaine alarme
        setNextAlarm15Min();
    }
    
    // Autres tâches...
    delay(100);
}

void handle15MinTick() {
    DateTime now = rtc.now();
    static int counter = 0;
    counter++;
    
    Serial.print("Alarme 15min #");
    Serial.print(counter);
    Serial.print(" - ");
    Serial.print(now.hour());
    Serial.print(":");
    Serial.print(now.minute());
    Serial.print(":");
    Serial.println(now.second());
    
    // Vos actions ici...
}

void clearAlarmFlag() {
    Wire.beginTransmission(0x68);
    Wire.write(0x0F); // Status register
    Wire.write(0x00); // Clear A1F flag
    Wire.endTransmission();
}

void setNextAlarm15Min() {
    DateTime now = rtc.now();
    DateTime nextAlarm = now + TimeSpan(0, 0, 15, 0);
    
    Wire.beginTransmission(0x68);
    Wire.write(0x08); // A1 Minutes
    Wire.write(decToBcd(nextAlarm.minute()));
    Wire.endTransmission();
    
    Wire.beginTransmission(0x68);
    Wire.write(0x09); // A1 Hours
    Wire.write(decToBcd(nextAlarm.hour()));
    Wire.endTransmission();
    
    Serial.print("Prochaine alarme programmée: ");
    Serial.print(nextAlarm.hour());
    Serial.print(":");
    Serial.println(nextAlarm.minute());
}

byte decToBcd(byte val) {
    return ((val/10*16) + (val%10));
}



// ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
// Version simplifiée avec alarme récurrente

void setupRecurringAlarm15Min() {
    // Alarme qui se répète automatiquement toutes les 15 minutes
    // en utilisant seulement les minutes
    
    DateTime now = rtc.now();
    int nextMinute = ((now.minute() / 15) + 1) * 15;
    if (nextMinute >= 60) {
        nextMinute = 0;
    }
    
    Wire.beginTransmission(0x68);
    Wire.write(0x07); // A1 Seconds - ignore
    Wire.write(0x80);
    Wire.endTransmission();
    
    Wire.beginTransmission(0x68);
    Wire.write(0x08); // A1 Minutes - match
    Wire.write(decToBcd(nextMinute));
    Wire.endTransmission();
    
    Wire.beginTransmission(0x68);
    Wire.write(0x09); // A1 Hours - ignore
    Wire.write(0x80);
    Wire.endTransmission();
    
    Wire.beginTransmission(0x68);
    Wire.write(0x0A); // A1 Day - ignore
    Wire.write(0x80);
    Wire.endTransmission();
    
    // Control register
    Wire.beginTransmission(0x68);
    Wire.write(0x0E);
    Wire.write(0x05); // INTCN=1, A1IE=1
    Wire.endTransmission();
}

// :::::::::::::::::::::::::::::::::::::
// pour deboguer

void printAlarmStatus() {
    Wire.beginTransmission(0x68);
    Wire.write(0x0F); // Status register
    Wire.endTransmission();
    Wire.requestFrom(0x68, 1);
    
    if (Wire.available()) {
        byte status = Wire.read();
        Serial.print("Status: 0x");
        Serial.print(status, HEX);
        Serial.print(" - A1F: ");
        Serial.println((status & 0x01) ? "SET" : "CLEAR");
    }
}
*/
