#include "chaincall.hpp"
#include <iostream>

int add_1(int v)
{
    return v + 1;
}

int power_2(int v)
{
    return v * v;
}

int div_2(int v)
{
    return v / 2;
}

int add(int v1, int v2)
{
    return v1 + v2;
}

std::tuple<int, int> tuple_1_2()
{
    return {1, 2};
}

int main(int argc, const char **argv)
{
    using namespace std;

    auto v1 = chaincall::chain() >> [] { return 42; } >> add_1 >> power_2 >> div_2;
    auto v2 = chaincall::chain(2) >> add_1 >> power_2 >> div_2;
    auto v3 = chaincall::chain(1, 2) >> [](int a, int b) { return a + b; } >> power_2 >> div_2;
    auto v4 = chaincall::chain() >> [] { return make_tuple(3, 7); } >> [](int a, int b) { return a * b; } >> power_2 >> div_2;

    cout << v1.value << endl; //(42+1)**2/2 = 924
    cout << v2.value << endl; //(2+1)**2/2 = 4
    cout << v3.value << endl; //(1+2)**2/2 = 4
    cout << v4.value << endl; //(3*7)**2/2 = 220


    auto composed_func = chaincall::pipe([](int a, int b) { return a * b; }, power_2, div_2);

    cout << composed_func(2, 7).value << endl; //(2*7)**2/2 = 98

    return 0;
}