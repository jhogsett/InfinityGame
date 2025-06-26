#include "clock_class.h"

Clock::Clock() {
    // Initialize with midnight (00:00:00)
    window_start_millis = millis();
    time_offset_ms = 0;
}

void Clock::setTime(byte hours, byte minutes, byte seconds) {
    window_start_millis = millis();
    time_offset_ms = timeToSeconds(hours, minutes, seconds) * 1000UL;
}

void Clock::getCurrentTime(byte &hours, byte &minutes, byte &seconds) {
    updateWindow();
    
    unsigned long current_ms = millis() - window_start_millis + time_offset_ms;
    long total_seconds = current_ms / 1000L;
    
    secondsToTime(total_seconds, hours, minutes, seconds);
}

void Clock::incrementTime(byte hours, byte minutes, byte seconds) {
    long increment_seconds = timeToSeconds(hours, minutes, seconds);
    time_offset_ms += increment_seconds * 1000UL;
    
    // Handle overflow past 24 hours
    if (time_offset_ms >= MS_PER_DAY) {
        time_offset_ms -= MS_PER_DAY;
        window_start_millis += MS_PER_DAY;
    }
}

void Clock::resetSeconds() {
    byte hours, minutes, seconds;
    getCurrentTime(hours, minutes, seconds);
    setTime(hours, minutes, 0);
}

long Clock::getTimeInSeconds() {
    updateWindow();
    
    unsigned long current_ms = millis() - window_start_millis + time_offset_ms;
    return current_ms / 1000L;
}

void Clock::updateWindow() {
    unsigned long current_millis = millis();
    
    // Check if we've crossed into a new 24-hour period
    if ((current_millis - window_start_millis + time_offset_ms) >= MS_PER_DAY) {
        window_start_millis += MS_PER_DAY;
    }
}
