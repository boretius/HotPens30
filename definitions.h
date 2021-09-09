#define MELDESTRING		"HotPens "
#define VERSION			"2.1"

#define BAR_HEIGHT       5 //Should be no bigger than 5

/*
 * TIPS ARE SPECIFIED FOR 450 DEGREE MAX
 * If read 1023 on Analog in, the tip is turned off automatically
 */
#define TEMP_MAX    450
#define TEMP_MIN    199
#define TEMP_STBY   150
#define TEMP_COLD   (adc_offset + 15)
#define TEMP_COLD2  (adc_offsetB + 15)
#define KALO_MAX	49.9
#define KALG_MAX	1.9
#define KAL_MIN		0.25

#define SHUTOFF_ACTIVE
#define BOOTHEAT_ACTIVE

#define STANDBY_TIMEOUT 240 // seconds without any significant temperature drop, if exceeded it will standby
#define OFF_TIMEOUT     900 // seconds in standby before turning off

#define TEMP_RISE             30 //threshold temperature, that must be exceeded delta in given time:
#define TEMP_RISE2            15 //threshold temperature, that must be exceeded delta in given time:
#define TEMP_UNDER_THRESHOLD  200 // x (TIME_COMPUTE_IN_MS + DELAY_BEFORE_MEASURE)
#define TEMP_UNDER_THRESHOLD2 200 // x (TIME_COMPUTE_IN_MS + DELAY_BEFORE_MEASURE)
#define THRES_MAX_DECEED       2 //max times the threshold temperature may be undercut by the current temperature
#define THRES_MAX_DECEED2      10 //max times the threshold temperature may be undercut by the current temperature


//Temperature in degree to rise at least in given time
#define TEMP_MIN_RISE         10
//Time in that the temperature must rise by the set temperature
#define TEMP_RISE_TIME      1000
//#################################################################
#define TIRQ_PIN	2 
#define HEATER_PWM  3 // ! OUT
#define TASTER_PIN  4 // Push-Button INT
#define HEATER_PWM2 5 // ! OUT Kanal2 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#define HELLIGKEIT	6
#define T_CS_PIN	7 // temp H fuer Touch sonst ist Display hell
#define TFT_DC		8
#define TFT_CS      10 //!
#define Rlinks		A2
#define Rrechts		A3
#define TEMP_SENSE  A6
#define TEMP_SENSE2 A7 // Kanal2 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

//#define Relais		A2 
//#define LED			A0
//#define LED2		A1
//#################################################################

#define kp12s          2	//2
#define ki12s          0.01
#define kd12s          0.1	// 0.2

#define kp24s          2
#define ki24s          0.01
#define kd24s          0.0
#define MESSREIHEN	5
#define TIME_COMPUTE_IN_MS          50		// 10 Heizzeit
#define DELAY_BEFORE_MEASURE        5		// Wartezeit vor Messung
#define DELAY_MAIN_LOOP             100		// wartezeit zur Wertübernahme

#define ADC_TO_TEMP_GAIN            0.34 //default value if no calibration is performed
#define ADC_TO_TEMP_OFFSET			35.20  //default value if no calibration is performed
#define ADC_TO_TEMP_GAINB           0.41 //default value if no calibration is performed
#define ADC_TO_TEMP_OFFSETB         24.81  //default value if no calibration is performed

/************************EEPROM Adressen************************************/
#define EEPROM_STBY		1
#define EEPROM_STBY2	2
#define EEPROM_W40		3		// aktiver Kanal
#define EEPROM_Dimmerspeicher 4
#define EEPROM_KAL4_IDX	5		// Zeiger auf die aktuellen Kalibrier-Werte Gain und Offset
#define EEPROM_KAL5_IDX	6		// Zeiger auf die aktuellen Kalibrier-Werte Gain und Offset
#define EEPROM_12SF		7		// Regelcharakteristik 		
#define EEPROM_24SF		8
#define EEPROM_VERSION	9		// Meldestring?? 11 Bytes
#define BASIS			(EEPROM_VERSION + 11)	// Basisadresse für das Array "Temperaturwahl (stored = int16)" 16 bytes
#define ADC_BASIS_GN	(BASIS + 16)
#define ADC_BASIS_OF	(ADC_BASIS_GN + sizeof(adc_gain))
#define ADC_BASIS_GNB	(ADC_BASIS_OF + sizeof(adc_offset))
#define ADC_BASIS_OFB	(ADC_BASIS_GNB + sizeof(adc_gainB))


