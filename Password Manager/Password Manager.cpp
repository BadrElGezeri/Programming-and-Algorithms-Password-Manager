// Possible Libaries to use for AES encryption

#include "cryptlib.h"
#include "rijndael.h"
#include "modes.h"
#include "files.h"
#include "osrng.h"
#include "hex.h"

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main()
{
    string userPassword,applicationName;
    int userChoice;
    cout << R"(
-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*--*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
 ____                                     _   __  __                                   
|  _ \ __ _ ___ _____      _____  _ __ __| | |  \/  | __ _ _ __   __ _  __ _  ___ _ __ 
| |_) / _` / __/ __\ \ /\ / / _ \| '__/ _` | | |\/| |/ _` | '_ \ / _` |/ _` |/ _ \ '__|
|  __/ (_| \__ \__ \\ V  V / (_) | | | (_| | | |  | | (_| | | | | (_| | (_| |  __/ |   
|_|   \__,_|___/___/ \_/\_/ \___/|_|  \__,_| |_|  |_|\__,_|_| |_|\__,_|\__, |\___|_|   
                                                                       |___/           
-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*--*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
)" << endl;
    cout << "Welcome to your All-in-One Password Manager" << endl;
    cout << "Please enter your password to proceed" << endl;
    cin >> userPassword;
    cout << "Choose one of the following options" << endl;
    cout << R"(
1- Retreive Passwords
2- Enter a new password entry
3- Generate a Password
4- Exit Application
)";
    cin >> userChoice;
    if (userChoice == 4) {
        exit;
    }
    else if (userChoice == 1) {
        cout << "Enter the application name or website you would like to retreive the password for:";
        cin >> applicationName;
        //Search for the application name in the datatbase and retreive all accounts associated with it.
    }
    else if (userChoice == 2){
        //New Password Entry (Username/email and Password)
    }
    else if (userChoice == 3) {
        //Password Generation
    }
    else {
        cout << "Invalid Input";
    }
    

    return 0;
}
