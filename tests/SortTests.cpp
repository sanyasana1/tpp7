#include <gtest/gtest.h>

#include <stdexcept>
#include <vector>

#include "SearchUtils.h"
#include "SortableArray.h"
#include "SortExceptions.h"

namespace {

const int kInsertion = static_cast<int>(SortMethod::Insertion);
const int kStandard = static_cast<int>(SortMethod::Standard);

SortableArray makeArray(std::size_t capacity, const std::vector<int>& values)
{
    SortableArray arr(capacity);
    for (int v : values)
    {
        arr.add(v);
    }
    return arr;
}

} 


TEST(SortableArrayTest, CreatesEmptyArrayWithCapacity)
{
    SortableArray arr(5);
    EXPECT_EQ(arr.size(), 0u);
    EXPECT_EQ(arr.capacity(), 5u);
}

TEST(SortableArrayTest, AddsAndReadsValues)
{
    SortableArray arr(3);
    arr.add(3);
    arr.add(1);
    EXPECT_EQ(arr.size(), 2u);
    EXPECT_EQ(arr.at(0), 3);
    EXPECT_EQ(arr.at(1), 1);
}

TEST(SortableArrayTest, MinMaxOfValues)
{
    SortableArray arr = makeArray(5, {4, 1, 7});
    EXPECT_EQ(arr.minValue(), 1);
    EXPECT_EQ(arr.maxValue(), 7);
}

TEST(SortableArrayTest, AverageOfIntegerValues)
{
    SortableArray arr = makeArray(3, {2, 4, 6});
    EXPECT_NEAR(arr.averageValue(), 4.0, 1e-12);
}

TEST(SortableArrayTest, AverageWithRepeatingFraction)
{
    SortableArray arr = makeArray(3, {1, 2, 4});
    EXPECT_NEAR(arr.averageValue(), 7.0 / 3.0, 1e-12);
}

TEST(SortableArrayTest, SortsWholeArrayWithInsertion)
{
    SortableArray arr = makeArray(5, {5, 2, 9, 1, 7});
    arr.sortRange(0, arr.size(), kInsertion);
    const std::vector<int> expected = {1, 2, 5, 7, 9};
    EXPECT_EQ(arr.toVector(), expected);
}

TEST(SortableArrayTest, SortsWholeArrayWithStandardMethod)
{
    SortableArray arr = makeArray(5, {5, 2, 9, 1, 7});
    arr.sortRange(0, arr.size(), kStandard);
    const std::vector<int> expected = {1, 2, 5, 7, 9};
    EXPECT_EQ(arr.toVector(), expected);
}

TEST(SortableArrayTest, SortsOnlySelectedRange)
{
    SortableArray arr = makeArray(5, {9, 8, 7, 6, 5});
    arr.sortRange(1, 4, kInsertion); 
    const std::vector<int> expected = {9, 6, 7, 8, 5};
    EXPECT_EQ(arr.toVector(), expected);
}

TEST(SortableArrayTest, ReplaceAllSucceeds)
{
    SortableArray arr = makeArray(3, {1});
    arr.replaceAll({3, 1});
    const std::vector<int> expected = {3, 1};
    EXPECT_EQ(arr.toVector(), expected);
}


TEST(SortableArrayBoundaryTest, SingleElementRangeIsValid)
{
    SortableArray arr = makeArray(4, {4, 3, 2, 1});
    arr.sortRange(2, 3, kStandard);
    const std::vector<int> expected = {4, 3, 2, 1};
    EXPECT_EQ(arr.toVector(), expected);
}

TEST(SortableArrayBoundaryTest, FillsArrayToExactCapacity)
{
    SortableArray arr(2);
    arr.add(1);
    arr.add(2);
    EXPECT_EQ(arr.size(), arr.capacity());
}

TEST(SortableArrayBoundaryTest, CapacityOneAcceptsOneValue)
{
    SortableArray arr(1);
    arr.add(10);
    EXPECT_EQ(arr.size(), 1u);
    EXPECT_EQ(arr.minValue(), 10);
    EXPECT_EQ(arr.maxValue(), 10);
}


TEST(SortableArrayErrorTest, ZeroCapacityIsRejected)
{
    EXPECT_THROW((void)SortableArray(0), std::invalid_argument);
}

TEST(SortableArrayErrorTest, AddRejectsWhenFull)
{
    SortableArray arr = makeArray(2, {1, 2});
    EXPECT_THROW(arr.add(3), ArrayCapacityException);
    EXPECT_EQ(arr.size(), 2u);
}

TEST(SortableArrayErrorTest, CapacityExceptionCarriesDiagnostics)
{
    SortableArray arr = makeArray(2, {1, 2});
    try
    {
        arr.add(3);
        ADD_FAILURE() << "ArrayCapacityException was not thrown";
    }
    catch (const ArrayCapacityException& ex)
    {
        EXPECT_EQ(ex.capacity(), 2u);
        EXPECT_EQ(ex.requested(), 3u);
    }
}

TEST(SortableArrayErrorTest, ExtremesOfEmptyArrayAreRejected)
{
    SortableArray arr(3);
    EXPECT_THROW(arr.minValue(), EmptyArrayException);
    EXPECT_THROW(arr.maxValue(), EmptyArrayException);
    EXPECT_THROW(arr.averageValue(), EmptyArrayException);
}

