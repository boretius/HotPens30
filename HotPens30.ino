/*************************************************************
HotPens 2021 September
Idee: Hubert Schmucker
Design: und Umsetzung: Michael Boretius
unter Zuhilfenahme von Code aus "Maiskolben"
Der Code läuft auf der Platine HotPens Rev-2.0 Juni 2021
**************************************************************/
#include <gfxfont.h>
#include <Adafruit_SPITFT_Macros.h>
#include <Adafruit_SPITFT.h>
#include <Adafruit_GrayOLED.h>
#include <Adafruit_GFX.h>
#include <EEPROM.h>
#include <qdec.h>
#include <PID_v1.h>
#include <EveryTimerB.h>
#include <Adafruit_SPIDevice.h>
#include <Adafruit_I2CRegister.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_BusIO_Register.h>
#include <Adafruit_ILI9341.h>
#include <XPT2046_Touchscreen.h>
#include "definitions.h"
#include "Functions.h"
#include "Display.h"
/*
 * Only used for testing, do not use.
 */
//#define INSTALL 
//#define TEST_ADC24
//#define TEST_ADC12
#define TEST_ADC
#define Timer1 TimerB2 // use TimerB2 as a drop in replacement for Timer1


void setup(void) {
	cnt_compute2 = TIME_COMPUTE_IN_MS;// / 2;	// Startwert
	pinMode(TIRQ_PIN, INPUT_PULLUP);
	pinMode(T_CS_PIN, OUTPUT);	// temp fuer Touch
	digitalWrite(HEATER_PWM, LOW);
	pinMode(HEATER_PWM, OUTPUT);
	digitalWrite(HEATER_PWM2, LOW);
	pinMode(HEATER_PWM2, OUTPUT); // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	pinMode(TEMP_SENSE, INPUT);
	pinMode(TEMP_SENSE2, INPUT); // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	pinMode(TASTER_PIN, INPUT_PULLUP);
	pinMode(HELLIGKEIT, OUTPUT);
	pinMode(TFT_CS, OUTPUT);
	/*pinMode(Relais, OUTPUT);
	pinMode(LED, OUTPUT);
	pinMode(LED2, OUTPUT);*/
	digitalWrite(TFT_CS, HIGH);
	digitalWrite(HELLIGKEIT, LOW);
	Serial.begin(115200);
	qdec.begin();
	ts.begin(); // Initialisiert den Touch-Interrupt
	ts.setRotation(3); // deflt Nullpkt links oben, Y nach unten, x nach rechts
	tft.begin();

	//TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT
	Timer1.initialize();
	Timer1.attachInterrupt(timer_isr);
	Timer1.setPeriod(1000);		// 1000 µsec
	attachInterrupt(digitalPinToInterrupt(TASTER_PIN), Taster, FALLING);
	attachInterrupt(digitalPinToInterrupt(ROTARY_PIN_A), IsrForQDEC, CHANGE);
	attachInterrupt(digitalPinToInterrupt(ROTARY_PIN_B), IsrForQDEC, CHANGE);
	//TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT

	/***********************************************************************
		  INIT
	************************************************************************/
	tft.fillScreen(BLACK);
		/****************************************************************************
					Startbildschirm und Setup
		****************************************************************************/
		tft.setRotation(3);
		tft.fillScreen(BLACK);
		tft.setTextSize(2);
		tft.setCursor(0, 0);
		tft.setTextColor(WHITE);
		tft.setCursor(20, 86);
		tft.setTextColor(YELLOW);
		tft.setTextSize(2);
		tft.print(MELDESTRING);
		tft.print(VERSION);
		tft.setCursor(35, 104);
		tft.print("Welcome!");
		/*************************************************************************
					Grundinitialisierung Drücken Drehreglerknopf
		**************************************************************************/
		// lese "Meldestring" vom EEPROM bei diff: init sonst weiter (gesetzt bei init)
		for (long i = 0; i < 0xfffff; i++)
		{
			if (!digitalRead(TASTER_PIN))
			{
				tft.fillScreen(BLACK);
				//EEPROM.update(EEPROM_CHECK, EE_CHECK);
				Erstinit();		// default Gain und Offset in EEPROM schreiben
				updateEEPROM();
				tft.setCursor(35, 104);
				tft.print("Init done");
				for (int16_t j = 0; j < 12300; j++)
				{
					//random();
				}
				break;
			}
		}
		//interrupts();
		/***********************************************************************
					Neuer NANO?
		************************************************************************/
		tft.fillScreen(BLACK);
		tft.setCursor(35, 104);
		int j;
		Serial.print(Meldung);
		Serial.println("");
		for (int i = EEPROM_VERSION; i < BASIS; i++)
		{
			if (Meldung[i-EEPROM_VERSION] != EEPROM.read(i)) break;
			j=i;
		}
		if (j!= BASIS-1) {				// wennEEPROM leer oder alt
			Erstinit();				// default KAL-werte in EEPROM 
			updateEEPROM();
			tft.print("Init done");
			delay(1000);
		}
		else						// laden der Konfigwerte aus EEPROM
		{
			/********************************************************************************
			lesen der Konfigwerte aus EEPROM in den Arbeitsbereich sofern dieser gültig ist
			ansonsten nutzen der Defaultwerte
			********************************************************************************/
			stby = EEPROM.read(EEPROM_STBY);
			stby2 = EEPROM.read(EEPROM_STBY2);
			W40 = EEPROM.read(EEPROM_W40);
			regelChr12slow = EEPROM.read(EEPROM_12SF);
			regelChr24slow = EEPROM.read(EEPROM_24SF);
			Dimmerspeicher = EEPROM.read(EEPROM_Dimmerspeicher);
			Kal40_IDX = EEPROM.read(EEPROM_KAL4_IDX); // Zugriff auf Kal-Daten von vier Spitzen
			Kal150_IDX = EEPROM.read(EEPROM_KAL5_IDX); // Zugriff auf Kal-Daten von vier Spitzen
			for (uint8_t i = 0; i < 4; i++) {			// BASIS + 16 bytes
				stored[i] = EEPROM.read(BASIS + i * 2) << 8;
				stored[i] |= EEPROM.read(BASIS + 1 + i * 2);
				stored2[i] = EEPROM.read(BASIS + 8 + i * 2) << 8;
				stored2[i] |= EEPROM.read(BASIS + 9 + i * 2);
			}
			HolDa(Kal40_IDX, true);
			HolDa(Kal150_IDX, false);
			switch (Kal40_IDX)
			{
			case(1):
				Kal40o = true;
				Kal40l = false;
				break;
			case(2):
				Kal40o = false;
				Kal40l = true;
				break;
			case(3):
				Kal40o = false;
				Kal40l = false;
				break;
			default:
				Kal40o = true;
				Kal40l = true;
				break;
			}
			switch (Kal150_IDX)
			{
			case(1):
				Kal150o = true;
				Kal150l = false;
				break;
			case(2):
				Kal150o = false;
				Kal150l = true;
				break;
			case(3):
				Kal150o = false;
				Kal150l = false;
				break;
			default:
				Kal150o = true;
				Kal150l = true;
				break;
			}

			set_t = stored[0];
			set_t2 = stored2[0];
			//tft.print("Werte geladen");
			//delay(1000);
		}
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	analogWrite(6, Dimmerspeicher * 10);
	last_measured = getTemp12V();
	last_measured2 = getTemp24V();
	heaterPID.SetMode(AUTOMATIC);
	heaterPID2.SetMode(AUTOMATIC);
	sendNext = millis();
	Strom = true;
	drawHmenu();
	aktivB();	// aktiver Button
}
/************************************************************************************************


+++++++++++++++++++++++++++++++++++++  Ende Setup ++++++++++++++++++++++++++++++++++++++++++++++++


************************************************************************************************/
//111111111111111111111111111111111111111111111111111111111111111111111111111111
//  INTERRUPT SERVICE  timer_isr
//111111111111111111111111111111111111111111111111111111111111111111111111111111
void timer_isr(void) {
	if (cnt_compute >= TIME_COMPUTE_IN_MS) {
		analogWrite(HEATER_PWM, 0); //switch off heater to let the low pass settle
		analogWrite(HEATER_PWM2, 0); //switch off heater to let the low pass settle
		if (cnt_compute >= TIME_COMPUTE_IN_MS + DELAY_BEFORE_MEASURE) {
			analogRead(TEMP_SENSE);//Switch ADC MUX
			analogRead(TEMP_SENSE2);//Switch ADC MUX
			adcValue = adcValue + analogRead(TEMP_SENSE);
			adcValue2 = adcValue2 + analogRead(TEMP_SENSE2);
			if (cnt_compute >= TIME_COMPUTE_IN_MS + DELAY_BEFORE_MEASURE + MESSREIHEN)
			{
				cnt_compute = 0;
				compute12V();
				compute24V();
				analogWrite(HEATER_PWM, pwm);
				analogWrite(HEATER_PWM2, pwm2);
				//Trigger(); // für Signalverfolgung am Oska
			}
		}
	}
	cnt_compute++;
}
/*#######################################################################
 



					Menüsteuerung





//#######################################################################

/************************************************************************************************
*
*			Haupt menü
*
*************************************************************************************************/

