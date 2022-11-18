#include <Arduino_LSM9DS1.h>  //Click here to get the library: http://librarymanager/All#Arduino_LSM9DS1
#include "arduinoFFT.h"
#include <stdlib.h>


//PRIVATE DEFINE
#define NumSamples 119
#define NumRecord 20


arduinoFFT FFT = arduinoFFT(); /* Create FFT object */

double Buffer_Ax[NumSamples * NumRecord];
double Buffer_Ay[NumSamples * NumRecord];
double Buffer_Az[NumSamples * NumRecord];

uint32_t Index_u32 = 0;
char InChar = 'C';
uint8_t OneEntry_u8 = 0;
uint8_t IndexSamples_u8 = 0;


void setup() {
  // put your setup code here, to run once:
  Serial.begin( 115200 );
  while ( !Serial );
  // initialize the IMU
  if ( !IMU.begin() )
  {
    Serial.println( "Failed to initialize IMU!" );
    while( 1 );
  }
  // print out the samples rates of the IMUs
  Serial.print("Accelerometer sample rate = ");
  Serial.print(IMU.accelerationSampleRate());
  Serial.println(" Hz");
  Serial.println("Ax,Ay,Az");
}

void Bay_print( void )
 {
  uint32_t Index_u32 = 0;
  while ( Index_u32 < NumRecord * NumSamples ) 
  {
    Serial.print( Buffer_Ax[Index_u32] );
    Serial.print(",");
    Serial.print( Buffer_Ay[Index_u32] );
    Serial.print(",");
    Serial.print( Buffer_Az[Index_u32] );
    Serial.print("\r\n");
    Index_u32++;
  }
}

void loop() 
{
  // put your main code here, to run repeatedly:
  /* Data Acquisition */
  float Ax, Ay, Az;
  while ( ( Index_u32 < ( NumSamples * NumRecord ) ) && ( OneEntry_u8 == 0 ) ) 
  {
      // read the acceleration data
      uint64_t next_tick = micros() + ( 500 / 119.0 );
      IMU.readAcceleration( Ax, Ay, Az );
      Buffer_Ax[ Index_u32 ] = ( Ax );
      Buffer_Ay[ Index_u32 ] = ( Ay );
      Buffer_Az[ Index_u32 ] = ( Az );
      Index_u32++;
      if( next_tick > micros() )
      {
        delayMicroseconds( next_tick - micros() );
      }  
  }
  //After collecting data type caracter C to print data collected in terminal
  if ( OneEntry_u8 == 0U ) 
  {
    while ( InChar != Serial.read() );
    Bay_print();
  }
  OneEntry_u8 = 1U;
}