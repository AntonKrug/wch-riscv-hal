// Type your code here, or load an example.
#include <array>
#include <vector>
#include <list>

using namespace std;

template<typename T, int N>
struct A
{
     using value_type = T;
     int val = N;
     T someVar;
};

// struct Gpio {
//     constexpr static std::array<int,2> a();

//     constexpr Gpio output(const int val) {
//         a[0] = val;
//         return *this;
//     }

//     void commit() {
//         volatile int *p = (volatile int*)0x5;
//         *p = 2;
//     }
// };


// template<int current>
// struct Gpio {
//     std::array<int,5> val;

//     Gpio(const std::array<int,5> orig, const int newVal) {
//         for (int i=0; i<current; i++) {
//             val[i] = orig[i];
//         }
//         val[current] = newVal;
//     }

//     Gpio set(int newVal) {
//         Gpio<current+1> ret(val, newVal);
//         return ret;
//     }

//     int getVal() {
//         return val[current];
//     }

// };
    //inline __attribute__((always_inline))
//        sum = actions.size();
//__attribute__((optimize("-funroll-loops")))

enum class ActionType{ set, mode, pullUp, pullDown};

struct Pins {
    int portAddress;
    std::array<int, 64> pinNumbers;
};

struct Action {
    Pins pins;
    ActionType action;
    int value;
};


template<Action ... val>
struct Chain {
    static constexpr array<Action, sizeof...(val)> actions = { val ... };

    template<Pins pins>
    [[nodiscard]] constexpr auto on() const {
        constexpr Action newAction(pins, ActionType::set, 1);
        constexpr Chain<newAction, val ...> ans;
        return ans;
    }

    template<Pins pins>
    [[nodiscard]] constexpr auto off() const {
        constexpr Action newAction(pins, ActionType::set, 0);
        constexpr Chain<newAction, val ...> ans;
        return ans;
    }

    template<Pins pins>
    [[nodiscard]] constexpr auto in() const {
        constexpr Action newAction(pins, ActionType::mode, 0);
        constexpr Chain<newAction, val ...> ans;
        return ans;
    }

    template<Pins pin>
    [[nodiscard]] constexpr auto out() const {
        constexpr Action newAction(pin, ActionType::mode, 1);
        constexpr Chain<newAction, val ...> ans;
        return ans;
    }

    // inline __attribute__((always_inline))
    // __attribute__((optimize("-funroll-loops")))
    // __attribute__((optimize("-O3")))
    [nodiscard] constexpr int sum() const {
        int sum = 0;
        for (auto action : actions) {
             sum += action.pin.pinNumber;
        }

        return sum;
    }
};


//__attribute__((optimize("-funroll-loops")))
int main() {
    constexpr Pins TDI = { 0x10, { 0x1 }};
    constexpr Pins TMS = { 0x10, { 0x2 }};
    constexpr Pins TDO = { 0x10, { 0x3 }};
    constexpr Chain<> chain;
    constexpr auto ans = chain.in<TDO>().out<TDI>().on<TDI>().on<TMS>();
    return ans.sum();
}