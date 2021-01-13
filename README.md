# ChainCall

## Usage

```cpp

    #include "chaincall.hpp"

    ...
    
    auto v1 = chaincall::chain() >> []{return 42;} >> add_1 >> power_2 >> div_2;
    auto v2 = chaincall::chain(2) >> add_1 >> power_2 >> div_2;
    auto v3 = chaincall::chain(1,2) >> [](auto a,auto b){return a+b;} >> power_2 >> div_2;
    auto v4 = chaincall::chain() >> [] { return make_tuple(3, 7); } >> [](auto a, auto b) { return a * b; } >> power_2 >> div_2;

    cout << v1.value << endl; //924
    cout << v2.value << endl; //4
    cout << v3.value << endl; //4
    cout << v4.value << endl; //220
    
    ...
    
```
