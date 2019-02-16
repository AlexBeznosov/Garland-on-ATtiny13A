#define F_CPU 9600000 //fuse LOW=0x7a
#include <avr/interrupt.h>
#include <util/delay.h>

uint8_t counter = 0
	  ;
uint8_t lev_r
      , lev_g
	  , lev_b
	  ;
uint8_t buf_lev_r
      , buf_lev_g
	  , buf_lev_b
	  ;

ISR ( TIM0_OVF_vect ) {  //обработка прерывания по переполнению таймера

	if ( ++counter == 0 ) { //счетчик перехода таймера через ноль
		buf_lev_r = lev_r; //значения длительности ШИМ
		buf_lev_g = lev_g; //значения длительности ШИМ
		buf_lev_b = lev_b; //значения длительности ШИМ
		PORTB |= (1<<PB0)|(1<<PB1)|(1<<PB2);
	}
	
	if ( counter == buf_lev_r ) PORTB &= ~(1<<PB1); //подаем 0 на канал
	if ( counter == buf_lev_g ) PORTB &= ~(1<<PB2); //по достижении
	if ( counter == buf_lev_b ) PORTB &= ~(1<<PB0); //заданной длительности.
	
}

int main(void) {
	DDRB  |= (1<<PB0)|(1<<PB1)|(1<<PB2);     // установка PortB пины 0,1,2 выходы
	// таймер работает без предделителя
	TCCR0B = _BV(CS00);
	// прерывания по переполнению таймера разрешены
	TIMSK0 = _BV(TOIE0);
	// глобально прерывания так же разрешены
	sei();
	
	while (1) {
		for (int mode = 0; mode<= 6; mode++ ) {			
			flight_f(mode);		
			flight_b(mode);		
		}	
	}
}

void flight_f(int sel) {
	for (int i = 0; i <= 255; i++) {
		mode_work(sel, i);
		_delay_ms(10);
	}
}

void flight_b(int sel) {
	for (int i = 255; i >= 0; i--) {
		mode_work(sel, i);
		_delay_ms(10);
	}
}

void mode_work(int sel,int i) {
	switch ( sel ) {
		case 0:
		rgb(i, 0, 0);
		break;
		case 1:
		rgb(0, i, 0);
		break;
		case 2:
		rgb(0, 0, i);
		break;
		case 3:
		if ( i <= 85 ) {
			rgb(i, i, i);
		}
		break;
		case 4:
		if ( i <= 127 ) {
			rgb(i, i, 0);
		}
		break;
		case 5:
		if ( i <= 127 ) {
			rgb(0, i, i);
		}
		break;
		case 6:
		if ( i <= 127 ) {
			rgb(i, 0, i);
		}
		break;
	}
}

void rgb(int r, int g, int b) {
  lev_r = r; //Красный цвет
  lev_g = g; //Зеленый цвет
  lev_b = b; //Синий цвет
}
