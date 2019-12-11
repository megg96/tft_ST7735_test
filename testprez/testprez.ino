//========================================DANE===============================================
#include <Adafruit_ST7735_EDIT.h> //biblioteka zedytowana pod odpowiedniego rozmiaru lcd
#include <Adafruit_GFX.h>         //biblioteka komend rysujacych

#define BLACK   0x0000            //definicja kolorow
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
#define GREY    0x2108

#define cs   10                   //definicja wyjsc GPIO
#define dc  9
#define rst  8

//extern uint8_t car[];           //testowa bitmapa(nieuzywane)


Adafruit_ST7735 screen = Adafruit_ST7735(cs, dc, rst);  //inicjalizacja lcd

int t = 50;                 //czas zwloki po odswiezeniu wyswietlacza, im mniejsza wartosc tym wieksze odswiezanie
int H = screen.width();     //wysokosc lcd
int W = screen.height();    //szerokosc lcd

int lastmapp = 0;           //poprzednia wartosc predkosci

int k = 0;                  //zmienna funkcji testowej

char buf[17];               //tablica uzyta do poprawnego wyswietlania wielkosci(funkcja sprintf)

int reading = 0; // Value to be displayed
int p = 0;
#define GREEN2RED 4
#define BLUE2BLUE 2
int xpos = 25, ypos = 20, gap = 100, radius = 50;

//=======================================SETUP==============================================
void setup() {
  
  Serial.begin(115200);     //inicjalizacja protu szeregowego
  screen.initR(EDIT);       //rozpoczeczie komunikacji z lcd
  screen.setRotation(1);    //obrocenie orientacji lcd
  
  screen.setFont();         //wybor fontu
  screen.setTextSize(7);    //wybor wielkosci fontu
  screen.setTextColor(WHITE, BLACK);  //ustawienie kolorow tekstu(i tla)
  delay(100);
  screen.fillScreen(BLACK); //wypelnienie calego ekranu kolorem
  screen.setCursor(0, 0);   //ustawienie kursora w wybrane miejsce

  //base();                   //wywolanie funkcji wyswietlajacej baze pomiarowa

}
//=======================================LOOP===============================================
void loop() {

  for(reading=0; reading<50; reading++){    //funkcja testowa(km/h)
  
    ringMeter(reading,0,50, xpos,ypos,radius,"Celsius",BLUE2BLUE); // Draw analogue meter
    delay(t);             //czas zwloki do kolejnego odwiezenia
  }
  
  for(reading=50; reading>0; --reading){    //funkcja testowa(km/h)

    ringMeter(reading,0,50, xpos,ypos,radius,"Celsius",BLUE2BLUE); // Draw analogue meter
    delay(t);             //czas zwloki do kolejnego odwiezenia
  }

}

//=======================================FUNKCJE=================================================
void base(){                 //definicja funkcji wyswietlajaca baze pomiarowa
  screen.fillScreen(BLACK);  //wyczyszczenie ekranu

  screen.drawLine(0, H/3.5, W, H/3.5, WHITE);   //podzielenie ekranu na sekcje
  screen.drawLine(0, H/3.5, W, H/3.5, WHITE);
  screen.drawLine(0, H/1.5, W, H/1.5, WHITE);
  
  screen.setTextSize(1);              //wyswietlanie predkosci
  screen.setTextColor(WHITE, BLACK);
  screen.setCursor((W/1.3), (H/2.5));
  screen.print("km/h");
  //screen.setTextSize(2);
  //screen.setCursor((W/1.9), (H/2.7));
  //screen.print("135");
  
  screen.setTextSize(1);              //wyswietlanie biegu
  screen.setCursor((W/7), (H/2));
  screen.print("GEAR");
  screen.setTextSize(2);
  screen.setCursor((W/5.5), (H/2.8));
  screen.print("4");

  screen.setTextSize(1);              //wyswietlanie temperatury
  screen.setCursor((W/7), (H/1.1));
  screen.print("OIL TEMP");
  screen.setCursor((W/2.5), (H/1.25));
  screen.print("C");
  screen.setTextSize(2);
  screen.setCursor((W/6), (H/1.3));
  screen.print("105");

  screen.setTextSize(1);              //wyswietlanie paliwa
  screen.setCursor((W/1.8), (H/1.1));
  screen.print("FUEL LEVEL");
  screen.setCursor((W/1.2), (H/1.25));
  screen.print("%");
  screen.setTextSize(2);
  screen.setCursor((W/1.5), (H/1.3));
  screen.print("67");
}

void v_bar(){                         //definicja funkcji paska predkosci
  int w = 13;                         //przerwa pomiedzy paskami
  int mapp = map(k, 0, 50, 0, 9);     //skalowanie danych
  int diff = mapp - lastmapp;         //roznica kolejnych pomiarow

  for(int i=0; i<=abs(diff); i++){    //funkcja sprawdzajaca ile paskow nalezy dorysowac/usunac
    if(diff>0){
      screen.fillRect((W/10) + w*lastmapp+1, H/15, 10, 25, WHITE);
    }
    else if(diff<0){
      screen.fillRect((W/10) + w*lastmapp+1, H/15, 10, 25, BLACK);
    }
  }
  
  sprintf(buf, "%3d", k);             //zapisanie predkosci jako spring
  screen.setTextColor(WHITE, BLACK);
  screen.setTextSize(2);
  screen.setCursor((W/1.9), (H/2.7));
  screen.print(buf);                  //wyswietlenie predkosci
  
  lastmapp = mapp;                    //przypisanie wartosci aktualnej jako poprzednia
}

void drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t color){    //definicja funkcji bitmapy, na razie nie uzywana

  int16_t i, j, byteWidth = (w + 7) / 8;
  uint8_t byte;

  for(j=0; j<h; j++) {
    for(i=0; i<w; i++) {
      if(i & 7) byte <<= 1;
      else      byte   = pgm_read_byte(bitmap + j * byteWidth + i / 8);
       if(byte & 0x80) screen.drawPixel(x+i, y+j, color);
    }
  }
}

// #########################################################################
//  Draw the meter on the screen, returns x coord of righthand side
// #########################################################################
int ringMeter(int value, int vmin, int vmax, int x, int y, int r, char *units, byte scheme)
{
  // Minimum value of r is about 52 before value text intrudes on ring
  // drawing the text first is an option
  
  x += r; y += r;   // Calculate coords of centre of ring
  int w = r / 3;    // Width of outer ring is 1/4 of radius 
  int angle = 150;  // Half the sweep angle of meter (300 degrees)
  int v = map(value, vmin, vmax, -angle, angle); // Map the value to an angle v
  byte seg = 8; // Segments are 3 degrees wide = 100 segments for 300 degrees
  byte inc = 12; // Draw segments every 3 degrees, increase to 6 for segmented ring
  // Variable to save "value" text colour from scheme and set default
  int colour = BLUE;
 
  // Draw colour blocks every inc degrees
  for (int i = -angle+inc/2; i < angle-inc/2; i += inc) {
    // Calculate pair of coordinates for segment start
    float sx = cos((i - 90) * 0.0174532925);
    float sy = sin((i - 90) * 0.0174532925);
    uint16_t x0 = sx * (r - w) + x;
    uint16_t y0 = sy * (r - w) + y;
    uint16_t x1 = sx * r + x;
    uint16_t y1 = sy * r + y;

    // Calculate pair of coordinates for segment end
    float sx2 = cos((i + seg - 90) * 0.0174532925);
    float sy2 = sin((i + seg - 90) * 0.0174532925);
    int x2 = sx2 * (r - w) + x;
    int y2 = sy2 * (r - w) + y;
    int x3 = sx2 * r + x;
    int y3 = sy2 * r + y;

    if (i < v) { // Fill in coloured segments with 2 triangles
      switch (scheme) {
        case 0: colour = RED; break; // Fixed colour
        case 1: colour = GREEN; break; // Fixed colour
        case 2: colour = BLUE; break; // Fixed colour
        case 3: colour = rainbow(map(i, -angle, angle, 0, 127)); break; // Full spectrum blue to red
        case 4: colour = rainbow(map(i, -angle, angle, 70, 127)); break; // Green to red (high temperature etc)
        case 5: colour = rainbow(map(i, -angle, angle, 127, 63)); break; // Red to green (low battery etc)
        default: colour = BLUE; break; // Fixed colour
      }
      screen.fillTriangle(x0, y0, x1, y1, x2, y2, colour);
      screen.fillTriangle(x1, y1, x2, y2, x3, y3, colour);
      //text_colour = colour; // Save the last colour drawn
    }
    else // Fill in blank segments
    {
      screen.fillTriangle(x0, y0, x1, y1, x2, y2, GREY);
      screen.fillTriangle(x1, y1, x2, y2, x3, y3, GREY);
    }
  }
  // Convert value to a string
  char buf[10];
  byte len = 2; if (value > 999) len = 4;
  dtostrf(value, len, 0, buf);
  buf[len] = ' '; buf[len] = 0; // Add blanking space and terminator, helps to centre text too!
  // Set the text colour to default
  screen.setTextSize(1);

  if(value>9){
    screen.setTextColor(colour,BLACK);
    screen.setCursor(x-25,y-10);
    screen.setTextSize(1);
    screen.print(buf);
  }
  if(value<10){
    screen.setTextColor(colour,BLACK);
    screen.setCursor(x-25,y-10);
    screen.setTextSize(1);
    screen.print(buf);
  }

    screen.setTextColor(WHITE,BLACK);
  
    screen.setCursor(x-39,y+75);
    screen.setTextSize(1);
    //screen.print(units); // Units display
  
  // Calculate and return right hand side x coordinate
  return x + r;
}

unsigned int rainbow(byte value)
{
  // Value is expected to be in range 0-127
  // The value is converted to a spectrum colour from 0 = blue through to 127 = red

  byte red = 0; // Red is the top 5 bits of a 16 bit colour value
  byte green = 0;// Green is the middle 6 bits
  byte blue = 0; // Blue is the bottom 5 bits
  byte quadrant = value / 32;

  if (quadrant == 0) {
    blue = 31;
    green = 2 * (value % 32);
    red = 0;
  }
  if (quadrant == 1) {
    blue = 31 - (value % 32);
    green = 63;
    red = 0;
  }
  if (quadrant == 2) {
    blue = 0;
    green = 63;
    red = value % 32;
  }
  if (quadrant == 3) {
    blue = 0;
    green = 63 - 2 * (value % 32);
    red = 31;
  }
  return (red << 11) + (green << 5) + blue;
}
