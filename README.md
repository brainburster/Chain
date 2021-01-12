# ChainCall

## Usage

''' cpp

    #include "chain.hpp"

    ...
    
    auto v1 = chaincall::chain() >> _42 >> add_1 >> power >> div_2;
    auto v2 = chaincall::chain(2) >> add_1 >> power >> div_2;
    auto v3 = chaincall::chain(1,2) >> add >> power >> div_2;
    auto v4 = chaincall::chain() >> tuple_1_2 >> add >> power >> div_2;

    cout << v1.value << endl; //924
    cout << v2.value << endl; //4
    cout << v3.value << endl; //4
    cout << v4.value << endl; //4
    
    ...
    
'''
