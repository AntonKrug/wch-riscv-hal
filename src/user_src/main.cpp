#include <array>

#define WCH_TARGET_WCH_W32V307

// #include <iostream>

#include "platform_configuration/system.h"
#include "utils/delay.h"
#include "utils/literals/all.h"
#include "firmware_build_info.h"
#include "system/soc_types.h"
#include "peripheral/ch32v00x/usart.h"
#include "system/gpio/ops_fusion.h"

using namespace peripheral;


// TODO: section check of IRQ being aligned

// void irqHandler() {
//     while (true) {
//
//     }
// }

#include "system/gpio/concepts.h"
#include "system/gpio/op.h"

struct FakeLcdDriver { // NOLINT

    [[nodiscard]] static constexpr auto configure_pins() {
        constexpr soc::gpio::OpsFusion fusion;
        constexpr auto led = soc::GpioPort.a.get_pin<0U>(); // NOLINT(*-static-accessed-through-instance)
        constexpr auto op = led.mode_input_op_ct<gpio::PinInputDrive::floating>(); // NOLINT(*-static-accessed-through-instance)

        return fusion.enroll<op>(); // NOLINT(*-static-accessed-through-instance)
    }

};



namespace soc::irq::handler {

    void __attribute__((retain, used)) supply_voltage_detection() {
        // using namespace literals::delay;
        // using namespace literals::timer;
        //
        // utils::delayMs(1.5_hour_to_ms);
    }

}

// [[noreturn]] and make it infinite loop or do on system level?
// https://gcc.gnu.org/bugzilla/show_bug.cgi?id=80515
// NOLINTBEGIN(readability-static-accessed-through-instance)
// TODO: idea for names: user_main, firmware_main, main_user_application
extern "C" [[noreturn]] void __attribute__((retain, used, section(".text.main_user")))
main_user(void) {
    char a[3] ="hi";
    a[0] = a[0] + 1;
    // prepare_system_for_main();

    constexpr auto p0 = soc::GpioPort.a.get_pin<0>();
    // constexpr soc::gpio::OpsFusion f0;
    constexpr soc::gpio::OpsFusion f1;

    // constexpr auto other_fusion = f0.enroll<p0.mode_output_op_ct<gpio::PinOutputSlewRateCt::normal, false, gpio::PinOutputDrive::push_pull>()>();

    // constexpr auto f2 = FakeLcdDriver::configure_pins<f1>();

    f1
        .enroll<p0.output_high_op_ct()>()
        // .enroll_ops_tuple<FakeLcdDriver::configure_pins_ops_tuple()>()
        .enroll(FakeLcdDriver::configure_pins())
        // .enroll(FakeLcdDriver::configure_pins<f1>())
        .execute();

    // constexpr auto up = p0.write_op_ct<1U>();
    // p0.write_op_ct<1U>();

    // soc::gpio::execute_op<up>();
    // soc::gpio::execute_op<op>();


    // Firmware build info
    // std::cout << "Version: " << firmwareBuildInfo::version << " date: " << firmwareBuildInfo::date << " time: " << firmwareBuildInfo::time << std::endl;
    // std::cout << "Versiondssdds: " << firmwareBuildInfo::version << " date:dsdsdsds " << firmwareBuildInfo::date << " time: " << firmwareBuildInfo::time << std::endl;

    // Custom literals
    using namespace literals::delay;
    using namespace literals::timer;
    using namespace literals::usart;

    // constexpr auto timer = 100_ms_to_hz;
    // std::cout << timer << std::endl;
    utils::delayMs(1.5_hour_to_ms);
    // constexpr auto timerFrequencySetting = 20_ns_to_hz;
    // auto something = Soc::Usart::Channel1Mapping0.device;


    // UART div calculations
    // This should fail as they are close frequencies
    // constexpr auto div11 = Usart::calculateUsartDivCT<2_mhz_to_hz, 1_mbaud>();
    // std::cout << "Usart DIV " << div11 << std::endl;
    // constexpr auto time = 1_hour_to_ms + 30_min_to_ms;

    // UART - This should pass
    // constexpr auto div = Usart::calculateUsartDivCT<11.0592_mhz_to_hz, 9.6_kbaud>();
    //
    // Peripheral::Usart::Device<SoC::PeripheralAddreses::usart1> device2;
    // // std::cout << "Uart base address " << device2.baseAddressUint32 << std::endl;
    //
    // Rcc::PeripheralBus2Reset a {
    //     .ioAuxiliary = Rcc::Reset::reset,
    // };
    //
    // Rcc::PeripheralBus2ClockEnable b {
    //     .portA = Rcc::ModuleClock::on,
    // };


    constexpr auto led  = soc::GpioPort.d.get_pin<0U>();
    led.mode_output_ct<gpio::PinOutputSlewRateCt::fast, false, gpio::PinOutputDrive::push_pull>();
    while (true) {
        led = 1U;
        utils::delayMs(1.5_hour_to_ms);
        led = 0U;
        utils::delayMs(1.5_hour_to_ms);
    }


    // constexpr auto swio  = soc::GpioPort.D.get_pin<1U>();
    // swio.sequence().executeExact();


    // swio.mode_output_ct<gpio::PinOutputSlewRateCt::normal, false, gpio::PinOutputDrive::push_pull>();

    // GPIO experiments
    // constexpr auto tdi  = soc::GpioPort.A.get_pin(0);
    // constexpr auto tms  = Soc::GpioPort.A.GetPin(1);
    // constexpr auto tdo  = Soc::GpioPort.A.GetPin(2);
    // constexpr auto data = Soc::GpioPort.C.GetPin({ 2, 4, 5, 7 });
    // // data = argc;
    //
    // auto var = tdi.mode<peripheral::gpio::PinMode::inputPullUpOrDown>().on<tdi>().commitExact();
    // GpioPort::PinsSet<tdi, tdo, tms>() = 11;
    //
    // constexpr GpioPort::Sequence<> changes;
    // constexpr auto ans = changes.mode<tdi, GpioPort::PinMode::inputPullUpOrDown>().off<tdo>().on<tdi>().on<data>();

    //changes.on<tdi>().commitExact();
    //constexpr auto ans = changes.on<tdi>();
    //return tdi.mode<Driver::GpioPort::PinMode::outputOpenDrain>().on<tdi>().commitExact();
    //constexpr auto b = tdi.actuate().on<tdo>().commitExact();

    // return ans.executeExact();

    while (true) {

    }
}
// NOLINTEND(readability-static-accessed-through-instance)