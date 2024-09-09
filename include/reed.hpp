#pragma once

#include <stdint.h>

class Reed {
   public:
    const int outClk;
    const int outData;
    const int outLch;
    const int inClk;
    const int inData;
    const int inLch;

    Reed(int outClk, int outData, int outLch, int inClk, int inData, int inLch);
    void init();

    uint64_t read();
};
