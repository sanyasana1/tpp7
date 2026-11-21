#pragma once

#include <cstddef>
#include <vector>

#include "SortExceptions.h"


enum class SortMethod : int
{
    Insertion = 0,
    Standard = 1 
};


class SortableArray
{
public:
    explicit SortableArray(std::size_t capacity);

    std::size_t size() const noexcept;
    std::size_t capacity() const noexcept;

    std::vector<int> toVector() const;

    int at(std::size_t index) const;

    int minValue() const;
    int maxValue() const;
    double averageValue() const;


    void add(int value);
    void replaceAll(const std::vector<int>& values);
    void sortRange(std::size_t first, std::size_t last, int methodCode);

private:
    std::vector<int> items_;
    std::size_t capacity_;
};