/***************************************************************************/

#define Grau            0xC618
#define Hellblau        0x441f
#define Schwarz         0x0000
#define Dunkelgrau      0x39E7
#define AQUA	0x5D1C		// blau
#define	BLACK   0x0000
#define	BLUE    0x001F
#define	RED     0xF800
#define	GREEN   0x07E0
#define GREENYELLOW 0xAFE5 
#define CYAN    0x07FF		// hell
#define DARKGREY    0x7BEF 
#define DARKGREEN   0x03E0 
#define DARKCYAN    0x03EF	// sehr dunkel
#define LIGHTGREY   0xC618
#define MAGENTA 0xF81F	//pink
#define MAROON      0x7800	// sehr dunkel
#define NAVY        0x000F 
#define OLIVE       0x7BE0  
#define ORANGE      0xFD20	// dunkel
#define PINK        0xF81F	// Magenta leuchtender	ili-PINK: heller-Pink
#define PURPLE      0x780F	// sehr dunkel
#define OCKER	0xFE00
#define BRAUN	0xFC00
#define TEST	0xFF00
#define YELLOW  0xFFE0	// hell  
#define WHITE   0xFFFF
#define GRAY    0x94B2
#define DimmerMax 24
#define PRESET_KAL	40	// Pause zum hochzählen des Kalibrier-Vorgabewertes

typedef enum ERROR_TYPE {
	NO_ERROR,
	EXCESSIVE_FALL,
	NOT_HEATING,
	NO_TIP,
	BATTERY_LOW,
	USB_ONLY
} error_type;

/**************************** Setup ***************************************/
String Meldung = String(MELDESTRING VERSION);		// 11 Bytes
boolean  stby = true;
boolean  stby2 = true;
boolean W40 = true;  // festlegen des aktiven Kanals //
uint8_t aktives_Feld_T = 0; // aktives Feld Hauptmenü merkt gewählte Speichertemperatur 
//uint8_t aktives_Feld_R = 0; // aktives Feld Untermenü dito 150W
//uint8_t aktives_Feld_K = 0; // aktives Feld Kalmenü
uint8_t	Kal40_IDX = 0;	// aktuelle Gain und Offset-Werte 0;1;2;3
uint8_t	Kal150_IDX = 0;	// aktuelle Gain und Offset-Werte 0;1;2;3
boolean Kal40o;			// zur Darstellung im Kal-Menü
boolean Kal150o;		// o=Obere Button-Reihe
boolean Kal40l;			// linke Position
boolean Kal150l;
boolean regelChr12slow;
boolean regelChr24slow;
int16_t stored[4] = { TEMP_MIN, 290, 330, 405 };	// 8
int16_t stored2[4] = { TEMP_MIN, 291, 331, 410 };	// 8
float adc_offset = ADC_TO_TEMP_OFFSET;
float adc_gain = ADC_TO_TEMP_GAIN;
float adc_offsetB = ADC_TO_TEMP_OFFSETB;
float adc_gainB = ADC_TO_TEMP_GAINB;

