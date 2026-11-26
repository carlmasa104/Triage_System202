#include "AdminMenu.h"
#include "StaffRegistration.h"
#include "StaffRemoval.h"
#include "StaffStorage.h"
#include "HistoryLog.h"
#include "TriagePreview.h"
#include "Patient.h"
#include "ClinicStats.h"
#include <iostream>
#include <limits>

using namespace std;

void runAdminMenu(const string& adminUser) {
    currentLoggedInUser = adminUser;
    int choice = 0;
    do {
        clearScreen();
        printHeader();
        printTriagePreview();
        cout << CYAN << "Logged in as: " << adminUser << " [ADMIN]" << RESET << "\n";
        cout << YELLOW << "--- ADMINISTRATOR MENU ---" << RESET << "\n";
        cout << "[1] Register Nurse / Doctor\n";
        cout << "[2] Unregister Nurse / Doctor\n";
        cout << "[3] View History Log\n";
        cout << "[0] Logout\n";
        cout << "Enter choice: ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            choice = -1;
            continue;
        }

        if (choice == 1) {
            registerNurseOrDoctor();
        } else if (choice == 2) {
            unregisterNurseOrDoctor();
        } else if (choice == 3) {
            viewHistoryLog();
        } else if (choice == 0) {
            logSystemEvent(adminUser, "LOGOUT", "Admin session ended");
            currentLoggedInUser = "SYSTEM";
        } else {
            cout << RED << "[!] Invalid option.\n" << RESET;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get();
        }
    } while (choice != 0);
}