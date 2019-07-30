/*  Example reading an MMA7660 accelerometer
    using I2C communication without blocking audio synthesis,
    using Mozzi sonification library.

    Demonstrates use of twi_nonblock functions
    to replace processor-blocking Wire methods.

    This is only a minimum demonstration fragment with no handler for MMA7660 errors.

    Note: The twi_nonblock code is not compatible with Teesy 3.1.

    Circuit: Audio output on digital pin 9.

		Mozzi documentation/API
		https://sensorium.github.io/Mozzi/doc/html/index.html

		Mozzi help/discussion/announcements:
    https://groups.google.com/forum/#!forum/mozzi-users

    Original ADXL345 sample code by Marije Baalman 2012.
    Small modifications by Tim Barrass to retain Mozzi compatibility.
    Converted from ADXL345 to MMA7660 by Roger Cheng

    This example code is in the public domain.
*/

#include <MozziGuts.h>
#include <Oscil.h> // oscillator template
#include <Ead.h> // exponential attack decay
#include <tables/sin2048_int8.h> // sine table for oscillator
#include <twi_nonblock.h>

#define CONTROL_RATE 128 // Hz, powers of 2 are most reliable

// use: Oscil <table_size, update_rate> oscilName (wavetable), look in .h file of table #included above
Oscil <SIN2048_NUM_CELLS, AUDIO_RATE> aSin(SIN2048_DATA);
Ead kEnvelope(CONTROL_RATE); // resolution will be CONTROL_RATE

int gain;

static volatile byte acc_status = 0;
#define ACC_IDLE 0
#define ACC_READING 1
#define ACC_WRITING 2

byte accbytedata[3];

#define MMA7660_ADDR  0x4c

#define MMA7660_X     0x00
#define MMA7660_MODE  0x07
  #define MMA7660_STAND_BY 0x00
  #define MMA7660_ACTIVE   0x01
#define MMA7660_SR    0x08      //sample rate register
  #define AUTO_SLEEP_32   0X02

void setup_accelero(){
  initialize_twi_nonblock();

  acc_writeTo(MMA7660_MODE, MMA7660_STAND_BY);
  acc_writeTo(MMA7660_SR, AUTO_SLEEP_32);
  acc_writeTo(MMA7660_MODE, MMA7660_ACTIVE);

  acc_status = ACC_IDLE;
}

/// ---------- non-blocking version ----------
void initiate_read_accelero(){
  // Reads num bytes starting from address register on device in to _buff array
  // indicate that we are transmitting
  //   transmitting = 1;
  // set address of targeted slave
  txAddress = MMA7660_ADDR;
  // reset tx buffer iterator vars
  txBufferIndex = 0;
  txBufferLength = 0;

  // put byte in tx buffer
  txBuffer[txBufferIndex] = MMA7660_X; // Start reading from register for X
  ++txBufferIndex;
  // update amount in buffer
  txBufferLength = txBufferIndex;

  twi_initiateWriteTo(txAddress, txBuffer, txBufferLength);
  acc_status = ACC_WRITING;
}

void initiate_request_accelero(){
  // reset tx buffer iterator vars
  txBufferIndex = 0;
  txBufferLength = 0;
  // indicate that we are done transmitting
  //   transmitting = 0;

  byte read = twi_initiateReadFrom(MMA7660_ADDR, 3);
  acc_status = ACC_READING;
}

void finalise_request_accelero(){
  byte read = twi_readMasterBuffer( rxBuffer, 3 );
  // set rx buffer iterator vars
  rxBufferIndex = 0;
  rxBufferLength = read;

  byte i = 0;
  while( rxBufferLength - rxBufferIndex > 0)         // device may send less than requested (abnormal)
  {
    accbytedata[i] = rxBuffer[rxBufferIndex];
    ++rxBufferIndex;
    i++;
  }

  acc_status = ACC_IDLE;
}
/// ----end------ non-blocking version ----------

// Writes val to address register on device
void acc_writeTo(byte address, byte val) {
  //   Wire.beginTransmission(MMA7660_ADDR); // start transmission to device
  twowire_beginTransmission(MMA7660_ADDR); // start transmission to device
  //   Wire.send(address);             // send register address
  twowire_send( address );
  //   Wire.send(val);                 // send value to write
  twowire_send( val );
  //   Wire.endTransmission();         // end transmission
  twowire_endTransmission();
}


void setup(){
  setup_accelero();
  startMozzi(CONTROL_RATE);
  aSin.setFreq(800);
  int attack = 30;
  int decay = 500;
  kEnvelope.set(attack,decay);
}


int8_t accx;
int8_t accy;
int8_t accz;

int envgain;

void updateControl(){
  envgain = (int) kEnvelope.next();
  if ( envgain < 5 ){
    if ( accx > 32){
      aSin.setFreq( 400 );
      kEnvelope.start();
    }
    else if ( accy > 32 ) {
      aSin.setFreq( 800 );
      kEnvelope.start();
    }
    else if ( accz > 32 ) {
      aSin.setFreq( 1200 );
      kEnvelope.start();
    }
  }

  switch( acc_status ){
  case ACC_IDLE:
    accx = accbytedata[0]; // accelerometer x reading
    accy = accbytedata[1]; // accelerometer y reading
    accz = accbytedata[2]; // accelerometer z reading
    initiate_read_accelero();
    break;
  case ACC_WRITING:
    if ( TWI_MTX != twi_state ){
      initiate_request_accelero();
    }
    break;
  case ACC_READING:
    if ( TWI_MRX != twi_state ){
      finalise_request_accelero();
    }
    break;
  }
  gain = envgain;
}


int updateAudio(){
  return (aSin.next()*gain)>>8;
}


void loop(){
  audioHook(); // required here
}
