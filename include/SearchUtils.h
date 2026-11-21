#pragma once

#include <cstddef>

int binarySearch(const int* data, std::size_t n, int value);

int countOccurences(const int* data, std::size_t n, int value);

int findFirst(const int* data, std::size_t n, int value);

int findLast(const int* data, std::size_t n, int value);


int kthElement(const int* data, std::size_t n, std::size_t k);
