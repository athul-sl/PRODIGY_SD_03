#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LEN 100
#define MAX_PHONE_LEN 20
#define MAX_EMAIL_LEN 100
#define CONTACT_FILE "contacts.dat"

typedef struct {
    char name[MAX_NAME_LEN];
    char phone[MAX_PHONE_LEN];
    char email[MAX_EMAIL_LEN];
} Contact;

void add_contact();
void view_contacts();
void edit_contact();
void delete_contact();
void load_contacts(Contact **contacts, int *count);
void save_contacts(Contact *contacts, int count);
void clear_buffer();

int main() {
    int choice;

    while (1) {
        printf("\nEnter option\n");
        printf("1. Add New Contact\n");
        printf("2. View Contacts\n");
        printf("3. Edit Contact\n");
        printf("4. Delete Contact\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        clear_buffer();

        switch (choice) {
            case 1:
                add_contact();
                break;
            case 2:
                view_contacts();
                break;
            case 3:
                edit_contact();
                break;
            case 4:
                delete_contact();
                break;
            case 5:
                printf("Exiting...\n");
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}

void load_contacts(Contact **contacts, int *count) {
    FILE *file = fopen(CONTACT_FILE, "r");
    if (!file) {
        *contacts = NULL;
        *count = 0;
        return;
    }

    fread(count, sizeof(int), 1, file);
    *contacts = malloc(*count * sizeof(Contact));
    fread(*contacts, sizeof(Contact), *count, file);
    fclose(file);
}

void save_contacts(Contact *contacts, int count) {
    FILE *file = fopen(CONTACT_FILE, "w");
    if (!file) {
        perror("Failed to open file");
        return;
    }

    fwrite(&count, sizeof(int), 1, file);
    fwrite(contacts, sizeof(Contact), count, file);
    fclose(file);
}

void add_contact() {
    Contact *contacts = NULL;
    int count = 0;

    load_contacts(&contacts, &count);

    contacts = realloc(contacts, (count + 1) * sizeof(Contact));
    if (!contacts) {
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }

    printf("Enter contact name: ");
    fgets(contacts[count].name, MAX_NAME_LEN, stdin);
    strtok(contacts[count].name, "\n");

    printf("Enter contact phone: ");
    fgets(contacts[count].phone, MAX_PHONE_LEN, stdin);
    strtok(contacts[count].phone, "\n");

    printf("Enter contact email: ");
    fgets(contacts[count].email, MAX_EMAIL_LEN, stdin);
    strtok(contacts[count].email, "\n");

    count++;
    save_contacts(contacts, count);
    free(contacts);
    printf("Contact added successfully.\n");
}

void view_contacts() {
    Contact *contacts = NULL;
    int count = 0;

    load_contacts(&contacts, &count);

    if (count == 0) {
        printf("No contacts found.\n");
    } else {
        for (int i = 0; i < count; i++) {
            printf("%d. Name: %s, Phone: %s, Email: %s\n", i + 1, contacts[i].name, contacts[i].phone, contacts[i].email);
        }
    }
    free(contacts);
}

void edit_contact() {
    Contact *contacts = NULL;
    int count = 0;

    load_contacts(&contacts, &count);
    view_contacts();

    if (count > 0) {
        int index;
        printf("Enter the number of the contact to edit: ");
        scanf("%d", &index);
        clear_buffer();
        index--;

        if (index >= 0 && index < count) {
            char buffer[MAX_NAME_LEN];
            
            printf("Enter new name (leave empty to keep current): ");
            fgets(buffer, MAX_NAME_LEN, stdin);
            if (buffer[0] != '\n') {
                strtok(buffer, "\n");
                strncpy(contacts[index].name, buffer, MAX_NAME_LEN - 1);
                contacts[index].name[MAX_NAME_LEN - 1] = '\0';
            }

            printf("Enter new phone (leave empty to keep current): ");
            fgets(buffer, MAX_PHONE_LEN, stdin);
            if (buffer[0] != '\n') {
                strtok(buffer, "\n");
                strncpy(contacts[index].phone, buffer, MAX_PHONE_LEN - 1);
                contacts[index].phone[MAX_PHONE_LEN - 1] = '\0';
            }

            printf("Enter new email (leave empty to keep current): ");
            fgets(buffer, MAX_EMAIL_LEN, stdin);
            if (buffer[0] != '\n') {
                strtok(buffer, "\n");
                strncpy(contacts[index].email, buffer, MAX_EMAIL_LEN - 1);
                contacts[index].email[MAX_EMAIL_LEN - 1] = '\0';
            }

            save_contacts(contacts, count);
            printf("Contact updated successfully.\n");
        } else {
            printf("Invalid contact number.\n");
        }
    }
    free(contacts);
}

void delete_contact() {
    Contact *contacts = NULL;
    int count = 0;

    load_contacts(&contacts, &count);
    view_contacts();

    if (count > 0) {
        int index;
        printf("Enter the number of the contact to delete: ");
        scanf("%d", &index);
        clear_buffer();
        index--;

        if (index >= 0 && index < count) {
            for (int i = index; i < count - 1; i++) {
                contacts[i] = contacts[i + 1];
            }
            contacts = realloc(contacts, (count - 1) * sizeof(Contact));
            count--;
            save_contacts(contacts, count);
            printf("Contact deleted successfully.\n");
        } else {
            printf("Invalid contact number.\n");
        }
    }
    free(contacts);
}

void clear_buffer() {
    int c;
    while ((c = getchar()) != EOF && c != '\n');
}
