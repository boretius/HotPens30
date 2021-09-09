#pragma once
#include <stdlib.h>

//#######################################################################################
//
// Dynamische Statusanzeige (Aufruf von LOOP)
//
//#######################################################################################
void display(void) {
	boolean yell = stby || (stby_layoff);
	boolean yell2 = stby2 || (stby_layoff);
	int16_t	temperature = cur_t;
	int16_t temperature2 = cur_t2;
	tft.drawCircle(SpalteT[1], ReiheT[1], 8, off ? RED : yell ? YELLOW : GREEN);
	tft.drawCircle(SpalteT[1], ReiheT[1], 7, off ? RED : yell ? YELLOW : GREEN);
	tft.fillRect(SpalteT[1] - 1, ReiheT[1] - 8, 3, 3, BLACK);
	tft.drawFastVLine(SpalteT[1], ReiheT[1] - 10, 10, off ? RED : yell ? YELLOW : GREEN);
	// 222222222222222222
	tft.drawCircle(SpalteT[4] + 52, ReiheT[1], 8, off2 ? RED : yell2 ? YELLOW : GREEN);
	tft.drawCircle(SpalteT[4] + 52, ReiheT[1], 7, off2 ? RED : yell2 ? YELLOW : GREEN);
	tft.fillRect(SpalteT[4] + 51, ReiheT[1] - 8, 3, 3, BLACK);
	tft.drawFastVLine(SpalteT[4] + 52, ReiheT[1] - 10, 10, off2 ? RED : yell2 ? YELLOW : GREEN);

		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// Anzeige der Vorwahltemperatur 
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		tft.setTextSize(2);
		tft.setTextColor(BLACK, Grau);
		if (aktives_Feld_T == 12)	tft.setTextColor(WHITE, Hellblau);		
		tft.setCursor(SpalteTs[1], ReiheTs[2]);
		tft.print(stored[1]);

		tft.setTextColor(BLACK, Grau);
		if (aktives_Feld_T == 13)	tft.setTextColor(WHITE, Hellblau);
		tft.setCursor(SpalteTs[1], ReiheTs[3]);
		tft.print(stored[2]);

		tft.setTextColor(BLACK, Grau);
		if (aktives_Feld_T == 14)	tft.setTextColor(WHITE, Hellblau);
		tft.setCursor(SpalteTs[1], ReiheTs[4]);
		tft.print(stored[3]);
		//22222222222222222222222222222222222222222
		// Anzeige der Vorwahltemperatur 
		//22222222222222222222222222222222222222222
		tft.setTextColor(BLACK, Grau);
		if (aktives_Feld_T == 42)	tft.setTextColor(WHITE, Hellblau);
		tft.setCursor(SpalteTs[4], ReiheTs[2]);
		tft.print(stored2[1]);

		tft.setTextColor(BLACK, Grau);
		if (aktives_Feld_T == 43)	tft.setTextColor(WHITE, Hellblau);
		tft.setCursor(SpalteTs[4], ReiheTs[3]);
		tft.print(stored2[2]);

		tft.setTextColor(BLACK, Grau);
		if (aktives_Feld_T == 44)	tft.setTextColor(WHITE, Hellblau);
		tft.setCursor(SpalteTs[4], ReiheTs[4]);
		tft.print(stored2[3]);

		tft.setTextColor(BLACK, W40 ? Hellblau : Grau);
		tft.setCursor(SpalteT[2] + 20, ReiheT[2] - 10);
		printTemp(set_t);
		tft.setCursor(SpalteT[3] + 20, ReiheT[2] - 10);
		tft.setTextColor(BLACK, W40 ? Grau : Hellblau);
		printTemp(set_t2);
		//----------------------------------------------
		tft.setTextSize(2);
		tft.setTextColor(BLACK, GREEN);
		tft.setCursor(SpalteT[2], ReiheT[4]);
		tft.print(" Menu");
		tft.setCursor(SpalteT[3], ReiheT[4]);
		tft.print("Strom");
		tft.setCursor(SpalteT[3], ReiheT[4] + 18);
		if (Strom) {
			tft.print("= EIN");
			//digitalWrite(Relais, LOW); // Strom EIN
		}
		else {
			tft.print("= AUS");
			//digitalWrite(Relais, HIGH); // Strom AUS
		}
		//----------------------------------------------------------
		// Anzeige SOLL-Temperatur
		//----------------------------------------------------------
		if (set_t_old != set_t || old_stby != (stby || stby_layoff) || force_redraw) {
			tft.setTextSize(3);
			tft.setCursor(SpalteT[2] - 1, ReiheT[2] + 10);		// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			tft.setTextColor(YELLOW, W40 ? Hellblau : Grau);
			if (stby || stby_layoff) {
				old_stby = true;
				tft.print(F("STBY"));
			}
			else {
				old_stby = false;
				set_t_old = set_t;
				tft.print("    ");
			}
		}
		// 22222222222222222222222222222222222222222222222222
		// Anzeige SOLL-Temperatur2
		// 22222222222222222222222222222222222222222222222222
		if (set_t_old2 != set_t2 || old_stby2 != (stby2 || stby_layoff) || force_redraw) {
			tft.setCursor(SpalteT[3] - 1, ReiheT[2] + 10);		// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			tft.setTextSize(3);
			tft.setTextColor(YELLOW, W40 ? Grau : Hellblau);
			if (stby2 || stby_layoff) {
				old_stby2 = true;
				tft.print(F("STBY"));
			}
			else {
				old_stby2 = false;
				set_t_old2 = set_t2;
				tft.print("    ");
			}
		}
/***************************************************************************************/
	/* 1212121212121212121212121212121212121212121212121212121212121212
	  Anzeige der gemessenen Temperatur
	 11111111111111111111111111111111111111111111*/
	if (cur_t_old != temperature || force_redraw) {
		tft.setCursor(SpalteT[2], ReiheT[1]);
		tft.setTextSize(3);
		if (temperature == 999) {
			tft.setTextColor(RED, Hellblau);
			tft.print(F("ERR "));
			tft.setCursor(SpalteT[2], ReiheT[3]);
			tft.setTextSize(2);
			tft.setTextColor(RED, BLACK);
			tft.print(F("noTip"));
			setError(NO_TIP);
		}
		else {
			if (cur_t_old == 999) {
				tft.fillRect(SpalteT[2], ReiheT[3], 72, 16, BLACK);
			}
			error = NO_ERROR;
			tft.setTextColor(off ? temperature < TEMP_COLD ? CYAN : RED : tft.color565(min(10,abs(temperature-target_t))*25, 250 - min(10,max(0,(abs(temperature-target_t)-10)))*25, 0), Hellblau);
			if (temperature < TEMP_COLD) {
				tft.print(F("COLD"));
			}
			else {
				tft.write(' ');

				printTemp(temperature);
			}
		}
		//------------------------------------------------------------------------
		// Anzeige Temperaturbalken
		//------------------------------------------------------------------------

		if (temperature < cur_t_old)
			tft.fillRect(max(0, (temperature - TEMP_COLD) / 2.4), 0, 160 - max(0, (temperature - TEMP_COLD) / 2.4), BAR_HEIGHT, BLACK);
		else if (cur_t != 999) {
			for (int16_t i = 0; i < max(0, (temperature - TEMP_COLD) / 2.4); i++) {
				tft.drawFastVLine(i, 0, BAR_HEIGHT, tft.color565(min(255, max(0, i * 5)), min(255, max(0, 450 - i * 2.5)), 0));
			}
		}
		cur_t_old = temperature;
	}
	/* 24 24 24 24 24 24 24 24 24 24 24 24 24 24 24 24 24 24 24 24 24 24 24 24
	* 
	  Anzeige der gemessenen Temperatur

	 24 24 24 24 24 24 24 24 24 24 24 24 24 24 24 24 24 24 24 24 24 24 24 24*/
	if (cur_t_old2 != temperature2 || force_redraw) {
		tft.setCursor(SpalteT[3], ReiheT[1]);
		tft.setTextSize(3);
		if (temperature2 == 999) {
			tft.setTextColor(RED, Hellblau);
			tft.print(F("ERR "));
			tft.setCursor(SpalteT[3], ReiheT[3]);
			tft.setTextSize(2);
			tft.setTextColor(RED, BLACK);
			tft.print(F("noTip"));
			setError2(NO_TIP);
		}
		else {
			if (cur_t_old2 == 999) {
				tft.fillRect(SpalteT[3], ReiheT[3], 72, 16, BLACK);
			}
			error2 = NO_ERROR;
			tft.setTextColor(off2 ? temperature2 < TEMP_COLD2 ? CYAN : RED : tft.color565(min(10, abs(temperature2 - target_t2)) * 25, 250 - min(10, max(0, (abs(temperature2 - target_t2) - 10))) * 25, 0), Hellblau);
			if (temperature2 < TEMP_COLD2) {
				tft.print(F("COLD"));
			}
			else {
				tft.write(' ');
				printTemp(temperature2);
			}
		}
		// 22222222222222222222222222222222222222222222222222
		// Anzeige Temperaturbalken
		// 22222222222222222222222222222222222222222222222222
		if (temperature2 < cur_t_old2)
			tft.fillRect(max(161, 161 + ((temperature2 - TEMP_COLD2) / 2.5)), 0, 320 - max(0, (temperature2 - TEMP_COLD2) / 2.5), BAR_HEIGHT, BLACK);

		else if (cur_t2 != 999) {
			for (int16_t i = 0; i < max(0, (temperature2 - TEMP_COLD2) / 2.5); i++) {
				tft.drawFastVLine((i + 160), 0, BAR_HEIGHT, tft.color565(min(255, max(0, i * 5)), min(255, max(0, 450 - i * 2.5)), 0));
			}
		}
		cur_t_old2 = temperature2;
	}
	force_redraw = false;
}
//#################  Ende Display ######################################
