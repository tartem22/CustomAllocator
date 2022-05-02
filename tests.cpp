#include <gtest/gtest.h>

#include <vector>

#include "custom_allocator.h"
#include "custom_forward_list.h"

TEST(test_custom_allocator, test_allocate)
{
    std::vector<int, CustomAllocator<int, 10>> v;
    try
    {
        for (int i = 0; i < 10; i++)
            v.push_back(i);
    }
    catch (...)
    {
        FAIL() << "Allocation fail";
    }
}

TEST(test_custom_allocator, test_overallocate)
{
    std::vector<int, CustomAllocator<int, 10>> v;
    try
    {
        for (int i = 0; i < 20; i++)
            v.push_back(i);
    }
    catch (...)
    {
        FAIL() << "Overallocation fail";
    }
}

TEST(test_custom_forward_list, test_emplace)
{
    CustomForwardList<int, CustomAllocator<int, 10>> fwl;
    try
    {
        for (int i = 0; i < 20; i++)
            fwl.emplace_front(i);
    }
    catch (...)
    {
        FAIL() << "Emplace fail";
    }
}

TEST(test_custom_forward_list, test_iterator)
{
    CustomForwardList<int, CustomAllocator<int, 10>> fwl;
    for (int i = 0; i < 20; i++)
            fwl.emplace_front(i);
    try
    {
        auto it = fwl.begin();
        while(it != fwl.end())
        {
            it = it++;
        }   
    }
    catch (...)
    {
        FAIL() << "Iterator fail";
    }
}
