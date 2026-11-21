#include "SortableArray.h"

#include <algorithm>
#include <cstddef>
#include <stdexcept>
#include <vector>

namespace {

void insertionSort(std::vector<int>& v, std::size_t first, std::size_t last)
{
    for (std::size_t i = first + 1; i < last; ++i)
    {
        const int key = v[i];
        std::size_t j = i;
        while (j > first && v[j - 1] > key)
        {
            v[j] = v[j - 1];
            --j;
        }
        v[j] = key;
    }
}

SortMethod toMethod(int code)
{
    switch (code)
    {
    case 0: return SortMethod::Insertion;
    case 1: return SortMethod::Standard;
    default: throw InvalidSortMethodException(code);
    }
}

}
SortableArray::SortableArray(std::size_t capacity)
    : capacity_(capacity)
{
    if (capacity_ == 0)
    {
        throw std::invalid_argument("SortableArray capacity must be positive");
    }
}

std::size_t SortableArray::size() const noexcept
{
    return items_.size();
}

std::size_t SortableArray::capacity() const noexcept
{
    return capacity_;
}

std::vector<int> SortableArray::toVector() const
{
    return items_;
}

int SortableArray::at(std::size_t index) const
{
    return items_.at(index);
}

int SortableArray::minValue() const
{
    if (items_.empty())
    {
        throw EmptyArrayException("minValue");
    }
    return *std::min_element(items_.begin(), items_.end());
}

int SortableArray::maxValue() const
{
    if (items_.empty())
    {
        throw EmptyArrayException("maxValue");
    }
    return *std::max_element(items_.begin(), items_.end());
}

double SortableArray::averageValue() const
{
    if (items_.empty())
    {
        throw EmptyArrayException("averageValue");
    }
    long double sum = 0.0L;
    for (int v : items_)
    {
        sum += v;
    }
    return static_cast<double>(sum / static_cast<long double>(items_.size()));
}

void SortableArray::add(int value)
{
    if (items_.size() >= capacity_)
    {
        throw ArrayCapacityException(capacity_, items_.size() + 1);
    }
    items_.push_back(value);
}

void SortableArray::replaceAll(const std::vector<int>& values)
{
    if (values.size() > capacity_)
    {
        throw ArrayCapacityException(capacity_, values.size());
    }

    std::vector<int> prepared(values);
    items_.swap(prepared);
}

void SortableArray::sortRange(std::size_t first, std::size_t last, int methodCode)
{
    const SortMethod method = toMethod(methodCode);
    if (first >= last || last > items_.size())
    {
        throw InvalidRangeException(first, last, items_.size());
    }

    std::vector<int> prepared(items_);
    if (method == SortMethod::Insertion)
    {
        insertionSort(prepared, first, last);
    }
    else
    {
        std::sort(prepared.begin() + static_cast<std::ptrdiff_t>(first),
                  prepared.begin() + static_cast<std::ptrdiff_t>(last));
    }

    items_.swap(prepared);
}
