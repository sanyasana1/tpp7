#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>

void throwInteger()
{
    throw 42;
}

void throwString()
{
    throw std::string("Invalid array values");
}

void throwStandardException()
{
    throw std::invalid_argument("Invalid sort range");
}

void demonstrateSlicing()
{
    try
    {
        throw std::invalid_argument("Invalid argument");
    }
    catch (const std::exception& ex)
    {
        throw ex;
    }
}

void demonstrateCorrectRethrow()
{
    try
    {
        throw std::invalid_argument("Invalid argument");
    }
    catch (const std::exception&)
    {
        throw;
    }
}

int main()
{
    std::cout << "=== Example 1: int exception ===\n";
    try
    {
        throwInteger();
    }
    catch (const std::exception& ex)
    {
        std::cout << "std::exception: " << ex.what() << '\n';
    }
    catch (int error)
    {
        std::cout << "Caught int: " << error << '\n';
    }

    std::cout << "\n=== Example 2: string exception ===\n";
    try
    {
        throwString();
    }
    catch (const std::exception& ex)
    {
        std::cout << "std::exception: " << ex.what() << '\n';
    }
    catch (const std::string& error)
    {
        std::cout << "Caught string: " << error << '\n';
    }

    std::cout << "\n=== Example 3: standard exception ===\n";
    try
    {
        throwStandardException();
    }
    catch (const std::exception& ex)
    {
        std::cout << "Caught std::exception: " << ex.what() << '\n';
    }

    std::cout << "\n=== Example 4: catch(...) catches any type ===\n";
    try
    {
        throwInteger();
    }
    catch (...)
    {
        std::cout << "Caught by catch(...)\n";
    }

    std::cout << "\n=== Example 5: object slicing with throw ex; ===\n";
    try
    {
        demonstrateSlicing();
    }
    catch (const std::invalid_argument&)
    {
        std::cout << "Caught invalid_argument\n";
    }
    catch (const std::exception&)
    {
        std::cout << "Caught base std::exception\n";
    }

    std::cout << "\n=== Example 6: correct rethrow with throw; ===\n";
    try
    {
        demonstrateCorrectRethrow();
    }
    catch (const std::invalid_argument&)
    {
        std::cout << "Caught invalid_argument\n";
    }
    catch (const std::exception&)
    {
        std::cout << "Caught base std::exception\n";
    }

    return 0;
}
