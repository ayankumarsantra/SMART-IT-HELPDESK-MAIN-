#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct Ticket   //structure
{
    int id;
    char emp[20];
    char problem[100];
    char category[30];
    int priority;
    char status[20];
};

#define MAX 100
#define DATA_FILE "ticket_log.txt"
#define LOG_FILE "supportdata.txt"

struct Ticket tickets[MAX];

int count = 0;
char user[20];
int admin = 0;

//function decleration

void createTicket();
void viewTickets();
void searchTicket();
void updateStatus();
void loadTickets();
void saveTickets();
void setCategory(char problem[], char category[], int *priority);
void deleteTicket();
// main function
int main()
{
    int login, choice;
    loadTickets();
    while (1)
    {
        printf("          WELCOME TO IT HELP DESK\n");
        printf("--------------------------------------------\n");
        printf("1. Admin Login\n");
        printf("2. Employee Login\n");
        printf("3. Exit\n");
        printf("--------------------------------------------\n");
        printf("Enter choice: ");
        //fixed input
        if (scanf("%d", &login) != 1)
        {
            printf("\nInvalid input! Please enter a number.\n");

            /* Clear wrong input */
            while (getchar() != '\n');

            continue;
        }

  //exit
        if (login == 3)
        {
            printf("\nProgram closed.\n");
            break;
        }
        //admin login
        if (login == 1)
        {
            char pass[20];
            printf("Enter Admin Password: ");
            scanf("%19s", pass);
            if (strcmp(pass, "admin123") == 0)
            {
                admin = 1;
                strcpy(user, "Admin");
                printf("\nAdmin login successful.\n");
            }
            else
            {
                printf("\nWrong password.\n");
                continue;
            }
        }
             //employee login
        else if (login == 2)
        {
            admin = 0;
            printf("Enter Employee ID: ");
            scanf("%19s", user);
            printf("\nEmployee login successful.\n");
        }
        else
        {
            printf("\nWrong choice. Please select 1, 2 or 3.\n");
            continue;
        }
        //dashboard
        while (1)
        {
            printf("\n----------------------------------------\n");
            printf("          %s DASHBOARD\n", user);
            printf("------------------------------------------\n");

            printf("1. Create Ticket\n");
            printf("2. View Tickets\n");
            printf("3. Search Ticket\n");
            printf("4. Update Status\n");
            printf("5. delete resolve ticket\n");
            printf("6. Logout\n");
            printf("--------------------------------------------\n");

            printf("Enter choice: ");
        //fixed input
            if (scanf("%d", &choice) != 1)
            {
                printf("\nInvalid input! Please enter a number.\n");
                //clear wrong input
                while (getchar() != '\n');
                continue;
            }
            switch (choice)
            {
                case 1:
                    createTicket();
                    break;

                case 2:
                    viewTickets();
                    break;

                case 3:
                    searchTicket();
                    break;

                case 4:
                    updateStatus();
                    break;

                    case 5:
                    deleteTicket();
                     break;

                     case 6:
                      printf("\nLogged out.\n");
                       goto logout;

                default:
                    printf("\nWrong choice! Select 1 to 5.\n");
            }
        }
logout:
        ;
    }
    return 0;
}
//create ticket
void createTicket()
{
    struct Ticket t;
    FILE *file;
    if (admin)
    {
        printf("\nAdmin cannot create a ticket.\n");
        return;
    }
    if (count >= MAX)
    {
        printf("\nTicket limit reached.\n");
        return;
    }
   //generate ticket id 
    if (count == 0)
    {
        t.id = 1001;//you may put any id no based on your choice
    }
    else
    {
        t.id = tickets[count - 1].id + 1;
    }

    strcpy(t.emp, user);
    //clear new line from previous scanf
    while (getchar() != '\n');
    printf("\nEnter Problem: ");
    if (fgets(t.problem, sizeof(t.problem), stdin) == NULL)//fgets for read the space which you used for describe your type of problem
    {
        printf("\nError reading problem.\n");
        return;
    }
//remove newline
    t.problem[strcspn(t.problem, "\n")] = '\0';//this line find \n and replace with \0;
    // Automatic Category and Priority 
    setCategory(t.problem, t.category, &t.priority);
    strcpy(t.status, "Open");//set initialstatus
// Store Ticket
    tickets[count] = t;//copies the temporary ticket into the main ticket array
    count++;
    // Save data 
    saveTickets();
    // Save log 
    file = fopen(LOG_FILE, "a");
    if (file != NULL)
    {
        fprintf(file,"ID: %d | Employee: %s | Problem: %s | " "Category: %s | Priority: P%d | Status: %s\n",
                t.id,
                t.emp,
                t.problem,
                t.category,
                t.priority,
                t.status);

        fclose(file);

    }
    printf("\nTicket created successfully.\n");
    printf("Ticket ID : %d\n", t.id);
    printf("Category  : %s\n", t.category);
    printf("Priority  : P%d\n", t.priority);
}

