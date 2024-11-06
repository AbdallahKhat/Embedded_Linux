
/**
 * we’re going to be writing a user registration application.
 * This will allow users to register with the system, providing both their name and age, 
 * and we’ll store this information in our own custom type.
 * We’ll also provide the ability for a user to be looked up by an ID,
 * retrieving their information.
 */

//1. define depend.
#include <iostream>
#include <string>
#include <array>
#include <limits>

/**
 * 2. Extract Nouns: [ Users - Name, Age ]
 * 3. Extract verbs: [ Register, Fetch ]
 */

/* Macros */
#define max_user_num      100                // Max number of users for this application

/* Enums */
enum class e_option
{
    Add = 1,
    Fetch = 2,
    Quit = 3,
    Error = 4
};

/* Function Declarations */

void          print_msg(const std::string& msg);  // prints a message

void          show_options();                     // Show all selectable options

e_option      select_option();                    // Select Desired option

unsigned int  get_id();                           // Get User ID Desired to be Fetched

/* Operator Overloading */

std::istream& operator>>(std::istream& input_stream, e_option& result);

/* Constant expressions */

constexpr int       default_user_num  = 0;               // Max number of users for this application
constexpr e_option  default_op        = e_option::Add;   // Default option from enum class option

/* Classes */

class c_user
{
    private:

    //Variables
    int curr_user_num = default_user_num;

    std::array<std::string, max_user_num> name{};
    std::array<int, max_user_num> age{};

    public:

    //Functions
    void add()          // Add new user (Store: Name, Age)
    {
        if(curr_user_num < max_user_num)
        {
            print_msg("\nAdd User. Please Enter user name and age:\n");

            print_msg("Name: ");
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // Clear the newline from the input buffer
            std::getline(std::cin, name[curr_user_num]);  // Use getline to read full name with spaces
        
            print_msg("Age: ");
            std::cin >> age[curr_user_num];

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
            print_msg("User Name: " + name[ID]                + '\n');
            print_msg("User Age: "  + std::to_string(age[ID]) + '\n');
        }
        else
        {
            print_msg("No User Registred With This ID!\n"); 

        }

    }


};

/**
 * 
 * Main Function
 * 
 */

int main()
{
    e_option op = default_op;

    c_user user;

    bool isRunning = true;

    print_msg("User SignUp Application\n");

    while (isRunning == true)
    {
        show_options();

        op = select_option();

        switch (op)
        {
        case e_option::Add:
            user.add();
            break;

        case e_option::Fetch:
            user.fetch(get_id());
            break;

        case e_option::Quit:
            isRunning = false;
            break;

        case e_option::Error:
        default:
            print_msg("\nInvalid Option! Please try again.\n");
            break;
        }
    }
    
}


/**
 * 
 * Functions Definitions 
 * 
 */

void print_msg(const std::string& msg)
{
    std::cout << msg;
}

void show_options()
{
    print_msg("\nPlease select an option:\n");

    print_msg("1: Add Record\n");
    print_msg("2: Fetch Record\n");
    print_msg("3: Quit\n");
}

e_option select_option()
{
    e_option selected_op = default_op;                //Selected option from user

    print_msg("\nEnter option: ");

    std::cin >> selected_op;

    // Check for invalid input
    if (std::cin.fail()) {
        std::cin.clear();  // Clear the error state
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // Discard invalid input
        selected_op = e_option::Error;  // Set a default to avoid undefined behavior
    }

    return selected_op;
}

unsigned int get_id()
{
    int user_id = default_user_num;
    print_msg("\n Please enter user ID:\n");
    print_msg("User ID: ");
    
    std::cin >> user_id;

    return user_id;
}


/**
 * 
 *  Operator Overloading Definitions
 * 
 */

std::istream& operator>>(std::istream& input_stream, e_option& result)
{
    int int_option;

    input_stream >> int_option;

    // convert integer ---> option enum.

    result = static_cast<e_option>(int_option);

    return input_stream;

}