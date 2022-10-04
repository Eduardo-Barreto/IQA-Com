#define button 3
#define led 13

// Pulse time = period * 64 * 0.0625us. (1/16Mhz = 0.0625us, prescaler = 64)
#define maxVelocity = 100  // 100 * 64 * 0.0625us = 400us
#define minVelocity = 1750 // 1750 * 64 * 0.0625us = 7000us

// 1/16Mhz = 0.0625us, prescaler = 64
// Pulse time = period * 64 * 0.0625us
int velocity = 100;
volatile int period = map(velocity, 1, 100, 1750, 100); // Pulse time = 1750 * 64 * 0.0625us = 7000us

volatile unsigned long stepCounter = 0;
volatile bool step = true;

void disableInterrups()
{
    TCCR1A = 0;
    TCCR1B = 0;
    TIMSK1 &= ~(1 << OCIE1A); // disable timer compare interrupt
}

void configureTimer1()
{
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1 = 0; // initialize counter value to 0

    TCCR1B |= (1 << WGM12);              // modo CTC
    TCCR1B |= (1 << CS11) | (1 << CS10); // prescaler de 64
    TIMSK1 |= (1 << OCIE1A);             // enable timer compare interrupt
    OCR1A = period - 1;
}

void setup()
{
    disableInterrups();

    pinMode(led, OUTPUT);
    pinMode(button, INPUT_PULLUP);
    digitalWrite(led, 1);
    while (digitalRead(button))
    {
    }
    digitalWrite(led, 0);
    configureTimer1();

    DDRD = B01110000; // D4, D5 e D6 outputs

    Serial.begin(9600);
}

ISR(TIMER1_COMPA_vect)
{
    if (!step)
        return;
    PORTD |= 0b00100000;
    PORTD &= !0b00100000;
    stepCounter++;
    OCR1A = period - 1;
}

void loop()
{
}