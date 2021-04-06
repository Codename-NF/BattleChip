#include "ai_firm.h"
#include <stdio.h>

int ai_where_to_shoot(unsigned long fired_0, unsigned long fired_1, unsigned long fired_2, unsigned long fired_3,
                    unsigned long hits_0, unsigned long hits_1, unsigned long hits_2, unsigned long hits_3, unsigned long ships)
{
    *(ai_alg_base+1) = fired_0;
    *(ai_alg_base+2) = fired_1;
    *(ai_alg_base+3) = fired_2;
    *(ai_alg_base+4) = fired_3;
    *(ai_alg_base+5) = hits_0;
    *(ai_alg_base+6) = hits_1;
    *(ai_alg_base+7) = hits_2;
    *(ai_alg_base+8) = hits_3;
    *(ai_alg_base+9) = ships;

    *(ai_alg_base) = 0; //Run command

    int result = *(ai_alg_base); // This might stall the command, I'm not sure yet
    return result;
}