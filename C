#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENAME "customers.txt"
#define MAX_NAME_SIZE 50
#define MAX_PASSWORD_SIZE 50

typedef struct {
    int id;
    char name[MAX_NAME_SIZE];
    char password[MAX_PASSWORD_SIZE];
    double balance;
} customer;

void display_menu() {
    printf("* Bank Application *\n\n");
    printf("1. Add Customer\n");
    printf("2. View All Customers\n");
    printf("3. Find Customer\n");
    printf("4. Delete Customer\n");
    printf("5. Update Customer Password\n");
    printf("6. Update Customer Balance\n");
    printf("7. Withdraw Money\n");
    printf("8. Deposit Money\n");
    printf("9. Exit\n");
}

void update_balance(int id, double amount) {
    FILE *fp = fopen(FILENAME, "rb+");
    if (fp == NULL) {
        perror("Error opening file");
        return;
    }

    customer current_customer;
    int found = 0;

    while (fread(&current_customer, sizeof(customer), 1, fp) == 1) {
        if (id == current_customer.id) {
            found = 1;
            current_customer.balance += amount;
            fseek(fp, -sizeof(customer), SEEK_CUR);
            if (fwrite(&current_customer, sizeof(customer), 1, fp) != 1) {
                perror("Error writing to file");
            }
            break;
        }
    }

    if (!found) {
        printf("Customer not found!\n");
    }

    fclose(fp);
}

void add_customer() {
    FILE *fp = fopen(FILENAME, "ab");
    if (fp == NULL) {
        perror("Error opening file");
        return;
    }

    customer new_customer;
    printf("Enter customer ID: ");
    scanf("%d", &new_customer.id);

    printf("Enter customer name: ");
    scanf("%s", new_customer.name);

    printf("Enter customer password: ");
    scanf("%s", new_customer.password);

    new_customer.balance = 0.0;

    if (fwrite(&new_customer, sizeof(customer), 1, fp) != 1) {
        perror("Error writing to file");
    }

    fclose(fp);
}

void view_all_customers() {
    FILE *fp = fopen(FILENAME, "rb");
    if (fp == NULL) {
        perror("Error opening file");
        return;
    }

    customer current_customer;

    while (fread(&current_customer, sizeof(customer), 1, fp) == 1) {
        printf("ID: %d, Name: %s, Balance: %.2f\n", current_customer.id, current_customer.name, current_customer.balance);
    }

    fclose(fp);
}

void find_customer() {
    int id;
    printf("Enter customer ID to find: ");
    scanf("%d", &id);

    FILE *fp = fopen(FILENAME, "rb");
    if (fp == NULL) {
        perror("Error opening file");
        return;
    }

    customer current_customer;
    int found = 0;

    while (fread(&current_customer, sizeof(customer), 1, fp) == 1) {
        if (id == current_customer.id) {
            found = 1;
            printf("ID: %d, Name: %s, Balance: %.2f\n", current_customer.id, current_customer.name, current_customer.balance);
            break;
        }
    }

    if (!found) {
        printf("Customer not found!\n");
    }

    fclose(fp);
}

void delete_customer() {
    int id;
    printf("Enter customer ID to delete: ");
    scanf("%d", &id);

    FILE *fp = fopen(FILENAME, "rb");
    if (fp == NULL) {
        perror("Error opening file");
        return;
    }

    FILE *temp_fp = fopen("temp.txt", "wb");
    if (temp_fp == NULL) {
        perror("Error creating temporary file");
        fclose(fp);
        return;
    }

    customer current_customer;
    int found = 0;

    while (fread(&current_customer, sizeof(customer), 1, fp) == 1) {
        if (id == current_customer.id) {
            found = 1;
        } else {
            if (fwrite(&current_customer, sizeof(customer), 1, temp_fp) != 1) {
                perror("Error writing to temporary file");
            }
        }
    }

    fclose(fp);
    fclose(temp_fp);

    if (!found) {
        printf("Customer not found!\n");
        remove("temp.txt");
    } else {
        remove(FILENAME);
        rename("temp.txt", FILENAME);
        printf("Customer deleted successfully!\n");
    }
}

void update_customer_password() {
    int id;
    char new_password[MAX_PASSWORD_SIZE];
    printf("Enter customer ID to update password: ");
    scanf("%d", &id);

    printf("Enter new password: ");
    scanf("%s", new_password);

    FILE *fp = fopen(FILENAME, "rb+");
    if (fp == NULL) {
        perror("Error opening file");
        return;
    }

    customer current_customer;
    int found = 0;

    while (fread(&current_customer, sizeof(customer), 1, fp) == 1) {
        if (id == current_customer.id) {
            found = 1;
            fseek(fp, -sizeof(customer), SEEK_CUR);
            strncpy(current_customer.password, new_password, MAX_PASSWORD_SIZE - 1);
            current_customer.password[MAX_PASSWORD_SIZE - 1] = '\0'; // Ensure null-termination
            if (fwrite(&current_customer, sizeof(customer), 1, fp) != 1) {
                perror("Error writing to file");
            }
            break;
        }
    }

    fclose(fp);

    if (!found) {
        printf("Customer not found!\n");
    } else {
        printf("Password updated successfully!\n");
    }
}

void withdraw_money() {
    int id;
    double amount;
    printf("Customer ID: ");
    scanf("%d", &id);
    printf("Withdrawal amount: ");
    scanf("%lf", &amount);
    update_balance(id, -amount);
}

void deposit_money() {
    int id;
    double amount;
    printf("Customer ID: ");
    scanf("%d", &id);
    printf("Deposit amount: ");
    scanf("%lf", &amount);
    update_balance(id, amount);
}

int main() {
    int choice;

    do {
        display_menu();
        printf("Enter your choice (1-9): ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                add_customer();
                break;
            case 2:
                view_all_customers();
                break;
            case 3:
                find_customer();
                break;
            case 4:
                delete_customer();
                break;
            case 5:
                update_customer_password();
                break;
            case 6:
                // Update Customer Balance
                // Implement this function
                break;
            case 7:
                withdraw_money();
                break;
            case 8:
                deposit_money();
                break;
            case 9:
                printf("Exiting program. Goodbye!\n");
                break;
            default:
                printf("Invalid choice. Please enter a number between 1 and 9.\n");
        }
    } while (choice != 9);

    return 0;
}
