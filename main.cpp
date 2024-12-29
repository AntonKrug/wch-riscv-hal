#include <array>

#include "system/ch32v003/soc.h"
#include "peripheral/ch32v00x/gpio.h"

using namespace BundledPeripheral;

// NOLINTBEGIN(readability-static-accessed-through-instance)
int main(int argc, char *argv[]) {
    constexpr auto tdi  = Soc::Gpio::A.GetPin(0);
    constexpr auto tms  = Soc::Gpio::A.GetPin(1);
    constexpr auto tdo  = Soc::Gpio::A.GetPin(2);
    constexpr auto data = Soc::Gpio::B.GetPins({ 2, 4, 5, 7 });
    // data = argc;

    // auto var = tdi.mode<Driver::Gpio::PinMode::outputPushPull>().on<tdi>().commitExact();
    Gpio::PinsPack<tdi, tdo, tms>() = 11;

    constexpr Gpio::Actuations<> changes;
    constexpr auto ans = changes.mode<tdi, Gpio::PinConfig::inputAnalog>().off<tdo>().on<tdi>().on<tms>().on<data>();
    //changes.on<tdi>().commitExact();

    //constexpr auto ans = changes.on<tdi>();

    // return tdi.mode<Driver::Gpio::PinMode::outputOpenDrain>().on<tdi>().commitExact();
    //constexpr auto b = tdi.actuate().on<tdo>().commitExact();


    return ans.commitExact();
}
// NOLINTEND(readability-static-accessed-through-instance)