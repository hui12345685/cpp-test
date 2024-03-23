#include <stdint.h>
#include <type_traits>
template <uint16_t N>
struct Fib {
    constexpr static int value = Fib<N - 2>::value + Fib<N - 1>::value;
};

//integral_constant中有一个成员叫value
template <>
struct Fib<0> : std::integral_constant<uint64_t, 0> {};

template <>
struct Fib<1> : std::integral_constant<uint64_t, 1> {};

class Solution {
public:
    int fib(int n) {
        switch (n) {
        case 0:
            return Fib<0>::value;
        case 1:
            return Fib<1>::value;
        case 2:
            return Fib<2>::value;
        case 3:
            return Fib<3>::value;
        case 4:
            return Fib<4>::value;
        case 5:
            return Fib<5>::value;
        case 6:
            return Fib<6>::value;
        case 7:
            return Fib<7>::value;
        case 8:
            return Fib<8>::value;
        case 9:
            return Fib<9>::value;
        case 10:
            return Fib<10>::value;
        case 11:
            return Fib<11>::value;
        case 12:
            return Fib<12>::value;
        case 13:
            return Fib<13>::value;
        case 14:
            return Fib<14>::value;
        case 15:
            return Fib<15>::value;
        case 16:
            return Fib<16>::value;
        case 17:
            return Fib<17>::value;
        case 18:
            return Fib<18>::value;
        case 19:
            return Fib<19>::value;
        case 20:
            return Fib<20>::value;
        case 21:
            return Fib<21>::value;
        case 22:
            return Fib<22>::value;
        case 23:
            return Fib<23>::value;
        case 24:
            return Fib<24>::value;
        case 25:
            return Fib<25>::value;
        case 26:
            return Fib<26>::value;
        case 27:
            return Fib<27>::value;
        case 28:
            return Fib<28>::value;
        case 29:
            return Fib<29>::value;
        case 30:
            return Fib<30>::value;
        default:
            break;
        }
        return Fib<0>::value;
    }
};
