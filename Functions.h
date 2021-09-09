#pragma once
//11111111111111111111111111111111111111111111111111111111111111111111111111111111111111
//
//
//
//
//
//   INTERRUPT SERVICE  Drehregler 
//
//
//
//
//
//11111111111111111111111111111111111111111111111111111111111111111111111111111111111111
void IsrForQDEC(void) { // do absolute minimum possible in any ISR ...
	::SimpleHacks::QDECODER_EVENT event = qdec.update();
	if (event & ::SimpleHacks::QDECODER_EVENT_CW) {
		if (set < TEMP_MAX)		set++;
		if (setko < KALO_MAX)		setko = setko + 0.1;
		if (setkg > KAL_MIN)		setkg = setkg + 0.001;
		if (Kal12V) { if (error == NO_ERROR)		if (setkg < KALG_MAX)		setkg = setkg - 0.002; }
		else 		{ if (error2 == NO_ERROR)		if (setkg < KALG_MAX)		setkg = setkg - 0.002; }
		rotaryCount = rotaryCount + 1;
		DrehrichtungRechts = true;
	}
	else if (event & ::SimpleHacks::QDECODER_EVENT_CCW) {
		if (set > TEMP_MIN)		set--;
		if (setko > KAL_MIN)		setko = setko - 0.1;
		if (setkg < KALG_MAX)		setkg = setkg - 0.001;
		if (Kal12V) { if (error == NO_ERROR)		if (setkg > KAL_MIN)		setkg = setkg + 0.002; }
		else		{ if (error2 == NO_ERROR)		if (setkg > KAL_MIN)		setkg = setkg + 0.002; }
		rotaryCount = rotaryCount - 1;
		DrehrichtungRechts = false;
	}
	return;
}
//11111111111111111111111111111111111111111111111111111111111111111111111111111111111111

void setOff(boolean state) {
	if (state == off) return;
	if (!state) {
		analogWrite(HEATER_PWM, 0);
	}
	else
		//setStandby(false);
		last_on_state = millis() / 1000;
	off = state;
	wasOff = true;
	last_measured = cur_t;
}
void setOff2(boolean state) {
	if (state == off2) return;
	if (!state) {
		analogWrite(HEATER_PWM2, 0);
	}
	else
		//setStandby2(false);
		last_on_state2 = millis() / 1000;
	off2 = state;
	wasOff2 = true;
	last_measured2 = cur_t2;
}


void KalScreen() {		// Anzeige der zuvor gewählten Spitze 
	tft.fillScreen(ILI9341_BLACK);
	tft.setTextColor(WHITE, BLACK);
	tft.setTextSize(2);
	tft.setCursor(0, CursorZ2[0]);
	tft.print(Kal12V ? "Kalibrierung-40W:  " : "Kalibrierung150W: ");
	tft.setCursor(240, CursorZ2[0]);
	tft.print("T");
	tft.print(Kal12V ? Kal40_IDX + 1 : Kal150_IDX + 1);
	/*tft.setCursor(280, CursorZ2[0]);
	if (Kal12V) tft.print(regelChr12slow ? "S" : "F");
	else tft.print(regelChr24slow ? "S" : "F");*/
}
void KalText(char Text[]) {
	tft.fillRect(0, 90, 380, 85, ILI9341_BLACK);	// Meldefenster oben, Höhe 90 leeren
	tft.setCursor(0, CursorZ2[4]);
	tft.setTextColor(ILI9341_YELLOW, ILI9341_BLACK);
	tft.print(Text);
}

void MeldungW(uint8_t Wert) {
	tft.setCursor(SpalteT[4], ReiheT[3] - 10); // Cursorposition
	tft.setTextSize(3);
	tft.setTextColor(RED, BLACK);
	tft.fillRect(SpalteT[4], ReiheT[3], 150, 21, BLACK);
	if (Wert < 10) tft.write(' ');
	tft.print(Wert);
}
void MeldungT(char Text[], int Pix) {
	tft.setCursor(SpalteT[2], ReiheT[3] - 10); // Cursorposition
	tft.setTextSize(Pix);
	tft.setTextColor(RED, BLACK);
	tft.fillRect(SpalteT[2], ReiheT[3], 150, 21, BLACK);
	tft.print(Text);
}

void aktivB() {
	tft.fillRoundRect(SpalteM[2], ReiheM[2], Rectb, Recth, 9, W40 ? Hellblau : Grau);  // Button aktiv
	tft.fillRoundRect(SpalteM[3], ReiheM[2], Rectb, Recth, 9, W40 ? Grau : Hellblau);  // Button aktiv
}



