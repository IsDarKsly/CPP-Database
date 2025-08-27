#pragma once

#include <iostream>
#include <ios>
#include <limits>
#include <cstdlib>
#include <string>
#include <iomanip>

#include "entry.hpp"
#include "stringll.hpp"

#if defined(DEBUGMODE)
    #define DEBUG(x) std::cout << "DEBUG: " << x << std::endl;
#else
    #define DEBUG(x) ;
#endif

/// @brief The overall dataset, should have an array for catagories and entries. Should be self balancing and capable of sorting
class dataset
{
private:
    static dataset* instance;

    int catagory_array_size = 0;
    int catagory_count = 0;

    int entry_array_size = 0;
    int entry_count = 0;
public:

    std::string* catagories = nullptr;

    entry* entries = nullptr;

    dataset()
    {
        DEBUG("dataset object created");
        if(instance == nullptr) instance = this;
    }

    ~dataset()
    {
        DEBUG("dataset deconstructor called");
        delete[] catagories;
        delete[] entries;
    }

    static int get_cat_count() { return instance->catagory_count; }

    static int get_entry_count() { return instance->entry_count; }

    static void populate_catagories(std::string* catagories,int count)
    {
        DEBUG("dataset populate_catagories called");

        instance->catagories = new std::string[count];
        instance->catagory_count = count;
        instance->catagory_array_size = count;

        for(int i = 0; i < count; i++)
        {
            instance->catagories[i] = catagories[i];
        }
    }

    static void add_catagory(std::string catagory)
    {
        DEBUG("dataset add_catagory called");

        if(instance->catagory_count>=(instance->catagory_array_size/2))
        {
            resize_cat_array();
        }

        instance->catagories[instance->catagory_count] = catagory;
        instance->catagory_count++;
    }

    static bool remove_catagory(std::string catagory)
    {
        DEBUG("dataset remove_catagory called");

        bool found = false;
        for(int i = 0; i < instance->catagory_count; i++)
        {
            if(!found && instance->catagories[i] == catagory)
            {
                found = true;
            }
            if(i < instance->catagory_count-1 && found)
            {
                instance->catagories[i] = instance->catagories[i+1];
            }
        }
        instance->catagory_count--;
        return found;
    }

    static void resize_cat_array()
    {
        DEBUG("dataset resize_cat_array called");

        instance->catagory_array_size == 0 ? instance->catagory_array_size = 5 : instance->catagory_array_size *=2;
        std::string* new_arr = new std::string[instance->catagory_array_size];

        for(int i = 0; i < instance->catagory_count; i++)
        {
            new_arr[i] = instance->catagories[i];
        }

        if(instance->catagories != nullptr) delete[] instance->catagories;

        instance->catagories = new_arr;
    }

    static std::string get_catagory(int index)
    {
        DEBUG("dataset get_catagory called");

        if(index < 0 || index > instance->catagory_count)
        {
            std::cerr << "Out of range call for get_catagory" << std::endl;
            exit(1); 
        }

        return instance->catagories[index];

    }

    static void populate_entries(entry* entries, int count)
    {
        DEBUG("dataset populate_entries called");

        instance->entries = new entry[count];
        instance->entry_count = count;
        instance->entry_array_size = count;

        for(int i = 0; i < count; i++)
        {
            instance->entries[i] = entries[i];
        }
    }

    static void add_entries(entry& entry_item)
    {
        DEBUG("dataset add_entries called");

        if(instance->entry_count>=(instance->entry_array_size/2))
        {
            resize_entries_array();
        }

        instance->entries[instance->entry_count] = entry_item;
        instance->entry_count++;

        DEBUG("dataset add_entries finished");
    }

    /// @brief 
    /// @param entry 
    static void remove_entries(entry* _entry)
    {
        DEBUG("dataset remove_entries called");

        bool found = false;
        for(int i = 0; i < instance->entry_count; i++)
        {
            if(!found && &instance->entries[i] == _entry)
            {
                found = true;
                DEBUG("Found!");
            }
            if(found && i < instance->entry_count-1)
            {
                instance->entries[i] = instance->entries[i+1];
                DEBUG("Shuffling: " << i << " to " << i+1);
            }
        }
        instance->entry_count--;
        DEBUG("dataset remove_entries finished");
    }

    static void resize_entries_array()
    {
        DEBUG("dataset resize_entries_array called");

        instance->entry_array_size == 0 ? instance->entry_array_size = 5 : instance->entry_array_size *=2;
        entry* new_arr = new entry[instance->entry_array_size];

        for(int i = 0; i < instance->entry_count; i++)
        {
            DEBUG("dataset entry copy made for index " << i );
            new_arr[i] = instance->entries[i];
        }

        if(instance->entries != nullptr) delete[] instance->entries;

        instance->entries = new_arr;

        DEBUG("dataset resize_entries_array finished");
    }

    static entry& get_entry(int index)
    {
        DEBUG("dataset get_entry called");

        if(index < 0 || index > instance->entry_count)
        {
            std::cerr << "Out of range call for get_entry" << std::endl;
            exit(1); 
        }

        return instance->entries[index];
    }

    /// @brief Will formulate the db into string format to display in the console
    static void display_database()
    {
        DEBUG("dataset display_database called");

        std::cout << std::left;
        for(int i = 0; i < instance->catagory_count; i++)
        {
            std::cout << std::setw(20) << instance->catagories[i];
        }

        std::cout << std::endl;

        for(int i = 0; i < instance->entry_count; i++)
        {
            for(int x = 0; x < instance->entries[i].size; x++)
            {
                //DEBUG("Entry position: " << i << ", " << (instance->entries[i].element == nullptr ? "Does not exist," : "Exists,") << " Element: " << x);
                std::cout << std::setw(20) << instance->entries[i].element[x];
            }
            std::cout << std::endl;
        }
    }

    static entry* searchentries(std::string query, int* i)
    {
        DEBUG("Beginning Search for entry " << query);
        for( ;(*i) < instance->entry_count; (*i)++)
        {
            DEBUG("Position " << *i);
            for(int x = 0; x < instance->entries[*i].size; x++)
            {
                DEBUG("Position X " << x);
                if(!query.compare(instance->entries[*i].element[x]))
                {
                    return &instance->entries[*i];
                }
            }
        }
        return nullptr;
    }

    static std::string getcatagorystream()
    {
	std::string retstring = "";

	for(int i = 0; i < instance->catagory_count-1; i++)
	{
		retstring += instance->catagories[i];
		retstring += ",";
	}
	retstring += instance->catagories[instance->catagory_count-1];
	retstring += "\n";

	return retstring;
    }

    static std::string getentrystream()
    {
    	std::string retstring = "";

	for(int i = 0; i < instance->entry_count; i++)
	{
		for(int x = 0; x < instance->entries[i].size-1; x++)
		{
			retstring += instance->entries[i].element[x];
			retstring += ",";
		}

		retstring += instance->entries[i].element[instance->entries[i].size-1];
		retstring += "\n";
	}

	return retstring;
 
    }
};

dataset* dataset::instance = nullptr;