void HauptFunktion(uint8_t Taste) {
	// Taste entspricht "aktives_Feld" von Touch-Int
	switch (Taste)
	{
	case (11):	//  ------------  ZEILE 1 ------  Einschalten 40W
		if (off) {
			if (!W40)
			{
				W40 = true;
				aktivB();	// schaltet stby-Button nach W40
			}
			setOff(false);
			set_t = stored[0];
			set = set_t;
		}
		else setOff(true);
		force_redraw = true;	// Anzeige stby
		Strom = false;
		updateEEPROM();
		break;
	case (21):
		if (!W40)
		{
			W40 = true;
			aktivB();
			force_redraw = true;
		}
		aktives_Feld = 22;
		updateEEPROM();
		break;
	case (31):
		if (W40)
		{	// Umschaltvorgang
			W40 = false;
			aktivB();
			force_redraw = true;
		}
		aktives_Feld = 32;
		updateEEPROM();
		break;
	case (41): // Einschalten 150W
		if (off2) {
			if (W40)
			{
				W40 = false;
				aktivB();
			}
			setOff2(false);
			set_t2 = stored2[0];
			set = set_t2;
		}
		else setOff2(true);
		force_redraw = true;
		Strom = false;
		updateEEPROM();
		break;
		/*****************************  2.Zeile  **********************************
		* 
		*				STBY-Button
		* 
		***************************************************************************/
	case (22):
		/*if (!W40)
		{
			W40 = true;
			aktivB();
			force_redraw = true;
		}*/
		if (stby) 	setStandby(false);
		else 		setStandby(true);
		set_t = stored[0];
		set = set_t;
		updateEEPROM();
		break;
	case (32):
		/*if (W40)
		{	// Umschaltvorgang
			W40 = false;
			aktivB();
			force_redraw = true;
		}*/
		if (stby2) 	setStandby2(false);
		else 		setStandby2(true);
		set_t2 = stored2[0];
		set = set_t2;
		updateEEPROM();
		break;
		/***************************************************************************
		* 
		* 
		* gespeicherte Temperaturen 
		* 
		* 
		****************************************************************************/ 
	case(12):
		rotaryCount_alt = set;
		set = stored[1];
		set_t = stored[1];
		stored[0] = set_t;
		tft.fillRoundRect(SpalteM[Touch_Spalte], ReiheM[Touch_Zeile], Rectb, Recth, 9, Hellblau);  // Button aktiv
		tft.setTextColor(BLACK, Hellblau);
		merken();
		aktives_Feld_T = aktives_Feld;	// beibehalten des aktiven Hintergrunds wenn STBY
		break;
	case(13):
		rotaryCount_alt = set;
		set = stored[2];
		set_t = stored[2];
		stored[0] = set_t;
		tft.fillRoundRect(SpalteM[Touch_Spalte], ReiheM[Touch_Zeile], Rectb, Recth, 9, Hellblau);  // Button aktiv
		tft.setTextColor(BLACK, Hellblau);
		aktives_Feld_T = aktives_Feld;
		merken();
		break;
	case(14):
		rotaryCount_alt = set;
		set = stored[3];
		set_t = stored[3];
		stored[0] = set_t;
		tft.fillRoundRect(SpalteM[Touch_Spalte], ReiheM[Touch_Zeile], Rectb, Recth, 9, Hellblau);  // Button aktiv
		tft.setTextColor(BLACK, Hellblau);
		aktives_Feld_T = aktives_Feld;
		merken();
		break;
	case(42):
		rotaryCount_alt = set;
		set = stored2[1];
		set_t2 = stored2[1];
		stored2[0] = set_t2;
		tft.fillRoundRect(SpalteM[Touch_Spalte], ReiheM[Touch_Zeile], Rectb, Recth, 9, Hellblau);  // Button aktiv
		aktives_Feld_T = aktives_Feld;
		merken();
		break;
	case(43):
		rotaryCount_alt = set;
		set = stored2[2];
		set_t2 = stored2[2];
		stored2[0] = set_t2;
		tft.fillRoundRect(SpalteM[Touch_Spalte], ReiheM[Touch_Zeile], Rectb, Recth, 9, Hellblau);  // Button aktiv
		aktives_Feld_T = aktives_Feld;
		merken();
		break;
	case(44):
		rotaryCount_alt = set;
		set = stored2[3];
		set_t2 = stored2[3];
		stored2[0] = set_t2;
		tft.fillRoundRect(SpalteM[Touch_Spalte], ReiheM[Touch_Zeile], Rectb, Recth, 9, Hellblau);  // Button aktiv
		aktives_Feld_T = aktives_Feld;
		merken();
		break;
		/*************************************************************************************************


			Untermenü:  Kalibrierung, Slow, fast, Bildschirmhelligkeit, Zurück


		**************************************************************************************************/
	case(24):
		UMenu = true;	//  Aufruf aus LOOP
		setOff(true);
		setOff2(true);
		KalKalt = false;
		KalWarm = false;
		drawUMenu();
		break;
	case(34):		// Umschaltfunktion Strom vom Relais 
		setOff(true);
		setOff2(true);
		if (Strom)		Strom = false;
		else			Strom = true;
		//StromStat();
		aktives_Feld = 0;
		break;
	default:
		break;
	}
}

