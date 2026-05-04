#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "medicine.h"

struct Medicine med[MAX];
struct Supplier supplier[MAX];
int count = 0;
int supplierCount = 0;

static void clearInputBuffer() {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {
    }
}

int compareDates(char *date1, char *date2) {
    int d1, m1, y1, d2, m2, y2;
    sscanf(date1, "%d/%d/%d", &d1, &m1, &y1);
    sscanf(date2, "%d/%d/%d", &d2, &m2, &y2);
    
    if (y1 != y2) return y1 - y2;
    if (m1 != m2) return m1 - m2;
    return d1 - d2;
}

int getDaysDifference(char *date) {
    int d, m, y;
    sscanf(date, "%d/%d/%d", &d, &m, &y);
    
    time_t now = time(NULL);
    struct tm *timeinfo = localtime(&now);
    
    struct tm futureDate = {0};
    futureDate.tm_mday = d;
    futureDate.tm_mon = m - 1;
    futureDate.tm_year = y - 1900;
    futureDate.tm_hour = 0;
    futureDate.tm_min = 0;
    futureDate.tm_sec = 0;
    
    time_t future = mktime(&futureDate);
    time_t current = mktime(timeinfo);
    
    int diff = (int)((future - current) / (60 * 60 * 24));
    return diff;
}

void loadMedicines() {
    FILE *fp = fopen("medicines.txt", "r");
    char line[256];

    if (fp == NULL) {
        printf("ERROR: medicines.txt not found!\n");
        return;
    }

    count = 0;

    while (count < MAX && fgets(line, sizeof(line), fp) != NULL) {
        if (sscanf(line, "%d|%49[^|]|%d|%f|%10[^|]|%d",
                   &med[count].id,
                   med[count].name,
                   &med[count].stock,
                   &med[count].price,
                   med[count].expiryDate,
                   &med[count].supplierId) == 6) {
            count++;
            continue;
        }

        // Backward compatibility for old single-word name format.
        if (sscanf(line, "%d %49s %d %f %10s %d",
                   &med[count].id,
                   med[count].name,
                   &med[count].stock,
                   &med[count].price,
                   med[count].expiryDate,
                   &med[count].supplierId) == 6) {
            count++;
        }
    }

    fclose(fp);
}

void saveMedicines() {
    FILE *fp = fopen("medicines.txt", "w");

    if (fp == NULL) {
        printf("ERROR!\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        fprintf(fp, "%d|%s|%d|%.2f|%s|%d\n",
                med[i].id,
                med[i].name,
                med[i].stock,
                med[i].price,
                med[i].expiryDate,
                med[i].supplierId);
    }

    fclose(fp);
}

void showMedicines() {
    if (count == 0) {
        printf("No medicines available!\n");
        return;
    }

    printf("\nID\tName\t\tStock\tPrice\tExpiry\t\tSupplier\n");

    for (int i = 0; i < count; i++) {
        printf("%d\t%s\t%d\t%.2f\t%s\t%d\n",
               med[i].id,
               med[i].name,
               med[i].stock,
               med[i].price,
               med[i].expiryDate,
               med[i].supplierId);
    }
}

void addMedicine() {
    struct Medicine newMed;
    int idExists;

    if (count >= MAX) {
        printf("Cannot add more medicines. Storage is full!\n");
        return;
    }

    do {
        idExists = 0;

        printf("\nEnter medicine ID: ");
        scanf("%d", &newMed.id);

        for (int i = 0; i < count; i++) {
            if (med[i].id == newMed.id) {
                idExists = 1;
                printf("Medicine ID already exists! Please re-enter a unique ID.\n");
                break;
            }
        }
    } while (idExists);

    clearInputBuffer();
    printf("Enter medicine name: ");
    scanf(" %49[^\n]", newMed.name);

    printf("Enter stock quantity: ");
    scanf("%d", &newMed.stock);

    printf("Enter price: ");
    scanf("%f", &newMed.price);

    printf("Enter expiry date (DD/MM/YYYY): ");
    scanf("%s", newMed.expiryDate);

    printf("Enter supplier ID: ");
    scanf("%d", &newMed.supplierId);

    med[count] = newMed;
    count++;

    saveMedicines();

    printf("\nMedicine added successfully.\n");
}