//---------------------------------------------------------
// Touchscreen Raster berechnen
//---------------------------------------------------------
byte Zeile(uint16_t n) {
	if (n < 1200)
		return 1;
	if (n < 2100)
		return 2;
	if (n < 3000)
		return 3;
	else
		return 4;
}
byte Spalte(uint16_t n) {
	if (n < 1500)
		return 1;
	if (n < 2100)
		return 2;
	if (n < 3000)
		return 3;
	else
		return 4;
}


void	Button40W(boolean aktiv, uint8_t ButtonNr) {
	tft.setTextColor(BLACK, MAGENTA);
	switch (ButtonNr)
	{
	case(0):
		tft.setCursor(SpalteT[1], ReiheT[1]);
		if (aktiv)		tft.setTextColor(Kal40l ? WHITE : BLACK, Kal40l ? MAGENTA : MAGENTA);
		tft.print("40W_1");
		break;
	case(1):
		tft.setCursor(SpalteT[2], ReiheT[1]);
		if (aktiv)		tft.setTextColor(Kal40l ? BLACK : WHITE, Kal40l ? MAGENTA : MAGENTA);
		tft.print("40W_2");
		break;
	case(2):
		tft.setCursor(SpalteT[1], ReiheT[2] - 10);
		if (aktiv)		tft.setTextColor(Kal40l ? WHITE : BLACK, Kal40l ? MAGENTA : MAGENTA);
		tft.print("40W_3");
		break;
	default:
		tft.setCursor(SpalteT[2], ReiheT[2] - 10);
		if (aktiv)		tft.setTextColor(Kal40l ? BLACK : WHITE, Kal40l ? MAGENTA : MAGENTA);
		tft.print("40W_4");
		break;
	}
}

void	Button150W(boolean aktiv, uint8_t ButtonNr) {
	tft.setTextColor(BLACK, MAGENTA);
	switch (ButtonNr)
	{
	case(0):
		tft.setCursor(SpalteT[3], ReiheT[1]);
		if (aktiv)		tft.setTextColor(Kal150l ? WHITE : BLACK, Kal150l ? MAGENTA : MAGENTA);
		tft.print("150_1");
		break;
	case(1):
		tft.setCursor(SpalteT[4], ReiheT[1]);
		if (aktiv)		tft.setTextColor(Kal150l ? BLACK : WHITE, Kal150l ? MAGENTA : MAGENTA);
		tft.print("150_2");
		break;
	case(2):
		tft.setCursor(SpalteT[3], ReiheT[2] - 10);
		if (aktiv)		tft.setTextColor(Kal150l ? WHITE : BLACK, Kal150l ? MAGENTA : MAGENTA);
		tft.print("150_3");
		break;
	default:
		tft.setCursor(SpalteT[4], ReiheT[2] - 10);
		if (aktiv)		tft.setTextColor(Kal150l ? BLACK : WHITE, Kal150l ? MAGENTA : MAGENTA);
		tft.print("150_4");
		break;
	}
}


void drawHmenu() {
	tft.fillScreen(ILI9341_BLACK);
	tft.fillRoundRect(SpalteM[2], ReiheM[1], Rectb, 54, 9, Hellblau);
	tft.fillRoundRect(SpalteM[3], ReiheM[1], Rectb, 54, 9, Hellblau);
	tft.fillRoundRect(SpalteM[1], ReiheM[2], Rectb, Recth, 9, Grau);
	tft.fillRoundRect(SpalteM[2], ReiheM[2], Rectb, Recth, 9, Grau);
	tft.fillRoundRect(SpalteM[3], ReiheM[2], Rectb, Recth, 9, Grau);
	tft.fillRoundRect(SpalteM[1], ReiheM[3], Rectb, Recth, 9, Grau);
	tft.fillRoundRect(SpalteM[1], ReiheM[4], Rectb, Recth, 9, Grau);
	tft.fillRoundRect(SpalteM[4], ReiheM[2], Rectb, Recth, 9, Grau);
	tft.fillRoundRect(SpalteM[4], ReiheM[3], Rectb, Recth, 9, Grau);
	tft.fillRoundRect(SpalteM[4], ReiheM[4], Rectb, Recth, 9, Grau);
	tft.fillRoundRect(SpalteM[2], ReiheM[4], Rectb, Recth, 9, GREEN);
	tft.fillRoundRect(SpalteM[3], ReiheM[4], Rectb, Recth, 9, GREEN);

	tft.setTextSize(2);
	tft.setTextColor(WHITE, BLACK);
	tft.setCursor(SpalteT[1] + 20, ReiheT[1] - 10);
	tft.print("T");
	tft.print(Kal40_IDX + 1);		// 0-3
	tft.setCursor(SpalteT[1], ReiheT[1] + 20);
	tft.setTextSize(1);
	tft.print("40 Watt");

	tft.setTextSize(2);
	tft.setCursor(SpalteT[4], ReiheT[1] - 10);
	tft.print("T");
	tft.print(Kal150_IDX + 1);
	tft.setTextSize(1);
	tft.setCursor(SpalteT[4], ReiheT[1] + 20);
	tft.print("150 Watt");
}


