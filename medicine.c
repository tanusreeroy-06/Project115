#include <stdio.h>
#include <string.h>
#include "medicine.h"
#include <stdlib.h>

static int readLineTrim(FILE *file, char *buf, size_t size) {
    if (fgets(buf, (int)size, file) == NULL) return 0;
    buf[strcspn(buf, "\r\n")] = '\0';
    return 1;
}

void setupFile() {
    FILE *file;

    file = fopen("medicines.txt", "a");

    if (file == NULL) {
        printf("File could not be created.\n");
        return;
    }

    fclose(file);
}

void addMedicine() {
    FILE *file;
    struct Medicine med;

    file = fopen("medicines.txt", "a");

    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    printf("\nEnter medicine ID: ");
    scanf("%d", &med.id);

    printf("Enter medicine name: ");
    scanf(" %[^\n]", med.name);

    printf("Enter quantity: ");
    scanf("%d", &med.quantity);

    printf("Enter price: ");
    scanf("%f", &med.price);

    fprintf(file, "%d\n", med.id);
    fprintf(file, "%s\n", med.name);
    fprintf(file, "%d\n", med.quantity);
    fprintf(file, "%.2f\n", med.price);

    fclose(file);

    printf("\nMedicine added successfully.\n");
}

void viewAllMedicines() {
    FILE *file;
    struct Medicine med;
    int found = 0;
    char line[256];

    file = fopen("medicines.txt", "r");

        if (file == NULL) {
            printf("No medicine file found.\n");
            return;
        }

        printf("\n========== All Medicines ==========");

        while (1) {
            if (!readLineTrim(file, line, sizeof(line))) break;
            if (line[0] == '\0') continue;

            med.id = (int)strtol(line, NULL, 10);

            if (!readLineTrim(file, med.name, sizeof(med.name))) break;

            if (!readLineTrim(file, line, sizeof(line))) break;
            med.quantity = (int)strtol(line, NULL, 10);

            if (!readLineTrim(file, line, sizeof(line))) break;
            med.price = strtof(line, NULL);

            printf("\nMedicine ID: %d", med.id);
            printf("\nMedicine Name: %s", med.name);
            printf("\nQuantity: %d", med.quantity);
            printf("\nPrice: %.2f\n", med.price);

            found = 1;
        }

    if (!found) {
        printf("\nNo medicines found.\n");
    }

    fclose(file);
}

void searchMedicine() {
    FILE *file;
    struct Medicine med;
    char line[256];
    int searchId;
    int found = 0;

    file = fopen("medicines.txt", "r");

    if (file == NULL) {
        printf("No medicine file found.\n");
        return;
    }

    printf("\nEnter medicine ID to search: ");
    scanf("%d", &searchId);

    while (1) {
        if (!readLineTrim(file, line, sizeof(line))) break;
        if (line[0] == '\0') continue;

        med.id = (int)strtol(line, NULL, 10);

        if (!readLineTrim(file, med.name, sizeof(med.name))) break;

        if (!readLineTrim(file, line, sizeof(line))) break;
        med.quantity = (int)strtol(line, NULL, 10);

        if (!readLineTrim(file, line, sizeof(line))) break;
        med.price = strtof(line, NULL);

        if (med.id == searchId) {
            printf("\nMedicine found:\n");
            printf("Medicine ID: %d\n", med.id);
            printf("Medicine Name: %s\n", med.name);
            printf("Quantity: %d\n", med.quantity);
            printf("Price: %.2f\n", med.price);

            found = 1;
            break;
        }
    }

    if (!found) {
        printf("\nMedicine not found.\n");
    }

    fclose(file);
}

void mainMenu() {
    int choice;

    while (1) {
        printf("\n\n========== Pharmacy Management System ==========\n");
        printf("1. Add medicine\n");
        printf("2. View all medicines\n");
        printf("3. Search medicine\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addMedicine();
                break;

            case 2:
                viewAllMedicines();
                break;

            case 3:
                searchMedicine();
                break;

            case 4:
                printf("\nExiting program.\n");
                return;

            default:
                printf("\nInvalid choice. Try again.\n");
        }
    }
}