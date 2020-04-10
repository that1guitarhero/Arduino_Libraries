#ifndef _RELAY_H    
#define _RELAY_H   

// Header file to implement a Relay class for controlling various relays

#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)

//Code in here will only be compiled if an Arduino Mega is used.
const int MAXNUMPINS = 53;
#endif 

#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__)

//Code in here will only be compiled if an Arduino Uno (or older) is used.
const int MAXNUMPINS = 16;
#endif 


// Enumerated type definition for Relay State

typedef enum RelayState
{
  OFF, 
  ON, 
  UNDEFINED
};


class Relay
{

  private:
    int pin;
    RelayState state;
    bool initialized = false;

  public:

    // Constructor providing pin initialization
    // Use: Relay myRelay = new Relay(x);
    Relay(int nPin)
    {
      if (nPin < MAXNUMPINS)
      {
        // Set the pin mode to output, the state to off, and the initialized status to true
        pin = nPin;        
        pinMode(pin, OUTPUT);

        initialized = true;

        // Set the initial state to Off
        Off();
      }
    }

    // Constructor providing pin and state initailizations
    // Use: Relay myRelay = new Relay(x, y);
    Relay(int nPin, RelayState initState)
    {
      if (nPin < MAXNUMPINS)
      {
        // Set the pin mode to output, the state to off, and the initialized status to true
        pin = nPin;        
        pinMode(pin, OUTPUT);

        initialized = true;
        
        // Check the desired init state and set the state
        if (initState == ON)
        {
          On();
        }
        else if (initState == OFF)
        {
          Off();
        }
        else
        {
          // Bad state, clear the init
          initialized = false;
          Off();
        }
      }
    }

    // Sets the physical state state of the pin controlling the relay to high (5V/3.3V) and sets
    // the logical state of the relay to ON
    void On()
    {
      // Check to make sure the relay is initialized
      if (initialized)
      {
        // Check the current state for OFF
        if (isOff())
        {
          // Write the pin high and change the logical state
          digitalWrite(pin, HIGH);
          state = ON;
        }
      }
    }

    // Sets the physical state state of the pin controlling the relay to low (0V) and sets
    // the logical state of the relay to OFF
    void Off()
    {
      // Check to make sure the relay is initialized
      if (initialized)
      {
        // Check the current state for ON
        if (isOn())
        {
          // Write the pin high and change the logical state
          digitalWrite(pin, LOW);
          state = OFF;
        }
      }
    }

    // Changes the current relay state to its opposite value. 5V/3.3V goes to 0V, 
    // logical state goes from ON to OFF and vice-versa
    void Toggle()
    {
      // Check to make sure the relay is initialized
      if (initialized)
      {
        // Check the current state
        if (isOn())
        {
          // State is on, so turn off
          Off();
        }
        else if (isOff())
        {
          // State is off, so turn on
          On();
        }
        else
        {
          // Bad state, clear the init
          initialized = false;
          Off();
        }
      }
    }

    // Performs a logical state check to determine if the relay is currently on
    // Returns: True if the relay is in the ON state, false otherwise
    bool isOn()
    {
      return state == ON;
    }

    // Performs a logical state check to determine if the relay is currently off
    // Returns: True if the relay is in the OFF state, false otherwise
    bool isOff()
    {
      return state == OFF;
    }    

    bool isInitialized()
    {
      return initialized == true;
    }
};

#endif