void sellMedicine() {
    int id, q;
    float total;
    int found = 0;

    FILE *fp = fopen("sales.txt", "a");

    if (fp == NULL) {
        printf("ERROR in opening sales file!\n");
        return;
    }

    showMedicines();

    printf("\nEnter Medicine ID: ");
    scanf("%d", &id);

    for (int i = 0; i < count; i++) {
        if (med[i].id == id) {
            found = 1;

            // Check if medicine is expired
            if (getDaysDifference(med[i].expiryDate) < 0) {
                printf("ERROR: Medicine is expired! Cannot sell.\n");
                fclose(fp);
                return;
            }

            printf("Enter Quantity: ");
            scanf("%d", &q);

            if (q > med[i].stock) {
                printf("Not enough stock!\n");
                fclose(fp);
                return;
            }

            total = (float)q * med[i].price;
            med[i].stock -= q;

            printf("\n    BILL    \n");
            printf("Name: %s\n", med[i].name);
            printf("Qty: %d\n", q);
            printf("Expiry: %s\n", med[i].expiryDate);
            printf("Total: %.2f\n", total);

                fprintf(fp, "%s|%d|%.2f\n",
                    med[i].name, q, total);

            fclose(fp);

            saveMedicines();

            printf("Sale successful!\n");
            return;
        }
    }

    if (!found) {
        printf("Medicine not found!\n");
    }

    fclose(fp);
}

void salesReport() {
    FILE *fp = fopen("sales.txt", "r");
    char line[256];

    if (fp == NULL) {
        printf("No sales record found!\n");
        return;
    }

    char name[50];
    int q;
    float total;

    printf("\n--- SALES REPORT ---\n");

    while (fgets(line, sizeof(line), fp) != NULL) {
        if (sscanf(line, "%49[^|]|%d|%f", name, &q, &total) == 3 ||
            sscanf(line, "%49s %d %f", name, &q, &total) == 3) {
            printf("%s | Quantity: %d | Total: %.2f\n",
                   name, q, total);
        }
    }

    fclose(fp);
}

