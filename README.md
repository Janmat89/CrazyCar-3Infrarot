
Basis Version CrazyCar-V1.35

CrazyCar-V1.39-a
	- der SensorWinkel wird als globale Vatiable in global.h definiert
		#define	 SensorWinkelLinks 70, SensorWinkelRechts 70, SensorWinkelGeradeaus 100
		Aenderung in global.h, CrazyCar-V1.39.c und EinAusgabeUSB.c
		
	- int16_t mya, myb, myc ImportHilfsVariable 
		Aenderung in global.h, global.c, EinAusgabeUSB.c
		
	//Zur Ausgabe eigener Daten im Terminal und EEPROM
	- int16_t ausgabe1, ausgabe2, ausgabe3; char ausgabeT[] 
		Aenderung in global.h, global.c, EinAusgabeUSB.c, df.c
			  In ausgabeUSB2(void){} Ausgabe von ausgabe1 bis ausgabe3 und ausgabeT[]
			  
	- in CrazyCar-V1.39.c, s4() freigegeben, akkuSpannungPruefen() auskommentiert
	- in CrazyCar-V1.39.c, s3() freigegeben, 
	- s4() jetzt für die Datenaufzeichung ins eeprom verwendet
	
	- in cc-lib.c, fahr(), alle Werte zwischen (>-18) && (< 18) werden auf STOP gesetzt
				//bei Werten <16 und > -16 besteht die Gefahr das sich das Fahrzeug nicht bewegt,
				//so dass ein Teil die Leistung im FR in Wärme umgesetzt wird
	
	- neue Variable "logOffAus" zur entzerrten Ausgabe des Logoff-Textes
		in EinAusgabeUSB.c neue Funktion void ausgabeUSBlogoff(void) 
		in eingabeUSB(void), neue Kommandos werden zur vollen Sekunde ausgewertet 
			if ( !(datenSatzZaehler % 100) )
		Log-Ausgabe aufgeräumt, Mod + Akku nur noch mit Log-Off
				
	- Laufzeitkontrolle:
		uint16_t abtastzeitZyklus:	max Aufenthaltsdauer im Steuerzyklusses in der letzen Sekunde
		uint16_t abtastzeitMAX:    max Aufenthaltsdauer im Steuerzyklusses seit Programmstart, 
											log: ZzMax
				
	- 	in eingabeUSB(void):
			Eingabe von negativen Werten für mya bis myc ermöglicht
			  
	- in testServo(void): die Ausgabe von freeRam() entfernt.	
	
	- cc-lib.c
		int8_t getServo(void); //liefert den aktuellen Servowert
		int8_t getFahr(void);  //liefert den aktuellen Wert vom Fahrtenregler		
		int8_t getServoM(int16_t wert_OCR1A); //bestimmt aus dem Parameter den Servowert
		int8_t getFahrM(int16_t wert_OCR1B);  //bestimmt aus dem Parameter den Wert vom Fahrtenregler
		
	- myFunktions.c : int16_t ro(void); 	  //statt int16_t pReglerServoRechts(void)
	
	- in CrazyCar-V1.39.c, akkuSpannungPruefen(800); jetzt in der Endlosschleife
		nur prüfen bei fahr(0)
	
		
	offen: mehrere Kommandos pro Eingabe		
