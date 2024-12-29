#include <array>
#include <vector>
#include <list>

using namespace std;

namespace Gpio {

    class IRegister {
    public:
        [[nodiscard]] virtual constexpr int getBaseAddress() const = 0;
//        virtual constexpr int operator()() const = 0;
        // ReSharper disable once CppNonExplicitConversionOperator
  //      virtual constexpr operator int() const = 0; // NOLINT(*-explicit-constructor)
        virtual ~IRegister() = default;
    };

    template<int BaseAddress>
    struct Register final : IRegister {
        //  using value_type = T;
        constexpr static int baseAddress       = BaseAddress;         // CFGLR
        constexpr static int inputData         = BaseAddress + 0x08;  // INDR
        constexpr static int outputData        = BaseAddress + 0x0C;  // OUTDR
        constexpr static int setReset          = BaseAddress + 0x10;  // BSHR
        constexpr static int reset             = BaseAddress + 0x14;  // BCR
        constexpr static int configurationLock = BaseAddress + 0x18;  // LCKR

        [[nodiscard]] constexpr int getBaseAddress() const override {
            return baseAddress;
        }

        // constexpr int operator()() const override {
        //     return baseAddress;
        // };
        //
        // // ReSharper disable once CppNonExplicitConversionOperator
        // constexpr operator int() const override { // NOLINT(*-explicit-constructor)
        //     return baseAddress;
        // };
    };

    enum class ActionType{ set, mode, pullUp, pullDown};

    struct Pins {
        const IRegister *port;
        const array<int, 8> pinNumbers;
    };

    struct Action {
        const Pins pins;
        const ActionType action;
        const int value;
    };


    template<Action ... Val>
    struct CTChain {
        static constexpr array<Action, sizeof...(Val)> actions = { Val ... };

        template<Pins Pins>
        [[nodiscard]] constexpr auto on() const {
            constexpr Action newAction(Pins, ActionType::set, 1);
            constexpr CTChain<newAction, Val ...> ans;
            return ans;
        }

        template<Pins Pins>
        [[nodiscard]] constexpr auto off() const {
            constexpr Action newAction(Pins, ActionType::set, 0);
            constexpr CTChain<newAction, Val ...> ans;
            return ans;
        }

        template<Pins Pins>
        [[nodiscard]] constexpr auto in() const {
            constexpr Action newAction(Pins, ActionType::mode, 0);
            constexpr CTChain<newAction, Val ...> ans;
            return ans;
        }

        template<Pins Pins>
        [[nodiscard]] constexpr auto out() const {
            constexpr Action newAction(Pins, ActionType::mode, 1);
            constexpr CTChain<newAction, Val ...> ans;
            return ans;
        }

        [[nodiscard]] constexpr int sum() const {
            int sum = 0;
            for (auto action : actions) {
                sum += action.pins.pinNumbers[0];
            }

            return sum;
        }
    };

}

struct System {
    constexpr static Gpio::Register<0x40010800> gpioA = {};
    constexpr static Gpio::Register<0x40011000> gpioB = {};
    constexpr static Gpio::Register<0x40011400> gpioC = {};
};

// inline __attribute__((always_inline))
// __attribute__((optimize("-funroll-loops")))
// __attribute__((optimize("-O3")))


int main() {
    constexpr Gpio::Pins TDI  = { &System::gpioA, { 0 }};
    constexpr Gpio::Pins TMS  = { &System::gpioA, { 1 }};
    constexpr Gpio::Pins TDO  = { &System::gpioA, { 2 }};
    constexpr Gpio::Pins Data = { &System::gpioB, { 0, 1, 2, 3 }};
    constexpr Gpio::CTChain<> chain;
    constexpr auto ans = chain.in<TDO>().out<TDI>().on<TDI>().on<TMS>().on<Data>();
    return ans.sum();
}



/* newer


// inline __attribute__((always_inline))
// __attribute__((optimize("-funroll-loops")))
// __attribute__((optimize("-O3")))




// NOLINTBEGIN(readability-static-accessed-through-instance)
int main(int argc, char *argv[]) {
    //    constexpr Driver::Gpio::Pins tdi  = Soc::Gpio::A.type::GetPin(0);
    constexpr auto tdi  = Soc::Gpio::A.GetPin(0);
    constexpr auto tms  = Soc::Gpio::A.GetPin(1);
    constexpr auto tdo  = Soc::Gpio::A.GetPin(2);
    constexpr auto data = Soc::Gpio::B.GetPins({ 0, 1, 2, 3 });
    //data = argc;
    constexpr Driver::Gpio::ChangesCt<> changes;
    constexpr auto ans = changes.in<tdo>().on<tdi>().on<tms>().on<data>();
    //changes.on<tdi>().commitExact();

    //constexpr auto ans = changes.on<tdi>();
    return ans.commitExact();
}
// NOLINTEND(readability-static-accessed-through-instance)

*/