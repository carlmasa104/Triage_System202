#ifndef AGE_CALCULATOR_H
#define AGE_CALCULATOR_H

#include <string>

// Prompts the admin for a birthday (YYYY-MM-DD), stores it in birthDateOut,
// and returns the automatically calculated age.
int promptAndCalculateAge(std::string& birthDateOut);

// Calculates age in years from a "YYYY-MM-DD" string using the current system date.
int calculateAgeFromBirthDate(const std::string& birthDate);

#endif