/************************************************************************************************
* 
*
*
*
*
*			Untermenü
*
*
*
*
*************************************************************************************************/

void drawUMenu() {
	tft.fillScreen(ILI9341_BLACK);
	tft.setTextSize(2);
	if (Kal40o)
	{	// nur oben
		tft.fillRoundRect(SpalteM[1], ReiheM[1], Rectb, Recth, 9, MAGENTA);  // Button aktiv
		Button40W(true, 0);
		tft.fillRoundRect(SpalteM[2], ReiheM[1], Rectb, Recth, 9, MAGENTA);  // Button aktiv
		Button40W(true, 1);
		tft.fillRoundRect(SpalteM[1], ReiheM[2], Rectb, Recth, 9, MAGENTA);  // Button aktiv
		Button40W(false, 2);
		tft.fillRoundRect(SpalteM[2], ReiheM[2], Rectb, Recth, 9, MAGENTA);  // Button aktiv
		Button40W(false, 3);
	}
	else
	{	// nur unten
		tft.fillRoundRect(SpalteM[1], ReiheM[1], Rectb, Recth, 9, MAGENTA);  // Button aktiv
		Button40W(false, 0);
		tft.fillRoundRect(SpalteM[2], ReiheM[1], Rectb, Recth, 9, MAGENTA);  // Button aktiv
		Button40W(false, 1);
		tft.fillRoundRect(SpalteM[1], ReiheM[2], Rectb, Recth, 9, MAGENTA);  // Button aktiv
		Button40W(true, 2);
		tft.fillRoundRect(SpalteM[2], ReiheM[2], Rectb, Recth, 9, MAGENTA);  // Button aktiv
		Button40W(true, 3);
	}
	if (Kal150o)
	{
		tft.fillRoundRect(SpalteM[3], ReiheM[1], Rectb, Recth, 9, MAGENTA);  // Button aktiv
		Button150W(true, 0);
		tft.fillRoundRect(SpalteM[4], ReiheM[1], Rectb, Recth, 9, MAGENTA);  // Button aktiv
		Button150W(true, 1);
		tft.fillRoundRect(SpalteM[3], ReiheM[2], Rectb, Recth, 9, MAGENTA);  // Button aktiv
		Button150W(false, 2);
		tft.fillRoundRect(SpalteM[4], ReiheM[2], Rectb, Recth, 9, MAGENTA);  // Button aktiv
		Button150W(false, 3);
	}
	else
	{
		tft.fillRoundRect(SpalteM[3], ReiheM[1], Rectb, Recth, 9, MAGENTA);  // Button aktiv
		Button150W(false, 0);
		tft.fillRoundRect(SpalteM[4], ReiheM[1], Rectb, Recth, 9, MAGENTA);  // Button aktiv
		Button150W(false, 1);
		tft.fillRoundRect(SpalteM[3], ReiheM[2], Rectb, Recth, 9, MAGENTA);  // Button aktiv
		Button150W(true, 2);
		tft.fillRoundRect(SpalteM[4], ReiheM[2], Rectb, Recth, 9, MAGENTA);  // Button aktiv
		Button150W(true, 3);
	}

	tft.setTextColor(BLACK, GREEN);
	tft.fillRoundRect(SpalteM[1], ReiheM[4], Rectb, Recth, 9, GREEN);  // Button aktiv
	tft.setCursor(SpalteT[1], ReiheT[4]);
	tft.print("Kal");
	tft.fillRoundRect(SpalteM[2], ReiheM[4], Rectb, Recth, 9, GREEN);  // Button aktiv
	tft.setCursor(SpalteT[2], ReiheT[4]);
	tft.print("Back");
	tft.fillRoundRect(SpalteM[3], ReiheM[4], Rectb, Recth, 9, GREEN);  // Button aktiv
	tft.setCursor(SpalteT[3], ReiheT[4]);
	tft.print("Dim");
	tft.fillRoundRect(SpalteM[4], ReiheM[4], Rectb, Recth, 9, GREEN);  // Button aktiv
	tft.setCursor(SpalteT[4], ReiheT[4]);
	tft.print("Kal");
}

