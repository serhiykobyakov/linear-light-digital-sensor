// Photodiode + TIA + ADC
// Version 11.09.2022
//
// (C) Serhiy Kobyakov


//
//  |ADS1220 pin label| Pin Function         |Arduino Connection|
//  |-----------------|:--------------------:|-----------------:|
//  | DRDY            | Data ready Output pin|  D6              |
//  | MISO            | Slave Out            |  D12             |
//  | MOSI            | Slave In             |  D11             |
//  | SCLK            | Serial Clock         |  D13             |
//  | CS              | Chip Select          |  D7              |
//  | DVDD            | Digital VDD          |  -               |
//  | DGND            | Digital Gnd          |  Gnd             |
//  | AN0-AN3         | Analog Input         |  -               |
//  | AVDD            | Analog VDD           |  -               |
//  | AGND            | Analog Gnd           |  -               |
//

#include <stdint.h>
#include <Protocentral_ADS1220.h>
#include <SPI.h>

#define precis 6 // how many numbers after decimal separator I want to print

// TIA
#define Rf 10690000.0 // Rc in TIA (~10M)

// ADC
#define VREF 5.0
float Vref = VREF;
#define VFSR VREF/PGA
#define FSR (((long int)1<<23)-1)
#define ADS1220_CS_PIN    7
#define ADS1220_DRDY_PIN  6

Protocentral_ADS1220 pc_ads1220;
int32_t adc_data;
float Sum = 0.0f;
int32_t lastVal = 0;
int32_t tmpVal = 0;
float Res = 0.0f;
long counter = 0;
bool Measure = false;


void setup() {
  pinMode(ADS1220_DRDY_PIN, INPUT_PULLUP);  

  Serial.begin(115200);
  
  pc_ads1220.begin(ADS1220_CS_PIN,ADS1220_DRDY_PIN);
  pc_ads1220.set_data_rate(DR_20SPS);
  pc_ads1220.set_pga_gain(PGA_GAIN_1);
  pc_ads1220.PGA_OFF();
  pc_ads1220.select_mux_channels(MUX_SE_CH2);
  pc_ads1220.writeRegister(0x02, B11010000);
// AABBCDDD
// AA: Voltage reference selection
//    00 Internal 2.048-V reference selected (default)
//    01 External reference selected using dedicated REFP0 and REFN0 inputs
//    10 External reference selected using AIN0/REFP1 and AIN3/REFN1 inputs
//    11 Analog supply (AVDD – AVSS) used as reference
//
// BB: FIR filter configuration
//    00 No 50-Hz or 60-Hz rejection (default)
//    01 Simultaneous 50-Hz and 60-Hz rejection
//    10 50-Hz rejection only
//    11 60-Hz rejection only
//
// C: Low-side power switch configuration
//    0 : Switch is always open (default)
//    1 : Switch automatically closes when the START/SYNC command is sent
//      and opens when the POWERDOWN command is issued
//
// DDD: IDAC current setting
//    000: Off (default)
//

  pc_ads1220.set_conv_mode_continuous();          //Set continuous conversion mode
  delay(25);
  pc_ads1220.Read_Data_Samples();  // read the first value which is "-1"

  delay(20);
  Serial.println("Ready!"); // Use it for boards with non-native USB ports
}


void setGain() {
  delay(5);
  if (Serial.available() > 0) {
    char newGain = (char)Serial.read();
    delay(20);
    switch (newGain) {
     case '1':
       pc_ads1220.set_pga_gain(PGA_GAIN_1);
       pc_ads1220.PGA_OFF();
       pc_ads1220.writeRegister(0x02, B11010000);
       Vref = VREF;
       Serial.println("1");
       break;
         
     case '2':
       pc_ads1220.set_pga_gain(PGA_GAIN_2);
       pc_ads1220.PGA_OFF();
       pc_ads1220.writeRegister(0x02, B11010000);
       Vref = VREF/2;
       Serial.println("2");
       break;
         
     case '4':
       pc_ads1220.set_pga_gain(PGA_GAIN_4);
       pc_ads1220.PGA_OFF();
       pc_ads1220.writeRegister(0x02, B11010000);
       Vref = VREF/4;
       Serial.println("4");
       break; 
        
     default:
       Serial.println("0");
       break;    
    }
  }
}


void loop() {
 if (Serial.available() > 0 ) { 
  char char_in = (char)Serial.read();
    
  switch (char_in) {
// "?" - identification
    case '?':
        //delay(4);
        Serial.println("Photodiode");
        break;

// "v" - start gathering values into Sum
    case 'v':
        Measure = true;
        Sum = (float)(lastVal);  
        counter = 1;
        Serial.println("v");
        break;
 
// "w" - stop gathering values and report
    case 'w':
        if (Measure) {
          Measure = false;
          Res = (float)((Sum*Vref*100)/FSR);
          counter = 0;
          Sum = 0.0f;
        }
        else {
          Res = (float)((lastVal*Vref*100)/FSR);
        }
        Serial.println(Res, precis);
        break; 
 
// "A" - get FSR
    case 'A':
        Serial.println(FSR);
        break;

// "R" - get Resistance value
    case 'R':
        Serial.println(Rf);
        break;

// "V" - get Vref
    case 'V':
        Serial.println(Vref, precis);
        break;
          
// "g" - gain
    case 'g':
        setGain();
        break;
        
    default:
        break;        
  }
 }
 
 tmpVal = pc_ads1220.Read_WaitForData();   
 if (tmpVal > 0) {
   lastVal=tmpVal;
   if (Measure) {
     Sum = Sum + ((float)lastVal - Sum)/counter;
     counter++;
   }
 }  

  
} // end of main loop
