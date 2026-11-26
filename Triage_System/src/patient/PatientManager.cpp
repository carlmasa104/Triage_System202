#include "PatientManager.h"
#include "DischargeValidator.h" 
#include "AgeCalculator.h"
#include "ComplaintManager.h"
#include "TriageLogic.h"
#include "ClinicStats.h"
#include "HistoryLog.h"
#include "StaffStorage.h"
#include "StorageManager.h" 
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <limits>
#include <iomanip>
#include <sstream>

using namespace std;

vector<Patient> patients;

void printHeader();

// OPTION [1]: REGISTER A NEW PATIENT
// This is the CREATE function of the system.
// It collects patient information, calculates urgency, and stores the patient.
void registerPatient() {
    // [INSERTED]: Clear the terminal and show the header to match the other menu screens.
    clearScreen();
    printHeader();
    cout << YELLOW << "--- [1] REGISTER PATIENT (Create) ---" << RESET << "\n";

    // Create a temporary patient record.
    Patient p;

    // Create a simple patient ID.
    // Example: first patient gets P-0101, second gets P-0102, and so on.
    p.id = "P-010" + to_string(patients.size() + 1);

    // Important: clear input buffer before reading text with getline.
    cin.ignore();
    cout << "Enter Patient Name (Last, First): ";
    getline(cin, p.name);

    // Ask for the birthday and automatically calculate the patient's age.
    p.age = promptAndCalculateAge(p.birthDate);

    cout << "Enter Blood Pressure (e.g., 160/95): ";
    cin >> p.bp;
    p.systolicBp = extractSystolic(p.bp);

    cout << "Enter Heart Rate (HR): ";
    cin >> p.hr;

    cout << "Enter Temperature (e.g., 38.5): ";
    cin >> p.temp;

    // [INSERTED]: Use the standardized complaint picker so complaints group cleanly into counts.
    p.complaint = promptStandardComplaint();

    // This is the heart of triage: use the values to assign a severity score and tag.
    evaluateAgeBasedTriage(p);

    // Save the patient into the global vector.
    patients.push_back(p);

    // [INSERTED]: Instantly save the updated patient list to text storage so it isn't lost.
    savePatientsToFile();

    logSystemEvent(
        currentLoggedInUser,
        "REGISTER_PATIENT",
        "ID: " + p.id + " | Name: " + p.name +
        " | Complaint: " + p.complaint + " | Triage: " + p.triage
    );

    cout << "\n" << GREEN << "[+] Patient registered! Deviation Score: " << p.deviationScore << " | Triage Assigned: " << p.triage << RESET << "\n";
    cout << "Press Enter to return to Main Menu...";
    cin.get();
}

// OPTION [3]: UPDATE AN EXISTING PATIENT'S VITALS
// This is the update function.
// It finds a patient by ID, asks for new values, then recalculates triage.
void updatePatient() {
    clearScreen();
    printHeader();
    cout << YELLOW << "--- [3] UPDATE VITALS & ACUITY (Update) ---" << RESET << "\n";

    // If there are no patients, stop early and explain the situation.
    if (patients.empty()) {
        cout << "\n" << RED << "[!] No patients available to update." << RESET << "\n\n";
        cout << "Press Enter to return to Main Menu...";
        cin.ignore();
        cin.get();
        return;
    }

    cin.ignore();
    string pid;
    cout << "Enter Patient ID to update (e.g., P-0101): ";
    getline(cin, pid);

    bool found = false;

    // Search the list for the matching patient ID.
    for (auto& p : patients) {
        if (p.id == pid) {
            found = true;
            cout << "\nUpdating record for: " << p.name << " (Age: " << p.age << ")\n";

            // Ask for each field one by one.
            // If the user enters nothing, the original value stays unchanged.
            cout << "New BP [" << p.bp << "]: ";
            string newBp;
            getline(cin, newBp);
            if (!newBp.empty()) {
                p.bp = newBp;
                p.systolicBp = extractSystolic(p.bp);
            }

            cout << "New Heart Rate [" << p.hr << "]: ";
            string newHrStr;
            getline(cin, newHrStr);
            if (!newHrStr.empty()) p.hr = stoi(newHrStr);

            cout << "New Temp [" << p.temp << "]: ";
            string newTempStr;
            getline(cin, newTempStr);
            if (!newTempStr.empty()) p.temp = stod(newTempStr);

            // Recalculate the triage after new values are entered.
            evaluateAgeBasedTriage(p);

            // [INSERTED]: Save changes to the text file immediately after updating vitals.
            savePatientsToFile();

            ostringstream tempStream;
            tempStream << fixed << setprecision(1) << p.temp;

            logSystemEvent(
                currentLoggedInUser,
                "UPDATE_VITALS",
                "ID: " + p.id + " | Name: " + p.name +
                " | BP: " + p.bp + ", HR: " + to_string(p.hr) +
                ", Temp: " + tempStream.str() + "C | Triage: " + p.triage
            );

            cout << "\n" << GREEN << "[+] Record updated! New Deviation Score: " << p.deviationScore << " | New Triage: " << p.triage << RESET << "\n";
            break;
        }
    }

    // If no patient matches, tell the user clearly.
    if (!found) {
        cout << "\n" << RED << "[!] Patient not found!" << RESET << "\n";
    }

    cout << "Press Enter to return to Main Menu...";
    cin.get();
}

