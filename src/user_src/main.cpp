#include <array>

#define WCH_TARGET_WCH_W32V307

#include <iostream>

#include "system.h"
#include "utils/delay.h"
#include "utils/literals/all.h"
#include "firmware_build_info.h"
#include "system/soc_types.h"
#include "peripheral/ch32v00x/usart.h"
#include "system/startup/soc_init.h"

using namespace Peripheral;


// TODO: section check of IRQ being aligned


// NOLINTBEGIN(readability-static-accessed-through-instance)
int main(int argc, char *argv[]) {

    prepare_system_for_main();

    // Firmware build info
    // std::cout << "Version: " << firmwareBuildInfo::version << " date: " << firmwareBuildInfo::date << " time: " << firmwareBuildInfo::time << std::endl;
    // std::cout << "Versiondssdds: " << firmwareBuildInfo::version << " date:dsdsdsds " << firmwareBuildInfo::date << " time: " << firmwareBuildInfo::time << std::endl;

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

    // UART - This should pass
    constexpr auto div = Usart::calculateUsartDivCT<11.0592_mhz_to_hz, 9.6_kbaud>();

    Peripheral::Usart::Device<SoC::PeripheralAddreses::usart1> device2;
    // std::cout << "Uart base address " << device2.baseAddressUint32 << std::endl;

    Rcc::PeripheralBus2Reset a {
        .ioAuxiliary = Rcc::Reset::reset,
    };

    Rcc::PeripheralBus2ClockEnable b {
        .portA = Rcc::ModuleClock::on,
    };


    // GPIO experiments
    constexpr auto tdi  = Soc::Gpio.A.GetPin(0);
    constexpr auto tms  = Soc::Gpio.A.GetPin(1);
    constexpr auto tdo  = Soc::Gpio.A.GetPin(2);
    constexpr auto data = Soc::Gpio.C.GetPin({ 2, 4, 5, 7 });
    // data = argc;

    // auto var = tdi.mode<Driver::Gpio::PinMode::outputPushPull>().on<tdi>().commitExact();
    Gpio::PinsSet<tdi, tdo, tms>() = 11;

    constexpr Gpio::Sequence<> changes;
    constexpr auto ans = changes.mode<tdi, Gpio::PinMode::inputPullUpOrDown>().off<tdo>().on<tdi>().on<data>();

    //changes.on<tdi>().commitExact();
    //constexpr auto ans = changes.on<tdi>();
    //return tdi.mode<Driver::Gpio::PinMode::outputOpenDrain>().on<tdi>().commitExact();
    //constexpr auto b = tdi.actuate().on<tdo>().commitExact();

    return ans.executeExact();
}
// NOLINTEND(readability-static-accessed-through-instance)