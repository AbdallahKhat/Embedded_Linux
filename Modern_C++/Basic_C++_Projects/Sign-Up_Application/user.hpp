#ifndef USER_HPP
#define USER_HPP


/* Dependancies */
#include <iostream>
#include <string>
#include <array>
#include <limits>

/* Macros */
#define max_user_num      100                // Max number of users for this application

/* Typedefs */
typedef std::string     name_t;
typedef unsigned int   age_t;

/* Enums */
enum class e_option
{
    INIT = 0,
    ADD = 1,
    FETCH = 2,
    QUIT = 3,
    ERROR = 4
};

/* Function Declarations */

void          print_msg(const std::string& msg);  // prints a message

void          show_options();                     // Show all selectable options

e_option      select_option();                    // Select Desired option

unsigned int  get_id();                           // Get User ID Desired to be Fetched

/* Operator Overloading */

std::istream& operator>>(std::istream& input_stream, e_option& result);

/* Constant expressions */

constexpr int       default_user_num  = 0;                 // Max number of users for this application
constexpr e_option  default_op        = e_option::INIT;   // Default option from enum class option

/* Struct */
struct s_user_Info
{
    name_t name;
    age_t  age;
};

/* Classes */

class user
{
    private:

    //Variables
    int curr_user_num = default_user_num;

    std::array<s_user_Info, max_user_num> user_info{};
    public:

    //Functions
    void add()          // Add new user (Store: Name, Age)
    {
        if(curr_user_num < max_user_num)
        {
            print_msg("\nAdd User. Please Enter user name and age:\n");

            print_msg("Name: ");
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // Clear the newline from the input buffer
            std::getline(std::cin, user_info[curr_user_num].name);  // Use getline to read full name with spaces
        
            print_msg("Age: ");
            std::cin >> user_info[curr_user_num].age;

            curr_user_num++;

            print_msg("\nUser record added successfully.\n");
        }
        else
        {
            print_msg("Can Not Register!\n"); 
            print_msg("Reached maximum number of registered users: [" + std::to_string(curr_user_num) + "]\n");
        }
    }

    void fetch(unsigned int ID)        //Fetch user's data by ID (Fetch: Name, Age)
    {
        if(ID <= curr_user_num)
        {
            print_msg("User Name: " + user_info[ID].name                + '\n');
            print_msg("User Age: "  + std::to_string(user_info[ID].age) + '\n');
        }
        else
        {
            print_msg("No User Registred With This ID!\n"); 

        }

    }


};

#endif // USER_HPP