TEST(SortableArrayErrorTest, InvalidRangeIsRejected)
{
    SortableArray arr = makeArray(5, {5, 4, 3, 2, 1});
    EXPECT_THROW(arr.sortRange(3, 2, kInsertion), InvalidRangeException); 
    EXPECT_THROW(arr.sortRange(0, 6, kInsertion), InvalidRangeException); 
    EXPECT_THROW(arr.sortRange(2, 2, kInsertion), InvalidRangeException); 
}

TEST(SortableArrayErrorTest, InvalidRangeIsCatchableAsBaseClass)
{
    SortableArray arr = makeArray(5, {5, 4, 3, 2, 1});
    EXPECT_THROW(arr.sortRange(0, 9, kStandard), SortException);
}

TEST(SortableArrayErrorTest, InvalidRangeCarriesDiagnostics)
{
    SortableArray arr = makeArray(5, {5, 4, 3, 2, 1});
    try
    {
        arr.sortRange(3, 9, kInsertion);
        ADD_FAILURE() << "InvalidRangeException was not thrown";
    }
    catch (const InvalidRangeException& ex)
    {
        EXPECT_EQ(ex.first(), 3u);
        EXPECT_EQ(ex.last(), 9u);
        EXPECT_EQ(ex.size(), 5u);
    }
}

TEST(SortableArrayErrorTest, InvalidSortMethodIsRejected)
{
    SortableArray arr = makeArray(3, {3, 2, 1});
    EXPECT_THROW(arr.sortRange(0, 3, 5), InvalidSortMethodException);
    EXPECT_THROW(arr.sortRange(0, 3, -1), InvalidSortMethodException);
}

TEST(SortableArrayErrorTest, AtOutOfRangeThrowsStdOutOfRange)
{
    SortableArray arr = makeArray(5, {1, 2, 3, 4, 5});
    EXPECT_THROW(arr.at(5), std::out_of_range);
}


TEST(SortableArrayStrongGuaranteeTest, SortRangePreservesStateOnInvalidRange)
{
    SortableArray arr = makeArray(5, {5, 2, 9, 1, 7});
    const std::vector<int> before = arr.toVector();

    EXPECT_THROW(arr.sortRange(3, 9, kStandard), InvalidRangeException);

    EXPECT_EQ(arr.toVector(), before);
}

TEST(SortableArrayStrongGuaranteeTest, SortRangePreservesStateOnInvalidMethod)
{
    SortableArray arr = makeArray(5, {5, 2, 9, 1, 7});
    const std::vector<int> before = arr.toVector();

    EXPECT_THROW(arr.sortRange(0, 5, 42), InvalidSortMethodException);

    EXPECT_EQ(arr.toVector(), before);
}

TEST(SortableArrayStrongGuaranteeTest, ReplaceAllPreservesStateOnException)
{
    SortableArray arr = makeArray(3, {1, 2});
    const std::vector<int> before = arr.toVector();

    EXPECT_THROW(arr.replaceAll({5, 6, 7, 8}), ArrayCapacityException);

    ASSERT_EQ(arr.size(), 2u);
    EXPECT_EQ(arr.toVector(), before);
}

TEST(SortableArrayStrongGuaranteeTest, AddPreservesStateOnException)
{
    SortableArray arr = makeArray(2, {7, 8});
    const std::vector<int> before = arr.toVector();

    EXPECT_THROW(arr.add(9), ArrayCapacityException);

    EXPECT_EQ(arr.toVector(), before);
}

TEST(SortableArrayInvariantTest, SizeNeverExceedsCapacity)
{
    SortableArray arr(3);
    for (int v = 0; v < 10; ++v)
    {
        try
        {
            arr.add(v);
        }
        catch (const ArrayCapacityException&)
        {
           
        }
        EXPECT_LE(arr.size(), arr.capacity());
    }
    EXPECT_GT(arr.capacity(), 0u);
}


TEST(SearchUtilsTest, BinarySearchFindsIndexOrMinusOne)
{
    const int data[] = {1, 3, 5, 7};
    EXPECT_EQ(binarySearch(data, 4, 5), 2);
    EXPECT_EQ(binarySearch(data, 4, 4), -1);
    EXPECT_EQ(binarySearch(data, 0, 4), -1);
}

TEST(SearchUtilsTest, CountFirstAndLastOccurrences)
{
    const int data[] = {2, 4, 4, 4, 9};
    EXPECT_EQ(countOccurences(data, 5, 4), 3);
    EXPECT_EQ(findFirst(data, 5, 4), 1);
    EXPECT_EQ(findLast(data, 5, 4), 3);
    EXPECT_EQ(countOccurences(data, 5, 100), 0);
    EXPECT_EQ(findFirst(data, 5, 100), -1);
    EXPECT_EQ(findLast(data, 5, 100), -1);
}

TEST(SearchUtilsTest, KthElementReturnsOrderStatistic)
{
    const int data[] = {9, 4, 7};
    EXPECT_EQ(kthElement(data, 3, 1), 4);
    EXPECT_EQ(kthElement(data, 3, 3), 9);
}

TEST(SearchUtilsErrorTest, KthElementRejectsOutOfRangeK)
{
    const int data[] = {9, 4, 7};
    EXPECT_THROW(kthElement(data, 3, 0), std::out_of_range);
    EXPECT_THROW(kthElement(data, 3, 4), std::out_of_range);
}
