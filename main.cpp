#include <iostream>
#include <functional>
#include "chaincall.hpp"

int main(int argc, const char **argv)
{
    using namespace std;
    using namespace chaincall;
    using namespace std::placeholders;
    
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

    return 0;
}