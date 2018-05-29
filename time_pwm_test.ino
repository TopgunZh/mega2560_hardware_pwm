struct time_param_t {
  void (*time_callback)();
  volatile unsigned int tcnt;

  unsigned short pwm_period;
  unsigned char clock_select_bits;
};

struct time_param_t time2 = {0};
struct time_param_t time3 = {0};
struct time_param_t time4 = {0};
struct time_param_t time5 = {0};


#define TIMER4_RESOLUTION 65536UL

void time4_set_period(unsigned long period_us){
   /********************** set period **************************/
  const unsigned long cycles = (F_CPU / 2000000) * period_us;
  if (cycles < TIMER4_RESOLUTION) {
    time4.clock_select_bits = _BV(CS40);
    time4.pwm_period = cycles;
  } else
  if (cycles < TIMER4_RESOLUTION * 8) {
    time4.clock_select_bits = _BV(CS41);
    time4.pwm_period = cycles / 8;
  } else
  if (cycles < TIMER4_RESOLUTION * 64) {
    time4.clock_select_bits = _BV(CS41) | _BV(CS40);
    time4.pwm_period = cycles / 64;
  } else
  if (cycles < TIMER4_RESOLUTION * 256) {
    time4.clock_select_bits = _BV(CS42);
    time4.pwm_period = cycles / 256;
  } else
  if (cycles < TIMER4_RESOLUTION * 1024) {
    time4.clock_select_bits = _BV(CS42) | _BV(CS40);
    time4.pwm_period = cycles / 1024;
  } else {
    time4.clock_select_bits = _BV(CS42) | _BV(CS40);
    time4.pwm_period = TIMER4_RESOLUTION - 1;
  }
  ICR4 = time4.pwm_period;
  TCCR4B = _BV(WGM43) | time4.clock_select_bits;    
}

void time4_set_duty(char pin, unsigned int duty){
  
  /******************* set pwm pin ***************************/
  unsigned long dutyCycle = time4.pwm_period;
  dutyCycle *= duty;
  dutyCycle >>= 10;
  switch( pin ){
    case 3:
                DDRH  |= 1<<3;
                TCCR4A |= _BV(COM4A1);
                OCR4A = dutyCycle;
      break;
    case 4:
                DDRH  |= 1<<4;
                TCCR4A |= _BV(COM4B1);
                OCR4B = dutyCycle;
      break;
    case 5:
                DDRH  |= 1<<5;
                TCCR4A |= _BV(COM4C1);
                OCR4C = dutyCycle;
      break;
  }

  TCCR4B = _BV(WGM43) | time4.clock_select_bits;
  
}

void time4_pwm_init(unsigned long period_us){
  TCCR4B = _BV(WGM43);        // set mode as phase and frequency correct pwm, stop the timer
  TCCR4A = 0;                 // clear control register A 
  time4_set_period(period_us);  
}





const int fanPin = 6;
void setup() {
  // put your setup code here, to run once:
  time4_pwm_init(20000);
  time4_set_duty( 3, (2.7/21.0) * 1023 );


/*  for (float dutyCycle = 30.0; dutyCycle < 100.0; dutyCycle++) {
    Timer3.pwm(fanPin, (dutyCycle / 100) * 1023);
    delay(500);
  }*/
}

void loop() {
  // put your main code here, to run repeatedly:

}
