// Bank-account program reads a random-access file sequentially,
// updates data already written to the file, creates new data to
// be placed in the file, and deletes data previously in the file.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// clientData structure definition
struct clientData
{
    unsigned int acctNum;
    char lastName[15];
    char firstName[10];
    char phone[15];
    double balance;
};

// prototypes
unsigned int enterChoice(void);
void textFile(FILE *readPtr);
void updateRecord(FILE *fPtr);
void newRecord(FILE *fPtr);
void deleteRecord(FILE *fPtr);
void searchByName(FILE *fPtr);
void transferMoney(FILE *fPtr);
void displayAccounts(FILE *fPtr);
int login(void);

int main(int argc, char *argv[])
{
    FILE *cfPtr;
    unsigned int choice;

    // Login authentication
    if (!login())
    {
        printf("Authentication Failed.\n");
        return 1;
    }

    // open file
    if ((cfPtr = fopen("credit.dat", "rb+")) == NULL)
    {
        printf("%s: File could not be opened.\n", argv[0]);
        exit(-1);
    }

    // menu loop
    while ((choice = enterChoice()) != 8)
    {
        switch (choice)
        {
        case 1:
            textFile(cfPtr);
            break;

        case 2:
            updateRecord(cfPtr);
            break;

        case 3:
            newRecord(cfPtr);
            break;

        case 4:
            deleteRecord(cfPtr);
            break;

        case 5:
            searchByName(cfPtr);
            break;

        case 6:
            transferMoney(cfPtr);
            break;

        case 7:
            displayAccounts(cfPtr);
            break;

        default:
            puts("Incorrect choice");
            break;
        }
    }

    fclose(cfPtr);
    return 0;
}

// create formatted text file
void textFile(FILE *readPtr)
{
    FILE *writePtr;
    int result;

    struct clientData client = {0, "", "", "", 0.0};

    if ((writePtr = fopen("accounts.txt", "w")) == NULL)
    {
        puts("File could not be opened.");
    }
    else
    {
        rewind(readPtr);

        fprintf(writePtr, "%-6s%-16s%-11s%-15s%10s\n",
                "Acct", "Last Name", "First Name", "Phone", "Balance");

        while (!feof(readPtr))
        {
            result = fread(&client, sizeof(struct clientData), 1, readPtr);

            if (result != 0 && client.acctNum != 0)
            {
                fprintf(writePtr, "%-6d%-16s%-11s%-15s%10.2f\n",
                        client.acctNum,
                        client.lastName,
                        client.firstName,
                        client.phone,
                        client.balance);
            }
        }

        fclose(writePtr);
    }
}

// update balance
void updateRecord(FILE *fPtr)
{
    unsigned int account;
    double transaction;

    struct clientData client = {0, "", "", "", 0.0};

    printf("Enter account to update (1 - 100): ");
    scanf("%u", &account);

    fseek(fPtr, (account - 1) * sizeof(struct clientData), SEEK_SET);

    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0)
    {
        printf("Account #%d has no information.\n", account);
    }
    else
    {
        printf("%-6d%-16s%-11s%-15s%10.2f\n\n",
               client.acctNum,
               client.lastName,
               client.firstName,
               client.phone,
               client.balance);

        printf("Enter charge (+) or payment (-): ");
        scanf("%lf", &transaction);

        client.balance += transaction;

        printf("%-6d%-16s%-11s%-15s%10.2f\n",
               client.acctNum,
               client.lastName,
               client.firstName,
               client.phone,
               client.balance);

        fseek(fPtr, -sizeof(struct clientData), SEEK_CUR);

        fwrite(&client, sizeof(struct clientData), 1, fPtr);
    }
}

// delete record
void deleteRecord(FILE *fPtr)
{
    struct clientData client;
    struct clientData blankClient = {0, "", "", "", 0.0};

    unsigned int accountNum;

    printf("Enter account number to delete (1 - 100): ");
    scanf("%u", &accountNum);

    fseek(fPtr, (accountNum - 1) * sizeof(struct clientData), SEEK_SET);

    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0)
    {
        printf("Account %d does not exist.\n", accountNum);
    }
    else
    {
        fseek(fPtr, (accountNum - 1) * sizeof(struct clientData), SEEK_SET);

        fwrite(&blankClient, sizeof(struct clientData), 1, fPtr);

        printf("Account deleted successfully.\n");
    }
}

