#include "hardware/clocks.h"
#include "hardware/vreg.h"
#include "pico/multicore.h"
#include "pico/stdlib.h"
#include "edc.h"
#include "picostation.h"
#include "pseudo_atomics.h"
#include <cstdio>
#include <csignal>

uint32_t c_sectorMax = 333000;  // 74:00:00

int main() {
#ifdef PICO_RP2350
    vreg_set_voltage(VREG_VOLTAGE_1_10);
    sleep_ms(100);
    set_sys_clock_khz(135600, true);
#else
    vreg_set_voltage(VREG_VOLTAGE_1_15);
    sleep_ms(100);
    set_sys_clock_khz(271200, true);
#endif

    initPseudoAtomics();
    eccedc_init();

    picostation::initHW();
    multicore_launch_core1(picostation::core1Entry);  // I2S Thread

    picostation::core0Entry();  // Reset, playback speed, Sled, soct, subq
    __builtin_unreachable();
}