/***************************  Erstinitialisierung ***********************************/

void Erstinit() {
	for (int i = 0; i < 200; i++)	EEPROM.write(i, 0);	// löschen
	EEPROM.write(EEPROM_KAL4_IDX, 0);
	EEPROM.write(EEPROM_KAL5_IDX, 0);
	for (uint8_t i = 0; i < 4; i++) { // schreiben auf EEPROM Stringweise wg. float
		EEPROM.put(ADC_BASIS_GN + i * 4 * sizeof(adc_gain), adc_gain);
		EEPROM.put(ADC_BASIS_OF + i * 4 * sizeof(adc_offset), adc_offset);
		EEPROM.put(ADC_BASIS_GNB + i * 4 * sizeof(adc_gainB), adc_gainB);
		EEPROM.put(ADC_BASIS_OFB + i * 4 * sizeof(adc_offsetB), adc_offsetB);
	}
}

/******************************************
	sichern der Konfig-Daten
*******************************************/
void updateEEPROM(void) {			// schreibt nur die geänderten Werte
	for (int i = EEPROM_VERSION; i < BASIS; i++)	EEPROM.update(i, Meldung[i - EEPROM_VERSION]);
	EEPROM.update(EEPROM_STBY, stby);		// schreiben Byteweise
	EEPROM.update(EEPROM_STBY2, stby2);
	EEPROM.update(EEPROM_W40, W40);
	EEPROM.update(EEPROM_12SF, regelChr12slow);
	EEPROM.update(EEPROM_24SF, regelChr24slow);
	EEPROM.update(EEPROM_Dimmerspeicher, Dimmerspeicher);
	EEPROM.update(EEPROM_KAL4_IDX, Kal40_IDX);
	EEPROM.update(EEPROM_KAL5_IDX, Kal150_IDX);
	for (uint8_t i = 0; i < 4; i++) {
		EEPROM.update(BASIS + i * 2, stored[i] >> 8);
		EEPROM.update(BASIS + 1 + i * 2, stored[i] & 0xFF);
		EEPROM.update(BASIS + 8 + i * 2, stored2[i] >> 8);
		EEPROM.update(BASIS + 9 + i * 2, stored2[i] & 0xFF);
	}
}
void SichDa(int i, boolean W40) {
	if (W40)
	{
		EEPROM.put(ADC_BASIS_GN + i * 4 * sizeof(adc_gain), adc_gain);
		EEPROM.put(ADC_BASIS_OF + i * 4 * sizeof(adc_offset), adc_offset);
	}
	else
	{
		EEPROM.put(ADC_BASIS_GNB + i * 4 * sizeof(adc_gainB), adc_gainB);
		EEPROM.put(ADC_BASIS_OFB + i * 4 * sizeof(adc_offsetB), adc_offsetB);
	}
}

void HolDa(int i, boolean W40) {
	if (W40)
	{
		EEPROM.get(ADC_BASIS_GN + i * 4 * sizeof(adc_gain), adc_gain);	// get liest stringweise wg. long
		EEPROM.get(ADC_BASIS_OF + i * 4 * sizeof(adc_offset), adc_offset);
	}
	else {
		EEPROM.get(ADC_BASIS_GNB + i * 4 * sizeof(adc_gainB), adc_gainB);
		EEPROM.get(ADC_BASIS_OFB + i * 4 * sizeof(adc_offsetB), adc_offsetB);
	}
	Serial.print(adc_gain);
	Serial.print(" ");
	Serial.print(adc_offset);
	Serial.print(" ");
	Serial.print(adc_gainB);
	Serial.print(" ");
	Serial.println(adc_offsetB);
}

