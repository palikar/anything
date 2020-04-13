#pragma once

namespace ay
{

template<typename T, int N>
constexpr int dim(T (&)[N])
{
    return N;
}


}  // namespace ay
