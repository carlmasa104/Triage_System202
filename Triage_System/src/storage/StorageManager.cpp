#include "StorageManager.h"
#include "TriageLogic.h"
#include "AgeCalculator.h" // <-- [INSERTED]: Needed so loaded birthdays can recalculate current age.
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

// Saves the current active patient list into a text file on your computer
void savePatientsToFile() {
    ofstream outFile("patients_database.txt");
    
    if (!outFile.is_open()) {
        cout << "[!] Error: Could not open file for saving.\n";
        return;
    }

    // Loop through every patient and write their attributes to the text file
    for (const auto& p : patients) {
        outFile << p.id << "|" 
                << p.name << "|" 
                << p.birthDate << "|" // <-- [INSERTED]: Save the patient's birthday (YYYY-MM-DD).
                << p.age << "|" 
                << p.bp << "|" 
                << p.hr << "|" 
                << p.temp << "|" 
                << p.complaint << "\n";
    }
    
    outFile.close();
}

// Loads saved patient records from the text file into the program when it starts
void loadPatientsFromFile() {
    ifstream inFile("patients_database.txt");
    
    if (!inFile.is_open()) {
        // If the file doesn't exist yet (first time running), just return silently
        return; 
    }

    patients.clear(); // Clear current memory list before loading
    string line;
    int lineNumber = 0;

    while (getline(inFile, line)) {
        ++lineNumber;
        vector<string> fields;
        size_t fieldStart = 0;
        while (true) {
            size_t separator = line.find('|', fieldStart);
            if (separator == string::npos) {
                fields.push_back(line.substr(fieldStart));
                break;
            }
            fields.push_back(line.substr(fieldStart, separator - fieldStart));
            fieldStart = separator + 1;
        }

        Patient p;

        if (fields.size() == 7) {
            // Legacy format: id|name|age|bp|hr|temp|complaint
            p.id = fields[0];
            p.name = fields[1];
            p.age = stoi(fields[2]);
            p.bp = fields[3];
            p.hr = stoi(fields[4]);
            p.temp = stod(fields[5]);
            p.complaint = fields[6];
        } else if (fields.size() == 8) {
            // Current format: id|name|birthDate|age|bp|hr|temp|complaint
            p.id = fields[0];
            p.name = fields[1];
            p.birthDate = fields[2];
            p.age = stoi(fields[3]);

            int updatedAge = calculateAgeFromBirthDate(p.birthDate);
            if (updatedAge >= 0) {
                p.age = updatedAge;
            }

            p.bp = fields[4];
            p.hr = stoi(fields[5]);
            p.temp = stod(fields[6]);
            p.complaint = fields[7];
        } else {
            cout << "[!] Error: Skipping malformed patient record on line "
                 << lineNumber << " (expected 7 or 8 fields).\n";
            continue;
        }

        p.systolicBp = extractSystolic(p.bp);

        // Re-calculate their triage score based on their age and vitals
        evaluateAgeBasedTriage(p);

        // Add them back into the active vector queue
        patients.push_back(p);
    }

    inFile.close();
}