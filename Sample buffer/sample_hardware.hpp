/*  ELEC351 Coursework 2019
    Low power environmental sensor
    Ben Jasper & Dean Hunt   */

#include "mbed.h"

#define DATA_BUFFER_SIZE 600

//External voids and variables
extern bool serialLoggingAllowed;
extern void queueSD(int status);
extern void mountSD(void);
extern void request_sd_activity(void);
extern void flushToSD(void);
extern void ejectSD(void);
extern EventQueue lpQueue;
extern Ticker takeSamples;
extern void updateSampleRate(float rate);
extern void dataSamplingStatus(bool status);
extern void dataLoggingStatus(bool status);
extern void oneTimeSerialData(void);
extern void serialReadBuffer(void);
extern void printTimeStamp(time_t);
extern void runSamples(void);
extern void startup_init(void);
#ifndef __sample_hardware__
#define __sample_hardware__

//#define BME
#ifdef BME
#include "BME280.h"
#else
#include "BMP280.h"
#endif
#include "TextLCD.h"
#include "SDBlockDevice.h"
#include "FATFileSystem.h"

enum ELEC350_ERROR_CODE {OK, FATAL};

extern DigitalOut onBoardLED;
extern DigitalOut redLED;
extern DigitalOut yellowLED;
extern DigitalOut greenLED;

extern InterruptIn onBoardSwitch;
//extern Serial pc;
extern AnalogIn adcIn;

#ifdef BME
extern BME280 sensor;
#else
extern BMP280 sensor;
#endif

extern TextLCD lcd;
extern SDBlockDevice sd;

extern void post();
extern void errorCode(ELEC350_ERROR_CODE err);

#endif