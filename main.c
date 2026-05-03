#include "medicine.h"

int main() {
    int choice;
    do {
        printf("\n1. Show Expired Medicines\n2. Show Nearly Expired Medicines\n3. Supplier Management\n4. Exit\nEnter choice: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1: showExpiredmedicines(); break;
            case 2: showNearExpiredMedicines(); break;
            case 3: supplierMenu(); break;
        }
    } while (choice != 4);
    return 0;
}
