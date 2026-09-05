#ifndef ADCTOUCHSENSOR_h
#define ADCTOUCHSENSOR_h

#include "Arduino.h"

#if defined(ARDUINO_GENERIC_STM32F103C) 
# define ADCTOUCH_STM32_GROUND_CHANNEL  15 
# define ADCTOUCH_STM32_ADC             ADC1
# define ADCTOUCH_INTERNAL_GROUNDING
#endif

#if defined(ARDUINO_ARCH_AVR)
# define ADCTOUCH_INTERNAL_GROUNDING
# define INPUT_ANALOG INPUT
#endif

// Kiểm tra nếu là kiến trúc STM32 nói chung
#if defined(ARDUINO_ARCH_STM32) || defined(ARDUINO_GENERIC_STM32F103C) || defined(ARDUINO_ARCH_STM32F1)
  #define ADCTOUCH_DEFAULT_DELAY 20
  #define ADCTOUCH_DIVIDER        4
// Kiểm tra nếu là kiến trúc AVR (Uno, Nano...)
#elif defined(ARDUINO_ARCH_AVR)
  #define ADCTOUCH_DEFAULT_DELAY 0
  #define ADCTOUCH_DIVIDER        1
// Nếu là các board khác (phòng hờ) thì vẫn cho giá trị mặc định để khỏi lỗi
#else
  #define ADCTOUCH_DEFAULT_DELAY 20
  #define ADCTOUCH_DIVIDER        1
#endif

class ADCTouchSensor
{
    private:
        float reference;
        int groundedPin;
        int touchPin;
        int touchDigitalPin;
        unsigned delayTime;
        bool valid;

    private:
        void groundPortable();
#ifdef ADCTOUCH_INTERNAL_GROUNDING        
        void ground();
#endif
    
	public:
        ADCTouchSensor(int pin, int sacrificialPin=-1, unsigned delayTimeMicroseconds=ADCTOUCH_DEFAULT_DELAY);
        bool begin(unsigned samples=500);
        float read(unsigned samples = 5);
        float readRaw(unsigned samples = 5);
        float readRaw_new(unsigned samples = 5);
        float readRaw_new_2(unsigned samples = 5);
        float readRaw_new_3(unsigned samples = 5);
        int readRaw1();
        int readRaw1_new();
        int readRaw1_new_2();
        int readRaw1_new_3();
};
#endif