#pragma once

template <typename T>
struct has_reserve {
    struct good { char dummy; };
    struct bad { char dummy[2]; };

    template <class U, float (U::*)()>
    struct SFINAE {};

    template <typename  U>
    static good test(SFINAE<U, &U::reserve>*);

    template <typename>
    static bad test(...);

    static const bool value = sizeof(test<T>(nullptr)) == sizeof(good);
};

class  TestReserve {
public:
    float reserve();
};

class Bar {
public:
    int type;
};
