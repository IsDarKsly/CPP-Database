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

/// @brief Simple linked list class for reading the catagories, could have likely used something better, but wanted to finally use a linked list.
class stringll
{
private:
    static stringll* root;

public:
    
    stringll(std::string name)
    {
        DEBUG("stringll constructor called");
        this->name = name;

        if(root == nullptr)
        {
            root = this;
        }
    }

    ~stringll()
    {
        DEBUG("stringll deconstructor called");

        delete(next);
    }

    std::string name;
    stringll* next = nullptr;

    static void add(std::string name)
    {
        DEBUG("stringll add called");
        if(root == nullptr)
        {
            root = new stringll(name);
        }
        else
        {
            stringll* current = root;
            while(current->next != nullptr)
            {
                current = current->next;
            }
            current->next = new stringll(name);
        }
    }

    static std::string* getarray(int count)
    {
        DEBUG("stringll getarray called");
        if(root == nullptr)
        {
            std::cerr << "ROOT IS NULL ON THE LINKED LIST\n" << std::endl;
            exit(1);
        }

        std::string* array = new std::string[count];
        stringll* current = root;

        int i = 0;
        while(current != nullptr)
        {
            //std::cout << "Copying value: " << current->name << std::endl;
            array[i] = current->name;
            i++;
            current = current->next;
        }

        return array;
    }

    static void DEBUG_PRINT_ALL()
    {
        DEBUG("stringll DEBUG_PRINT_ALL called");
        if(root == nullptr)
        {
            std::cerr << "ROOT IS NULL ON THE LINKED LIST\n" << std::endl;
            exit(1);
        }
        stringll* current = root;

        while(current != nullptr)
        {
            std::cout << "Value: " << current->name << std::endl;
            current = current->next;
        }
    }   
};

stringll* stringll::root = nullptr;