#include <stdio.h>
#include <ctype.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>//works only on windows operating system
#define MAX 1000

char *stack;     //stack used for storing text characters
char *undoStack; //stack used to store undone characters
char *redostack; //stack used to store redone characters
int top = 0;
int undoTop = 0;
int redotop = 0;
int count = 0;
char c, fname[50];

void setConsoleColor(int);
void Text_pad();
void import_file();
void show_menu();
void handleBackspace();

int main()
{
    // Allocate initial memory for stacks
    stack = (char *)malloc(MAX * sizeof(char));
    undoStack = (char *)malloc(MAX * sizeof(char));
    redostack = (char *)malloc(MAX * sizeof(char));

    if (!stack || !undoStack || !redostack)
    {
        printf("Memory allocation failed\n");
        return 1;
    }

    show_menu();
    free(stack);   //free is used to deallocate the memory allocated using malloc
    free(undoStack);
    free(redostack);
    return 0;
}

void setConsoleColor(int color)   //function is used to apply a new colour theme to the text in terminal
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

void show_menu()
{
    system("cls"); //used to clear the terminal window
    char ch;
    printf("\n\t\t\t\t\tTEXT EDITOR\n\n\n");
    printf("\t\t1. PRESS S TO START TYPING\n\n\t\t2. PRESS R TO IMPORT A FILE AND EDIT\n\n\t\t3. PRESS E TO EXIT\n\n");
    printf("\n\t\tEnter your choice: ");
    scanf(" %c", &ch);
    switch (ch)
    {
        case 's':
        case 'S':
            Text_pad();//function to invoke textpad
            break;
        case 'r':
        case 'R':
            import_file();//function to import a textfile from directory to our program
            break;
        case 'e':
        case 'E':
            exit(1);
        default:
            show_menu();//by default,calls the menu
    }
}

void handleBackspace() //special function to handle backspaces
{
    if (top > 0)
        {
        if (stack[top - 1] == '\n')
            {
            int i = top - 2;
            printf("\b \b");  // Move cursor back and remove the newline
            printf("\033[A");  // ANSI escape code to move cursor up one line
            while (i >= 0 && stack[i] != '\n')
            {
                i--;
                printf("\033[C");  // ANSI escape code to move cursor right
            }
        }
        else
            {
            printf("\b \b");  // Move cursor back and remove character
            }
        top--;
        undoStack[undoTop++] = stack[top];
       }
}

void Text_pad()
{
    system("cls");
    setConsoleColor(9);
    printf("\n\t\t\t\t\tTEXT EDITOR\n\nPRESS CTRL+Z TO UNDO\nPRESS CTRL+R TO REDO\nPRESS CTRL+S TO SAVE FILE\nPRESS CTRL+N TO SAVE AS NEW FILE\nPRESS CTRL+E TO EXIT\n\nSTART TYPING\n\n");

    for (int i = 0; i < top; i++)
        {
            putchar(stack[i]);
        }

    while (1)
    {
        c = getch();  // Read a single character

        if (c == 5) {  // ASCII value of Ctrl+E is 5
            break;
        } else if (c == 8) {  // ASCII value of backspace is 8
            handleBackspace();
        } else if (c == 26) {  // ASCII value of Ctrl+Z is 26
            if (undoTop > 0) {
                stack[top++] = undoStack[--undoTop];
                if (stack[top - 1] == '\n') {
                    printf("\n");
                } else {
                    printf("%c", stack[top - 1]);
                }
            }
        } else if (c == 18) {  // ASCII value of Ctrl+R is 18
            handleBackspace();
        } else if (c == 13) {  // ASCII value of Enter is 13
            stack[top++] = '\n';
            printf("\n");
        } else if (c == 19) {  // ASCII value of Ctrl+S is 19
            // Save the file
            FILE *file = fopen(fname, "w");//used to perform file operations in c
            if (file == NULL) {
                printf("Cannot open file for saving\n");
            } else {
                for (int i = 0; i < top; i++)
                    {
                    fputc(stack[i], file);
                    }
                fclose(file);
                printf("\nFile saved as '%s'\n", fname);
            }
        } else if (c == 14) {  // ASCII value of Ctrl+N is 14
            char new_filename[100];
            printf("\nEnter the new filename to save as: ");
            scanf("%s", new_filename);

            FILE *file = fopen(new_filename, "w");
            if (file == NULL) {
                printf("Cannot open file for saving\n");
            } else {
                for (int i = 0; i < top; i++) {
                    fputc(stack[i], file);
                }
                fclose(file);
                printf("\nFile saved as '%s'\n", new_filename);
            }
        }
        else
        {
            stack[top++] = c;
            printf("%c", c);
            undoTop = 0;
        }
    }

    printf("\nDo you want to go back (y/n): ");
    // Clearing input buffer before reading a new character
    while ((c = getchar()) != '\n' && c != EOF) { }
    c = getchar();//READ A SINGLE CHARACTER

    if (c == 'y' || c == 'Y')
        {
        show_menu();
    }
    else
        {
        printf("\nExiting...\n");
        exit(0);
        }
}


void import_file()
{
    system("cls");
    FILE *file;
    char filename[100], ch;
    printf("\tEnter the filename to be opened: ");
    scanf("%s", filename);
    file = fopen(filename, "r");
    if (file == NULL) {
        printf("\tCannot open file \n");//empty file
        exit(0);
    }
    strcpy(fname, filename);
    system("cls");
    setConsoleColor(9);
    printf("\tContents of the file %s:\n\n", filename);
    top = 0;
    while ((ch = fgetc(file)) != EOF) {  //checking the end of the file
        stack[top++] = ch;
    }
    fclose(file);

    printf("\n\nEditing mode:\n");
    Text_pad();

    file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening file for saving!\n");
        exit(0);
    }

    for (int i = 0; i < top; i++) {
        fputc(stack[i], file);
    }

    fclose(file);
    printf("\nChanges saved to %s\n", filename);

    // Clear the input buffer
    while ((c = getchar()) != '\n' && c != EOF) {}

    printf("\nDo you want to go back(y/n): ");
    c = getchar();
    if (c == 'y' || c == 'Y') {
        show_menu();
    } else {
        printf("\nExiting...\n");
        exit(0);
    }
}
