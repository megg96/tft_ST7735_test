# tft_ST7735_test

Potrzebne do dzialania:
- ArduinoIDE + biblioteki STMduino: https://github.com/stm32duino/wiki/wiki/Getting-Started, pamietac o konfiguracji portu
- plytka NUCLEO obslugiwana przez STMduino, tutaj uzyta 446RE
- przeniesc pliki z folderu "libraries" do bibliotek Arduino, domyslnie znajdujacych sie w : C:\Users\"nazwa"\Documents\Arduino\libraries

Folder zawiera biblioteki graficzne napisane przez Adafruit edytowane pod uzyty wyswietlacz: https://www.waveshare.com/1.8inch-lcd-module.htm

Konfiguracja:
  - Adafruit_ST7735_EDIT.cpp - pod funkcja "void Adafruit_ST7735::initR" znajduja sie rozne opcje inicjalizacji w zaleznosci od wyswietlacza, jezeli zmienia sie sprzet trzeba podac rozmiar oraz offset ekranu
  - testdoprez.ino - na poczatku sa definiowane kolory oraz GPIO do SPI

Program wgrac na plytke z pliku "testdoprez.ino".
