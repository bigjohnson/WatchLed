/*
  Watchguard Firebox 500/750/1000 led boards controlled by Arduino
  see docs and pinout at
  https://github.com/bigjohnson/WatchLed
*/

#define ON LOW
#define OFF HIGH

#define DELAY 100

// Arduino's port to Watchguard leds board

#define CLK1 A0
#define CLK2 A1
#define CLK3 A2
#define CLK4 A3
#define CLK5 A4
#define OE A5 // could be connect to ground

#define D1 2
#define D2 3
#define D3 4
#define D4 5
#define D5 6
#define D6 7
#define D7 8
#define D8 9

// End hardware setup

// Port led description
// the port is the decine

#define Disarm 0
#define Armed 1
#define Sys_A 2
#define Sys_B 3

#define TtoEallowT 10
#define TtoEallowB 11
#define OtoEallowB 12
#define OtoEallowT 13
#define E 14
#define T 15
#define O 16

#define EtoTallowT 20
#define EtoTallowB 21
#define OtoTallowR 22
#define OtoTallowL 23
#define TtoOallowL 24
#define TtoOallowR 25
#define EtoOallowT 26
#define EtoOallowB 27

unsigned char allLed[] = { Disarm, Armed, Sys_A, Sys_B, \
                           TtoEallowT, TtoEallowB, OtoEallowB, OtoEallowT, E, T, O, \
                           EtoTallowT, EtoTallowB, OtoTallowR, OtoTallowL, TtoOallowL, TtoOallowR, EtoOallowT, EtoOallowB
                         };

unsigned char allLedSize = sizeof(allLed);
unsigned char allLedCount = 0;

#define TRAFFIC CLK4
#define LOAD CLK5

#define PORTS 3
#define LEDS 8
boolean ports[PORTS][LEDS];

unsigned char contatore = 0;
char segno = 1;

void setup() {
  // put your setup code here, to run once:

  digitalWrite(OE, LOW);
  pinMode(OE, OUTPUT);

  for (unsigned char i = D1; i <= D8; i++) {
    digitalWrite(i, OFF);
    pinMode(i, OUTPUT);
  }
  for (unsigned char i = CLK1; i <= CLK5; i++) {
    digitalWrite(i, HIGH);
    pinMode(i, OUTPUT);
  }
  for (unsigned char i = CLK1; i <= CLK5; i++) {
    digitalWrite(i, LOW);
  }

  for ( unsigned char pi = 0; pi < PORTS ; pi++) {
    for ( unsigned char li = 0; li < LEDS ; li++) {
      ports[pi][li] = OFF;
    }
  }

}

void loop() {
  allLedCount = allLedCount + 1;
  if (allLedCount > allLedSize) {
    allLedCount = 1;
  }

  writeLed(allLed[allLedCount - 1], ON);
  //writeLed( TtoEallowT , ON);
  delay(DELAY);
  //digitalWrite(D8, OFF);
  //clock(DELAY);
  writeLed(allLed[allLedCount - 1], OFF);
  //writeLed( TtoEallowT , OFF);
  delay(DELAY);
  contatore = contatore + segno;
  writeValue(TRAFFIC, contatore);
  writeValue(LOAD, 8 - contatore);
  if ( contatore == 8 || contatore == 0) {
    segno = segno * -1;
  }
}

// function for power on/off named led
void writeLed( unsigned char led, boolean state) {
  unsigned char port = led / 10;
  led = led - ( port * 10 );
  ports[port][led] = state;
  for (unsigned char e = 0; e < LEDS; e++) {
    digitalWrite(e + D1, ports[port][e]);
  }
  clock(port + CLK1);
}

// function for the two TRAFFIC and LOAD bars write a number to 0 (all off) to 8 (all on)
void writeValue( unsigned char port, unsigned char value ) {
  for ( unsigned char i = 1 ; i <= 8; i++) {
    if ( value >= i ) {
      digitalWrite((i - 1) + D1, ON);
    } else {
      digitalWrite((i - 1) + D1, OFF);
    }
  }
  clock(port);
}

void clock(unsigned char chip) {
  digitalWrite(chip, LOW);
  digitalWrite(chip, HIGH);
}
