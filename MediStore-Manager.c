#include<stdio.h>
#include<string.h>

struct Medicine
{
    char name[100];
    int quantity;
    float price;
};

// update and delete medicine
void updateORdelete(int choice)
{
    // Choice 1 = update ; Choice 2 = delete
    FILE *fp = fopen("pharmacy.dat","rb");

    FILE *temp = fopen("temp.dat","wb");

    struct Medicine m;
    char targetName[100];
    int found = 0;

    printf("Enter The Medicine Name to %s:",
           (choice == 1)? "Update" : "Delete");
    scanf("%s ", targetName);

    while(fread(&m, sizeof(struct Medicine), 1, fp))
    {
        if(strcmp(m.name, targetName) == 0)
        {
            found = 1;
            if(choice == 1)
            {
                //update logic
                printf("Enter New Quantity: ");
                scanf("%d",&m.quantity);

                printf("Enter New Price: ");
                scanf("%f",&m.price);

                fwrite(&m,sizeof(struct Medicine), 1, temp);
            }
        }
        //choice 2
        else
        {
            fwrite(&m, sizeof(struct Medicine), 1, temp);
        }
    }

    fclose(fp);
    fclose(temp);
    remove("pharmacy.dat");
    rename("temp.dat", "pharmacy.dat");

    if(found)
    {
        printf("Operation Successful!\n");
    }
    else
    {
        printf("Medicine Not Found!\n");
    }
}


    // increase & decrease stock
    void manageStock(int isIncrease)
    {
        // isIncrease 1 = increase, 0 = decrease
        FILE *fp = fopen("pharmacy.dat", "rb+");

        struct Medicine m;
        char targetName[100];
        int qty, found = 0;

        printf("Enter The Medicine Name: ");
        scanf("%s ", targetName);

        while(fread(&m, sizeof(struct Medicine), 1, fp))
        {
            if(strcmp(m.name, targetName) == 0)
            {
                printf("Current Stock: %d.Enter amount to %s : ",
                       m.quantity , isIncrease ? "add" : "reduce");

                scanf("%d", &qty);

                if(isIncrease)
                {
                    m.quantity += qty;
                }
                else
                {
                    m.quantity -= qty;
                    if(m.quantity < 0)
                    {
                        m.quantity = 0;
                    }
                }

                fseek(fp, -sizeof(struct Medicine), SEEK_CUR);

                found = 1;
                break;
            }
        }
        fclose(fp);
        if (found)
        {
            printf("Stock updated sucessfully!\n");
        }
        else
        {
            printf("Medicine not found!\n");
        }
    }

    //low stock & out of stock check
    void checkStockAlerts()
    {
        FILE *fp = fopen("pharmacy.dat", "rb");

        struct Medicine m;
        int hasAlert = 0;

        printf("\n----STOCK STATUS ALERT----\n");

        while(fread(&m, sizeof(struct Medicine), 1, fp))
        {
            if(m.quantity == 0)
            {
                printf("[OUT OF STOCK]: %s\n ",m.name);

                hasAlert = 1;
            }
            else if(m.quantity < 10)
            {
                // below 10 means low stock
                printf("[LOW STOCK] : %s(ONLY %d LEFT)\n", m.name, m.quantity);

                hasAlert = 1;
            }
        }
        fclose(fp);
        if(!hasAlert)
        {
            printf("All stock levels are normal.\n");
        }

    }

    //done by roy


















































