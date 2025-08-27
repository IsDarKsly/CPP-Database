#include <iostream>
#include <ios>
#include <limits>
#include <cstdlib>
#include <random>
#include <string>
#include <iomanip>
#include <fstream>

#include "entry.hpp"
#include "stringll.hpp"
#include "database.hpp"


#if defined(_WIN32)
    #define OS_NAME "Windows"
    #define CLEAR system("cls")
#elif defined(_WIN64)
    #define OS_NAME "Windows"
    #define CLEAR system("cls")
#elif defined(__linux__)
    #define OS_NAME "Linux"
    #define CLEAR system("clear")
#endif

#if defined(DEBUGMODE)
    #define DEBUG(x) std::cout << "DEBUG: " << x << std::endl;
#else
    #define DEBUG(x) ;
#endif



int mainmenu();

void getinput(int* p_int, const char * line, int min, int max);

void createmode();

void loadmode();

void datamode();

void createentry();

void deleteentry();

void sampledata();

void savedata();

int main()
{
    dataset set;
    return mainmenu();
}

int mainmenu()
{

    std::cout << "Welcome to the C++ DB app!" << std::endl;
    std::cout << "What would you like to do?" << std::endl;

    while(1)
    {
        int input;
        getinput(&input, "[1] Create DB\n[2] Load DB", 1, 2);

        if(input == 1) createmode();
        else loadmode();
    }
    return 0;
}

/// @brief Gets a users constrained input in integer format. Supports up to 10 options
/// @param p_int pointer to the decision integer
/// @param line repeatable line
/// @param min minimum value, cannot be less than 0
/// @param max maximum value, cannot be greater than 9
/// @return 
void getinput(int* p_int, const char * line, int min, int max)
{
    bool parsed = false;

    char user_input;

    try
    {
        if(min < 0 || max > 9 || min > max)
        {
            throw 420;
        }

        while(!parsed)
        {
            std::cout << line << std::endl;
            
            std::cin >> user_input;

            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            
            *p_int = user_input - '0';

            CLEAR;

            if(*p_int >= min && *p_int <= max)
            {
                parsed = true;
            }
            else
            {
                std::cout << "Incorrect input, try again!" << std::endl;
            }
        }
    }
    catch(int e)
    {
        std::cerr << "Error number: " << e << std::endl;
        switch (e)
        {
        case 420:
            std::cerr << "Min or Max parameters out of bound" << std::endl;

            if(min < 0) std::cerr << "Min ("<< min << ") less than zero" << std::endl;
            if(max > 9) std::cerr << "Max ("<< max << ") greater than 9" << std::endl;
            if(min > max) std::cerr << "Min ("<< min << ") greater than Max ("<< max << ")" << std::endl;
            break;
        
        default:
            break;
        }

        exit(e);
    }
}

void createmode()
{
    int entries = 0;
    std::string userinput;

    std::cout << "Let's begin by entering some catagories" << std::endl;
    std::cout << "Enter DONE when finished" << std::endl;

    while(1)
    {
        std::cin >> userinput;
        if(userinput == "DONE")
        {
            if(entries == 0)
            {
                std::cout << "A database needs catagories!" << std::endl;
                return;
            }
            break;
        }
        else
        {
            int choice;
            getinput(&choice, "Are you sure?\n[1] Yes\n[2] No", 1, 2);
            
            if(choice == 1)
            {
                entries++;
                stringll::add(userinput);
            }
            else
            {
                std::cout << "Input removed" << std::endl;
            }
            std::cout << "Let's continue entering some catagories" << std::endl;
            std::cout << "Enter DONE when finished" << std::endl;
        }
    }

    std::string* arr = stringll::getarray(entries);

    std::cout << "Catagories are as follows:" << std::endl;

    for(int i = 0; i < entries; i++)
    {
        std::cout << arr[i] << std::endl;
    }

    dataset::populate_catagories(arr, entries);

    datamode();

}

void generateentry(std::string input)
{
    entry newEntry;
    newEntry.size = dataset::get_cat_count();
    newEntry.element = new std::string[newEntry.size];

    std::string readword = "";
    int current_index = 0;
    for(int i = 0; i < input.length(); i++)
    {
        if(input[i] == ',') 
        {
            newEntry.element[current_index] = readword;
            current_index++;
            readword = "";
            continue;
        }
        readword += input[i];
    }
    newEntry.element[current_index] = readword;

    dataset::add_entries(newEntry);
}

void generatecatagories(std::string input)
{
    int cat_count = 1;
    std::string readword = "";
    for(int i = 0; i < input.length(); i++)
    {
        if(input[i] == ',') 
        {
            cat_count++;
            stringll::add(readword);
            readword = "";
            continue;
        }
        readword += input[i];
    }
    stringll::add(readword);

    std::string* arr = stringll::getarray(cat_count);

    dataset::populate_catagories(arr, cat_count);
}

