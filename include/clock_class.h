#ifndef CLOCK_CLASS_H
#define CLOCK_CLASS_H

#include <Arduino.h>

#define MS_PER_DAY 86400000UL // 24 * 60 * 60 * 1000

class Clock {
public:
    Clock();
    
    // Set the current time (establishes the 24-hour window and offset)
    void setTime(byte hours, byte minutes, byte seconds);
    
    // Get the current time
    void getCurrentTime(byte &hours, byte &minutes, byte &seconds);
    
    // Increment the time by specified amounts
    void incrementTime(byte hours, byte minutes, byte seconds);
    
    // Reset seconds to 0 (for time setting)
    void resetSeconds();
    
    // Get total seconds since start of current day
    long getTimeInSeconds();

private:
    unsigned long window_start_millis;  // millis() value at start of 24-hour window
    unsigned long time_offset_ms;       // offset in milliseconds from window start
    
    // Update the 24-hour window if needed (called automatically)
    void updateWindow();
    
    // Convert time components to total seconds
    inline long timeToSeconds(byte hours, byte minutes, byte seconds) {
        return (long)hours * 3600L + (long)minutes * 60L + (long)seconds;
    }
    
    // Convert total seconds to time components
    inline void secondsToTime(long totalSeconds, byte &hours, byte &minutes, byte &seconds) {
        totalSeconds %= 86400L; // 24 * 60 * 60
        hours = totalSeconds / 3600L;
        totalSeconds %= 3600L;
        minutes = totalSeconds / 60L;
        seconds = totalSeconds % 60L;
    }
};

#endif // CLOCK_CLASS_H
