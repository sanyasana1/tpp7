#include <exception>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <vector>

#include "ScopeLogger.h"
#include "SearchUtils.h"
#include "SortableArray.h"
#include "SortExceptions.h"

namespace {

void printArray(const char* title, const SortableArray& arr)
{
    std::cout << title << ": [";
    const std::vector<int> values = arr.toVector();
    for (std::size_t i = 0; i < values.size(); ++i)
    {
        if (i != 0)
        {
            std::cout << ", ";
        }
        std::cout << values[i];
    }
    std::cout << "]\n";
}

void sortLevel3(SortableArray& arr)
{
    ScopeLogger logger("sortLevel3");
    std::cout << "Sorting range [3, 9) ...\n";
    arr.sortRange(3, 9, static_cast<int>(SortMethod::Standard));
}

void sortLevel2(SortableArray& arr)
{
    ScopeLogger logger("sortLevel2");
    try
    {
        sortLevel3(arr);
    }
    catch (const SortException& ex)
    {
        std::cout << "[LOG] sortLevel2 caught: " << ex.what() << '\n';
        throw;
    }
}

void sortLevel1(SortableArray& arr)
{
    ScopeLogger logger("sortLevel1");
    sortLevel2(arr);
}

} 

int main()
{
    std::cout << std::fixed << std::setprecision(2);

    SortableArray arr(6);
    for (int v : {5, 2, 9, 1, 7})
    {
        arr.add(v);
    }
    printArray("Initial", arr);

    arr.sortRange(0, arr.size(), static_cast<int>(SortMethod::Insertion));
    printArray("Sorted", arr);
    std::cout << "Min: " << arr.minValue()
              << ", Max: " << arr.maxValue()
              << ", Average: " << arr.averageValue() << '\n';

    const std::vector<int> sorted = arr.toVector();
    std::cout << "binarySearch(7) index: "
              << binarySearch(sorted.data(), sorted.size(), 7) << '\n';

    std::cout << "\n--- Error scenarios ---\n";

    try
    {
        arr.sortRange(0, 3, 7);
    }
    catch (const InvalidSortMethodException& ex)
    {
        std::cout << "InvalidSortMethodException: " << ex.what() << '\n';
    }

    try
    {
        arr.add(10);
        arr.add(11);
    }
    catch (const ArrayCapacityException& ex)
    {
        std::cout << "ArrayCapacityException: capacity=" << ex.capacity()
                  << ", requested=" << ex.requested() << '\n';
    }

    try
    {
        SortableArray empty(3);
        const int m = empty.minValue();
        std::cout << "Min of empty array: " << m << '\n';
    }
    catch (const EmptyArrayException& ex)
    {
        std::cout << "EmptyArrayException: " << ex.what() << '\n';
    }

    try
    {
        std::cout << arr.at(42) << '\n';
    }
    catch (const std::out_of_range&)
    {
        std::cout << "std::out_of_range: index 42 is outside the array\n";
    }

    std::cout << "\n--- Strong guarantee ---\n";
    printArray("Before replaceAll", arr);
    try
    {
        arr.replaceAll({1, 2, 3, 4, 5, 6, 7});
    }
    catch (const ArrayCapacityException& ex)
    {
        std::cout << "replaceAll rejected: capacity=" << ex.capacity()
                  << ", requested=" << ex.requested() << '\n';
    }
    printArray("After failed replaceAll", arr);

    std::cout << "\n--- Stack unwinding ---\n";
    try
    {
        sortLevel1(arr);
    }
    catch (const std::exception& ex)
    {
        std::cout << "[MAIN] Exception: " << ex.what() << '\n';
    }

    printArray("Final", arr);
    std::cout << "Program continues normally.\n";
    return 0;
}