void loadmode()
{
    std::cout << "Enter the Path of the file you want to attempt opening!" << std::endl;
    std::string path;

    std::cin >> path;

    std::ifstream file;
    file.open(path);

    if(file.is_open())
    {
        std::cout << "File opened! Attempting to parse..." <<std::endl;
        
        std::string filestring;
        std::getline(file, filestring);
        generatecatagories(filestring);

        while(std::getline(file, filestring)) generateentry(filestring);

        file.close();

        std::cout << "File Parsed!" <<std::endl;

        datamode();
    }
    else
    {
        std::cout << "File open failure..." << std::endl;
    }
}

void datamode()
{
    std::cout << "What would you like to do?" << std::endl;

    int userchoice;

    while(1)
    {
        DEBUG("Datamode loop");

        getinput(&userchoice, "[1] Create Entry\n[2] Remove Entry\n[3] Add Catagory\n[4] Remove Catagory\n[5] View Data\n[6] Analyze Data\n[7] Generate Sample Data\n[8] Save Data\n[9] Exit", 1, 9);

        DEBUG("Datamode - evaluating input " << userchoice);
        switch (userchoice)
        {
        case 1:
            /* Create Entry */
            createentry();
            break;
        case 2:
            /* Remove Entry */
            deleteentry();
            break;
        case 3:
            /* Add Catagory */
            break;
        case 4:
            /* Remove Catagory */
            break;
        case 5:
            /* View Data */
            dataset::display_database();
            break;
        case 6:
            /* Analyze Data */
            break;
        case 7:
            /* Sample Data */
            sampledata();
            break;
        case 8:
            /* Save Data */
            savedata();
            break;
        case 9:
            /* Exit App */
            exit(0);
            break;    
        default:
            break;
        }
    }
    DEBUG("Exiting datamode while loop");
}

void createentry()
{
    std::cout << "Let's create an entry to this database!" << std::endl;

    int entry_count = 0;
    std::string user_input;
    
    entry user_entry;

    user_entry.size = dataset::get_cat_count();
    user_entry.element = new std::string[user_entry.size];
    

    while(entry_count < dataset::get_cat_count())
    {
        std::cout << "Enter an entry for the given catagory: " << dataset::get_catagory(entry_count) << std::endl;
        std::cin >> user_input;

        int choice;
        getinput(&choice, "Are you sure?\n[1] Yes\n[2] No", 1, 2);

        if(choice == 1)
        {
            user_entry.element[entry_count] = user_input;
            entry_count++;
        }
    }

    dataset::add_entries(user_entry);
    std::cout << "New entry added!" << std::endl;
}

void deleteentry()
{
    std::cout << "Enter a keyword for an entry and i'll search for it" << std::endl;
    std::string user_input;
    std::cin >> user_input;
    entry* possibleentry = nullptr;
    
    int choice = 2;
    int searchprogress = 0;
    while(choice == 2)
    {
        possibleentry = dataset::searchentries(user_input, &searchprogress);
        if(possibleentry == nullptr)
        {
            std::cout << "Could not find this entry!" << std::endl;
            return;
        }
        searchprogress++;
        std::cout << "Is this the entry?" << std::endl;
        std::cout << possibleentry->getstring() << std::endl;

        getinput(&choice, "\n[1] Yes\n[2] No", 1, 2);
    }

    dataset::remove_entries(possibleentry);
}

/// @brief AI GENERATED FUNCTION, should just help me poulate the database quickly
/// @param len 
/// @return 
std::string random_string(std::size_t len) {
    static const char charset[] =
        "abcdefghijklmnopqrstuvwxyz"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "0123456789";

    static thread_local std::mt19937 rng{std::random_device{}()};
    static thread_local std::uniform_int_distribution<std::size_t> dist(0, sizeof(charset) - 2);

    std::string result;
    result.reserve(len);
    for (std::size_t i = 0; i < len; ++i) {
        result.push_back(charset[dist(rng)]);
    }
    return result;
}

void sampledata()
{
    int cat_count = dataset::get_cat_count();

    for (int i = 0; i < 20; i++) {
        entry gen_entry;
        gen_entry.size = cat_count;
        gen_entry.element = new std::string[gen_entry.size];

        for (int j = 0; j < gen_entry.size; j++) {
            gen_entry.element[j] = random_string(8); // 8-char random string
        }

        dataset::add_entries(gen_entry);
    }
}

void savedata()
{
    	std::cout << "Enter the Path of the file you want to save to!" << std::endl;
    	std::string path;
    	std::cin >> path;
	std::ofstream file;
	file.open(path);

	if(file.is_open())	//We have an open file
	{
		file << dataset::getcatagorystream();
		file << dataset::getentrystream();
		file.close();
	}
	
}
