#include <cstdlib>
#include <stdio.h>

#include "../lib/standard.cpp"

#define SAMPLE_SIZE 10000000

int main () {
    int trueCount = 0;
    srand(111111);
    for (int i = 0; i < SAMPLE_SIZE; i++) {
        bool result = returnTrueProbPerc(30);
        if (result) {trueCount++;}
    }
    float percentage = ((float)trueCount/SAMPLE_SIZE)*100;
    printf("%f\n", percentage);
    return 0;
}
