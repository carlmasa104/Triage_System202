#include "StaffMenu.h"
#include "StaffStorage.h"
#include "TriagePreview.h"
#include "Patient.h"
#include "PatientManager.h"
#include "HistoryLog.h"
#include "ClinicStats.h"
#include <iostream>
#include <limits>

using namespace std;

void viewTriageQueue();

void runStaffMenu(const string& staffUser) {
    currentLoggedInUser = staffUser;
    int choice = 0;
    do {
        clearScreen();
        printHeader();
        printTriagePreview();
        cout << CYAN << "Logged in as: " << staffUser << RESET << "\n";
        cout << YELLOW << "--- CLINIC TRIAGE MENU ---" << RESET << "\n";
        cout << "[1] Register Patient\n";
        cout << "[2] View Triage Queue\n";
        cout << "[3] Update Vitals & Acuity\n";
        cout << "[4] Discharge Patient\n";
        cout << "[5] Clinic Statistics\n";
        cout << "[0] Logout\n";
        cout << "Enter choice: ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            choice = -1;
            continue;
        }

        switch (choice) {
            case 1: registerPatient(); break;
            case 2: viewTriageQueue(); break;
            case 3: updatePatient(); break;
            case 4: dischargePatient(); break;
            case 5: clinicStatistics(); break;
            case 0:
                logSystemEvent(staffUser, "LOGOUT", "Staff session ended");
                currentLoggedInUser = "SYSTEM";
                break;
            default:
                cout << RED << "[!] Invalid option.\n" << RESET;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cin.get();
                break;
        }
    } while (choice != 0);
}