#ifndef MEDICINE_H
#define MEDICINE_H

struct Medicine {
    int id;
    char name[128];
    int quantity;
    float price;
};

void setupFile();
void addMedicine();
void viewAllMedicines();
void searchMedicine();
void mainMenu();

#endif