// Tanusree's Functions
void searchMedicine() {
    int searchId;
    int found = 0;

    printf("\nEnter medicine ID to search: ");
    scanf("%d", &searchId);

    for (int i = 0; i < count; i++) {
        if (med[i].id == searchId) {
            printf("\n--- Medicine Found ---\n");
            printf("ID: %d\n", med[i].id);
            printf("Name: %s\n", med[i].name);
            printf("Stock: %d\n", med[i].stock);
            printf("Price: %.2f\n", med[i].price);
            printf("Expiry Date: %s\n", med[i].expiryDate);
            printf("Supplier ID: %d\n", med[i].supplierId);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Medicine not found!\n");
    }
}

void updateMedicine() {
    int id;
    int found = 0;

    printf("\nEnter medicine ID to update: ");
    scanf("%d", &id);

    for (int i = 0; i < count; i++) {
        if (med[i].id == id) {
            found = 1;

            printf("\nCurrent details:\n");
            printf("Name: %s\n", med[i].name);
            printf("Stock: %d\n", med[i].stock);
            printf("Price: %.2f\n", med[i].price);
            printf("Expiry: %s\n", med[i].expiryDate);

            clearInputBuffer();
            printf("\nEnter new name: ");
            scanf(" %49[^\n]", med[i].name);

            printf("Enter new stock: ");
            scanf("%d", &med[i].stock);

            printf("Enter new price: ");
            scanf("%f", &med[i].price);

            printf("Enter new expiry date (DD/MM/YYYY): ");
            scanf("%s", med[i].expiryDate);

            saveMedicines();
            printf("Medicine updated successfully!\n");
            break;
        }
    }

    if (!found) {
        printf("Medicine not found!\n");
    }
}

void deleteMedicine() {
    int id;
    int found = 0;

    printf("\nEnter medicine ID to delete: ");
    scanf("%d", &id);

    for (int i = 0; i < count; i++) {
        if (med[i].id == id) {
            found = 1;

            for (int j = i; j < count - 1; j++) {
                med[j] = med[j + 1];
            }
            count--;

            saveMedicines();
            printf("Medicine deleted successfully!\n");
            break;
        }
    }

    if (!found) {
        printf("Medicine not found!\n");
    }
}

void increaseStock() {
    int id, quantity;
    int found = 0;

    printf("\nEnter medicine ID: ");
    scanf("%d", &id);

    for (int i = 0; i < count; i++) {
        if (med[i].id == id) {
            found = 1;

            printf("Current stock: %d\n", med[i].stock);
            printf("Enter quantity to add: ");
            scanf("%d", &quantity);

            med[i].stock += quantity;

            saveMedicines();
            printf("Stock increased! New stock: %d\n", med[i].stock);
            break;
        }
    }

    if (!found) {
        printf("Medicine not found!\n");
    }
}

void decreaseStock() {
    int id, quantity;
    int found = 0;

    printf("\nEnter medicine ID: ");
    scanf("%d", &id);

    for (int i = 0; i < count; i++) {
        if (med[i].id == id) {
            found = 1;

            printf("Current stock: %d\n", med[i].stock);
            printf("Enter quantity to reduce: ");
            scanf("%d", &quantity);

            if (quantity > med[i].stock) {
                printf("Cannot reduce more than available stock!\n");
                break;
            }

            med[i].stock -= quantity;

            saveMedicines();
            printf("Stock decreased! New stock: %d\n", med[i].stock);
            break;
        }
    }

    if (!found) {
        printf("Medicine not found!\n");
    }
}

void lowStockAlert() {
    printf("\n--- LOW STOCK ALERT (Stock <= 50) ---\n");
    int found = 0;

    for (int i = 0; i < count; i++) {
        if (med[i].stock <= 50 && med[i].stock > 0) {
            printf("ID: %d | Name: %s | Stock: %d\n",
                   med[i].id, med[i].name, med[i].stock);
            found = 1;
        }
    }

    if (!found) {
        printf("No medicines with low stock.\n");
    }
}

void outOfStockAlert() {
    printf("\n--- OUT OF STOCK ALERT ---\n");
    int found = 0;

    for (int i = 0; i < count; i++) {
        if (med[i].stock == 0) {
            printf("ID: %d | Name: %s\n", med[i].id, med[i].name);
            found = 1;
        }
    }

    if (!found) {
        printf("No out of stock medicines.\n");
    }
}

// Nowshin's Functions
void checkExpiredMedicines() {
    printf("\n--- EXPIRED MEDICINES ---\n");
    int found = 0;

    for (int i = 0; i < count; i++) {
        if (getDaysDifference(med[i].expiryDate) < 0) {
            printf("ID:     %d | Name: %s          | Expiry: %s |     Stock: %d\n",
                   med[i].id, med[i].name, med[i].expiryDate, med[i].stock);
            found = 1;
        }
    }

    if (!found) {
        printf("No expired medicines found.\n");
    }
}

void checkNearExpiryMedicines() {
    printf("\n--- NEAR EXPIRY MEDICINES (Within 30 days) ---\n");
    int found = 0;

    for (int i = 0; i < count; i++) {
        int days = getDaysDifference(med[i].expiryDate);
        if (days >= 0 && days <= 30) {
            printf("ID: %d | Name: %s | Expiry: %s | Days Left: %d\n",
                   med[i].id, med[i].name, med[i].expiryDate, days);
            found = 1;
        }
    }

    if (!found) {
        printf("No medicines expiring within 30 days.\n");
    }
}

void loadSuppliers() {
    FILE *fp = fopen("suppliers.txt", "r");

    if (fp == NULL) {
        supplierCount = 0;
        return;
    }

    supplierCount = 0;

    while (fscanf(fp, "%d %s %s %99[^\n]",
                  &supplier[supplierCount].id,
                  supplier[supplierCount].name,
                  supplier[supplierCount].contact,
                  supplier[supplierCount].address) == 4) {
        supplierCount++;
    }

    fclose(fp);
}

void saveSuppliers() {
    FILE *fp = fopen("suppliers.txt", "w");

    if (fp == NULL) {
        printf("ERROR opening suppliers file!\n");
        return;
    }

    for (int i = 0; i < supplierCount; i++) {
        fprintf(fp, "%d %s %s %s\n",
                supplier[i].id,
                supplier[i].name,
                supplier[i].contact,
                supplier[i].address);
    }

    fclose(fp);
}

void addSupplier() {
    struct Supplier newSupplier;

    printf("\nEnter supplier ID: ");
    scanf("%d", &newSupplier.id);

    printf("Enter supplier name: ");
    scanf("%s", newSupplier.name);

    printf("Enter contact number: ");
    scanf("%s", newSupplier.contact);

    printf("Enter address: ");
    scanf(" %[^\n]", newSupplier.address);

    supplier[supplierCount] = newSupplier;
    supplierCount++;

    saveSuppliers();

    printf("Supplier added successfully!\n");
}

void viewSuppliers() {
    if (supplierCount == 0) {
        printf("No suppliers available!\n");
        return;
    }

    printf("\n--- SUPPLIERS LIST ---\n");
    printf("ID\tName\t\tContact\t\tAddress\n");

    for (int i = 0; i < supplierCount; i++) {
        printf("%d\t%s\t%s\t%s\n",
               supplier[i].id,
               supplier[i].name,
               supplier[i].contact,
               supplier[i].address);
    }
}

void updateSupplier() {
    int id;
    int found = 0;

    printf("\nEnter supplier ID to update: ");
    scanf("%d", &id);

    for (int i = 0; i < supplierCount; i++) {
        if (supplier[i].id == id) {
            found = 1;

            printf("Enter new name: ");
            scanf("%s", supplier[i].name);

            printf("Enter new contact: ");
            scanf("%s", supplier[i].contact);

            printf("Enter new address: ");
            scanf(" %[^\n]", supplier[i].address);

            saveSuppliers();
            printf("Supplier updated successfully!\n");
            break;
        }
    }

    if (!found) {
        printf("Supplier not found!\n");
    }
}

void deleteSupplier() {
    int id;
    int found = 0;

    printf("\nEnter supplier ID to delete: ");
    scanf("%d", &id);

    for (int i = 0; i < supplierCount; i++) {
        if (supplier[i].id == id) {
            found = 1;

            for (int j = i; j < supplierCount - 1; j++) {
                supplier[j] = supplier[j + 1];
            }
            supplierCount--;

            saveSuppliers();
            printf("Supplier deleted successfully!\n");
            break;
        }
    }

    if (!found) {
        printf("Supplier not found!\n");
    }
}

void mainMenu() {
    int choice;

    loadSuppliers();

    while (1) {
        printf("\n==========PHARMACY MANAGEMENT SYSTEM ==========\n");
        printf("--- GENERAL OPTIONS ---\n");
        printf("1.  Show Medicines\n");
        printf("2.  Add Medicine\n");
        printf("3.  Sell Medicine\n");
        printf("4.  Sales Report\n");
        printf("--- STOCK MANAGEMENT ---\n");
        printf("5.  Search Medicine\n");
        printf("6.  Update Medicine\n");
        printf("7.  Delete Medicine\n");
        printf("8.  Increase Stock\n");
        printf("9.  Decrease Stock\n");
        printf("10. Low Stock Alert\n");
        printf("11. Out of Stock Alert\n");
        printf("--- EXPIRY MANAGEMENT  ---\n");
        printf("12. Check Expired Medicines\n");
        printf("13. Check Near Expiry Medicines\n");
        printf("--- SUPPLIER MANAGEMENT ---\n");
        printf("14. Add Supplier\n");
        printf("15. View Suppliers\n");
        printf("16. Update Supplier\n");
        printf("17. Delete Supplier\n");
        printf("18. Exit\n");

        printf("Enter choice: ");
        if (scanf("%d", &choice) != 1) {
            // Clear input buffer if scanf fails
            while (getchar() != '\n');
            printf("Invalid input!\n");
            continue;
        }

        switch (choice) {
            case 1:
                showMedicines();
                break;
            case 2:
                addMedicine();
                break;
            case 3:
                sellMedicine();
                break;
            case 4:
                salesReport();
                break;
            case 5:
                searchMedicine();
                break;
            case 6:
                updateMedicine();
                break;
            case 7:
                deleteMedicine();
                break;
            case 8:
                increaseStock();
                break;
            case 9:
                decreaseStock();
                break;
            case 10:
                lowStockAlert();
                break;
            case 11:
                outOfStockAlert();
                break;
            case 12:
                checkExpiredMedicines();
                break;
            case 13:
                checkNearExpiryMedicines();
                break;
            case 14:
                addSupplier();
                break;
            case 15:
                viewSuppliers();
                break;
            case 16:
                updateSupplier();
                break;
            case 17:
                deleteSupplier();
                break;
            case 18:
                printf("Exiting...\n");
                return;
            default:
                printf("Invalid choice!\n");
        }
    }
}