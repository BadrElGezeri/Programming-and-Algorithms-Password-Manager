#include <iostream>
#include <fstream>
#include <sstream>
#include "base64.cpp"
#include "base64.h"
#include <random>
#include <string>

using namespace std;

void afterSuccessfulLogin(), retrievePassword(), generatePassword();
string username;

class Node {
public:
    string data;
    Node* next;

    // Default constructor 
    Node()
    {
        data = "";
        next = NULL;
    }

    // Parameterised Constructor 
    Node(string data)
    {
        this->data = data;
        this->next = NULL;
    }
};

class Linkedlist {
    Node* head;

public:
    // Default constructor 
    Linkedlist() { head = NULL; }

    // Function to insert a 
    // node at the end of the 
    // linked list. 
    void insertNode(string);

    // Function to print the 
    // linked list. 
    void printList();

    // Function to delete the 
    // node at given position 
    void deleteNode(int);

    void overwriteFile(const string&);
};

void Linkedlist::deleteNode(int nodeOffset)
{
    Node* temp1 = head, * temp2 = NULL;
    int ListLen = 0;

    if (head == NULL) {
        cout << "List empty." << endl;
        return;
    }

    // Find length of the linked-list. 
    while (temp1 != NULL) {
        temp1 = temp1->next;
        ListLen++;
    }

    // Check if the position to be 
    // deleted is greater than the length 
    // of the linked list. 
    if (ListLen < nodeOffset) {
        cout << "[]Password entry does not exist"
            << endl;
        return;
    }

    // Declare temp1 
    temp1 = head;

    // Deleting the head. 
    if (nodeOffset == 1) {

        // Update head 
        head = head->next;
        delete temp1;
        cout << "[]Passowrd deleted successfully" << endl;
        return;
    }

    // Traverse the list to 
    // find the node to be deleted. 
    while (nodeOffset-- > 1) {

        // Update temp2 
        temp2 = temp1;

        // Update temp1 
        temp1 = temp1->next;
    }

    // Change the next pointer 
    // of the previous node. 
    temp2->next = temp1->next;
    cout << "[]Passowrd deleted successfully" << endl;
    // Delete the node 
    delete temp1;
}

void Linkedlist::insertNode(string data)
{
    // Create the new Node. 
    Node* newNode = new Node(data);

    // Assign to head 
    if (head == NULL) {
        head = newNode;
        return;
    }

    // Traverse till end of list 
    Node* temp = head;
    while (temp->next != NULL) {

        // Update temp 
        temp = temp->next;
    }

    // Insert at the last. 
    temp->next = newNode;
}

void Linkedlist::printList()
{
    Node* temp = head;

    // Check for empty list. 
    if (head == NULL) {
        cout << "List empty" << endl;
        return;
    }

    // Traverse the list. 
    while (temp != NULL) {
        cout << temp->data << " ";
        temp = temp->next;
    }
}

void Linkedlist::overwriteFile(const std::string& filename) {
    // Open the file for writing, truncating it
    ofstream file(filename, std::ios::trunc);

    // Check if the file is opened successfully
    if (!file.is_open()) {
        cout << "Unable to open file: " << filename << std::endl;
        return;
    }

    Node* temp = head;

    // Traverse the list and write each node's data to the file
    while (temp != NULL) {
        file << temp->data << std::endl;
        temp = temp->next;
    }

    // Close the file
    file.close();
}

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

static void validateUserChoice(int& userChoice,int lowerrange, int upperrange,string state) {
    int i = 0;
    while (userChoice < lowerrange || userChoice > upperrange || typeid(userChoice) != typeid(int)) {
        cout << "[]Invalid option picked, pick another option:" << endl;
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        cout << "> ";
        cin >> userChoice;
        if (i > 10) {
            system("CLS");
            cout << "";
            i = 0;
            if (state == "startup") {
                cout << "[]Pick one of the following options to proceed:" << endl;
                cout << R"(
[1]- Login
[2]- Sign up
[3]- Exit Application
)";
            }
            else if (state == "afterlogin") {
                cout << "[]Choose one of the following options" << endl;
                cout << R"(
[1]- List all saved passwords
[2]- Enter a new password entry
[3]- Generate a Password
[4]- Sign out
[5]- Exit Application
)";
            }
        }
        i++;
    }
    return;
}

