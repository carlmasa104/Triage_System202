#include "HistoryLog.h"
#include "Patient.h"
#include "ClinicStats.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <ctime>
#include <limits>

using namespace std;

static const string HISTORY_FILE = "system_history.txt";

static string currentTimestamp() {
    time_t now = time(nullptr);
    tm localTm{};
#ifdef _WIN32
    localtime_s(&localTm, &now);
#else
    localtime_r(&now, &localTm);
#endif
    char buf[24];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &localTm);
    return string(buf);
}

void logSystemEvent(const string& actor, const string& action, const string& details) {
    ofstream outFile(HISTORY_FILE, ios::app);
    if (!outFile.is_open()) {
        return;
    }

    string who = actor.empty() ? "SYSTEM" : actor;
    outFile << currentTimestamp() << " | " << who << " | " << action;
    if (!details.empty()) {
        outFile << " | " << details;
    }
    outFile << "\n";
    outFile.close();
}

void viewHistoryLog() {
    clearScreen();
    printHeader();
    cout << YELLOW << "--- [ADMIN] SYSTEM HISTORY LOG (LATEST TO OLDEST) ---" << RESET << "\n\n";

    ifstream inFile(HISTORY_FILE);
    if (!inFile.is_open()) {
        cout << RED << "[!] No history log entries recorded yet." << RESET << "\n\n";
        cout << "Press Enter to return to Admin Menu...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
        return;
    }

    vector<string> entries;
    string line;
    while (getline(inFile, line)) {
        if (!line.empty()) {
            entries.push_back(line);
        }
    }
    inFile.close();

    if (entries.empty()) {
        cout << RED << "[!] History log is currently empty." << RESET << "\n\n";
    } else {
        int displayNum = 1;
        for (size_t i = entries.size(); i > 0; --i) {
            const string& entry = entries[i - 1];
            string color = RESET;

            if (entry.find("UNREGISTER") != string::npos || entry.find("DISCHARGE") != string::npos || entry.find("FAILED") != string::npos) {
                color = RED;
            } else if (entry.find("REGISTER") != string::npos || entry.find("LOGIN") != string::npos) {
                color = GREEN;
            } else if (entry.find("UPDATE") != string::npos || entry.find("LOGOUT") != string::npos) {
                color = YELLOW;
            } else {
                color = CYAN;
            }

            cout << "[" << displayNum++ << "] " << color << entry << RESET << "\n";
        }
        cout << "\n";
    }

    cout << "Press Enter to return to Admin Menu...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}