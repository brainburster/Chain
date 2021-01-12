#include "chaincall.hpp"
#include <iostream>

int _42()
{
    return 42;
}

int add_1(int v)
{
    return v + 1;
}

int power(int v)
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

std::tuple<int,int> tuple_1_2(){
    return {1,2};
}


int main(int argc, const char** argv) {
    using namespace std;

    auto v1 = chaincall::chain() >> _42 >> add_1 >> power >> div_2;
    auto v2 = chaincall::chain(2) >> add_1 >> power >> div_2;
    auto v3 = chaincall::chain(1,2) >> add >> power >> div_2;
    auto v4 = chaincall::chain() >> tuple_1_2 >> add >> power >> div_2;

    cout << v1.value << endl;
    cout << v2.value << endl;
    cout << v3.value << endl;
    cout << v4.value << endl;

    return 0;
}