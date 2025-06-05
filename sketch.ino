#include <Wire.h>                     // I2C haberleşmesi için kütüphane
#include <LiquidCrystal_I2C.h>        // I2C protokolü ile LCD ekranı kontrol etmek için kütüphane
#include "DHT.h"                      // DHT sensörü için kütüphane

#define DHTPIN 4                      // DHT22 sensörünün bağlı olduğu pin
#define DHTTYPE DHT22                 // Sensör tipi DHT22
#define LED_PIN 2                     // LEDin bağlı olduğu pin

DHT dht(DHTPIN, DHTTYPE);             // DHT nesnesi oluşturma
LiquidCrystal_I2C lcd(0x27, 16, 2);   // LCD nesnesi oluşturma, adres 0x27, 16x2 boyutunda

unsigned long oncekiZamanLCD = 0;     // Son LCD güncelleme zamanını saklayan değişken
const long goruntulemeSuresi = 3000;  // LCDde her bilgi için gösterim süresi (ms)

float sicaklik, nem;                  // Sıcaklık ve nem verilerini tutma
bool lcdNemGoster = true;            // Başlangıçta nem gösteriliyor

void setup() {
  Serial.begin(115200);              // Seri iletişim başlatma
  pinMode(LED_PIN, OUTPUT);          // LED pini çıkış olarak ayarlanıyor
  lcd.init();                        // LCD başlatılıyor
  lcd.backlight();                   // LCD arka ışığı açılıyor
  dht.begin();                       // DHT sensörü başlatılıyor
}

void loop() {
  unsigned long suankiZaman = millis();  // Şu anki zaman millis() ile alınıyor

  // Sensörden sıcaklık ve nem verileri okunuyor
  sicaklik = dht.readTemperature();
  nem = dht.readHumidity();

  // Seri port üzerinden sıcaklık ve nem değerleri yazdırılıyor
  Serial.print("Nem: ");
  Serial.print(nem);
  Serial.print("% || Sıcaklık: ");
  Serial.print(sicaklik);
  Serial.println("°C");

  // Eğer sıcaklık 30°C'den fazlaysa LED yakılıyor, değilse söndürülüyor
  if (sicaklik > 30) {
    digitalWrite(LED_PIN, HIGH);
  } else {
    digitalWrite(LED_PIN, LOW);
  }

  // LCD ekranda 3 saniyede bir nem ve sıcaklık dönüşümlü olarak gösteriliyor
  if (suankiZaman - oncekiZamanLCD >= goruntulemeSuresi) {
    oncekiZamanLCD = suankiZaman;   // Son gösterim zamanı güncelleniyor

    lcd.clear();                     // LCD ekran temizleniyor

    if (lcdNemGoster) {             // Eğer nem gösterilecekse
      lcd.setCursor(4, 0);          // İlk satırda 4. sütundan başla
      lcd.print("NEM");             // "NEM" yazısı yaz
      lcd.setCursor(1, 1);          // İkinci satırda 1. sütundan başla
      lcd.print(nem, 2);            // Nem değerini 2 ondalık basamakla yaz
      lcd.print(" - ");
      lcd.print((nem <= 60) ? "LOW" : "HIGH");  // Neme göre LOW/HIGH bilgisi yazılır 60 ve altıysa LOW
    } else {                        // Eğer sıcaklık gösterilecekse
      lcd.setCursor(2, 0);          // İlk satırda 2. sütundan başla
      lcd.print("SICAKLIK");        // "SICAKLIK" yazısı yaz
      lcd.setCursor(1, 1);          // İkinci satırda 1. sütundan başla
      lcd.print(sicaklik, 2);       // Sıcaklık değerini 2 ondalık basamakla yaz
      lcd.print(" - ");
      
      // Sıcaklık seviyesine göre durum bilgisi yaz
      if (sicaklik < -10) lcd.print("COLD – VERY LOW TEMP.!");
      else if (sicaklik < 0) lcd.print("LOW"); //0ın altındaysa LOW
      else if (sicaklik <= 30) lcd.print("NORMAL"); //0 - 30 derece arası NORMAL
      else if (sicaklik <= 45 ) lcd.print("HIGH"); // 30 derece üstü HIGH
      else lcd.print("HOT – VERY HIGH TEMP.!");

    }

    lcdNemGoster = !lcdNemGoster;   // Bir sonraki döngüde diğer bilgi gösterilecek
  }

  // Döngü sürekli çalışıyor
}