//categorise and priorityised
void setCategory(char problem[], char category[], int *priority)
{
    strcpy(category, "General");
    *priority = 3;

    if (strstr(problem, "server") ||
        strstr(problem, "crash") ||
        strstr(problem, "down"))
    {
        strcpy(category, "Infrastructure");
        *priority = 1;
    }

    else if (strstr(problem, "network") ||
             strstr(problem, "wifi") ||
             strstr(problem, "internet"))
    {
        strcpy(category, "Network");
        *priority = 1;
    }

    else if (strstr(problem, "password") ||
             strstr(problem, "login"))
    {
        strcpy(category, "Security");
        *priority = 2;
    }
}
// view ticket                                      
void viewTickets()
{
    int i, p;
    int found = 0;
    if (count == 0)
    {
        printf("\nNo tickets available.\n");
        return;
    }
    printf("\n");
    printf("------------------------------------------------------------------------------------\n");

    printf("%-6s %-12s %-16s %-10s %-14s %s\n",// %-6s,%-12s for control spacing
           "ID",
           "Employee",
           "Category",
           "Priority",
           "Status",
           "Problem");

    printf("---------------------------------------------------------------------------------------\n");

    //Show P1 first, then P2, then P3 

    for (p = 1; p <= 3; p++)
    {
        for (i = 0; i < count; i++)//high priority ticket display first
        {
            if (tickets[i].priority == p)
            {
                if (admin || strcmp(tickets[i].emp, user) == 0)//admin vs employee viewing
                {
                    printf("%-6d %-12s %-16s P%-9d %-14s %s\n",
                           tickets[i].id,
                           tickets[i].emp,
                           tickets[i].category,
                           tickets[i].priority,
                           tickets[i].status,
                           tickets[i].problem);

                    found = 1;
                }
            }
        }
    }

    printf("***********************************************************************\n");
    if (found == 0)
    {
        printf("No tickets found.\n");
    }
}
// search ticket                                     
void searchTicket()
{
    int id;
    int i;

    if (count == 0)
    {
        printf("\nNo tickets available.\n");
        return;
    }
    printf("\nEnter Ticket ID: ");
    //fixed input
    if (scanf("%d", &id) != 1)
    {
        printf("\nInvalid Ticket ID. Please enter numbers only.\n");

        while (getchar() != '\n');

        return;
    }

    for (i = 0; i < count; i++)//check every ticket
    {
        if (tickets[i].id == id)
        {
            // Employee can see only own ticket 
            if (admin == 0 && strcmp(tickets[i].emp, user) != 0)//employee security
            {
                printf("\nYou cannot view this ticket.\n");
                return;
            }

            printf("\n******************************\n");

            printf("Ticket ID : %d\n",
                   tickets[i].id);

            printf("Employee  : %s\n",
                   tickets[i].emp);

            printf("Problem   : %s\n",
                   tickets[i].problem);

            printf("Category  : %s\n",
                   tickets[i].category);

            printf("Priority  : P%d\n",
                   tickets[i].priority);

            printf("Status    : %s\n",
                   tickets[i].status);

            printf("***********************************\n");

            return;
        }
    }

    printf("\nTicket not found.\n");
}

