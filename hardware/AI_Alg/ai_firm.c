#include <stdio.h>
#define ai_alg_base (volatile int *) 0xFF202040

// int main()
// {
//     int fired = 0x00000;
//     int hits = 0x00000;
//     int ships = 0xFFFFFFFFFFFFFFFF;

//     printf("Starting\n");
//     *(ai_alg_base+1) = fired;
//     *(ai_alg_base+2) = fired;
//     printf("1\n");
//     *(ai_alg_base+3) = hits;
//     *(ai_alg_base+4) = hits;
//     printf("2\n");
//     *(ai_alg_base+5) = ships;
//     printf("3\n");
//     *(ai_alg_base) = 0x0; // Run command
//     printf("4\n");

//     printf("Where to shoot: %x\n", *(ai_alg_base));
//     printf("5\n");
//     return(0);
// }

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