//##########################################################################
// Startbildschirm
//##########################################################################
void setStandby(boolean state) {
	if (stby_layoff) return;
	if (state == stby) return;
	stby = state;
	last_measured = cur_t;
	last_temperature_drop = millis();
	last_on_state = millis() / 1000;
}

void setStandby2(boolean state) {
	if (stby_layoff) return;
	if (state == stby2) return;
	stby2 = state;
	last_measured2 = cur_t2;
	last_temperature_drop2 = millis();
	last_on_state2 = millis() / 1000;
}


/************************************AUTOPOWER******************************************/
void AutoPower() {
	/* Timeout-Anzeige */
	if (!off) {
		int16_t tout;
		if (stby || stby_layoff) {
			tout = min(max(0, (last_on_state + OFF_TIMEOUT - (millis()) / 1000)), OFF_TIMEOUT);
		}
		else {
			tout = min(max(0, (last_temperature_drop + STANDBY_TIMEOUT - (millis()) / 1000)), STANDBY_TIMEOUT);
		}
		tft.setTextColor(stby ? RED : YELLOW, BLACK);
		tft.setTextSize(2);
		tft.setCursor(SpalteT[2] + 5, ReiheT[3] + 20);
		if (tout < 600) tft.write('0');
		tft.print(tout / 60);
		tft.write(':');
		if (tout % 60 < 10) tft.write('0');
		tft.print(tout % 60);
	}
	else if (cur_t2 != 999) {
		tft.fillRect(SpalteT[2], ReiheT[3] + 20, 60, 20, BLACK);
	}
	if (!off2) {
		int16_t tout2;
		if (stby2 || stby_layoff) {
			tout2 = min(max(0, (last_on_state2 + OFF_TIMEOUT - (millis()) / 1000)), OFF_TIMEOUT);
		}
		else {
			tout2 = min(max(0, (last_temperature_drop2 + STANDBY_TIMEOUT - (millis()) / 1000)), STANDBY_TIMEOUT);
		}
		tft.setTextColor(stby2 ? RED : YELLOW, BLACK);
		tft.setTextSize(2);
		tft.setCursor(SpalteT[3] + 5, ReiheT[3] + 20);
		if (tout2 < 600) tft.write('0');
		tft.print(tout2 / 60);
		tft.write(':');
		if (tout2 % 60 < 10) tft.write('0');
		tft.print(tout2 % 60);
	}
	else if (cur_t2 != 999) {
		tft.fillRect(SpalteT[3], ReiheT[3] + 20, 60, 20, BLACK);
	}

	/* Wiederanlauf */

	if (pwm > max(20, (cur_t - 150) / 50 * round(25 - min(15, v))) + 5) {
		//if (target_t-cur_t > 0.715*exp(0.0077*target_t)) {
		//if (cur_t / (double)target_t < STANDBY_TEMPERATURE_DROP) {
		if (autopower_repeat_under || stby) {
			if (stby && !wasOff) {
				setStandby(false);
			}
			else {
				last_temperature_drop = millis() / 1000;
			}
		}
		autopower_repeat_under = true;
	}
	else if (wasOff) {
		wasOff = false;
	}
	else {
		autopower_repeat_under = false; //over the max pwm for at least two times
	}

	if (!off && !stby && millis() / 1000 > (last_temperature_drop + STANDBY_TIMEOUT)) {
		setStandby(true);
	}
	if (!off && (stby || stby_layoff) && millis() / 1000 > (last_on_state + OFF_TIMEOUT)) {
		setOff(true);
	}

	/************************************/


	if (pwm2 > max(20, (cur_t2 - 150) / 50 * round(25 - min(15, v))) + 5) {
		//if (target_t-cur_t > 0.715*exp(0.0077*target_t)) {
		//if (cur_t / (double)target_t < STANDBY_TEMPERATURE_DROP) {
		if (autopower_repeat_under2 || stby2) {
			if (stby2 && !wasOff2) {
				setStandby2(false);
			}
			else {
				last_temperature_drop2 = millis() / 1000;
			}
		}
		autopower_repeat_under2 = true;
	}
	else if (wasOff2) {
		wasOff2 = false;
	}
	else {
		autopower_repeat_under2 = false; //over the max pwm for at least two times
	}

	if (!off2 && !stby2 && millis() / 1000 > (last_temperature_drop2 + STANDBY_TIMEOUT)) {
		setStandby2(true);
	}
	if (!off2 && (stby2 || stby_layoff) && millis() / 1000 > (last_on_state2 + OFF_TIMEOUT)) {
		setOff2(true);
	}

}
/****************************************************************************************************/
void setError(error_type e) {
	error = e;
	setOff(true);
}
void setError2(error_type e) {
	error2 = e;
	setOff2(true);
}

