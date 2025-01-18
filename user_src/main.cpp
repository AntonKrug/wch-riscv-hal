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
    constexpr auto tdi  = Soc::Gpio.A.GetPin(0);
    constexpr auto tms  = Soc::Gpio.A.GetPin(1);
    constexpr auto tdo  = Soc::Gpio.A.GetPin(2);
    constexpr auto data = Soc::Gpio.C.GetPin({ 2, 4, 5, 7 });
    // data = argc;

    using namespace Literals::Delay;
    using namespace Literals::Timer;
    using namespace Literals::Usart;
    constexpr auto timer = 100_ms_to_hz;

    std::cout << timer << std::endl;


    std::cout << "Version: " << firmwareBuildInfo::version << " date: " << firmwareBuildInfo::date << " time: " << firmwareBuildInfo::time << std::endl;

    //constexpr auto div = Usart::calculateUsartDivCT<2_mhz_to_hz, 1_mbaud>();

    //std::cout << div << std::endl;
    constexpr auto time = 1_hour_to_ms + 30_min_to_ms;


    Utils::delayMs(1.5_hour_to_ms);
    constexpr auto div = Usart::calculateUsartDivCT<20_mhz_to_hz, 9.6_kbaud>();
    constexpr auto timerFrequencySetting = 20_ns_to_hz;
    auto something = Soc::Usart::Channel1Mapping0.device;

    constexpr auto uartAddr = Peripheral::Usart::MakeBaseAddress<0x40013800>();
//    constexpr Peripheral::Gpio::BaseAddress  gpioAddr(0x40013800);
    Peripheral::Usart::Device<SoC::PeripheralAddreses::usart1> device2;
    Peripheral::Usart::Device<uartAddr> device3;

    std::cout << "Uart base address " << static_cast<std::uint32_t>(device2.baseAddress) << std::endl;

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