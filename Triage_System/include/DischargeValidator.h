#ifndef DISCHARGE_VALIDATOR_H
#define DISCHARGE_VALIDATOR_H

#include "Patient.h"

// Prompts the admin through all 5 discharge criteria and returns true only if all pass
bool validateDischargeEligibility(const Patient& p);

#endif