//update ticket status                           
void updateStatus()
{
    int id;
    int choice;
    int i;
    if (admin == 0)
    {
        printf("\nOnly Admin can update status.\n");
        return;
    }

    if (count == 0)
    {
        printf("\nNo tickets available.\n");
        return;
    }

    printf("\nEnter Ticket ID: ");

    //fixed input

    if (scanf("%d", &id) != 1)
    {
        printf("\nInvalid Ticket ID.\n");

        while (getchar() != '\n');

        return;
    }

    for (i = 0; i < count; i++)
    {
        if (tickets[i].id == id)
        {
            printf("\nCurrent Status: %s\n",
                   tickets[i].status);

            printf("1. In-Progress\n");
            printf("2. Resolved\n");

            printf("Enter choice: ");

            //fixed input

            if (scanf("%d", &choice) != 1)
            {
                printf("\nInvalid choice.\n");
                while (getchar() != '\n');
                return;
            }

            if (choice == 1)
            {
                strcpy(tickets[i].status,
                       "In-Progress");
            }

            else if (choice == 2)
            {
                strcpy(tickets[i].status,
                       "Resolved");
            }

            else
            {
                printf("\nWrong choice.\n");
                return;
            }

            saveTickets();

            printf("\nStatus updated successfully.\n");

            return;
        }
    }

    printf("\nTicket not found.\n");
}
//delete function
void deleteTicket()
{
    int id;
    int i, j;

    if (admin == 0)
    {
        printf("\nOnly Admin can delete tickets.\n");
        return;
    }

    if (count == 0)
    {
        printf("\nNo tickets available.\n");
        return;
    }

    printf("\nEnter Ticket ID to delete: ");

    if (scanf("%d", &id) != 1)
    {
        printf("\nInvalid Ticket ID.\n");
        while (getchar() != '\n');
        return;
    }

    for (i = 0; i < count; i++)
    {
        if (tickets[i].id == id)
        {
            /* Only Resolved tickets can be deleted */

            if (strcmp(tickets[i].status, "Resolved") != 0)
            {
                printf("\nTicket is not resolved.\n");
                printf("Only Resolved tickets can be deleted.\n");
                return;
            }

            /* Shift remaining tickets */

            for (j = i; j < count - 1; j++)
            {
                tickets[j] = tickets[j + 1];
            }

            count--;

            saveTickets();

            printf("\nResolved ticket deleted successfully.\n");
            return;
        }
    }

    printf("\nTicket not found.\n");
}
//load ticket
void loadTickets()
{
    FILE *file;

    file = fopen(DATA_FILE, "r");//open in read mode 

    if (file == NULL)
    {
        return; //file does not exist yet
    }

    count = 0;

    while (count < MAX &&
           fscanf(file,
                  "%d,%[^,],%[^,],%[^,],%d,%[^\n]\n",
                  &tickets[count].id,
                  tickets[count].emp,
                  tickets[count].category,
                  tickets[count].status,
                  &tickets[count].priority,
                  tickets[count].problem) == 6)//%[^,] means read charecter untill comma(,) is found
    {
        count++;
    }

    fclose(file);
}
// save ticket                                      
void saveTickets()
{
    FILE *file;
    int i;

    file = fopen(DATA_FILE, "w");//write / overwrite mode

    if (file == NULL)
    {
        printf("\nCannot open ticket_log.txt\n");
        return;
    }

    for (i = 0; i < count; i++)
    {
        fprintf(file,
                "%d,%s,%s,%s,%d,%s\n",
                tickets[i].id,
                tickets[i].emp,
                tickets[i].category,
                tickets[i].status,
                tickets[i].priority,
                tickets[i].problem);
    }

    fclose(file);//close the file 
}