void UnterFunktion(uint8_t Taste)	{
	switch (Taste)
	{
	case(14):			// Vorbelegung vor Aktivierung
		Kal12V = true;	// problem: hier bereits die Drehreglerwerte (grün) aktiviert ohne Initialisierung
		Kal24V = false;
		W40 = true;
		KalScreen();
		KalText("zuerst Einstellen der unteren Schwelltempeartur -kalt-, dann die Arbeitstemperatur mit -warm- einstellen");
		setOffset = true;		//Vorbelegung und Farbanzeige im Kal-Bildschirm
		PushB = false; // Ausschalten PushB=true
		//set_t = adc_offset;	 // Temperaturvorwahl für die Nullpunkteinstellung
		setko = adc_offset;
		drawKMenu();
		break;
	case(44):
		Kal12V = false;
		Kal24V = true;
		W40 = false;
		KalScreen();
		KalText("zuerst Einstellen der unteren Schwelltempeartur -kalt-, dann die Arbeitstemperatur mit -warm- einstellen");
		setOffset = true;		//Vorbelegung und Farbanzeige im Kal-Bildschirm
		PushB = false; // Ausschalten PushB=true
		setko = adc_offsetB;
		drawKMenu();
		break;
	case (24):				// ENDE ZURÜCK in HAUPTMENÜ
		EEPROM.update(EEPROM_Dimmerspeicher, Dimmerspeicher);
		DimmButton = false;
		fertig();
		break;
	case (34): //Bildschirmdimmer
		tft.fillRect(0, 0, 380, 180, ILI9341_BLACK);	// Meldefenster oben, Höhe 180 leeren
		if (DimmButton)
		{
			DimmButton = true;	
			rotaryCount_alt = rotaryCount;
			rotaryCount = Dimmerspeicher;	// Problem: dies ruft jedesmal den Dreher Int auf
		}
		MeldungT("Dimmwert", 3);
		MeldungW(Dimmerspeicher);
		break;
		/***************************  TIP-Auswahl ***************************Sp-Zei*/
	case(11):
		Kal40_IDX = 0;
		Kal40o = true;
		Kal40l = true;
		Button40W(true, 0);
		Button40W(false, 1);
		Button40W(false, 2);
		Button40W(false, 3);
			// speichert ausgewählte Spitze
		HolDa(Kal40_IDX, true);
		break;
	case(21):
		Kal40_IDX = 1;
		Kal40o = true;
		Kal40l = false;
		Button40W(false, 0);
		Button40W(true, 1);
		Button40W(false, 2);
		Button40W(false, 3);
		HolDa(Kal40_IDX, true);
		break;
	case(12):
		Kal40_IDX = 2;
		Kal40o = false;
		Kal40l = true;
		Button40W(false, 0);
		Button40W(false, 1);
		Button40W(true, 2);
		Button40W(false, 3);
		HolDa(Kal40_IDX, true);
		break;
	case(22):
		Kal40_IDX = 3;
		Kal40o = false;
		Kal40l = false;
		Button40W(false, 0);
		Button40W(false, 1);
		Button40W(false, 2);
		Button40W(true, 3);
		HolDa(Kal40_IDX, true);
		break;

	case(31):
		Kal150_IDX = 0;
		Kal150o = true;
		Kal150l = true;
		Button150W(true, 0);
		Button150W(false, 1);
		Button150W(false, 2);
		Button150W(false, 3);
		HolDa(Kal150_IDX, false);
		break;
	case(41):
		Kal150_IDX = 1;
		Kal150o = true;
		Kal150l = false;
		Button150W(false, 0);
		Button150W(true, 1);
		Button150W(false, 2);
		Button150W(false, 3);
		HolDa(Kal150_IDX, false);
		break;
	case(32):
		Kal150_IDX = 2;
		Kal150o = false;
		Kal150l = true;
		Button150W(false, 0);
		Button150W(false, 1);
		Button150W(true, 2);
		Button150W(false, 3);
		HolDa(Kal150_IDX, false);
		break;
	case(42):
		Kal150_IDX = 3;
		Kal150o = false;
		Kal150l = false;
		Button150W(false, 0);
		Button150W(false, 1);
		Button150W(false, 2);
		Button150W(true, 3);
		updateEEPROM();
		HolDa(Kal150_IDX, false);
		break;
	default:
		break;
	}

}
void fertig() {
	force_redraw = true;	// Arbeitsanzeige Text 1-Zeile 
	UMenu = false;			// flags löschen, Aufruf Hauptfunktion aus LOOP
	Kal12V = false;
	Kal24V = false;
	aktives_Feld = 0;
	aktives_Feld_T = 0;
	drawHmenu();
	aktivB();	// aktiver Button
	cur_t_old = cur_t;		// verhindert unter"Anzeige Temperaturbalken" tft.FillRec
	cur_t_old2 = cur_t2;
}

