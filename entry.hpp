#pragma once

#include <iostream>
#include <ios>
#include <limits>
#include <cstdlib>
#include <string>
#include <iomanip>

#if defined(DEBUGMODE)
    #define DEBUG(x) std::cout << "DEBUG: " << x << std::endl;
#else
    #define DEBUG(x) ;
#endif

/// @brief The entries to our database, should have as many string elements as the number of catagories
class entry
{
private:

public:
    std::string* element = nullptr;
    unsigned long long size = 0;

    entry(){DEBUG("entry object constructor called");}

    // Copy constructor (deep copy)
    entry(const entry& other)
        : element(other.size ? new std::string[other.size] : nullptr),
            size(other.size)
    {
        for (int i = 0; i < size; ++i) element[i] = other.element[i];
    }

    // Move constructor
    entry(entry&& other) noexcept
        : element(other.element), size(other.size)
    {
        other.element = nullptr;
        other.size = 0;
    }

    ~entry()
    {
        DEBUG("entry deconstructor called, this entry exists at: " << this << " with elements at " << element);
        delete[] element;
    }

    void allocate_array(long long s)
    {
        DEBUG("entry allocate_array called");
        delete[] element;
        this->size=s;
        element = size ? new std::string[size] : nullptr;
    }

    /// @brief Instead of changing the value of a given entry variable, will copy all data from a given right-hand entry
    /// @param other Some other entry object, will remain unchanged
    /// @return The same left hand variable with new data from the right hand variable
    entry& operator=(const entry& other)
    {
        DEBUG("SOFT COPY CALLED FOR ENTRY");
        if (this == &other) return *this;
        std::string* newdata = other.size ? new std::string[other.size] : nullptr;

        for(int i = 0; i < other.size; i++)
        {
            newdata[i] = other.element[i];
        }

        delete[] element;
        element = newdata;
        size = other.size;
        return *this;
    }

    /// @brief Instead of changing the value of a given entry variable, will copy all data from a given right-hand entry
    /// @param other Some other entry object, will destroy the incoming variable
    /// @return The same left hand variable with new data from the right hand variable
    entry& operator=(entry&& other) noexcept
    {
        DEBUG("FULL MOVE CALLED FOR ENTRY");
        if (this == &other) return *this;
        delete[] element;
        element = other.element;
        size = other.size;

        other.element = nullptr;
        other.size = 0;
        
        return *this;
    }

    std::string getstring()
    {
        std::string retstring = "|";
        for(int i = 0; i < size; i++)
        {
            retstring.append(element[i]);
            retstring.append("|");
        }
        return retstring;
    }

};

