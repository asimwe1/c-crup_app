#include <mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void finish_with_error(MYSQL *con)
{
    fprintf(stderr, "%s\n", mysql_error(con));
    mysql_close(con);
    exit(1);
}

void student_management(MYSQL *con)
{
    printf("\nSTUDENT MANAGEMENT\n");
    printf("1. Add Student\n");
    printf("2. Edit Student\n");
    printf("3. Delete Student\n");
    printf("4. Student List\n");
    printf("Q. Quit to Main Menu\n");

    char choice;
    printf("Enter your choice: ");
    scanf(" %c", &choice);

    switch (choice)
    {
    case '1':
    {
        char first_name[50], last_name[50];
        printf("Enter student's first name: ");
        scanf("%s", first_name);
        printf("Enter student's last name: ");
        scanf("%s", last_name);

        char query[500];
        sprintf(query, "INSERT INTO students (first_name, last_name) VALUES ('%s', '%s')", first_name, last_name);

        if (mysql_query(con, query))
        {
            finish_with_error(con);
        }
        else
        {
            printf("Student added successfully.\n");
        }
        break;
    }
    case '2':
    {
        int student_id;
        char first_name[50], last_name[50];
        printf("Enter student ID to edit: ");
        scanf("%d", &student_id);
        printf("Enter student's new first name: ");
        scanf("%s", first_name);
        printf("Enter student's new last name: ");
        scanf("%s", last_name);
        // Add cases for editing and deleting students

        char query[500];
        sprintf(query, "UPDATE students SET first_name = '%s', last_name = '%s' WHERE id = %d", first_name, last_name, student_id);

        if (mysql_query(con, query))
        {
            finish_with_error(con);
        }
        else
        {
            printf("Student information updated successfully.\n");
        }
        break;
    }

    case '3': {
            int student_id;
            printf("Enter student ID to delete: ");
            scanf("%d", &student_id);

            char query[500];
            sprintf(query, "DELETE FROM students WHERE id = %d", student_id);

            if (mysql_query(con, query)) {
                finish_with_error(con);
            } else {
                printf("Student deleted successfully.\n");
            }
            break;
        }

    case '4': {
            MYSQL_RES *result;
            MYSQL_ROW row;
            if (mysql_query(con, "SELECT * FROM students")) {
                finish_with_error(con);
            } else {
                result = mysql_store_result(con);
                if (result == NULL) {
                    finish_with_error(con);
                }
                printf("\nSTUDENT LIST\n");
                printf("ID\tFirst Name\tLast Name\n");
                while ((row = mysql_fetch_row(result))) {
                    printf("%s\t%s\t%s\n", row[0], row[1], row[2]);
                }
                mysql_free_result(result);
            }
            break;
        }

    case 'Q':
    case 'q':
        printf("Returning to main menu.\n");
        break;
    default:
        printf("Invalid choice.\n");
    }
}

void parent_management(MYSQL *con)
{
    printf("\nPARENT MANAGEMENT\n");
    printf("1. Add Parent\n");
    printf("2. Edit Parent\n");
    printf("Q. Quit to Main Menu\n");

    char choice;
    printf("Enter your choice: ");
    scanf(" %c", &choice);

    switch (choice)
    {
    case '1':
{
    char first_name[50], last_name[50], address[100], relationship[50], student_last_name[50];
    printf("Enter parent's first name: ");
    scanf("%s", first_name);
    printf("Enter parent's last name: ");
    scanf("%s", last_name);
    printf("Enter parent's address: ");
    scanf("%s", address);
    printf("Enter parent's relationship with student: ");
    scanf("%s", relationship);
    printf("Enter the student's last name: ");
    scanf("%s", student_last_name);

    // Get the student_id based on the student's last name
    char query_student_id[200];
    sprintf(query_student_id, "SELECT id FROM students WHERE last_name='%s'", student_last_name);
    if (mysql_query(con, query_student_id))
    {
        finish_with_error(con);
    }
    else
    {
        MYSQL_RES *result_student_id = mysql_store_result(con);
        if (result_student_id == NULL)
        {
            finish_with_error(con);
        }
        MYSQL_ROW row_student_id = mysql_fetch_row(result_student_id);
        if (row_student_id == NULL)
        {
            printf("Student with last name '%s' not found.\n", student_last_name);
            mysql_free_result(result_student_id);
            break;
        }
        int student_id = atoi(row_student_id[0]);
        mysql_free_result(result_student_id);

        // Now, insert the parent's data into the parents table
        char query_insert_parent[500];
        sprintf(query_insert_parent, "INSERT INTO parents (first_name, last_name, address, relationship, student_id) VALUES ('%s', '%s', '%s', '%s', %d)",
                first_name, last_name, address, relationship, student_id);
        if (mysql_query(con, query_insert_parent))
        {
            finish_with_error(con);
        }
        else
        {
            printf("Parent added successfully.\n");
        }
    }
    break;
}

    // Add functionality to handle other options
    // ...
    case 'Q':
    case 'q':
        printf("Returning to main menu.\n");
        break;
    default:
        printf("Invalid choice.\n");
    }
}

int main()
{
    MYSQL *con = mysql_init(NULL);
    if (con == NULL)
    {
        fprintf(stderr, "mysql_init() failed\n");
        exit(1);
    }
    if (mysql_real_connect(con, "localhost", "root", "", "school", 0, NULL, 0) == NULL)
    {
        finish_with_error(con);
    }

    char menu_choice;
    do
    {
        printf("\n-----------------------\n");
        printf("1. Student Management\n");
        printf("2. Parent Management\n");
        printf("Q. Quit\n");
        printf("-----------------------\n");
        printf("Enter your choice: ");
        scanf(" %c", &menu_choice);

        switch (menu_choice)
        {
        case '1':
            student_management(con);
            break;
        case '2':
            parent_management(con);
            break;
        case 'Q':
        case 'q':
            printf("Exiting program.\n");
            break;
        default:
            printf("Invalid choice. Please try again.\n");
        }
    } while (menu_choice != 'Q' && menu_choice != 'q');

    mysql_close(con);
    return 0;
}
