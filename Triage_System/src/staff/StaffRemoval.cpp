#include "StaffRemoval.h"
#include "StaffStorage.h"
#include "Patient.h"
#include "HistoryLog.h"
#include "ClinicStats.h"
#include <iostream>
#include <string>
#include <limits>

using namespace std;

void unregisterNurseOrDoctor() {
    clearScreen();
    printHeader();
    cout << YELLOW << "--- [ADMIN] UNREGISTER NURSE / DOCTOR ---" << RESET << "\n\n";

    if (staffAccounts.empty()) {
        cout << RED << "[!] No registered nurse or doctor accounts found." << RESET << "\n\n";
        cout << "Press Enter to return to Admin Menu...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
        return;
    }

    cout << CYAN << "Registered Staff Accounts:\n" << RESET;
    for (size_t i = 0; i < staffAccounts.size(); ++i) {
        cout << "  [" << (i + 1) << "] " << staffAccounts[i].username
             << " (" << staffAccounts[i].roleTitle << ")\n";
    }
    cout << "\n";

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    string targetUsername;
    cout << "Enter username to unregister (or press Enter to cancel): ";
    getline(cin, targetUsername);

    if (targetUsername.empty()) {
        cout << YELLOW << "\n[*] Unregistration cancelled." << RESET << "\n";
        cout << "Press Enter to return to Admin Menu...";
        cin.get();
        return;
    }

    if (targetUsername == FIXED_ADMIN_USER) {
        cout << RED << "\n[!] Cannot unregister the fixed administrator account." << RESET << "\n";
        cout << "Press Enter to return to Admin Menu...";
        cin.get();
        return;
    }

    string confirm;
    cout << "Confirm unregistering '" << targetUsername << "'? (y/n): ";
    getline(cin, confirm);

    if (confirm != "y" && confirm != "Y") {
        cout << YELLOW << "\n[*] Unregistration cancelled." << RESET << "\n";
        cout << "Press Enter to return to Admin Menu...";
        cin.get();
        return;
    }

    if (removeStaffAccountByUsername(targetUsername)) {
        logSystemEvent(FIXED_ADMIN_USER, "UNREGISTER_STAFF", "Unregistered account: " + targetUsername);
        cout << "\n" << GREEN << "[+] Account '" << targetUsername << "' has been unregistered and removed." << RESET << "\n";
    } else {
        cout << "\n" << RED << "[!] Username '" << targetUsername << "' not found." << RESET << "\n";
    }

    cout << "Press Enter to return to Admin Menu...";
    cin.get();
}