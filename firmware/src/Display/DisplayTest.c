
#include "DisplayTest.h"

static  uint32_t    testTimerTimeNow;
static  uint32_t    testTimerTimeOld;
static  uint32_t    testTimerInterval   = 50;
static  bool        testIncrease        = true;

void DisplayTestInit() {
    testTimerTimeNow = to_ms_since_boot(get_absolute_time());
    testTimerTimeOld = testTimerTimeNow;
}

bool DisplayTestUpdate(uint8_t* value) {
    testTimerTimeNow = to_ms_since_boot(get_absolute_time());
    if(testTimerInterval < (testTimerTimeNow - testTimerTimeOld)) {
        testTimerTimeOld = testTimerTimeNow;
        *value += (testIncrease ? +1 : -1);
        if(*value == 0x00 || *value == 0xFF) {
            testIncrease = !testIncrease;
        }
        return true;
    }
    return false;
}
