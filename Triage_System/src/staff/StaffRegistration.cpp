#include "StaffRegistration.h"
#include "StaffStorage.h"
#include "Patient.h"
#include "HistoryLog.h"
#include "ClinicStats.h"
#include <iostream>
#include <string>
#include <limits>

using namespace std;

void registerNurseOrDoctor() {
    clearScreen();
    printHeader();
    cout << YELLOW << "--- [ADMIN] REGISTER NURSE / DOCTOR ---" << RESET << "\n\n";

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    string roleChoice;
    string roleTitle = "Nurse";
    cout << "Select Role ([1] Nurse, [2] Doctor): ";
    getline(cin, roleChoice);
    if (roleChoice == "2") {
        roleTitle = "Doctor";
    }

    string username, password;
    while (true) {
        cout << "Enter new " << roleTitle << " username: ";
        getline(cin, username);
        if (username.empty()) {
            cout << RED << "[!] Username cannot be empty.\n" << RESET;
            continue;
        }
        if (isUsernameTaken(username)) {
            cout << RED << "[!] That username is already taken. Try another.\n" << RESET;
            continue;
        }
        break;
    }

    while (true) {
        cout << "Set password for " << username << ": ";
        getline(cin, password);
        if (password.empty()) {
            cout << RED << "[!] Password cannot be empty.\n" << RESET;
            continue;
        }
        break;
    }

    staffAccounts.push_back({username, password, roleTitle});
    saveStaffAccounts();
    logSystemEvent(FIXED_ADMIN_USER, "REGISTER_STAFF", "Registered " + roleTitle + ": " + username);

    cout << "\n" << GREEN << "[+] " << roleTitle << " account '" << username << "' registered successfully!" << RESET << "\n";
    cout << "Press Enter to return to Admin Menu...";
    cin.get();
}