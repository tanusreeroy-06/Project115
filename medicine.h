#ifndef MEDICINE_H
#define MEDICINE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct medicine {
    int id;
    char name[50];
    char category[50];
    float price;
    int quantity;
    char expiry[15];
};

struct supplier {
    int id;
    char name[50];
    char phone[20];
    char company[50];
};

// Function prototypes
int isExpired(char expiry[], int day, int month, int year);
int isNearExpiry(char expiry[], int day, int month, int year);
void showExpiredmedicines();
void showNearExpiredMedicines();
int canSell(struct medicine m, int d, int mn, int y);

void addSupplier();
void viewSupplier();
void updateSupplier();
void deleteSupplier();
void supplierMenu();

#endif

