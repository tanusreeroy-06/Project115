#include "medicine.h"

int isExpired(char expiry[], int day, int month, int year) {
    int exp_d, exp_m, exp_y;
    sscanf(expiry, "%d/%d/%d", &exp_d, &exp_m, &exp_y);
    if (exp_y < year) return 1;
    if (exp_y == year && exp_m < month) return 1;
    if (exp_y == year && exp_m == month && exp_d < day) return 1;
    return 0;
}

int isNearExpiry(char expiry[], int day, int month, int year) {
    int exp_d, exp_m, exp_y;
    sscanf(expiry, "%d/%d/%d", &exp_d, &exp_m, &exp_y);
    if (exp_y == year && exp_m == month && exp_d >= day) return 1;
    return 0;
}

void showExpiredmedicines() {
    FILE *fp = fopen("medicine.txt", "r");
    struct medicine m;
    if (fp == NULL) { printf("File not found!\n"); return; }
    int d, mn, y;
    printf("Enter todays date (dd/mm/yyyy): ");
    scanf("%d/%d/%d", &d, &mn, &y);
    printf("\n________ EXPIRED MEDICINES ________\n");
    while (fscanf(fp, "%d %s %s %f %d %s", &m.id, m.name, m.category, &m.price, &m.quantity, m.expiry) != EOF) {
        if (isExpired(m.expiry, d, mn, y)) {
            printf("ID: %d | NAME: %s | Expiry: %s\n", m.id, m.name, m.expiry);
        }
    }
    fclose(fp);
}

void showNearExpiredMedicines() {
    FILE *fp = fopen("medicine.txt", "r");
    struct medicine m;
    if (fp == NULL) { printf("File not found!\n"); return; }
    int d, mn, y;
    printf("Enter todays date (dd/mm/yyyy): ");
    scanf("%d/%d/%d", &d, &mn, &y);
    printf("\n________ NEAR EXPIRED MEDICINES ________\n");
    while (fscanf(fp, "%d %s %s %f %d %s", &m.id, m.name, m.category, &m.price, &m.quantity, m.expiry) != EOF) {
        if (isNearExpiry(m.expiry, d, mn, y)) {
            printf("ID: %d | NAME: %s | Expiry: %s\n", m.id, m.name, m.expiry);
        }
    }
    fclose(fp);
}

void addSupplier() {
    FILE *fp = fopen("supplier.txt", "a");
    struct supplier s;
    printf("Enter supplier ID: "); scanf("%d", &s.id);
    printf("Enter supplier NAME: "); scanf("%s", s.name);
    printf("Enter supplier PHONE: "); scanf("%s", s.phone);
    printf("Enter supplier COMPANY: "); scanf("%s", s.company);
    fprintf(fp, "%d %s %s %s\n", s.id, s.name, s.phone, s.company);
    fclose(fp);
    printf("Supplier added successfully\n");
}

void viewSupplier() {
    FILE *fp = fopen("supplier.txt", "r");
    struct supplier s;
    if (fp == NULL) { printf("File not found!\n"); return; }
    printf("\n____ Supplier LIST _____\n");
    while (fscanf(fp, "%d %s %s %s", &s.id, s.name, s.phone, s.company) != EOF) {
        printf("\nID: %d\nNAME: %s\nPHONE: %s\nCOMPANY: %s\n", s.id, s.name, s.phone, s.company);
    }
    fclose(fp);
}

void deleteSupplier() {
    FILE *fp = fopen("supplier.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    struct supplier s;
    int id, found = 0;
    if (fp == NULL) { printf("File not found!\n"); return; }
    printf("Enter Supplier ID to delete: "); scanf("%d", &id);
    while (fscanf(fp, "%d %s %s %s", &s.id, s.name, s.phone, s.company) != EOF) {
        if (s.id != id) fprintf(temp, "%d %s %s %s\n", s.id, s.name, s.phone, s.company);
        else found = 1;
    }
    fclose(fp); fclose(temp);
    remove("supplier.txt"); rename("temp.txt", "supplier.txt");
    if (found) printf("Supplier deleted successfully\n");
    else printf("Supplier not found!\n");
}

void updateSupplier() {
    FILE *fp = fopen("supplier.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    struct supplier s;
    int id, found = 0;
    if (fp == NULL) { printf("File not found!\n"); return; }
    printf("Enter Supplier ID to update: "); scanf("%d", &id);
    while (fscanf(fp, "%d %s %s %s", &s.id, s.name, s.phone, s.company) != EOF) {
        if (s.id == id) {
            printf("Enter New Name: "); scanf("%s", s.name);
            printf("Enter New Phone: "); scanf("%s", s.phone);
            printf("Enter New Company: "); scanf("%s", s.company);
            found = 1;
        }
        fprintf(temp, "%d %s %s %s\n", s.id, s.name, s.phone, s.company);
    }
    fclose(fp); fclose(temp);
    remove("supplier.txt"); rename("temp.txt", "supplier.txt");
    if (found) printf("Supplier updated successfully\n");
    else printf("Supplier not found!\n");
}

void supplierMenu() {
    int ch;
    do {
        printf("\n_______ SUPPLIER MENU ______\n");
        printf("1. Add Supplier\n2. View Supplier\n3. Update Supplier\n4. Delete Supplier\n5. Back\nEnter choice: ");
        scanf("%d", &ch);
        switch (ch) {
            case 1: addSupplier(); break;
            case 2: viewSupplier(); break;
            case 3: updateSupplier(); break;
            case 4: deleteSupplier(); break;
        }
    } while (ch != 5);
}

