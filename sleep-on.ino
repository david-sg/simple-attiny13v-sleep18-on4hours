
/*
 *  sleep w/wdt wakeup t13
 */
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/wdt.h>

const uint8_t LED_PIN = PB0;

// watchdog timeouts
const uint8_t WD_1s = 6;
const uint8_t WD_2s = 7;
const uint8_t WD_4s = 32;
const uint8_t WD_8s = 33;

// watchdog timeout interrupt
ISR( WDT_vect )
  {
    wdt_disable(); 
  }
 
/*
 * configure wdt timeout and interrupt clear WDE (reset)
 * configure sleep mode and put to sleep
 */
void wdtSleep ( uint8_t timeout )
  {
    MCUSR &= ~( _BV( WDRF ));             // clear WDRF (if set)   
    cli();
    WDTCR  = ( _BV( WDCE )|_BV( WDE ) );  // enable wdt config
    WDTCR  = ( _BV( WDTIE )|timeout );    // config timeout and interrupt only, clear WDE
    wdt_reset();
    sei();
//    set_sleep_mode ( SLEEP_MODE_PWR_DOWN );
    sleep_mode();                         // sleep till wdt timeout interrupt
  }

void setup()
  {
    DDRB |= _BV( LED_PIN );
    set_sleep_mode ( SLEEP_MODE_PWR_DOWN );
  }  // setup

void loop()
  {

    PORTB |= _BV( LED_PIN ); // led on
       
    // sleeping with output on 2250 is 5 hours
    for ( uint16_t i = 0 ; i < 2250 ; ++i )
      {
        wdtSleep ( WD_8s ); // 1s sleep
      }
    PORTB &= ~(_BV( LED_PIN )); // led off
   
    // sleeping with output off 8100 is 18 hours
    for ( uint16_t i = 0 ; i < 8100 ; ++i )  // note I changed i to
      {
        wdtSleep ( WD_8s ); // 1s sleep
      }
     


     
  }  // loop
