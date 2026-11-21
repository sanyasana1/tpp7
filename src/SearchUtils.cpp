#include "SearchUtils.h"

#include <algorithm>
#include <cstddef>
#include <iterator>
#include <stdexcept>
#include <vector>

namespace {

void checkData(const int* data, std::size_t n)
{
    if (n > 0 && data == nullptr)
    {
        throw std::invalid_argument("data pointer is null");
    }
}

}

int binarySearch(const int* data, std::size_t n, int value)
{
    checkData(data, n);
    const int* end = data + n;
    const int* it = std::lower_bound(data, end, value);
    if (it != end && *it == value)
    {
        return static_cast<int>(it - data);
    }
    return -1;
}

int countOccurences(const int* data, std::size_t n, int value)
{
    checkData(data, n);
    return static_cast<int>(std::count(data, data + n, value));
}

int findFirst(const int* data, std::size_t n, int value)
{
    checkData(data, n);
    const int* end = data + n;
    const int* it = std::find(data, end, value);
    return it == end ? -1 : static_cast<int>(it - data);
}

int findLast(const int* data, std::size_t n, int value)
{
    checkData(data, n);
    for (std::size_t i = n; i-- > 0;)
    {
        if (data[i] == value)
        {
            return static_cast<int>(i);
        }
    }
    return -1;
}

int kthElement(const int* data, std::size_t n, std::size_t k)
{
    checkData(data, n);
    if (k == 0 || k > n)
    {
        throw std::out_of_range("k must be in range [1, n]");
    }
    std::vector<int> tmp(data, data + n);
    std::nth_element(tmp.begin(),
                     tmp.begin() + static_cast<std::ptrdiff_t>(k - 1),
                     tmp.end());
    return tmp[k - 1];
}
