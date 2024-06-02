/*Werner Jessen + Dieter Rathmann */
/*Version: CrazyCar-V1.39 */
/*Pr�fung der AkkuSpannung, neue USB-Ausgabe, ledSchalterTest() */
/* nicht mehr ben�tigte Programmteile entfernt */
/* Historie wird in EERAM geschrieben, neue Kommandos */
/* Mako's fuer Outputs, FR+Servo */
/* Augabe individueller Daten mit (int16_t) h1,h2,h3 incl. Mittelwertbildung 
	Einschleusung von Daten via Kommando mya, myb, myc an die 
	Variable (int16_t) myInt1,myInt2, myInt3 , zB. mya65 myInt1=65 */

#include <avr/io.h>

#include "global.h"
#include "pwm.h"
#include "adc.h"
#include "uart.h"
#include "zyklus.h"
#include "df.h"
#include "EinAusgabeUSB.h"
#include "myFunktions.h"
#include "cc-lib.h"

void autonomFahren(void);

int main (void)
{
	initMikroController();     //Initialisierung des Mikrocontrollers
	//while(1) { testServo(); }//<== Initialisierung  Servo, 1X <==========

	initServo(912, 808, 696);  //<== Volllinks, Gerade, Vollrechts eintragen, Servo, 1X <==========
	initFahr(30);					// klein anfangen, bis Fahrzeug sicher faehrt

	servo(0);						//Ansteuerung des Servos (GERADEAUS)
	fahr(0);      					//Ansteuerung des Fahrtenreglers (STOP)
	
	//Betriebsmod lesen und speichern, Schalter s1() u. s2()
   mod = s1() +  2*s2(); //0=Terminalbetrieb, 1=fahren1(), 2=fahren2(), 3=fahren3()

	if( s4() == 1 ){				// Datenspeichern und Fahrzeug anhalten nach x Sekunden
		ledPB1(1);					//LED PB1 an
		historyDepthPointer=0; 	//neue Datenaufzeichung
		hspStart=1;					// beginne Speicherung bein naechsten Block
	}

//Endlosschleife mit freier Zykluszeit
	while(1) {

	//Sensordaten einlesen
		analogwertL=adc(0);			// lese Sensor links
		analogwertR=adc(1);			// lese Sensor rechts
		analogwertV=adc(2);			// lese Sensor vorne
	//	analogwertI=adc(3);			// Sensor Strom
	//	analogwertU=adc(4);			// Sensor Spannung
		analogwertAkku=adc(7);		// lese AkkuSpannung

//hier erfolgt die Umrechnung der digitalen Abstandswerte
// in den Abstandswert in cm (abstandlinks,abstandrechts,abstandvorne)
// SensorWinkel... als globale Vatiable in global.h definiert
		abstandlinks  = linearisierungAD(analogwertL, SensorWinkelLinks); //<== Eigene Funktion schreiben
		abstandrechts = linearisierungAD(analogwertR, SensorWinkelRechts); //analogwert, cosinusAlpha*100
		abstandvorne  = linearisierungAD(analogwertV, SensorWinkelGeradeaus);
//		akkuSpannungPruefen(mya);  	// Ist die AkkuSpannung unter dem Grenzwert?

//konstante zeitliche Abarbeitung des Steuerzyklusses
//Abtastzeit wird im Hintergrund mit Timerinterrupt erh�ht 
//ABTASTZEIT ist in gobal.h definiert, siehe zyklus.c
		if (abtastzeit>=ABTASTZEIT)
		{
			abtastzeit=0;             		//abtastzeit auf 0 zur�cksetzen
			datenSatzZaehler++;				// Anzahl der digitalisierten Analogwerte, pro Runde +1
			
			ledSchalterTest();				//<== Eigene Funktion schreiben in myFunktions.c

			autonomFahren();					// eigene Funktion in myFunktions.c: fahren1(), fahren2(), ...

			//Wenn s4 gesetzt ist oder Kommando hs, dann fahre solange bis der Datenspeicher voll ist.
			//Dient zur Auswertung der Strecke (Optimierung)
			if (historyDepthPointer >= hisDepth){	// Datenspeicher voll?
				fahr(0);      	  							// Fahrtenregler: Stop
				mod = 0;										// nur Terminal Ein- u. Ausgabe
			}
			datenspeichern();								// LiveDaten zur Analyse verdichten	
			terminal_aktiv();
			
			if (datenSatzZaehler >= 65000) 
				datenSatzZaehler = 0;		//Starte neu, wenn die Logzeit > 650 Sekunden
		}//endzyklus
	}//endfor
	return (0);
}//endmain


void autonomFahren(void){

	//mod =  Schalterauswertung von s1() und s2() (kann mit einem Kommando �berschrieben werden)
	switch(mod){
		case	0:  	ledPC3(0); ledPC2(0); //nur Terminal Ein- u. Ausgabe
						break;

		case	1:  	ledPC3(1); ledPC2(0); //Fahrzeug faehrt via Software
						fahren1();
						break;

		case	2:  	ledPC3(0); ledPC2(1); //Fahrzeug faehrt via Software
						fahren2();
						break;

		case	9:  	ledPC2_flash(100); ledPC3_flash(100,0);	 //Fahrzeug Stopp
						fahr(0);
						hspStart=0;					//keine Datenaufzeichung ins EEPROM						
						abtastzeitMAX=0;			//Zyklusmessung zur�cksetzen
						historyDepthPointer=0; 	//neue Datenaufzeichung
						break;

		default: 	ledPC2_flash(30); ledPC3_flash(30,1); // nichts definiert
						break;
	}
}