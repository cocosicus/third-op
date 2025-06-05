#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

#define MAX_REGION_LENGTH 128

typedef struct {
    int year;
    char region[MAX_REGION_LENGTH];
    double natural_population_growth;
    double birth_rate;
    double death_rate;
    double general_demographic_weight;
    double urbanization;
    int valid; // 1 - valid, 0 - invalid
} DemographyData;

#endif