/************************************************************************************************



*			Kalibrierungs menü



*************************************************************************************************/

void drawKMenu() {
	tft.setTextSize(2);
	ButtonK();		// offset
	ButtonW();		// gain
	tft.fillRoundRect(SpalteM[3], ReiheM[4], Rectb, Recth, 9, GREEN);  // Button aktiv
	tft.setCursor(SpalteT[3], ReiheT[4]);
	tft.setTextColor(BLACK, GREEN);
	tft.print(" Sich");
	tft.fillRoundRect(SpalteM[4], ReiheM[4], Rectb, Recth, 9, GREEN);  // Button aktiv
	tft.setCursor(SpalteT[4], ReiheT[4]);
	tft.setTextColor(Schwarz, GREEN);
	tft.print("Back");
	tft.setTextColor(RED, BLACK);
	aktives_Feld = 14; // default für Drehregler="offset" "EinstellwerteKal"
}
void ButtonK() {
	tft.fillRoundRect(SpalteM[1], ReiheM[4], Rectb, Recth, 9, KalKalt ? LIGHTGREY : Dunkelgrau);  // Button aktiv
	tft.setCursor(SpalteT[1], ReiheT[4]);
	tft.setTextColor(KalKalt ? ILI9341_BLUE : ILI9341_CYAN, KalKalt ? LIGHTGREY : Dunkelgrau);
	tft.print("kalt"); 
}
void ButtonW() {
	tft.fillRoundRect(SpalteM[2], ReiheM[4], Rectb, Recth, 9, KalWarm ? LIGHTGREY : Dunkelgrau);  // Button aktiv
	tft.setCursor(SpalteT[2]+10, ReiheT[4]);
	tft.setTextColor(KalWarm ? PINK : RED, KalWarm ? LIGHTGREY : Dunkelgrau);
	tft.print(Kal12V ? stored[0] : stored2[0]);
	//tft.print(" warm");
}

