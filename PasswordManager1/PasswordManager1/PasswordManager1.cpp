#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

void startup(), afterSuccessfulLogin();


string encrypt(string text)
{
    string result = "";
    int key = 4;

    // String alphabet
    string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890!@#$%^&*()_=-+";

    // Traverse text
    for (int i = 0; i < text.length(); i++) {
        // Find the position of the current character in the alphabet
        size_t pos = alphabet.find(text[i]);

        // If the character is found in the alphabet
        if (pos != string::npos) {
            // Shift the character by 's' positions
            pos = (pos + 4) % alphabet.length();
            // Append the encrypted character to the result
            result += alphabet[pos];
        }
        else {
            // If the character is not found in the alphabet, keep it unchanged
            result += text[i];
        }
    }

    // Return the resulting string
    return result;
}


string decrypt(string text)
{
    string result = "";
    int key = 4;
    // String alphabet
    string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890!@#$%^&*()_=-+";

    // Traverse text
    for (int i = 0; i < text.length(); i++) {
        // Find the position of the current character in the alphabet
        size_t pos = alphabet.find(text[i]);

        // If the character is found in the alphabet
        if (pos != string::npos) {
            // Shift the character by 's' positions in the opposite direction (decrypt)
            pos = (pos - 4 + alphabet.length()) % alphabet.length();
            // Append the decrypted character to the result
            result += alphabet[pos];
        }
        else {
            // If the character is not found in the alphabet, keep it unchanged
            result += text[i];
        }
    }

    // Return the resulting string
    return result;
}

void retrievePassword() {

}

void insertPassword() {

}

void generatePassword() {

}

bool hasSpace(const string& str) {
    for (char c : str) {
        if (c == ' ') {
            return true; // If space found, return true
        }
    }
    return false; // If no space found, return false
}

bool validatePassword(const string& username, const string& passwd) {
    ifstream file("users");
    string line;

    while (getline(file, line)) {
        // Find the position of the comma separating username and password
        size_t pos = line.find('|');

        if (pos != string::npos) {
            // Extract username and password
            string user = line.substr(0, pos);
            string pass = line.substr(pos + 1);

            // Check if the extracted username matches the input username
            if (user == username) {
                // If yes, check if the extracted password matches the input password
                if (decrypt(pass) == passwd) {
                    bool validate = true;
                    return validate;
                }
                
            }
        }
    }
    
    return false; // Username not found
}


void startup() {
    string userPassword, applicationName;
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
    cout << "Pick one of the following options to proceed:" << endl;
    cout << R"(
1- Login
2- Sign up
3- Exit Application
)";

    //cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    cin >> userChoice;
    while (userChoice < 1 || userChoice > 3) {
        cout << "Invalid option picked, pick another option:" << endl;
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        cin >> userChoice;
    }
    if (userChoice == 1) {
        string usernameLogin, passwordLogin;
        cout << "Login option picked" << endl;
        cout << "Enter your username" << endl;
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        getline(cin, usernameLogin);
        if (hasSpace(usernameLogin)) {
            cout << "Username cannot contain a space" << endl; // Tests for space entered in the username
        }
        else {
            cout << "Enter your password" << endl;
            getline(cin, passwordLogin);
            if (hasSpace(passwordLogin)) {
                cout << "Password cannot contain a space" << endl; // Tests for space entered in the password
            }
            else {
                if (validatePassword(usernameLogin, passwordLogin)) {
                    afterSuccessfulLogin();
                }
                else {
                    cout << "Incorrect password, try again";
                    startup();
                }
                
                //auto userEnteredPasswdHash = SHA256HashString(passwordLogin);
            }
        }

    }
    else if (userChoice == 2) {
        string usernameSignup, passwordSignup;
        cout << "Sign up option picked" << endl;
        cout << "Enter your username" << endl;
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        getline(cin, usernameSignup);
        if (hasSpace(usernameSignup)) {
            cout << "Username cannot contain a space" << endl; // Tests for space entered in the username
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            getline(cin, usernameSignup);
        }
        else {
            cout << "Enter your password" << endl;
            getline(cin, passwordSignup);
            if (hasSpace(passwordSignup)) {
                cout << "Password cannot contain a space" << endl; // Tests for space entered in the password
                cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                getline(cin, passwordSignup);
            }
            else {
                string encryptedPasswd = encrypt(passwordSignup);
                fstream file;
                file.open("users", std::ios_base::app);
                file << usernameSignup << "|" << encryptedPasswd << endl;
                startup();
            }
        }
    }
    else if (userChoice == 3) {
        exit(0);
    }
}


void afterSuccessfulLogin() {
    string userPassword, applicationName;
    int userChoice;
    cout << "Successfully logged in" << endl;
    cout << "Choose one of the following options" << endl;
    cout << R"(
1- Retreive Passwords
2- Enter a new password entry
3- Generate a Password
4- Exit Application
)";
    cin >> userChoice;
    if (userChoice == 4) {
        exit(0);
    }
    else if (userChoice == 1) {
        cout << "Enter the application name or website you would like to retreive the password for:";
        cin >> applicationName;
        //Search for the application name in the datatbase and retreive all accounts associated with it.
    }
    else if (userChoice == 2) {
        //New Password Entry (Username/email and Password)
    }
    else if (userChoice == 3) {
        //Password Generation
    }
    else {
        cout << "Invalid Input";
    }

}




int main()
{
    startup();
    return 0;
}
