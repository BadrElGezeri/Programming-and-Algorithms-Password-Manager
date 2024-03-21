#include <iostream>
#include <fstream>
#include <sstream>
#include "base64.cpp"
#include "base64.h"
#include <random>
#include <cstdlib> 
#include <string>

using namespace std;

void afterSuccessfulLogin(), retrievePassword(), generatePassword();
string username;

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

static void validateUserChoice(int& userChoice,int lowerrange, int upperrange) {

    while (userChoice < lowerrange || userChoice > upperrange || typeid(userChoice) != typeid(int)) {
        cout << "Invalid option picked, pick another option:" << endl;
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        cin >> userChoice;
    }
    return;
}

void retrievePassword(string username) {
    ifstream file;
    string line;
    file.open("passwords", std::ios_base::app);
    if (file.is_open()) {
        cout << "Platform - " << "Email/Username - " << "Password" << endl;
        while (getline(file, line)) {
            int i = 1;
            string decoded = base64_decode(line);
            size_t pos = decoded.find('|');
            if (pos != string::npos) {
                // Extract the username in the passwords file
                string user = decoded.substr(0, pos);
                if (username == user) {
                    size_t pos2 = decoded.find('|', pos + 1);
                    size_t pos3 = decoded.find('|', pos2 + 1);
                    if (pos2 != string::npos && pos3 != string::npos) {
                        string platform = decoded.substr(pos + 1, pos2 - pos - 1);
                        string userOrEmail = decoded.substr(pos2 + 1, pos3 - pos2 - 1);
                        string pass = decrypt(decoded.substr(pos3 + 1));
                        // Output the retrieved 
                        cout << platform << " - " << userOrEmail << " - " << pass << endl;
                    }
                }
            }
        }
    }
    else {
        cout << "Error while retreiving the password, try again" << endl;
    }
    cout << "All saved passwords" << endl;
    afterSuccessfulLogin();
}

void insertPassword(string username, string platform, string email, string passwd) {
    fstream file;
    file.open("passwords", std::ios_base::app);
    if (file.is_open()) {
        string encoded = base64_encode(username +"|" + platform + "|" + email + "|" + encrypt(passwd));
        file << encoded << endl;
        file.close();
        cout << "Password saved successfully!" << endl;
    }
    else {
        file.close();
        cout << "Error while trying to insert a password entry, try again" << endl;
    }

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
        string decoded = base64_decode(line);
        size_t pos = decoded.find('|');

        if (pos != string::npos) {
            // Extract username and password
            string user = decoded.substr(0, pos);
            string pass = decoded.substr(pos + 1);

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
    cout << "Pick one of the following options to proceed:" << endl;
    cout << R"(
1- Login
2- Sign up
3- Exit Application
)";

    //cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    cin >> userChoice;
    validateUserChoice(userChoice, 1, 3);
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
                    cout << "Successfully logged in" << endl;
                    username = usernameLogin;
                    afterSuccessfulLogin();
                }
                else {
                    cout << "Incorrect password, try again" << endl;
                    startup();
                }
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
                if (file.is_open()) {
                    int ID = rand();
                    string encoded = base64_encode(usernameSignup + "|" + encryptedPasswd);
                    file << encoded << endl;
                    file.close();
                    cout << "Signed up successfully!" << endl;
                    startup();
                }
                else {
                    cout << "Error while saving your account, try again" << endl;
                }
            

            }
        }
    }
    else if (userChoice == 3) {
        exit(0);
    }
}


void afterSuccessfulLogin() {
    string userUsernameOREmail, userPassword, applicationName,temp;
    int userChoice;
    cout << "Choose one of the following options" << endl;
    cout << R"(
1- List all saved passwords
2- Enter a new password entry
3- Generate a Password
4- Exit Application
)";
    cin >> userChoice;
    validateUserChoice(userChoice, 1, 4);
    if (userChoice == 4) {
        exit(0);
    }
    else if (userChoice == 1) {
        retrievePassword(username);
        //Search for the application name in the datatbase and retreive all accounts associated with it.
    }
    else if (userChoice == 2) {
        //New Password Entry (Username/email and Password)
        cout << "Enter the application name or website you would like to retreive the password for:" << endl;
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        getline(cin, applicationName);
        cout << "Enter an email/username:" << endl;
        getline(cin, userUsernameOREmail);
        cout << "Enter the password to save:" << endl;
        getline(cin, userPassword);
        insertPassword(username,applicationName, userUsernameOREmail, userPassword);
        afterSuccessfulLogin();
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
    startup();
    return 0;
}
