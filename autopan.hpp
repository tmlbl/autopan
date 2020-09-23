#include "usermodfx.h"
#include "dsp/simplelfo.hpp"
#include <cmath>

struct AutoPan {

    struct Params {
        float depth;

        Params(void):
            depth(1.f)
        { }
    };

    Params params;
    dsp::SimpleLFO lfo;
};