void KalFunktion(uint8_t Taste) {
	switch (Taste)
	{		/******************** KALT: Kal. Offset ***************************/
	case (14):				// Auswahl Setup Offset
		tft.fillRect(0, 15, 380, 80, ILI9341_BLACK);	// Anzeigebereich leeren
		if (Kal12V)
		{
			set_t = adc_offset;	 // Temperaturvorwahl für die Nullpunkteinstellung
			//setko = adc_offset;
	//		set_t = set_0;
			target_t = 0;		// Vorgabe des untersten Regelwertes
			setOff(false); // Heizung einschalten
		}
		else 
		{
			set_t2 = adc_offsetB; // Temperaturvorwahl für die Nullpunkteinstellung
			//setko = adc_offsetB;
//			set_t2 = set_0;
			target_t2 = 0;
			setOff2(false);
		}	// Vorwahl für Drehregler
		setOffset = true;
		preset_Kal_stop = false;
		KalKalt = true;
		KalWarm = false;
		tft.setCursor(240, CursorZ2[2]);	//Vorgabewert
		tft.print("        ");
		ButtonK();
		ButtonW();
		KalText("Die Temperatur messen und den gemessenen Temperaturwert mit dem Drehregler bei -Kalttemperatur- einstellen");
		break;
	case (24): 		/******************* Kal. Gain mit aktuelle eingestellter Temp ****************/
		tft.fillRect(0, 15, 380, 80, ILI9341_BLACK);	// Anzeigebereich leeren
		if (Kal12V) {
			setOff(false);
			target_t = stored[0];	// Voreinstellung
			setkg = adc_gain;
		}
		else {
			setOff2(false);
			target_t2 = stored2[0];		// Voreinstellung
			setkg = adc_gainB;
		}
		setOffset = false;
		//StartKal = true;
		KalKalt = false;
		KalWarm = true;
		ButtonK();
		ButtonW();
		KalText("Die Temperatur messen und gleichzeitig die Heizung mittels Drehregler auf den vorgegebenen Wert einstellen");
		break;
	case (34):				/***********  Store ************/
		setOff(true);			// Heizung aus
		setOff2(true);
		tft.setCursor(0, CursorZ2[8]);
		SichDa(Kal40_IDX, true);
		SichDa(Kal150_IDX, false);
		KalText("Einstellungen gespeichert");
		break;
	case (44):					// zurück
		setOff(true);			// Heizung aus
		setOff2(true);
		if (Kal12V)	set_t = stored[0];
		else		set_t2 = stored2[0];
		//StartKal = false;
		fertig();
		break;
	default:
		break;
	}
}