// OPTION [4]: Discharge a Patient
// This is the delete function.
// It validates discharge readiness first, then removes the patient from the queue.
void dischargePatient() {
    clearScreen();
    printHeader();
    cout << YELLOW << "--- [4] DISCHARGE PATIENT (Delete) ---" << RESET << "\n";

    // If the queue is empty, there is nothing to remove.
    if (patients.empty()) {
        cout << "\n" << RED << "[!] No patients in the queue to discharge." << RESET << "\n\n";
        cout << "Press Enter to return to Main Menu...";
        cin.ignore();
        cin.get();
        return;
    }

    cin.ignore();
    string pid;
    cout << "Enter Patient ID to discharge: ";
    getline(cin, pid);

    // Find the patient record first so we can validate eligibility before deleting
    auto it = find_if(patients.begin(), patients.end(), [&pid](const Patient& p) {
        return p.id == pid;
    });

    if (it != patients.end()) {
        // Run the 5-point discharge validation checklist
        if (validateDischargeEligibility(*it)) {
            string dischargedId = it->id;
            string dischargedName = it->name;
            string dischargedTriage = it->triage;

            patients.erase(it);

            // Update the text file storage to permanently remove the discharged patient.
            savePatientsToFile();

            logSystemEvent(
                currentLoggedInUser,
                "DISCHARGE_PATIENT",
                "ID: " + dischargedId + " | Name: " + dischargedName +
                " | Final Triage: " + dischargedTriage
            );

            cout << "\n" << GREEN << "[+] Patient " << pid << " has been discharged." << RESET << "\n";
        } else {
            cout << "\n" << YELLOW << "[*] Patient " << pid << " remains in the active queue." << RESET << "\n";
        }
    } else {
        cout << "\n" << RED << "[!] Patient ID not found." << RESET << "\n";
    }

    cout << "Press Enter to return to Main Menu...";
    cin.get();
}

// OPTION [2]: VIEW TRIAGE QUEUE
// Displays active patients sorted by triage priority.
void viewTriageQueue() {
    clearScreen();
    printHeader();
    cout << YELLOW << "--- [2] TRIAGE QUEUE ---" << RESET << "\n";

    if (patients.empty()) {
        cout << "\n" << RED << "[!] No patients currently in the queue." << RESET << "\n\n";
    } else {
        vector<Patient> sortedPatients = patients;
        sort(sortedPatients.begin(), sortedPatients.end(), comparePatients);

        for (size_t i = 0; i < sortedPatients.size(); ++i) {
            const Patient& p = sortedPatients[i];
            cout << (i + 1) << ". [" << p.triage << "] "
                 << p.id << " | " << p.name
                 << " (Age " << p.age << ") | BP: " << p.bp
                 << " | HR: " << p.hr
                 << " | Temp: " << p.temp
                 << " | Complaint: " << p.complaint << "\n";
        }
        cout << "\n";
    }

    cout << "Press Enter to return to Main Menu...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}