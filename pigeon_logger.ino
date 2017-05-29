
/*
 * Data_0 -> PC0 PIN_23/ADC0
 * Data_1 -> PC1 PIN_24/ADC1
 * Data_2 -> PC2 PIN_25/ADC2
 * Data_3 -> PC3 PIN_26/ADC3
 * Data_4 -> PC4 PIN_27/ADC4
 * Data_5 -> PC5 PIN_24/ADC5
 * 
 * Data_6 -> PB0 PIN_14/ICP
 * Data_7 -> PB1 PIN_15/OC1
 * 
 * StrobePin -> PD2 PIN_4/INT0
 */
byte Data = 0;
byte TempData = 0;
const int StrobePin = 2; //PD2 PIN_4/INT0 
const int BusyPin = 7; //PD7 PIN_13/AIN1
const int AcknowledgePin = 6; //PD6 PIN_12/AIN0
// Optional for SW control or tight to GND permanently. Can be used to block printing in case if SD Card is full.
const int PaperEndPin = 5; //PD5 PIN_11/T1 


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); 
  // prints title with ending line break 
  Serial.println("Setup starting");

  // The Data Direction Register of Port B. Port B bits [7:6] configured as inputs.
  // Set pull-up's
  PORTB = (1<<PB7)|(1<<PB6);
  // Configure as inputs
  DDRB = DDRB & B00111111 ;
  // The Data Direction Register of Port C. Port C bits [5:0] configured as inputs.
  // Set pull-up's
  PORTC = (1<<PC5)|(1<<PC4)|(1<<PC3)|(1<<PC2)|(1<<PC1)|(1<<PC0);
  // Configure as inputs
  DDRC = DDRC & B11000000 ;
  
  pinMode(StrobePin, INPUT_PULLUP);
  pinMode(BusyPin, OUTPUT);
  pinMode(AcknowledgePin, OUTPUT); 
  pinMode(PaperEndPin, OUTPUT);      
  
  // Set initial values on lines
  digitalWrite(BusyPin, LOW);
  digitalWrite(AcknowledgePin, HIGH);
  digitalWrite(PaperEndPin, LOW);
  
  // Enable IRQ for StrobePin at the end of setup.
  attachInterrupt(digitalPinToInterrupt(StrobePin), getOneChar, FALLING);

}

void loop() {
  // put your main code here, to run repeatedly:
  if(TempData != Data) {
    //Serial.println("Setup starting");
    Serial.print(Data); 
    TempData = Data;
    }
}

void getOneChar() {  
  Data = 0;
  // Set Busy line.
  digitalWrite(BusyPin, HIGH);
  // Wait 1ms before read data
  delay(1);
  // Get 8bit data.
  Data = Data | (PINB & B11000000); //Bits [7:6]
  Data = Data | (PINC & B00111111); //Bits [5:0]
  // Wait 1ms before Acknowledge.
  delay(1);
  digitalWrite(AcknowledgePin, LOW);
  delay(1);
  digitalWrite(AcknowledgePin, HIGH);
  digitalWrite(BusyPin, LOW);
}
