#ifndef _DCMOTOR_H
#define _DCMOTOR_H

// Header file to implement class for controlling DC Motors

#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)

//Code in here will only be compiled if an Arduino Mega is used.
const int MAXNUMPWMPINS = 13;
const int BOTTOMPWMPIN = 2;
#endif 

#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__)

//Code in here will only be compiled if an Arduino Uno (or older) is used.
const int MAXNUMPWMPINS = 8;
const int BOTTOMPWMPIN = 2;
#endif 

// Type definition for the motor states. Defined as follows:
// Idling - the motor is in the OFF state. Current is not being supplied
// Running - the motor is operational at the duty cycle configured in dutyCycle
// Invalid - invalid state that results in the initialization flag being set to false
//           All motor operations are prohibited in this state
typedef enum MotorState
{
  IDLING, 
  RUNNING, 
  INVALID
};

// Constant defining the conversion factor for converting the input percentage (given as % out of 100)
// to a range of 0 - 255 that can be used for the analog PWM write operation
const float PWMCONVERSIONFACTOR = 2.55;

class DCMotor
{
  private:
    int pin;
    int dutyCycle;
    MotorState state;
    bool initialized = false;

  public:

    // Constructor for specifying pin associated with the motor output
    DCMotor(int nPin)
    {
      // Check the pin for the available range
      if (nPin >= BOTTOMPWMPIN && nPin <= MAXNUMPWMPINS)
      {
        // Input value passes check so assign the pin
        pin = nPin;

        // Set the pin mode to output
        pinMode(pin, OUTPUT);

        // Set the duty cycle to initial value of 0
        dutyCycle = 0;

        // Ensure that the motor is in the stopped state
        Stop();
                
        // Set the initialization flag and initial state to IDLING
        state = IDLING;
        initialized = true;        
      }
    }

    // Constructor for specifying pin associated with the motor output as well as configured duty cycle
    DCMotor(int nPin, int DutyCycle)
    {
      // Check the pin for the available range
      if (nPin >= BOTTOMPWMPIN && nPin <= MAXNUMPWMPINS)
      {
        // Input value passes check so assign the pin
        pin = nPin;

        // Set the pin mode to output
        pinMode(pin, OUTPUT);        

        // Set the duty cycle to the given input value converted using the conversion factor
        dutyCycle = (int)(DutyCycle * PWMCONVERSIONFACTOR);

        // Ensure that the motor is in the stopped state
        Stop();
        
        // Set the initialization flag and initial state to IDLING
        state = IDLING;
        initialized = true;        
      }
    }

    // Method for modifying the configured duty cycle of the 
    void setDutyCycle(int DutyCycle)
    {
      // Check for the initialization
      if (initialized)
      {
        // Check the input value 
        if (DutyCycle >= 0 && DutyCycle <= 100)
        {
          dutyCycle = (int)(DutyCycle * PWMCONVERSIONFACTOR);
        }
      }
    }

    // Method that causes the motor to become operational at the currently configured duty cycle.
    // Writes the current duty cycle to the PWM analog output
    void Run()
    {
      // Check the initialization
      if (initialized)
      {
        // Now just be safe and check the duty cycle against the valid range
        if (dutyCycle >= 0 && dutyCycle <= 255)
        {
          analogWrite(pin, dutyCycle);

          // Set the current state to running
          state = RUNNING;
        }
      }
    }

    // Method that causes the motor to cease operational mode and enter an idle state
    // Writes a value of 0 through the analog write
    void Stop()
    {
      // No need to check the initialization as this forces the motor to a safe state
      // Write the PWM and set the state
      analogWrite(pin, 0);
      state = IDLING;
    }

    // Method to determine if the motor is currently running
    // Checks the state variable for the RUNNING value
    bool isRunning()
    {
      return state == RUNNING;
    }

    // Method to determine if the motor is currently idling
    // Checks the state variable for the IDLING value
    bool isIdle()
    {
      return state == IDLING;
    }
  
};


#endif
