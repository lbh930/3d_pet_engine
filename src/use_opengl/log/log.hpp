#include <iostream>

template <typename... T>

int Log(T... message)
{
    (std::cout<<...<<message);
    std::cout<<std::endl;
    return 0;
}