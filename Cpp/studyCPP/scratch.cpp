#include "pch.h"
#include "helper.hpp"

using namespace std;

int numberOfSteps(int num) {
    int rt = 0;
    while (num) {

        if (num & 1) {
            num -= 1;
            num >>= 1;

        }
        else {
            num >>= 1;
        }
        rt++;

    }
    return rt;
}


void scratch() {

    while (1) {
        numberOfSteps(14);
    }

}