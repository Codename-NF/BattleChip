#include <stdio.h>
// #define ai_alg_base (volatile int *) 0xFF202040

int main()
{
    // int fired = 0x0000000000000000;
    // int hits = 0x0000000000000000;
    // int ships = 0x000000000000001F;

    printf("Starting\n");
    // *(ai_alg_base+1) = fired;
    printf("1\n");
    // *(ai_alg_base+2) = hits;
    printf("2\n");
    // *(ai_alg_base+3) = ships;
    printf("3\n");
    // *(ai_alg_base) = 0x0; // Run command
    printf("4\n");

    // printf("Where to shoot: %x\n", *(ai_alg_base));
    printf("5\n");
    return(0);
}