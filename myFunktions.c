#include <avr/io.h>
#include <string.h>
#include <stdlib.h>

#include "uart.h"
#include "adc.h"
#include "global.h"
#include "zyklus.h"
#include "myFunktions.h"
#include "EinAusgabeUSB.h"
#include "df.h"
#include "cc-lib.h"

int16_t histerese=0;
int8_t ereignisBremsen=0;
int16_t pReglerServoRechts(void);
// <== Eigene Funktion und Bedingungen formulieren / schreiben
void fahren1(void){

//Variablen
const int wertvorne = 45;
int modus;


//Auswahl der Fahrtrichtung

if(abstandvorne <= wertvorne){
	
		//R�ckw�rts
		
		fahr(-18);
		
		//Modus Wahl
		
		modus = 1;
		
		
	}
	else{
		
		//Vorw�rts
		
		fahr(18);
		
		//Modus Wahl
		
		modus = 2;
		
	}
ausgabe2 = modus;
//Verhalten R�ckw�rts      //Anmerkung Hytserrese beim R�ckw�rtsfahren ? Verhalten ?

if( modus == 1){

	if( (getFahr() < 0) && (abstandlinks <= abstandrechts)){
	
		servo(10);
	
	}else{
	
	
		servo(-10);
	
	}

}




//Verhalten Vorw�rts Linksori.
/*
if( modus == 2){

	//Zu weit Links
	if((getFahr() > 0) && (abstandlinks < sollwert)){
	

	pReglerServoRechts();
	
	//Einschlagwinkel
	servo(y);
	
	
	}else if((getFahr() > 0) && (abstandlinks > sollwert)){		//zu weit rechts
	
	
	pReglerServoRechts();	
	
	//Einschlagwinkel
	servo(y);
	
	}


}

*/

//Verhalten Vorw�rts Mittigori.

if( modus == 2){

	//Zu weit Links oder rechts
	if((getFahr() > 0) && (abstandlinks < 160) && (abstandrechts< 160)){
	
	
	pReglerServoRechts();
	
	//Einschlagwinkel
	if((y <= 10) && (y >= -10)){
	
	servo(y);
	
	}else{
	
		if( y > 10){
		
		servo(10);
		
		}else{
		
		servo(-10);
		
		}
	
	
	}
	
	//Refernzwert Abfrage
	
	}else if((abstandlinks > 160) && (abstandrechts > 160)){
	
	
		
		servo(0);
	
	
	
	}else if((abstandlinks > 160) || (abstandrechts > 160)){
	
	
		if(abstandlinks > 160){
		
		servo(10);
		
		
		}else{
		
		
		servo(-10);
		
		
		}
	
	}

}










}

void fahren2(void){

}



uint16_t  linearisierungAD(uint16_t analogwert, uint8_t cosAlpha){
		//Die Funktion linearisiert den Analogwert mittels einer Hyperbel 2. Ordnung
		//Bei schr�ger Projektion auf ein Wand kann der reale Abstand
		// durch ein Multiplikation mit dem cosinusAlpha bestimmt werden.
		// die Variable cosAlpha entspricht dem cosinusAlpha X 100, d.h. 0� = 100, 45� = 70

		//Grenzwert festlegen!!
		//(analogwert-B) darf nicht 0 oder negativ werden!!
		// guter 
		
		
		
		uint16_t  abstand = 0;		//Variabel erzeugen und initialisieren 
		
		if((analogwert - B) > 0){
			
			abstand = A / (analogwert - B);   //bisheriger Cm
			abstand = abstand * cosAlpha;
				
		}else{
			
			abstand = 150;
		
		}
		
		abstand = abstand / 100; 	// 
		
		return abstand;				// Ergebnis zur�ckliefern,
}

int16_t ro(void){					//Kurzform von int16_t pReglerServoRechts(void)

	return y;
}

int16_t pReglerServoRechts(void){
	//ausrichten an der rechten Wand mit P-Regler
	//Funktion y(e) = me + b   z

	//==>  Variable sind in der global.h u. global.c definiert!  <====
	//int16_t m1=67;			//float Operation vermeiden
	//int16_t m2=100;			//Divisor Steigung
	//mit m=m1/m2=0,67, bei +- 15cm vom Sollwert

	//int16_t b=0;				//Durchbruch durch die Y-Achse
	//int16_t e;				//e=Regelabweichung in cm
	//int16_t sollwert = 35;//Sollwert 45 cm
	
	//int16_t y;				//y=Stellgroe�e / Winkel
	
	//bestimmen der Regelabweichung
	//z.B. Sollwert greade (35cm),	20cm volllinks, 50cm vollrechts
	
	e = abstandrechts - abstandlinks;				//Problem oben auf mitte ausgrichtet hier auf rechts fahren 35 cm in global festgelegt
	
		y=(m1*e) / m2;
	
	
			//Normal y=(m1*e) /(m2+b) aber b = 0
	
	ausgabe3 = y;
	return y;
}


void akkuSpannungPruefen(uint16_t schwellWert){
//Pr�fe die AkkuSpannung nur wenn das CrazyCar nicht f�hrt! 
// Alle LEDs blicken, wenn Akku-Spannung < schwellwert !!

}

void ledSchalterTest(void){

}

//Zur Ausgabe eingener Daten die Variable int16_t ausgabe1, ausgabe2, 
//ausgabe3, char ausgabeT[MAXTEXTA]; verwenden,
//bei den Variablen int16_t h1,h2,h3 erfolgt eine Mittelwertbildung �ber 500ms

//-------------------------------------------------------------------------