// create new account
void newRecord(FILE *fPtr)
{
    struct clientData client = {0, "", "", "", 0.0};

    unsigned int accountNum;

    printf("Enter new account number (1 - 100): ");
    scanf("%u", &accountNum);

    if (accountNum < 1 || accountNum > 100)
    {
        printf("Invalid account number.\n");
        return;
    }

    fseek(fPtr, (accountNum - 1) * sizeof(struct clientData), SEEK_SET);

    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum != 0)
    {
        printf("Account #%d already contains information.\n",
               client.acctNum);
    }
    else
    {
        printf("Enter lastname: ");
        scanf("%14s", client.lastName);

        printf("Enter firstname: ");
        scanf("%9s", client.firstName);

        printf("Enter phone: ");
        scanf("%14s", client.phone);

        printf("Enter balance: ");
        scanf("%lf", &client.balance);

        client.acctNum = accountNum;

        fseek(fPtr,
              (client.acctNum - 1) * sizeof(struct clientData),
              SEEK_SET);

        fwrite(&client, sizeof(struct clientData), 1, fPtr);

        printf("Account created successfully.\n");
    }
}

// search account by last name
void searchByName(FILE *fPtr)
{
    char searchName[15];
    int found = 0;

    struct clientData client = {0, "", "", "", 0.0};

    printf("Enter last name to search: ");
    scanf("%14s", searchName);

    rewind(fPtr);

    printf("\n%-6s%-16s%-11s%-15s%10s\n",
           "Acct", "Last Name", "First Name", "Phone", "Balance");

    while (fread(&client, sizeof(struct clientData), 1, fPtr))
    {
        if (client.acctNum != 0 &&
            strcmp(client.lastName, searchName) == 0)
        {
            printf("%-6d%-16s%-11s%-15s%10.2f\n",
                   client.acctNum,
                   client.lastName,
                   client.firstName,
                   client.phone,
                   client.balance);

            found = 1;
        }
    }

    if (!found)
    {
        printf("No account found with last name '%s'\n",
               searchName);
    }
}

// transfer money
void transferMoney(FILE *fPtr)
{
    unsigned int fromAcc, toAcc;
    double amount;

    struct clientData fromClient = {0, "", "", "", 0.0};
    struct clientData toClient = {0, "", "", "", 0.0};

    printf("Enter sender account number: ");
    scanf("%u", &fromAcc);

    printf("Enter receiver account number: ");
    scanf("%u", &toAcc);

    printf("Enter amount to transfer: ");
    scanf("%lf", &amount);

    if (fromAcc == toAcc)
    {
        printf("Cannot transfer to same account.\n");
        return;
    }

    fseek(fPtr, (fromAcc - 1) * sizeof(struct clientData), SEEK_SET);
    fread(&fromClient, sizeof(struct clientData), 1, fPtr);

    fseek(fPtr, (toAcc - 1) * sizeof(struct clientData), SEEK_SET);
    fread(&toClient, sizeof(struct clientData), 1, fPtr);

    if (fromClient.acctNum == 0 || toClient.acctNum == 0)
    {
        printf("One or both accounts do not exist.\n");
        return;
    }

    if (fromClient.balance < amount)
    {
        printf("Insufficient balance.\n");
        return;
    }

    fromClient.balance -= amount;
    toClient.balance += amount;

    fseek(fPtr, (fromAcc - 1) * sizeof(struct clientData), SEEK_SET);
    fwrite(&fromClient, sizeof(struct clientData), 1, fPtr);

    fseek(fPtr, (toAcc - 1) * sizeof(struct clientData), SEEK_SET);
    fwrite(&toClient, sizeof(struct clientData), 1, fPtr);

    printf("Transfer successful.\n");
}

// display all accounts
void displayAccounts(FILE *fPtr)
{
    struct clientData client = {0, "", "", "", 0.0};

    rewind(fPtr);

    printf("\n%-6s%-16s%-11s%-15s%10s\n",
           "Acct", "Last Name", "First Name", "Phone", "Balance");

    while (fread(&client, sizeof(struct clientData), 1, fPtr))
    {
        if (client.acctNum != 0)
        {
            printf("%-6d%-16s%-11s%-15s%10.2f\n",
                   client.acctNum,
                   client.lastName,
                   client.firstName,
                   client.phone,
                   client.balance);
        }
    }
}

// login authentication
int login(void)
{
    char username[20];
    char password[20];

    char correctUsername[] = "user";
    char correctPassword[] = "yalzo_here";

    printf("\n===== BANK LOGIN =====\n");

    printf("Enter username: ");
    scanf("%19s", username);

    printf("Enter password: ");
    scanf("%19s", password);

    if (strcmp(username, correctUsername) == 0 &&
        strcmp(password, correctPassword) == 0)
    {
        printf("\nLogin Successful!\n");
        return 1;
    }

    return 0;
}

// menu
unsigned int enterChoice(void)
{
    unsigned int menuChoice;

    printf("\nEnter your choice\n"
           "1 - store formatted accounts file\n"
           "2 - update an account\n"
           "3 - add a new account\n"
           "4 - delete an account\n"
           "5 - search account by name\n"
           "6 - transfer money\n"
           "7 - display all accounts\n"
           "8 - end program\n? ");

    scanf("%u", &menuChoice);

    return menuChoice;
}