void retrievePassword(string username) {
    Linkedlist list;
    ifstream file;
    string line;
    bool empty = true;
    file.open(username + "_passwords", std::ios_base::app);
    if (file.is_open()) {
        if (file.peek() == std::ifstream::traits_type::eof()) {
            cout << "[]No saved passwords" << endl;
            afterSuccessfulLogin();
        }
        else {
            cout << "No. - " << "Platform - " << "Email/Username - " << "Password" << endl;
            int i = 1;
            while (getline(file, line)) {
                list.insertNode(line);
                string decoded = base64_decode(line);
                size_t pos = decoded.find('|');
                if (pos != string::npos) {
                    // Extract the username in the passwords file
                    string user = decoded.substr(0, pos);
                    size_t pos2 = decoded.find('|', pos + 1);
                    size_t pos3 = decoded.find('|', pos2 + 1);
                    if (pos2 != string::npos && pos3 != string::npos) {
                        string platform = decoded.substr(pos + 1, pos2 - pos - 1);
                        string userOrEmail = decoded.substr(pos2 + 1, pos3 - pos2 - 1);
                        string pass = decrypt(decoded.substr(pos3 + 1));
                        // Output the retrieved 
                        cout << "[" << i << "] " << platform << " - " << userOrEmail << " - " << pass << endl;
                        i = i + 1;
                        empty = false;
                    }
                }
            }
        }
        file.close();
    }
    else {
        cout << "[]Error while retreiving the password, try again" << endl;
    }
    if (!empty) {
        cout << R"(
[1]- Main Menu
[2]- Delete Password Entry
)";
        int userChoicePass;
        cout << "> ";
        cin >> userChoicePass;
        if (userChoicePass == 2) {
            int passToDelete;
            cout << "[]Pick the number of the password that you would like to delete:" << endl;
            cout << "> ";
            cin >> passToDelete;
            list.deleteNode(passToDelete);
            list.overwriteFile(username + "_passwords");
            afterSuccessfulLogin();
        }
        else {
            afterSuccessfulLogin();
        }
        cout << "[]All saved passwords" << endl;
    }
}

void insertPassword(string username, string platform, string email, string passwd) {
    fstream file;
    file.open(username+"_passwords", std::ios_base::app);
    if (file.is_open()) {
        string encoded = base64_encode(username +"|" + platform + "|" + email + "|" + encrypt(passwd));
        file << encoded << endl;
        file.close();
        cout << "[]Password saved successfully!" << endl;
    }
    else {
        file.close();
        cout << "[]Error while trying to insert a password entry, try again" << endl;
    }

}

