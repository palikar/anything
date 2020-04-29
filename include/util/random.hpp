#pragma once

#include "std_header.hpp"

#include <random>
#include <ctime>

namespace ay::util
{

namespace detail
{
static std::random_device rand_dev;
static std::mt19937 rand_eng(rand_dev());
}
        
class Random
{
  private:


  public:
    static int uniform_int(int a, int b)
    {
        std::uniform_int_distribution<> distr(a, b);
        return distr(detail::rand_eng);
    }

    static float uniform_real(float a, float b)
    {
        std::uniform_real_distribution<> distr(a, b);
        return distr(detail::rand_eng);
    }

    static int random()
    {
        return std::rand();
    }


};

}
