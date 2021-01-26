# ChainCall

## Usage

```cpp

    #include "chaincall.hpp"

    ...
    
    //chain
    auto v1 = chaincall::chain() >> [] { return 42; } >> add_1 >> power_2 >> div_2;
    auto v2 = chaincall::chain(2) >> add_1 >> power_2 >> div_2;
    auto v3 = chaincall::chain(1, 2) >> [](int a, int b) { return a + b; } >> power_2 >> div_2;
    auto v4 = chaincall::chain() >> [] { return make_tuple(3, 7); } >> [](int a, int b) { return a * b; } >> power_2 >> div_2;

    cout << v1.value << endl; //(42+1)**2/2 = 924
    cout << v2.value << endl; //(2+1)**2/2 = 4
    cout << v3.value << endl; //(1+2)**2/2 = 4
    cout << v4.value << endl; //(3*7)**2/2 = 220

    //pipe
    auto composed_func1 = chaincall::pipe([](int a, int b) { return a * b; }, power_2, div_2);
    auto composed_func2 = chaincall::pipe() << add << power_2 << div_2;

    cout << composed_func1(2, 7).value << endl; //(2*7)**2/2 = 98
    cout << composed_func2(2, 7).value << endl; //(2+7)**2/2 = 40
    

    struct Person
    {
        std::string m_name;
        int m_age; 
        void greeting()
        {
            cout<< "Hello, My name is " << m_name << ", I am " << m_age << " years old" <<endl;
        }
        void setAge(int age)
        {
            m_age = age;
        }
        void setName(const std::string& name)
        {
            m_name = name;
        }
    };

    Person leehua{"Lee Hua",20};
    auto greeting = std::bind(&Person::greeting, &leehua);
    auto set_name = std::bind(&Person::setName, &leehua, _1);
    auto set_age = std::bind(&Person::setAge, &leehua, _1);

    auto add = [](int a,int b){return a+b;};

    //chain
    
    chain(18, 1) >> add >>set_age >> greeting;
    chain(std::string{"Xiao Lee"}) >> set_name >> greeting;

    //pipe

    auto add_setAge_greeting = pipe() >> add >> set_age >> greeting;
    auto setName_greeting = pipe() >> set_name >> greeting;
    
    add_setAge_greeting(22,1);
    setName_greeting(std::string{"Lee Hua"});

    ...
    
```
## Output

```

924
4
4
220
98
40

Hello, My name is Lee Hua, I am 19 years old
Hello, My name is Xiao Lee, I am 19 years old
Hello, My name is Xiao Lee, I am 23 years old
Hello, My name is Lee Hua, I am 23 years old

```