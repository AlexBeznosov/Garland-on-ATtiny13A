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

ISR ( TIM0_OVF_vect ) {  //��������� ���������� �� ������������ �������

	if ( ++counter == 0 ) { //������� �������� ������� ����� ����
		buf_lev_r = lev_r; //�������� ������������ ���
		buf_lev_g = lev_g; //�������� ������������ ���
		buf_lev_b = lev_b; //�������� ������������ ���
		PORTB |= (1<<PB0)|(1<<PB1)|(1<<PB2);
	}
	
	if ( counter == buf_lev_r ) PORTB &= ~(1<<PB1); //������ 0 �� �����
	if ( counter == buf_lev_g ) PORTB &= ~(1<<PB2); //�� ����������
	if ( counter == buf_lev_b ) PORTB &= ~(1<<PB0); //�������� ������������.
	
}

int main(void) {
	DDRB  |= (1<<PB0)|(1<<PB1)|(1<<PB2);     // ��������� PortB ���� 0,1,2 ������
	// ������ �������� ��� ������������
	TCCR0B = _BV(CS00);
	// ���������� �� ������������ ������� ���������
	TIMSK0 = _BV(TOIE0);
	// ��������� ���������� ��� �� ���������
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
  lev_r = r; //������� ����
  lev_g = g; //������� ����
  lev_b = b; //����� ����
}
