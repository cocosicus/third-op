#ifndef LOGIC_H
#define LOGIC_H

#include "context.h"

// Function to load data from CSV file
void loadData(Context* context, const char* filename);

// Function to calculate statistics
void calculateStats(Context* context, const char* region, int column);

#endif // LOGIC_H