void Trigger()
{
/*	digitalWrite(LED, 1);
	digitalWrite(LED, 0);*/
}

void printTemp(float t) {
	if (t < 100) tft.write(' ');
	if (t < 10) tft.write(' ');
	tft.print((int)t);
}

void Taster() {
	if (PushB)	PushB = false;
	else PushB = true;
	setOff(true); // ausschalten
	setOff2(true); // ausschalten
	for (int i = 0; i < 3000; i++)
	{
		//random();
	}
	return;
}


void merken() {
	if (aktives_Feld_alt != aktives_Feld) { // wiederherstellen zuvor gehighlightetes Button
		tft.fillRoundRect(SpalteM[Touch_Spalte_vorher], ReiheM[Touch_Zeile_vorher], Rectb, Recth, 9, Grau);
	}
	aktives_Feld_alt = aktives_Feld;
	Touch_Spalte_vorher = Touch_Spalte;
	Touch_Zeile_vorher = Touch_Zeile;
	lastLoopDisplayedRotaryCount = rotaryCount; // verhindert sogleich Einsprung in "Einstellwerte"
	updateEEPROM();
}


//#######################################################################
// Temperatur lesen
//#######################################################################
int getTemp12V(void) {
	uint16_t adc = adcValue / (MESSREIHEN + 1);
	adc_12 = adc;
	adcValue = 0;
#ifdef TEST_ADC
	//Serial.println(adc);
#endif
	if (adc >= 900) { //Illegal value, tip not plugged in - would be around 560deg
		analogWrite(HEATER_PWM, 0);
		if (!off)
			setError(NO_TIP); //Error3
		return 999;
	}
	return round(((float)adc) * adc_gain + adc_offset);
}
//2222222222222222222222222222222222222
int getTemp24V(void) {
	uint16_t adc2 = adcValue2 / (MESSREIHEN + 1);
	adc_24 = adc2;
	adcValue2 = 0;
#ifdef TEST_ADC
	//Serial.println(adc2);
#endif
	if (adc2 >= 1010) { //Illegal value, tip not plugged in - would be around 560deg
		analogWrite(HEATER_PWM2, 0);
		if (!off2)
			setError2(NO_TIP);
		return 999;
	}
	return round(((float)adc2) * adc_gainB + adc_offsetB);
}
//-----------------------------------------------------------------------------------------

