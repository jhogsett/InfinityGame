#include "buffers.h"
#include "morse.h"
#include "play_data.h"
#include "speaker.h"
#include "utils.h"
#include "chime.h"

void chime(int count){
    if(!option_clock_24h){
        count = count % 12;
        if(count == 0)
            count = 12;
    }
    switch(option_clock_chime){
        case CHIME_NONE:
            break;
        case CHIME_BEEP:
            beep();
            break;
        case CHIME_HOUR:
            for(int i = 0; i < count; i++){
                beep();
                delay(CHIME_BEEP_GAP);
            }
            break;
        case CHIME_CODE:
            sprintf_P(copy_buffer, PSTR("%d"), count);
            send_morse(copy_buffer);
            break;
    }
}
