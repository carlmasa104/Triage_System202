#include "AgeCalculator.h"
#include "Patient.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <ctime>

using namespace std;

// Helper to check if a year is a leap year
static bool isLeapYear(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

// Helper to get number of days in a given month/year
static int daysInMonth(int year, int month) {
    switch (month) {
        case 1: case 3: case 5: case 7: case 8: case 10: case 12:
            return 31;
        case 4: case 6: case 9: case 11:
            return 30;
        case 2:
            return isLeapYear(year) ? 29 : 28;
        default:
            return 0;
    }
}

// Gets current local year, month, and day from system clock
static void getCurrentDate(int& curYear, int& curMonth, int& curDay) {
    auto now = chrono::system_clock::now();
    time_t nowTime = chrono::system_clock::to_time_t(now);
    tm localTm{};

    #ifdef _WIN32
        localtime_s(&localTm, &nowTime);
    #else
        localtime_r(&nowTime, &localTm);
    #endif

    curYear = localTm.tm_year + 1900;
    curMonth = localTm.tm_mon + 1;
    curDay = localTm.tm_mday;
}

// Parses "YYYY-MM-DD" strictly into year, month, day
static bool parseBirthDate(const string& input, int& year, int& month, int& day) {
    if (input.size() != 10 || input[4] != '-' || input[7] != '-') {
        return false;
    }

    char dash1, dash2;
    stringstream ss(input);
    if (!(ss >> year >> dash1 >> month >> dash2 >> day) || !ss.eof()) {
        return false;
    }

    if (dash1 != '-' || dash2 != '-') {
        return false;
    }

    if (year < 1900 || month < 1 || month > 12) {
        return false;
    }

    int maxDays = daysInMonth(year, month);
    if (day < 1 || day > maxDays) {
        return false;
    }

    int curYear, curMonth, curDay;
    getCurrentDate(curYear, curMonth, curDay);

    // Reject dates in the future
    if (year > curYear ||
        (year == curYear && month > curMonth) ||
        (year == curYear && month == curMonth && day > curDay)) {
        return false;
    }

    return true;
}

int calculateAgeFromBirthDate(const string& birthDate) {
    int bYear, bMonth, bDay;
    if (!parseBirthDate(birthDate, bYear, bMonth, bDay)) {
        return -1;
    }

    int curYear, curMonth, curDay;
    getCurrentDate(curYear, curMonth, curDay);

    int age = curYear - bYear;

    // Subtract 1 year if birthday hasn't occurred yet this year
    if (curMonth < bMonth || (curMonth == bMonth && curDay < bDay)) {
        age--;
    }

    return (age < 0) ? 0 : age;
}

int promptAndCalculateAge(string& birthDateOut) {
    while (true) {
        cout << "Enter Birthday (YYYY-MM-DD): ";
        string input;
        getline(cin, input);

        int age = calculateAgeFromBirthDate(input);
        if (age >= 0) {
            birthDateOut = input;
            cout << "Calculated Age: " << age << "\n";
            return age;
        }

        cout << RED << "[!] Invalid date or future birthday. Please use YYYY-MM-DD (e.g., 2002-05-14)." << RESET << "\n";
    }
}