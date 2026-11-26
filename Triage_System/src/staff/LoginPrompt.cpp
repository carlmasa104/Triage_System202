#include "LoginPrompt.h"
#include "Patient.h"
#include "ClinicStats.h"
#include <iostream>
#include <limits>

using namespace std;

UserRole promptLogin(string& loggedInUser) {
    clearScreen();
    printHeader();
    cout << CYAN << "--- SYSTEM LOGIN ---" << RESET << "\n";
    cout << "Enter Administrator OR Nurse/Doctor credentials.\n\n";

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    string username, password;
    cout << "Username: ";
    getline(cin, username);
    cout << "Password: ";
    getline(cin, password);

    if (username == FIXED_ADMIN_USER && password == FIXED_ADMIN_PASS) {
        loggedInUser = username;
        cout << "\n" << GREEN << "[+] Logged in as Administrator." << RESET << "\n";
        cout << "Press Enter to continue...";
        cin.get();
        return UserRole::ADMIN;
    }

    for (const auto& acc : staffAccounts) {
        if (acc.username == username && acc.password == password) {
            loggedInUser = acc.username + " (" + acc.roleTitle + ")";
            cout << "\n" << GREEN << "[+] Logged in as " << loggedInUser << "." << RESET << "\n";
            cout << "Press Enter to continue...";
            cin.get();
            return UserRole::STAFF;
        }
    }

    cout << "\n" << RED << "[!] Invalid username or password." << RESET << "\n";
    cout << "Press Enter to return...";
    cin.get();
    return UserRole::NONE;
}