/*********************************************************************************************


					Drehregler


Übernahme der Werte
**********************************************************************************************/
void EinstellwerteKal(int Feldnummer) {
	switch (Feldnummer)
	{
	case(14):			// Basis 0, Offset
		if (Kal12V)	adc_offset = setko;
		else		adc_offsetB = setko;
		break;
	case(24):			// Gain
		if (Kal12V)	adc_gain = setkg;
		else		adc_gainB = setkg;
		tft.setTextSize(3);
		if (Kal12V)
		{
			if (error == NO_ERROR)
			{
				tft.setTextColor(GREEN, BLACK);
				tft.setCursor(300, CursorZ2[2]);
				tft.print(DrehrichtungRechts ? "^" : "_");
			}
		}
		else
			if (error2 == NO_ERROR)
			{
				tft.setTextColor(GREEN, BLACK);
				tft.setCursor(300, CursorZ2[2]);
				tft.print(DrehrichtungRechts ? "^" : "_");
			}
		tft.setTextSize(2);
		break;
	case(34):			
		break;
	default:
		break;
	}
}
void EinstellwerteH(int Feldnummer) { // Aufruf vom Int-Service bei jeder Dreh-Rastung
	// Feldnummer entspricht "aktives_Feld" von RotarxCount
	switch (Feldnummer)
	{
	case(12):
		stored[1] = set;
		break;
	case(13):
		stored[2] = set;
		break;
	case(14):
		stored[3] = set;
		break;
	case(42):
		stored2[1] = set;
		break;
	case(43):
		stored2[2] = set;
		break;
	case(44):
		stored2[3] = set;
		break;
	case(11):     // EIN-schalter
		set_t = set;
		stored[0] = set_t;
		break;
	case(21):     // Anzeige
		/*set_t = set;
		stored[0] = set_t;*/
		break;
	case(31):     // Anzeige
		/*set_t2 = set;
		stored2[0] = set_t;*/
		break;
	case(41):     // EIN-schalter
		set_t2 = set;
		stored2[0] = set_t2;
		break;
	case(22):     // Anzeige bei "Display"-Solltemp und ändert direkt den PID-Sollwert hoch
			set_t = set;
			stored[0] = set_t;
		break;
	case(32):    // Anzeige bei "Display"-Solltemp und ändert direkt den PID-Sollwert runter
			set_t2 = set;
			stored2[0] = set_t2;
		break;
	case(34):
		if (!DrehrichtungRechts && Dimmerspeicher <= DimmerMax)		Dimmerspeicher++;
		else if (DrehrichtungRechts && Dimmerspeicher >= 1)			Dimmerspeicher--;
		MeldungW(Dimmerspeicher); //Anzeige des Dimmwertes 
		analogWrite(6, Dimmerspeicher * 10);
		break;
	default:
		break;
	}
}
/*****************************************************************************************************


					Hauptschleife


******************************************************************************************************/
void loop(void) {
		if (ts.tirqTouched()) {
			if (ts.touched()) {
				TS_Point p = ts.getPoint();
				Touch_Zeile = Zeile(p.y);
				Touch_Spalte = Spalte(p.x);
				aktives_Feld = (Touch_Spalte * 10) + Touch_Zeile;
				if (!UMenu && !Kal12V && !Kal24V)  		HauptFunktion(aktives_Feld);
				else if (UMenu && !Kal12V && !Kal24V)	UnterFunktion(aktives_Feld);
				else if (Kal12V || Kal24V)				KalFunktion(aktives_Feld);
				while (digitalRead(TIRQ_PIN) == LOW)	random();
			}
		}
	
		newValue = rotaryCount;
	if (newValue != lastLoopDisplayedRotaryCount) {
		lastLoopDisplayedRotaryCount = newValue;
		if (!Kal12V && !Kal24V)	EinstellwerteH(aktives_Feld);
		else					EinstellwerteKal(aktives_Feld);
	}

	//analogWrite(HEAT_LED, pwm);
	//Switch to following if the oscillation of the led bothers you
	//digitalWrite(HEAT_LED, cur_t+5 < target || (abs((int16_t)cur_t-(int16_t)target) <= 5 && (millis()/(stby?1000:500))%2));
	
	if (sendNext <= millis()) {
		sendNext += 100;
#ifdef TEST_ADC12
		Serial.print(aktives_Feld);
		Serial.print(";");
		Serial.print(set_0);
		Serial.print(";");
		Serial.print(set_t);
		Serial.print(";");
		Serial.print(" ");
		Serial.print(adc_gain);
		Serial.print(";");
		Serial.print(adc_offset);
		Serial.print(" ");
		Serial.print(";");
		Serial.print(";");
		Serial.print(cur_t);
		Serial.print(";");
		Serial.print(pwm);
		Serial.print(";");
		Serial.println(error);
#endif TEST_ADC12

#ifdef TEST_ADC24
		Serial.print(off2);
		Serial.print(";");
		Serial.print(error2);
		Serial.print(";");
		Serial.print(threshold_counter2);
		Serial.print(";");
		Serial.print(cur_t2);
		Serial.print(";");
		Serial.print(adc_offsetB + TEMP_RISE2);
		Serial.print(";");
		Serial.print(";");
		//Serial.print(target_t2);
		Serial.print(";");
		Serial.println(pwm2);
#endif TEST_ADC24

#ifndef TEST_ADC
	/*	Serial.print(error);
		Serial.print(";");
		Serial.print(off);
		Serial.print(";");
		Serial.print(W40 ? adc_gain : adc_gainB);
		Serial.print(";");
		Serial.print(W40 ? adc_offset : adc_offsetB);
		Serial.print(" ");
		Serial.print(" ");
		Serial.print(preset_Kal_stop);
		Serial.print(";");
		Serial.print(pwm_old);
		Serial.print(";");*/
		Serial.print(stby);
		Serial.print(";");
		Serial.println(stby2);
#endif TEST_ADC
		Serial.flush();
		if (!UMenu && !Kal12V && !Kal24V) {
			display();
			AutoPower();
		}
/************************************************************************************
	

	Kalibriermenü
	
   
**************************************************************************************/
		if (!PushB)
		{
			if (Kal12V || Kal24V) // aus Untermenü
			{
				/*************** Zeile 2********************/
				tft.setTextColor(setOffset ? CYAN : PINK, BLACK);
				tft.setCursor(98, CursorZ2[1]);
				tft.print("Soll/Ist:");
				tft.setCursor(210, CursorZ2[1]);
				if (Kal12V)	printTemp(set_td);
				else printTemp(set_td2);
				tft.setCursor(250, CursorZ2[1]);
				tft.print("/");
				tft.setCursor(262, CursorZ2[1]);
				if (Kal12V)	printTemp(cur_t);
				else printTemp(cur_t2);
				/*************** Zeile 3********************/
				if (setOffset)
				{		// Anzeige "kalt"-Bereich
					tft.setTextColor(CYAN, BLACK);
					tft.setCursor(24, CursorZ2[2]);
					tft.print("Kalttemperatur:");
					tft.setCursor(210, CursorZ2[2]);
					tft.setTextColor(GREEN, BLACK);		// Drehregler: Wert speichern
					if (Kal12V)	printTemp(adc_offset);
					else printTemp(adc_offsetB);
				}
				else
				{
					// Anzeige "warm" = Temperaturvorwahl aus Store[0] im Auswahlbutton
					tft.setCursor(48, CursorZ2[2]);
					tft.print("Heizleistung:");					
					tft.setTextColor(GREEN, BLACK);
					if (Kal12V) { if (error != NO_ERROR) tft.setTextColor(PINK, BLACK); }
					else { if (error2 != NO_ERROR) tft.setTextColor(PINK, BLACK); }
					tft.setCursor(210, CursorZ2[2]);
					tft.print("------>");
					tft.setCursor(300, CursorZ2[2]);
					tft.setTextSize(3);
					tft.print("-");
					tft.setTextSize(2);
				}
				/****************  Zeile 4********************/
				tft.setTextColor(setOffset ? CYAN : PINK, BLACK);
				tft.setCursor(48, CursorZ2[3]);
				tft.print("Kontrollwert:");
				tft.setCursor(230, CursorZ2[3]);
				if (setOffset)
				{		// Anzeige "kalt"-Bereich
					if (error == NO_ERROR || error2 == NO_ERROR)
					{
						if (Kal12V)	printTemp(pwm);
						else printTemp(pwm2);
					}
				}
				else
				{		// Anzeige "warm"-Bereich		
					if (Kal12V) { if (error != NO_ERROR)	tft.setTextColor(GREEN, BLACK); }
					else { if (error2 != NO_ERROR)	tft.setTextColor(GREEN, BLACK); }
					tft.print(Kal12V ? adc_gain : adc_gainB);
				}
			}
		}
		
	}
	delay(DELAY_MAIN_LOOP);
}
