#ifndef M6532_HXX
#define M6532_HXX

class Console;
class System;

#include "bspf.h"
#include "Device.h"

/**
  RIOT

  @author  Bradford W. Mott
  @version $Id: M6532.hxx,v 1.3 1998/07/15 20:35:09 bwmott Exp $
*/
class M6532 : public Device
{
  public:
    /**
      Create a new 6532 for the specified console

      @param console The console the 6532 is associated with
    */
    M6532(const Console& console);
 
    /**
      Destructor
    */
    virtual ~M6532();

   public:
    /**
      Get a null terminated string which is the device's name (i.e. "M6532")

      @return The name of the device
    */
    virtual const char* name() const;

    /**
      Reset cartridge to its power-on state
    */
    virtual void reset();

    /**
      Notification method invoked by the system right before the
      system resets its cycle counter to zero.  It may be necessary
      to override this method for devices that remember cycle counts.
    */
    virtual void systemCyclesReset();

    /**
      Install 6532 in the specified system.  Invoked by the system
      when the 6532 is attached to it.

      @param system The system the device should install itself in
    */
    virtual void install(System& system);

   public:
    /**
      Get the byte at the specified address

      @return The byte at the specified address
    */
    virtual uInt8 peek(uInt16 address);

    /**
      Change the byte at the specified address to the given value

      @param address The address where the value should be stored
      @param value The value to be stored at the address
    */
    virtual void poke(uInt16 address, uInt8 value);

  private:
    // Reference to the console
    const Console& myConsole;

    // An amazing 128 bytes of RAM
    uInt8 myRAM[128];

    // Current value of my Timer
    uInt32 myTimer;

    // Log base 2 of the number of cycles in a timer interval
    uInt32 myIntervalShift;

    // Indicates the number of cycles when the timer was last set
    Int32 myCyclesWhenTimerSet;

    // Indicates when the timer was read after timer interrupt occured
    Int32 myCyclesWhenInterruptReset;

    // Indicates if a read from timer has taken place after interrupt occured
    bool myTimerReadAfterInterrupt;

    // Data Direction Register for Port A
    uInt8 myDDRA;

    // Data Direction Register for Port B
    uInt8 myDDRB;

  private:
    // Copy constructor isn't supported by this class so make it private
    M6532(const M6532&);
 
    // Assignment operator isn't supported by this class so make it private
    M6532& operator = (const M6532&);
};
#endif

