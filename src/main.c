#include "DSSV_bin.h"

int main()
{
   StudentPtr studentList = NULL;
   IndexEntry *indexTable = (IndexEntry *)malloc(MAX * INDEX_SIZE);

   short option;
   do
   {
      system("cls");
      textColor(11);
      printf("===============MENU===============\n");
      textColor(7);
      printf("1, Enter student information\n");
      printf("2, View student list\n");
      printf("3, Lookup student information\n");
      printf("4, Filter out failing students\n");
      printf("5, Update student information\n");
      printf("6, Sort student list\n");
      printf("7, Delete student by ID\n");
      printf("8, Save information to text file\n");
      printf("9, Save information to binary file\n");
      textColor(14);
      printf("0, Exit program\n");
      textColor(11);
      printf("Please choose an option (0 - 9): ");
      getMenuOption(&option, 9);
      textColor(7);

      if (option == 0)
      {
         textColor(14);
         printf("\n\nExiting program\n");
         system("pause");
         break;
      }
      system("cls");

      switch (option)
      {
      case 9:
      {
         if (studentList == NULL)
         {
            textColor(12);
            printf("No student information entered");
         }
         else
         {
            getchar();
            writeToBinaryFile(studentList);
         }
      }
      break;

      case 8:
      {
         if (studentList == NULL)
         {
            textColor(12);
            printf("No student information entered");
         }
         else
         {
            getchar();
            writeToTextFile(studentList);
         }
      }
      break;

      case 5:
      {
         if (studentList == NULL)
         {
            textColor(12);
            printf("No student information entered");
         }
         else
         {
            getchar();
            updateStudentInfo(studentList, indexTable);
         }
      }
      break;

      case 6:
      {
         if (studentList == NULL)
         {
            textColor(12);
            printf("No student information entered");
         }
         else
         {
            getchar();
            sortStudentList(studentList);
         }
      }
      break;

      case 7:
      {
         if (studentList == NULL)
         {
            textColor(12);
            printf("No student information entered");
         }
         else
         {
            getchar();
            deleteStudent(&studentList, indexTable);
         }
      }
      break;

      case 4:
      {
         if (studentList == NULL)
         {
            textColor(12);
            printf("No student information entered");
         }
         else
            filterFailingStudents(&studentList, indexTable);
      }
      break;

      case 3:
      {
         if (studentList == NULL)
         {
            textColor(12);
            printf("No student information entered");
         }
         else
         {
            getchar();
            searchStudent(studentList);
         }
      }
      break;

      case 2:
      {
         if (studentList == NULL)
         {
            textColor(12);
            printf("No student information entered");
         }
         else
         {
            textColor(11);
            printColumnHeaders();
            displayAllStudents(studentList);
         }
      }
      break;

      case 1:
      {
         short select;
         do
         {
            system("cls");
            textColor(11);
            printf("ENTER STUDENT INFORMATION\n");
            textColor(7);
            printf("1, Enter information from keyboard\n");
            printf("2, Enter information from file\n");
            printf("0, Return to main menu\n");
            textColor(11);
            printf("Please choose an option (0 - 2): ");
            getMenuOption(&select, 2);
            textColor(7);

            if (select == 0)
            {
               textColor(14);
               printf("\n\nReturning to main menu");
               break;
            }
            switch (select)
            {
            case 1:
            {
               system("cls");
               getchar();
               inputFromKeyboard(&studentList, indexTable);
            }
            break;
            case 2:
            {
               getchar();
               loadFromFile(&studentList, indexTable);
               getch();
            }
            break;
            }
         } while (1);
      }
      break;
      }
      getch();
   } while (1);
   free(indexTable);
   freeMemory(&studentList);
   return 0;
}