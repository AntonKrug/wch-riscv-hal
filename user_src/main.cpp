#include <array>

#define WCH_TARGET_WCH_W32V307

#include <iostream>

#include "system.h"
#include "utils/delay.h"
#include "utils/literals/all.h"
#include "firmware_build_info.h"
#include "system/soc_types.h"
#include "peripheral/ch32v00x/usart.h"

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

// TODO: section check of IRQ being aligned


// NOLINTBEGIN(readability-static-accessed-through-instance)
int main(int argc, char *argv[]) {
    // Firmware build info
    std::cout << "Version: " << firmwareBuildInfo::version << " date: " << firmwareBuildInfo::date << " time: " << firmwareBuildInfo::time << std::endl;

    // Custom literals
    using namespace Literals::Delay;
    using namespace Literals::Timer;
    using namespace Literals::Usart;

    constexpr auto timer = 100_ms_to_hz;
    std::cout << timer << std::endl;
    Utils::delayMs(1.5_hour_to_ms);
    constexpr auto timerFrequencySetting = 20_ns_to_hz;
    // auto something = Soc::Usart::Channel1Mapping0.device;

    // UART div calculations
    // This should fail as they are close frequencies
    // constexpr auto div11 = Usart::calculateUsartDivCT<2_mhz_to_hz, 1_mbaud>();
    // std::cout << "Usart DIV " << div11 << std::endl;
    // constexpr auto time = 1_hour_to_ms + 30_min_to_ms;

    // This should pass
    constexpr auto div = Usart::calculateUsartDivCT<20_mhz_to_hz, 9.6_kbaud>();

    Peripheral::Usart::Device<SoC::PeripheralAddreses::uart7> device2;
    std::cout << "Uart base address " << device2.baseAddressUint32 << std::endl;

    Rcc::PeripheralBus2Reset a {
        .ioAuxiliary = Rcc::Reset::reset,
    };

    Rcc::PeripheralBus2ClockEnable b {
        .portA = Rcc::ModuleClock::on,
    };


    // Utils::delay(100ms);

    constexpr auto tdi  = Soc::Gpio.A.GetPin(0);
    constexpr auto tms  = Soc::Gpio.A.GetPin(1);
    constexpr auto tdo  = Soc::Gpio.A.GetPin(2);
    constexpr auto data = Soc::Gpio.C.GetPin({ 2, 4, 5, 7 });
    // data = argc;

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