void compute12V(void) {
	if (!Kal12V)
	{
		cur_t = getTemp12V();
		if (off) {
			target_t = 0;
			if (cur_t < adc_offset + TEMP_RISE) {
				threshold_counter = TEMP_UNDER_THRESHOLD; //reset counter
			}
		}
		else {
			if (stby_layoff || stby) { // stby_Layoff ist statisch=0
				target_t = TEMP_STBY;
			}
			else {
				target_t = set_t;
			}			//-60 && last_measured != 999)
			if (cur_t - last_measured <= -30 && last_measured != 999) {
				setError(EXCESSIVE_FALL); //decrease of more than 30 degree is uncommon, short of ring and gnd is possible.
			}
			if (cur_t < adc_offset + TEMP_RISE) {
				if (threshold_counter == 0) {
					setError(NOT_HEATING); //temperature is not reached in desired time, short of sensor and gnd too?
				}
				else {
					threshold_counter--;
				}
			}
			else 	threshold_counter = THRES_MAX_DECEED; //reset counter to a smaller value to allow small oscillation of temperature
		}
	}		/*************************     Kalibrierung Offset  **************************/
	else if (setOffset && Kal12V)
	{	// den untersten Temperaturwert einregeln. Danach den Offset manuell festlegen
		uint16_t adc = adcValue / (MESSREIHEN + 1);
		adc_12 = adc;
		adcValue = 0;
		cur_t = adc_12;	// gemessener Wert der Lötspitze zur Anzeige
		//target_t = 1;		// Vorgabe des untersten Regelwertes
		if (!preset_Kal_stop && !off)
		{					// Set-Wert für PID-Regler nur soweit erhöhen, dass gerade die Heizung anspringt
			if (preset_Kal == 0)
			{
				preset_Kal = PRESET_KAL; // Wartezähler um Set-Wert zu incrementieren
				if (pwm_old == 0)			target_t++;
			}
			if (pwm_old > 0) preset_Kal_stop = true; // Kalttemperaturschwelle
			preset_Kal--;
		}
	}
	else if (!setOffset && Kal12V)
	{		/*************************     Kalibrierung Gain  **************************/
		uint16_t adc = adcValue / (MESSREIHEN + 1);	// Messwert am Eingang
		adc_12 = round(((float)adc) * adc_gain + adc_offset);	// ermittelter Wert 
		adcValue = 0; // target_t übernommen von "Kal_funktionen"
		cur_t = adc_12; // gemessener Wert der Lötspitze
	}
	set_td = target_t; // Setpoint für PID SOLL-Wert
	cur_td = cur_t;  // SteuerVariable für PID IST-Wert
	last_measured = cur_t;
	//PPPPPPPPPPPPPP
	//IIIIIIIIIIIIII
	//DDDDDDDDDDDDDD
	heaterPID.Compute(); // errechnet pid_val
	if (error != NO_ERROR || off)
		pwm = 0;
	else {
		pwm = min(255, pid_val);
		pwm_old = pwm;
	}
}
//22222222222222222222222222 compute 2222222222222222222222222222222222222222
void compute24V(void) {
	if (!Kal24V)
	{
		cur_t2 = getTemp24V();   // reagiert auf "no Tip" setzt Error 3
		if (off2) {
			target_t2 = 0;
			if (cur_t2 < adc_offsetB + TEMP_RISE2) {
				threshold_counter2 = TEMP_UNDER_THRESHOLD2; //reset counter
			}
		}
		else {
			if (stby_layoff || stby2) {
				target_t2 = TEMP_STBY;
			}
			else {
				target_t2 = set_t2;
			}
			if (cur_t2 - last_measured2 <= -30 && last_measured2 != 999) {
				setError2(EXCESSIVE_FALL); //(1)decrease of more than 30 degree is uncommon, short of ring and gnd is possible.
			}
			if (cur_t2 < adc_offsetB + TEMP_RISE2) { // 42,8+15
				if (threshold_counter2 == 0) {
					setError2(NOT_HEATING); //(2)temperature is not reached in desired time, short of sensor and gnd too?
				}
				else {
					threshold_counter2--;
				}
			}
			else	threshold_counter2 = THRES_MAX_DECEED2; //reset counter to a smaller value to allow small oscillation of temperature
		}
	}		/*************************     Kalibrierung Offset  **************************/
	else if (setOffset && Kal24V)
	{		/*************************     Kalibrierung Offset  **************************/
		uint16_t adc2 = adcValue2 / (MESSREIHEN + 1);
		adc_24 = adc2;
		adcValue2 = 0;
		cur_t2 = adc_24;	// gemessener Wert der Lötspitze
		//target_t2 = 1;		// Vorgabe des untersten Regelwertes
		if (!preset_Kal_stop && !off2)
		{					// Set-Wert für PID-Regler nur soweit erhöhen, dass gerade die Heizung anspringt
			if (preset_Kal == 0)
			{
				preset_Kal = PRESET_KAL; // Wartezähler um Set-Wert zu incrementieren
				if (pwm2_old == 0)			target_t2++;
			}
			if (pwm2_old > 0) preset_Kal_stop = true;
			preset_Kal--;
		}
	}		/*************************     Kalibrierung Gain  **************************/
	else if (!setOffset && Kal24V)		// target_t2 ist die Stellvorgabe vom Aufruf "KalFunktionen"
	{		// Kal Gain: Vorgabe: Kombination aus Messwert+Korrekturfaktoren. Variiert wird der Gain 
		uint16_t adc2 = adcValue2 / (MESSREIHEN + 1);	// Messwert am Eingang
		adc_24 = round(((float)adc2) * adc_gainB + adc_offsetB);	// ermittelter Wert 
		adcValue2 = 0; // target_t2 übernommen von "Kal_funktionen"
		cur_t2 = adc_24; // gemessener Wert der Lötspitze
	}
	set_td2 = target_t2; // Setpoint für PID SOLL-Wert
	cur_td2 = cur_t2;  // SteuerVariable für PID IST-Wert
	last_measured2 = cur_t2;
	//PPPPPPPPPPPPPP
	//IIIIIIIIIIIIII
	//DDDDDDDDDDDDDD
	heaterPID2.Compute(); // errechnet pid_val
	if (error2 != NO_ERROR || off2) // off true = 1
		pwm2 = 0;
	else
	{
		pwm2 = min(255, pid_val2);
		pwm2_old = pwm2;
	}
}



