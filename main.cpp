#include <iostream>
#include <map>
#include <vector>
#include <list>
#include <iterator>

#include "custom_allocator.h"
#include "custom_forward_list.h"

int main()
{
    // Standart factorial map
    auto factorialMap = std::map<int, int>{};

    factorialMap[0] = 1;
    for (int i = 1; i < 10; i++)
    {
        factorialMap[i] = factorialMap[i - 1] * i;
    }
    for (auto f : factorialMap)
        std::cout << f.second << " ";
    std::cout << std::endl;

    // Custom factorial map
    auto customFactorialMap = std::map<int, int,
                                       std::less<int>,
                                       CustomAllocator<int, 10>>{};

    customFactorialMap[0] = 1;
    for (int i = 1; i < 10; i++)
    {
        customFactorialMap[i] = customFactorialMap[i - 1] * i;
    }
    for (auto f : customFactorialMap)
        std::cout << f.second << " ";
    std::cout << std::endl;

    // Custom container with standart allocator
    auto customContainer = CustomForwardList<int>{};
    for (int i = 0; i < 10; i++)
        customContainer.emplace_front(i);
    std::copy(customContainer.begin(),
              customContainer.end(),
              std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;

    // Custom container with custom allocator
    auto customFLwithAllocator = CustomForwardList<int, 
        CustomAllocator<int, 10>>{};
    for (int i = 0; i < 10; i++)
        customFLwithAllocator.emplace_front(i);
    std::copy(customFLwithAllocator.begin(),
              customFLwithAllocator.end(),
              std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;

    return 0;
}