void generatePassword(int length) {
    const char genAlphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz!@#$%^&*()-+0123456789";
    int size = sizeof(genAlphabet) - 1;

    srand(time(nullptr));

    cout << "[]Generated password: ";
    for (int i = 0; i < length; ++i) {
        int index = rand() % size; // Generate a random index within the genAlphabet array

        cout << genAlphabet[index]; // Print the character at the randomly selected index
    }
    cout << endl;
    afterSuccessfulLogin();
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

bool validateUsernameAvailability(const string& username) {
    ifstream file("users");
    string line;
    while (getline(file, line)) {
        // Find the position of the comma separating username and password
        string decoded = base64_decode(line);
        size_t pos = decoded.find('|');

        if (pos != string::npos) {
            // Extract username and password
            string user = decoded.substr(0, pos);

            // Check if the extracted username matches the input username
            if (user == username) {
                return false;
            }
            else {

                return true;
            }
        }
    }
}

void startup() {
    string userPassword, applicationName;
    int userChoice;
    cout << "[]Pick one of the following options to proceed:" << endl;
    cout << R"(
[1]- Login
[2]- Sign up
[3]- Exit Application
)";

    cout << "> ";
    cin >> userChoice;
    validateUserChoice(userChoice, 1, 3,"startup");
    if (userChoice == 1) {
        string usernameLogin, passwordLogin;
        cout << "[]Login option picked" << endl << endl;
        cout << "[]Enter your username:" << endl;
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        cout << "> ";
        getline(cin, usernameLogin);
        while (hasSpace(usernameLogin) || empty(usernameLogin)){
            cout << "[]Username cannot contain a space and cannot be blank, try again:" << endl; // Tests for space entered in the username
            cout << "> ";
            getline(cin, usernameLogin);
        }
            cout << "[]Enter your password:" << endl;
            cout << "> ";
            getline(cin, passwordLogin);
        while (hasSpace(passwordLogin) || empty(passwordLogin)) {
            cout << "[]Password cannot contain a space and cannot be blank, try again:" << endl; // Tests for space entered in the password
            cout << "> ";
            getline(cin, passwordLogin);
        }
                if (validatePassword(usernameLogin, passwordLogin)) {
                    cout << "[]Successfully logged in" << endl;
                    username = usernameLogin;
                    afterSuccessfulLogin();
                }
                else {
                    cout << "[]User not found, try again" << endl;
                    startup();
                }
            }
        

    else if (userChoice == 2) {
        string usernameSignup, passwordSignup;
        cout << "[]Sign up option picked" << endl << endl;
        cout << "[]Enter your username" << endl;
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        cout << "> ";
        getline(cin, usernameSignup);
        while (!validateUsernameAvailability(usernameSignup)) {
            cout << "[]Username is used, try another username:" << endl; // Tests if the username is already used (Done by checking all the usernames in the users file)
            cout << "> ";
            getline(cin, usernameSignup);
        }
        while (hasSpace(usernameSignup) || empty(usernameSignup)) {
            cout << "[]Username cannot contain a space and cannot be blank, try again:" << endl; // Tests for space entered in the username & if the username is empty
            cout << "> ";
            getline(cin, usernameSignup);
        }
            cout << "[]Enter your password" << endl;
            cout << "> ";
            getline(cin, passwordSignup);
            while(hasSpace(passwordSignup) || empty(usernameSignup)) {
                cout << "[]Password cannot contain a space and cannot be blank, try again:" << endl; // Tests for space entered in the password & if the password is empty
                cout << "> ";
                getline(cin, passwordSignup);
            }
                string encryptedPasswd = encrypt(passwordSignup);
                fstream file;
                file.open("users", std::ios_base::app);
                if (file.is_open()) {
                    int ID = rand();
                    string encoded = base64_encode(usernameSignup + "|" + encryptedPasswd);
                    file << encoded << endl;
                    file.close();
                    cout << "[]Signed up successfully!" << endl;
                    startup();
                }
                else {
                    cout << "[]Error while saving your account, try again" << endl;
                }
            }
        
    
    else if (userChoice == 3) {
        exit(0);
    }
}

void afterSuccessfulLogin() {
    string userUsernameOREmail, userPassword, applicationName,temp;
    int userChoice;
    cout << "[]Choose one of the following options" << endl;
    cout << R"(
[1]- List all saved passwords
[2]- Enter a new password entry
[3]- Generate a Password
[4]- Sign out
[5]- Exit Application
)";
    cout << "> ";
    cin >> userChoice;
    validateUserChoice(userChoice, 1, 5,"afterlogin");
    if (userChoice == 5) {
        exit(0);
    }
    else if (userChoice == 1) {
        retrievePassword(username);
    }
    else if (userChoice == 2) {
        //New Password Entry (Username/email and Password)
        cout << "[]Enter the application name or website you would like to retreive the password for:" << endl;
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        cout << "> ";
        getline(cin, applicationName);
        cout << "[]Enter an email/username:" << endl;
        cout << "> ";
        getline(cin, userUsernameOREmail);
        cout << "[]Enter the password to save:" << endl;
        cout << "> ";
        getline(cin, userPassword);
        insertPassword(username,applicationName, userUsernameOREmail, userPassword);
        afterSuccessfulLogin();
    }
    else if (userChoice == 3) {
        int passwordLength;
        cout << "[]How long do you want your password to be?" << endl;
        cout << "(Minimum length is 8)" << endl;
        cout << "> ";
        cin >> passwordLength;
        while (passwordLength < 8) {
            cout << "[]Invalid password length, enter a number larger than 8:" << endl;
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cout << "> ";
            cin >> passwordLength;
        }
        generatePassword(passwordLength);
        cout << endl;
    }
    else if (userChoice == 4) {
        cout << "[]Signed out successfully" << endl;
        username = "";
        startup();
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
    cout << "[]Welcome to your All-in-One Password Manager" << endl;
    startup();
    return 0;
}
