#include "Patient.h"
#include "StorageManager.h"
#include "ClinicStats.h"
#include "TriagePreview.h"
#include "StaffStorage.h"
#include "LoginPrompt.h"
#include "AdminMenu.h"
#include "StaffMenu.h"
#include "HistoryLog.h"
#include <iostream>
#include <limits>

using namespace std;

int main() {
    loadPatientsFromFile();
    loadStaffAccounts();
    logSystemEvent("SYSTEM", "STARTUP", "Loaded patients and staff accounts");

    int choice = 0;
    do {
        clearScreen();
        printHeader();
        printTriagePreview();

        cout << "[1] Login\n";
        cout << "[0] Exit\n";
        cout << "Enter choice: ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            choice = -1;
            continue;
        }

        if (choice == 1) {
            string loggedInUser;
            UserRole role = promptLogin(loggedInUser);

            if (role == UserRole::ADMIN) {
                runAdminMenu(loggedInUser);
            } else if (role == UserRole::STAFF) {
                runStaffMenu(loggedInUser);
            }
        } else if (choice == 0) {
            logSystemEvent("SYSTEM", "SHUTDOWN", "Application exited normally");
        } else {
            cout << RED << "[!] Invalid choice.\n" << RESET;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get();
        }
    } while (choice != 0);

    return 0;
}