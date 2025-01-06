#include <array>

#define WCH_TARGET_WCH_W32V307

#include "system/ch32v003/soc.h"
#include "utils/delay.h"

using namespace Peripheral;

// namespace Gpio {
//     template<Pins P>
//     void UserInit() {
//         P.sequence().mode<P, PinMode::outputPushPullNormalSpeed>().executeExact();
//     }
// }

// auto operator"" _s( int x) -> int { return x*1000*1000; }
// auto operator"" _ms(int x) -> int { return x*1000; }
// auto operator"" _us(int x) -> int { return x; }

// int operator ""_s(int);
//
// void hi(int a) {
//
// }

constexpr long double operator""_deg_to_rad(long double deg)
{
    long double radians = deg * 3.14 / 180;
    return radians;
}


// NOLINTBEGIN(readability-static-accessed-through-instance)
int main(int argc, char *argv[]) {
    constexpr auto tdi  = Soc::Gpio.A.GetPin(0);
    constexpr auto tms  = Soc::Gpio.A.GetPin(1);
    constexpr auto tdo  = Soc::Gpio.A.GetPin(2);
    constexpr auto data = Soc::Gpio.C.GetPin({ 2, 4, 5, 7 });
    // data = argc;

    using namespace DelayLiterals;
    // constexpr auto time = 1_hour_to_ms + 30_min_to_ms;
    // Utils::delayMs(time);

    Rcc::PeripheralBus2Reset a {
        .ioAuxiliary = Rcc::Reset::reset,
    };

    Rcc::PeripheralBus2ClockEnable b {
        .portA = Rcc::ModuleClock::on,
    };

    // Utils::delay(100ms);

    // auto var = tdi.mode<Driver::Gpio::PinMode::outputPushPull>().on<tdi>().commitExact();
    Gpio::PinsSet<tdi, tdo, tms>() = 11;

    constexpr Gpio::Sequence<> changes;
    constexpr auto ans = changes.mode<tdi, Gpio::PinMode::inputPullUpOrDown>().off<tdo>().on<tdi>().on<data>();


    auto uart = Soc::Usart::Channel1Mapping1.device.registers.baudrate;

    //changes.on<tdi>().commitExact();

    //constexpr auto ans = changes.on<tdi>();

    // return tdi.mode<Driver::Gpio::PinMode::outputOpenDrain>().on<tdi>().commitExact();
    //constexpr auto b = tdi.actuate().on<tdo>().commitExact();


    return ans.executeExact();
}
// NOLINTEND(readability-static-accessed-through-instance)