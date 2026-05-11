#ifndef MEDICINE_H
#define MEDICINE_H

#define MAX 100

struct Medicine {
    int id;
    char name[50];
    int stock;
    float price;
    char expiryDate[11];  // DD/MM/YYYY
    int supplierId;
};

struct Supplier {
    int id;
    char name[50];
    char contact[20];
    char address[100];
};

void loadMedicines();
void saveMedicines();
void showMedicines();
void addMedicine();
void sellMedicine();
void salesReport();
void searchMedicine();
void updateMedicine();
void deleteMedicine();
void increaseStock();
void decreaseStock();
void lowStockAlert();
void outOfStockAlert();
void checkExpiredMedicines();
void checkNearExpiryMedicines();
void loadSuppliers();
void saveSuppliers();
void addSupplier();
void viewSuppliers();
void updateSupplier();
void deleteSupplier();
void mainMenu();

#endif

