
/**
 * we’re going to be writing a user registration application.
 * This will allow users to register with the system, providing both their name and age, 
 * and we’ll store this information in our own custom type.
 * We’ll also provide the ability for a user to be looked up by an ID,
 * retrieving their information.
 */


/* Dependancies */
#include <iostream>
#include <string>
#include <array>
#include <limits>
#include "user.hpp"


/**
 * 
 * Main Function
 * 
 */

int main()
{
    e_option op = default_op;

    user user;

    bool isRunning = true;

    print_msg("User SignUp Application\n");

    while (isRunning == true)
    {
        show_options();

        op = select_option();

        switch (op)
        {
        case e_option::ADD:
            user.add();
            break;

        case e_option::FETCH:
            user.fetch(get_id());
            break;

        case e_option::QUIT:
            isRunning = false;
            break;

        case e_option::ERROR:
        default:
            print_msg("\nInvalid Option! Please try again.\n");
            break;
        }
    }
    
}
