#include "user.hpp"

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
        selected_op = e_option::ERROR;  // Set a default to avoid undefined behavior
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