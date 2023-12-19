#include <EEPROM.h>
#include "EmonLib.h" // Include Emon Library
EnergyMonitor emon1;//
#define pinIrms 34 //define pin ADC
#define EEPROM_SIZE 512 //512 bytes or 512 different address
double Irms; //variable Irms to read current
int Alamat_EEPROM = 0; //start address to EEPROM

void setup()
{
  Serial.begin(115200);
  for (int i = 0; i <= 10; i++) {
    //membaca data dari EEPROM
    float arus = readFloat(Alamat_EEPROM + (i - 1) * sizeof(float));
    Serial.print("Data ke-");
    Serial.print(i);
    Serial.print(" dibaca dari EEPROM: ");
    Serial.println(arus, 3);
  }
  emon1.current(pinIrms, 9.73);// Current: input pin, calibration.
  delay(3000);
}

void loop()
{
  Irms = emon1.calcIrms(1480);  // Calculate Irms only
  double Irms1 = Irms * 230.0;
  Serial.print(Irms1);// Apparent power
  Serial.print(" ");
  Serial.println(Irms, 3);
  delay(20);

  //membuat angka terbaca menjadi 0, saat tidak ada beban.
  if (Irms < 0.0150) {
    Irms = 0;
    Serial.println(Irms, 3);
    delay(1000);
  }
  if (Irms > 0.0150) {
    Irms = Irms;
    Serial.println(Irms, 3);
    delay(1000);
  }

  //total data yang mau di simpan ke EEPROM = 10 data
  for (int i = 0; i <= 10; i++) {
    //menyimpan data ke EEPROM
    Irms = emon1.calcIrms(1480);
    writeFloat(Irms, Alamat_EEPROM + (i - 1)*sizeof(float));
    Serial.print("Data ke-");
    Serial.print(i);
    Serial.print(" disimpan ke EEPROM: ");
    Serial.println(Irms, 3);
    delay(500);
  }
  Serial.println("Data berhasil tersimpan ke EEPROM");
  delay(2000);

  //membaca data dari EEPROM yang tersimpang = 10 data
  for (int i = 0; i <= 10; i++) {
    //membaca data dari EEPROM
    float arus = readFloat(Alamat_EEPROM + (i - 1) * sizeof(float));
    Serial.print("Data ke-");
    Serial.print(i);
    Serial.print(" dibaca dari EEPROM: ");
    Serial.println(arus, 3);
  }
}

//membuat fungsi menyimpan data ke EEPROM
void writeFloat(float nilai, int alamat) {
  EEPROM.begin(EEPROM_SIZE);

  // Menyimpan data float byte per byte
  for (int i = 0; i < sizeof(float); ++i) {
    EEPROM.write(alamat + i, *((char*)&nilai + i));
  }

  // Menyimpan ke EEPROM
  EEPROM.commit();
  EEPROM.end();
}

//fungsi membaca data ke EEPROM
float readFloat(int alamat) {
  EEPROM.begin(EEPROM_SIZE);
  float nilai;

  // Membaca data byte per byte dan mengonversi ke float
  for (int i = 0; i < sizeof(float); ++i) {
    *((char*)&nilai + i) = EEPROM.read(alamat + i);
  }
  EEPROM.end();
  return nilai;
}
