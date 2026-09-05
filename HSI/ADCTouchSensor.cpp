#include "Arduino.h"
#include "ADCTouchSensor.h"

ADCTouchSensor::ADCTouchSensor(int pin, int sacrificialPin, unsigned delayTimeMicroseconds) {
    touchPin = pin;
    touchDigitalPin = pin;   
    reference = 0;
    groundedPin = sacrificialPin;
    delayTime = delayTimeMicroseconds;
    
    if (sacrificialPin >= 0) {
        valid = true;
    }   
    else {
        valid = false; 
    }
}
inline void ADCTouchSensor::groundPortable() {
    pinMode(groundedPin, OUTPUT); 
    digitalWrite(groundedPin, 0);
    if (delayTime > 0)
        delayMicroseconds(delayTime);
    analogRead(groundedPin);
}

#if defined(ADCTOUCH_STM32_GROUND_CHANNEL)
inline void ADCTouchSensor::ground() {
    if (delayTime>0) 
        delayMicroseconds(delayTime);
    adc_read(ADCTOUCH_STM32_ADC, ADCTOUCH_STM32_GROUND_CHANNEL);
}

#elif defined(ARDUINO_ARCH_AVR)

inline void ADCTouchSensor::ground() {
#if defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
    ADMUX = (ADMUX & 0xF0) | 0b1101;
#else
    ADMUX |=   0b11111;
#endif
    if (delayTime>0)
        delayMicroseconds(delayTime);

    ADCSRA |= (1 << ADSC);
   
    while((ADCSRA & (1 << ADSC))) ;
}
#endif
int ADCTouchSensor::readRaw1() {
    if (!valid)
        return -10000; 

    pinMode(touchDigitalPin, INPUT_PULLUP);
    groundPortable();
    unsigned int ST=micros();
#if defined(ADCTOUCH_STM32_GROUND_CHANNEL)
    else {
        ground();
    }
#endif        
    pinMode(touchDigitalPin, INPUT_ANALOG);
    int adcValue = analogRead(touchPin); 
    return adcValue;
}

int ADCTouchSensor::readRaw1_new() {
    if (!valid)
        return -10000; 
        
    pinMode(touchDigitalPin, INPUT_PULLUP);
    groundPortable();
    
    pinMode(touchDigitalPin, INPUT_ANALOG);
    
    int adcValue = analogRead(touchPin); 
    
    return adcValue; 
}
int ADCTouchSensor::readRaw1_new_2() {
    if (!valid)
        return -10000; 
        
    pinMode(touchDigitalPin, INPUT_PULLUP);
    if (delayTime > 0)
        delayMicroseconds(delayTime);
    analogRead(groundedPin);
    
    pinMode(touchDigitalPin, INPUT_ANALOG);
    
    int adcValue = analogRead(touchPin); 
    
    return adcValue; 
}
int ADCTouchSensor::readRaw1_new_3() {
    if (!valid)
        return -10000; 
        
    pinMode(touchDigitalPin, OUTPUT);
    digitalWrite(touchDigitalPin, 1);
    if (delayTime > 0)
        delayMicroseconds(delayTime);
    analogRead(groundedPin);
    
    pinMode(touchDigitalPin, INPUT_ANALOG);
    
    int adcValue = analogRead(touchPin); 
    
    return adcValue; 
} 
float ADCTouchSensor::readRaw(unsigned samples) {
    if (!valid || samples == 0)
        return -10000;

    long total = 0; 
    
    for (unsigned i = 0; i < samples; i++) {
        int Value1 = readRaw1();
        
        if (Value1 == -10000) {
            return -10000;
        }
        
        total += Value1;
    }
    return (float)total / ADCTOUCH_DIVIDER / samples;
}
float ADCTouchSensor::readRaw_new(unsigned samples) {
    if (!valid || samples == 0)
        return -10000;

    long total = 0; 
    
    for (unsigned i = 0; i < samples; i++) {
        int Value1 = readRaw1_new();
        
        if (Value1 == -10000) {
            return -10000;
        }
        
        total += Value1;
    }
    return (float)total / ADCTOUCH_DIVIDER / samples;
}
float ADCTouchSensor::readRaw_new_2(unsigned samples) {
    if (!valid || samples == 0)
        return -10000;

    long total = 0; 
    
    for (unsigned i = 0; i < samples; i++) {
        int Value1 = readRaw1_new_2();
        
        if (Value1 == -10000) {
            return -10000;
        }
        
        total += Value1;
    }
    return (float)total / ADCTOUCH_DIVIDER / samples;
}
float ADCTouchSensor::readRaw_new_3(unsigned samples) {
    if (!valid || samples == 0)
        return -10000;

    long total = 0; 
    
    for (unsigned i = 0; i < samples; i++) {
        int Value1 = readRaw1_new_3();
        
        if (Value1 == -10000) {
            return -10000;
        }
        
        total += Value1;
    }
    return (float)total / ADCTOUCH_DIVIDER / samples;
}
bool ADCTouchSensor::begin(unsigned samples) {
    if (valid) {
        reference = readRaw(samples);
        return true;
    }
    else
        return false;
}

float ADCTouchSensor::read(unsigned samples) {
    if (valid)
        return readRaw(samples) - reference;
    else
        return -10000;
}