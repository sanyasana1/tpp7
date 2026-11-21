#pragma once

#include <cstddef>
#include <stdexcept>
#include <string>

class SortException : public std::runtime_error
{
public:
    explicit SortException(const std::string& message)
        : std::runtime_error(message)
    {
    }
};

class InvalidRangeException : public SortException
{
public:
    InvalidRangeException(std::size_t first, std::size_t last, std::size_t size)
        : SortException("Invalid sort range: [" + std::to_string(first) + ", " +
                        std::to_string(last) + ") for size " + std::to_string(size)),
          first_(first),
          last_(last),
          size_(size)
    {
    }

    std::size_t first() const noexcept { return first_; }
    std::size_t last() const noexcept { return last_; }
    std::size_t size() const noexcept { return size_; }

private:
    std::size_t first_;
    std::size_t last_;
    std::size_t size_;
};

class EmptyArrayException : public SortException
{
public:
    explicit EmptyArrayException(const std::string& operation)
        : SortException("Operation '" + operation + "' requires a non-empty array"),
          operation_(operation)
    {
    }

    const std::string& operation() const noexcept { return operation_; }

private:
    std::string operation_;
};

class ArrayCapacityException : public SortException
{
public:
    ArrayCapacityException(std::size_t capacity, std::size_t requested)
        : SortException("Array capacity exceeded: capacity " + std::to_string(capacity) +
                        ", requested " + std::to_string(requested)),
          capacity_(capacity),
          requested_(requested)
    {
    }

    std::size_t capacity() const noexcept { return capacity_; }
    std::size_t requested() const noexcept { return requested_; }

private:
    std::size_t capacity_;
    std::size_t requested_;
};

class InvalidSortMethodException : public SortException
{
public:
    explicit InvalidSortMethodException(int code)
        : SortException("Invalid sort method code: " + std::to_string(code)),
          code_(code)
    {
    }

    int code() const noexcept { return code_; }

private:
    int code_;
};