/************************ Arbeitsvariablen*******************************/
float kp12 = kp12s;
float ki12 = ki12s;
float kd12 = kd12s;
float kp24 = kp24s;
float ki24 = ki24s;
float kd24 = kd24s;
volatile boolean off = true;
volatile boolean off2 = true;
volatile boolean stby_layoff = false;
boolean UMenu = false;
boolean PushB = false;  // toggle für Kalibrierung
boolean Kal12V = false; // Einsprung in die Kalibrierung
boolean Kal24V = false; // Einsprung in die Kalibrierung
boolean StartKal = false; // Anzeige der Kal-Parameter im Kal-Menü
boolean KalRdy = false;		// Abkühlung auf Kaltschwelle abwarten
boolean KalKalt = false;		// Abkühlung auf Kaltschwelle abwarten
boolean KalWarm = false;		// Abkühlung auf Kaltschwelle abwarten
boolean setOffset;		// für Farbdarstellung im Kalibrierungsbildschirm
boolean wasOff = true, old_stby = false;
boolean wasOff2 = true, old_stby2 = false;
boolean autopower_repeat_under = false, autopower_repeat_under2 = false;
boolean force_redraw = false;
boolean Strom;
//boolean force_menu = false;
volatile uint16_t pwm, threshold_counter, pwm_old;
volatile uint16_t pwm2, threshold_counter2, pwm2_old;
volatile int16_t cur_t, last_measured;
volatile int16_t cur_t2, last_measured2; // Kanal2
uint16_t adcValue = 0;
uint16_t adcValue2 = 0;
uint16_t adc_12,  adc_24;
//int16_t set_0;										// 2
int16_t set_t = TEMP_MIN, set_t_old, cur_t_old, target_t;
int16_t set_t2 = TEMP_MIN, set_t_old2, cur_t_old2, target_t2;
uint32_t sendNext;
uint16_t cnt_compute, cnt_compute2;
uint32_t last_temperature_drop, last_temperature_drop2; // Timeout-WakeUp
uint32_t last_on_state, last_on_state2; // Timeout-WakeUp
double pid_val, cur_td, set_td;
double pid_val2, cur_td2, set_td2;
float setko, setkg;			// Drehregler Kalibrierung Gain und Offset
float  v = 5;	// für pwm					// Versorgungsspannung ist fest
volatile error_type error = NO_ERROR, error2 = NO_ERROR;
/*******************************************************************************************/
uint8_t	preset_Kal; // Pause zum hochzählen des Kalibrier-Vorgabewertes
boolean preset_Kal_stop;
uint8_t Dimmerspeicher = 0;
uint8_t aktives_Feld = 0;
uint8_t aktives_Feld_alt;
uint8_t inaktives_Feld = 0;
uint8_t inaktives_Feld_alt = 0;
constexpr auto clrh = 21;
constexpr auto clrl = 55;
//uint8_t zeilenschritt = 59; // 32 entspricht der Zeichenhöhe 4
//uint8_t spaltenschritt = 79; // 24 entspricht der Zeichenbreite 4
uint8_t Touch_Zeile_vorher = 2;
uint8_t Touch_Spalte_vorher = 1;
uint8_t Touch_Zeile = 0;
uint8_t Touch_Spalte = 0;
const int Rectb = 76;
const int Recth = 56;
uint16_t SpalteM[] = { 0,1,81,161,241 };	// Zeichne Buttons
uint16_t ReiheM[] = { 0,4,61,121,181 };
uint16_t SpalteTs[] = { 0,20,91,174,263 };	// Anzeige Temperaturspeicher im Button	
uint16_t ReiheTs[] = { 0,19,84,144,204 };
uint16_t SpalteT[] = { 0,12,85,165,255 };	// sonstiger Text im Button
uint16_t ReiheT[] = { 0,20,78,138,198 };
#define Dz2 = 18; // delta Zeilenpixel bei Textgröße2
//uint16_t CursorZ2[] = { 0,18,36,54,72,90,108,126,144,162 };
//uint16_t CursorZ2[] = { 0,20,40,60,80,100,120,140,160,180 };
//uint16_t CursorZ2[] = { 0,22,44,66,88,110,132,154,176,198 };
uint16_t CursorZ2[] = { 0,23,46,69,92,115,138,161,184,207 };
/*******************************************************************************************/
uint8_t rotaryCount = 0;
uint8_t rotaryCount_alt;
uint8_t newValue;
boolean DimmButton = false;	// nur zum Start die Stellwerte Drehregler übernehmen
const int ROTARY_PIN_A = Rrechts; // the first pin connected to the rotary encoder
const int ROTARY_PIN_B = Rlinks; // the second pin connected to the rotary encoder
uint8_t lastLoopDisplayedRotaryCount = 0;
boolean DrehrichtungRechts;
uint16_t set; // Drehregler Temperatureinstellung
long encoderValue = 250; // Startwert
/*******************************************************************************************/
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

::SimpleHacks::QDecoder qdec(ROTARY_PIN_A, ROTARY_PIN_B, true); // the library class...
// Stores a relative value based on the clockwise / counterclockwise events

PID heaterPID(&cur_td, &pid_val, &set_td, kp12, ki12, kd12, DIRECT);
PID heaterPID2(&cur_td2, &pid_val2, &set_td2, kp24, ki24, kd24, DIRECT);

XPT2046_Touchscreen ts(T_CS_PIN, TIRQ_PIN);  // Param 2 - Touch IRQ Pin - interrupt enabled polling
/*********************************************************************************************/
