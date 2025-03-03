#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <stdbool.h>

#define ID_LENGTH 15
#define NAME_LENGTH 25
#define SCORE_LENGTH 4

struct Student;
typedef struct Student *StudentPtr;

struct Student
{
   char studentId[ID_LENGTH];
   char fullName[NAME_LENGTH];
   float score;
   StudentPtr next;
};

#define STUDENT_SIZE sizeof(struct Student)

struct Index
{
   char studentId[ID_LENGTH];
   int idx;
};

typedef struct Index IndexEntry;
#define INDEX_SIZE sizeof(IndexEntry)
#define MAX_PATH 100

char filePath[MAX_PATH], indexFilePath[MAX_PATH], tempFilePath[MAX_PATH];
int cursorX = 0, cursorY = 1, recordCount = 0;
bool isDataLoaded = false, isTempFile = true;
StudentPtr currentPtr = NULL;

void gotoxy(int x, int y)
{
   COORD position;
   position.X = x;
   position.Y = y;
   SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
}

void textColor(int colorCode)
{
   SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colorCode);
}

void getMenuOption(short *option, short maxOption)
{
   do
   {
      scanf("%hi", option);
      if (*option >= 0 && *option <= maxOption)
         break;
      textColor(14);
      printf("Enter again: ");
   } while (1);
}

void printColumnHeaders()
{
   printf("%-15s%-25s%-4s", "Student ID", "Full Name", "Score");
}

void displayStudent(StudentPtr student)
{
   textColor(7);
   printf("\n%-15s%-25s%-4.1f", student->studentId, student->fullName, student->score);
}

void removeNewline(char *str)
{
   int length = strlen(str);
   if (str[length - 1] == '\n')
      str[length - 1] = '\0';
}

void freeMemory(StudentPtr *listHead)
{
   StudentPtr tempPtr = NULL;
   while (*listHead != NULL)
   {
      tempPtr = *listHead;
      *listHead = (*listHead)->next;
      free(tempPtr);
   }
}

void sortIndexTable(IndexEntry *indexTable)
{
   bool isSorted;
   IndexEntry tempIndex;
   for (int i = 0; i < recordCount; ++i)
   {
      isSorted = true;
      for (int j = 0; j < recordCount - i - 1; ++j)
         if (stricmp(indexTable[j].studentId, indexTable[j + 1].studentId) > 0)
         {
            tempIndex = indexTable[j];
            indexTable[j] = indexTable[j + 1];
            indexTable[j + 1] = tempIndex;
            isSorted = false;
         }
      if (isSorted)
         break;
   }
}

void writeToTextFile(StudentPtr listHead)
{
   char textFilePath[MAX_PATH];
   printf("Enter the text file name to save information: ");
   fgets(textFilePath, MAX_PATH, stdin);
   removeNewline(textFilePath);
   FILE *filePtr = fopen(textFilePath, "w");
   fprintf(filePtr, "%-15s%-25s%-4s", "Student ID", "Full Name", "Score");
   StudentPtr tempPtr = listHead;
   while (isalnum(tempPtr->studentId[0]) && tempPtr != NULL)
   {
      fprintf(filePtr, "\n%-15s%-25s%-4.1f", tempPtr->studentId, tempPtr->fullName, tempPtr->score);
      tempPtr = tempPtr->next;
   }
   fclose(filePtr);
   textColor(14);
   printf("\nInformation saved successfully");
}

void writeToBinaryFile(StudentPtr listHead)
{
   char binaryFilePath[MAX_PATH];
   printf("Enter the binary file name to save information: ");
   fgets(binaryFilePath, MAX_PATH, stdin);
   removeNewline(binaryFilePath);
   FILE *filePtr = fopen(binaryFilePath, "wb");
   StudentPtr tempPtr = listHead;
   while (isalnum(tempPtr->studentId[0]) && tempPtr != NULL)
   {
      fwrite(tempPtr, STUDENT_SIZE, 1, filePtr);
      tempPtr = tempPtr->next;
   }
   fclose(filePtr);
   textColor(14);
   printf("\nInformation saved successfully");
}

int binarySearch(IndexEntry *indexTable, char *studentId, int *mid)
{
   int left = 0, right = recordCount - 1, compareResult;
   while (left <= right)
   {
      *mid = (left + right) / 2;
      compareResult = stricmp(indexTable[*mid].studentId, studentId);
      if (compareResult == 0)
         return indexTable[*mid].idx;
      else if (compareResult > 0)
         right = *mid - 1;
      else
         left = *mid + 1;
   }
   return -1;
}

void inputFromKeyboard(StudentPtr *listHead, IndexEntry *indexTable)
{
   int indexCounter = 0;
   int fileCompare = 1;
   if (isTempFile)
   {
      FILE *filePtr;
      printf("Enter the binary file name to save information: ");
      fgets(filePath, MAX_PATH, stdin);
      removeNewline(filePath);
      fileCompare = stricmp(filePath, tempFilePath);
      if (fileCompare)
      {
         filePtr = fopen(filePath, "wb");
         fclose(filePtr);
         recordCount = 0;
      }
      cursorY = 2;
   }
   else
      cursorY = 1;
   bool isDuplicate;
   StudentPtr newStudent, tempPtr, prevPtr;
   tempPtr = (StudentPtr)malloc(STUDENT_SIZE);
   if (isDataLoaded && fileCompare)
      freeMemory(listHead);
   textColor(11);
   printColumnHeaders();
   do
   {
      newStudent = (StudentPtr)malloc(STUDENT_SIZE);
      do
      {
         isDuplicate = false;
         textColor(7);
         gotoxy(cursorX, cursorY);
         fgets(newStudent->studentId, ID_LENGTH, stdin);
         removeNewline(newStudent->studentId);
         if (newStudent->studentId[0] == '\0')
         {
            textColor(14);
            printf("Input stopped%30s", " ");
            gotoxy(cursorX, cursorY + 1);
            textColor(7);
            sortIndexTable(indexTable);
            strcpy(indexFilePath, filePath);
            strcpy(strstr(indexFilePath, ".bin"), "_index.bin");
            FILE *indexFilePtr = fopen(indexFilePath, "wb");
            for (int i = 0; i < recordCount; ++i)
               fwrite(indexTable + i, INDEX_SIZE, 1, indexFilePtr);
            fclose(indexFilePtr);
            free(tempPtr);
            free(newStudent);
            if (!fileCompare)
               isTempFile = true;
            else
               isTempFile = isDataLoaded = false;
            cursorY = 1;
            getch();
            return;
         }
         FILE *checkFilePtr = fopen(filePath, "rb");
         while (!feof(checkFilePtr))
         {
            fread(tempPtr, STUDENT_SIZE, 1, checkFilePtr);
            if (!stricmp(tempPtr->studentId, newStudent->studentId))
            {
               textColor(12);
               isDuplicate = true;
               printf("Student with ID %s already exists    ", newStudent->studentId);
               gotoxy(cursorX, cursorY);
               printf("%15s", " ");
               gotoxy(cursorX, cursorY);
               break;
            }
         }

         if (!isDuplicate)
         {
            gotoxy(cursorX, cursorY + 1);
            printf("%50s", " ");
            break;
         }
         fclose(checkFilePtr);
      } while (1);

      fflush(stdin);
      gotoxy(cursorX + ID_LENGTH, cursorY);
      fgets(newStudent->fullName, NAME_LENGTH, stdin);
      removeNewline(newStudent->fullName);
      gotoxy(cursorX + ID_LENGTH + NAME_LENGTH, cursorY);
      scanf("%f", &newStudent->score);
      getchar();
      gotoxy(cursorX, cursorY + 2);
      textColor(14);
      printf("Student %s information added successfully", newStudent->studentId);
      FILE *appendFilePtr = fopen(filePath, "ab");
      fwrite(newStudent, STUDENT_SIZE, 1, appendFilePtr);
      fclose(appendFilePtr);
      indexCounter = recordCount;
      strcpy(indexTable[indexCounter].studentId, newStudent->studentId);
      indexTable[indexCounter].idx = indexCounter;
      recordCount++;
      ++cursorY;
      gotoxy(cursorX, cursorY);
      if (*listHead == NULL)
      {
         newStudent->next = *listHead;
         currentPtr = *listHead = newStudent;
      }
      else
      {
         prevPtr = currentPtr;
         newStudent->next = prevPtr->next;
         prevPtr->next = newStudent;
         currentPtr = currentPtr->next;
      }
   } while (1);
}

void loadFromFile(StudentPtr *listHead, IndexEntry *indexTable)
{
   recordCount = 0;
   isTempFile = isDataLoaded = true;
   if (*listHead != NULL)
      freeMemory(listHead);
   printf("Enter the binary file name to read information: ");
   fgets(filePath, MAX_PATH, stdin);
   removeNewline(filePath);
   FILE *filePtr = fopen(filePath, "rb");
   if (filePtr == NULL)
   {
      textColor(12);
      printf("\nFile does not exist");
      return;
   }
   if (feof(filePtr))
   {
      textColor(14);
      printf("\nFile %s has no data", filePath);
      return;
   }
   StudentPtr prevPtr, tempPtr;
   do
   {
      tempPtr = (StudentPtr)malloc(STUDENT_SIZE);
      fread(tempPtr, STUDENT_SIZE, 1, filePtr);
      if (*listHead == NULL)
      {
         tempPtr->next = *listHead;
         currentPtr = *listHead = tempPtr;
      }
      else
      {
         prevPtr = currentPtr;
         tempPtr->next = prevPtr->next;
         prevPtr->next = tempPtr;
         currentPtr = currentPtr->next;
      }
   } while (!feof(filePtr));
   fclose(filePtr);
   textColor(14);
   printf("\nData loaded from file %s successfully", filePath);
   strcpy(indexFilePath, filePath);
   strcpy(strstr(indexFilePath, ".bin"), "_index.bin");
   FILE *indexFilePtr = fopen(indexFilePath, "rb");
   do
   {
      fread(indexTable + recordCount, INDEX_SIZE, 1, indexFilePtr);
      if (feof(indexFilePtr))
         break;
      recordCount++;
   } while (1);
   fclose(indexFilePtr);
   strcpy(tempFilePath, filePath);
}

void searchStudent(StudentPtr listHead)
{
   boolean isIdFound = true, isNameFound = true;
   StudentPtr tempPtr;
   char searchId[ID_LENGTH], searchName[NAME_LENGTH], idUpper[ID_LENGTH], nameUpper[NAME_LENGTH], keyPressed;
   do
   {
      system("cls");
      textColor(11);
      printColumnHeaders();
      gotoxy(cursorX, cursorY);
      textColor(14);
      fgets(searchId, ID_LENGTH, stdin);
      removeNewline(searchId);
      textColor(7);
      if (searchId[0] != '\0')
      {
         isIdFound = true;
         tempPtr = listHead;
         while (tempPtr != NULL)
         {
            if (isalnum(tempPtr->studentId[0]))
            {
               if (strstr(strlwr(strcpy(idUpper, tempPtr->studentId)), strlwr(searchId)) != NULL)
               {
                  displayStudent(tempPtr);
                  isIdFound = false;
               }
               tempPtr = tempPtr->next;
            }
            else
            {
               tempPtr = tempPtr->next;
               continue;
            }
         }
         if (isIdFound)
         {
            gotoxy(cursorX, cursorY + 2);
            textColor(12);
            printf("No information found");
         }
      }
      do
      {
         textColor(14);
         gotoxy(cursorX + ID_LENGTH, cursorY);
         fgets(searchName, NAME_LENGTH, stdin);
         removeNewline(searchName);
         if (searchId[0] == '\0' && searchName[0] == '\0')
         {
            textColor(14);
            printf("\nExiting search");
            return;
         }
         else if (searchId[0] != '\0' && searchName[0] == '\0')
         {
            break;
         }
         if (isIdFound && searchId[0] != '\0')
         {
            gotoxy(cursorX, cursorY);
            printf("%15s", " ");
         }
         else
         {
            system("cls");
            textColor(11);
            printColumnHeaders();
            textColor(14);
            gotoxy(cursorX + ID_LENGTH, cursorY);
            printf("%-25s", searchName);
         }

         gotoxy(cursorX, cursorY + 1);
         isNameFound = true;
         if (searchName[0] != '\0')
         {
            tempPtr = listHead;
            while (tempPtr != NULL)
            {
               if (isalnum(tempPtr->studentId[0]))
               {
                  if (strstr(strlwr(strcpy(nameUpper, tempPtr->fullName)), strlwr(searchName)) != NULL)
                  {
                     displayStudent(tempPtr);
                     isNameFound = false;
                  }
                  tempPtr = tempPtr->next;
               }
               else
               {
                  tempPtr = tempPtr->next;
                  continue;
               }
            }
         }

         if (isNameFound && searchName[0] != '\0')
         {
            gotoxy(cursorX, cursorY + 2);
            textColor(12);
            printf("No information found");
         }

         do
         {
            keyPressed = getch();
            if (keyPressed == 75 || keyPressed == 77)
               break;
         } while (1);

         system("cls");
         textColor(11);
         if (keyPressed == 75)
            break;
         printColumnHeaders();
      } while (1);

   } while (1);
}

void filterFailingStudents(StudentPtr *listHead, IndexEntry *indexTable)
{
   FILE *expelledFile = fopen("DropoutStudents.txt", "w");
   fprintf(expelledFile, "%-15s%-25s%-4s", "Student ID", "Full Name", "Score");
   StudentPtr tempPtr = NULL;
   int count = 0;
   textColor(11);
   printColumnHeaders();
   while (*listHead != NULL)
   {
      if ((*listHead)->score < 4 && isalnum((*listHead)->studentId[0]))
      {
         ++count;
         tempPtr = *listHead;
         displayStudent(tempPtr);
         fprintf(expelledFile, "\n%-15s%-25s%-.1f", tempPtr->studentId, tempPtr->fullName, tempPtr->score);
         *listHead = (*listHead)->next;
         free(tempPtr);
      }
      else
         break;
   }

   if (*listHead == NULL)
      return;

   StudentPtr prevPtr;
   currentPtr = prevPtr = *listHead;
   while (currentPtr->next != NULL)
   {
      currentPtr = currentPtr->next;
      if (currentPtr->score < 4 && isalnum(currentPtr->studentId[0]))
      {
         tempPtr = currentPtr;
         prevPtr->next = currentPtr->next;
         ++count;
         displayStudent(tempPtr);
         fprintf(expelledFile, "\n%-15s%-25s%-.1f", tempPtr->studentId, tempPtr->fullName, tempPtr->score);
         free(tempPtr);
         currentPtr = prevPtr;
      }
      else
         prevPtr = currentPtr;
   }

   fclose(expelledFile);

   if (!count)
   {
      textColor(14);
      system("cls");
      printf("No students have been expelled");
   }
   else
   {
      count = 0;
      FILE *filePtr = fopen(filePath, "wb");
      FILE *indexFilePtr = fopen(indexFilePath, "wb");
      tempPtr = *listHead;
      while (isalnum(tempPtr->studentId[0]) && tempPtr != NULL)
      {
         fwrite(tempPtr, STUDENT_SIZE, 1, filePtr);
         strcpy(indexTable[count].studentId, tempPtr->studentId);
         indexTable[count].idx = count;
         ++count;
         tempPtr = tempPtr->next;
      }
      fclose(filePtr);
      recordCount = count;
      sortIndexTable(indexTable);
      for (int i = 0; i < count; ++i)
         fwrite(indexTable + i, INDEX_SIZE, 1, indexFilePtr);
      fclose(indexFilePtr);
   }
}

void deleteStudent(StudentPtr *listHead, IndexEntry *indexTable)
{
   char studentId[ID_LENGTH], confirmDelete;
   StudentPtr prevPtr;
   int updatedCount = 0;
   bool foundStudent = false;
   do
   {
      do
      {
         textColor(7);
         printf("Enter the student ID to delete: ");
         textColor(14);
         fgets(studentId, ID_LENGTH, stdin);
         removeNewline(studentId);
         if (studentId[0] == '\0')
         {
            textColor(14);
            system("cls");
            printf("Delete canceled");
            if (foundStudent)
            {
               updatedCount = 0;
               FILE *filePtr = fopen(filePath, "wb");
               FILE *indexFilePtr = fopen(indexFilePath, "wb");
               currentPtr = *listHead;
               while (isalnum(currentPtr->studentId[0]) && currentPtr != NULL)
               {
                  fwrite(currentPtr, STUDENT_SIZE, 1, filePtr);
                  strcpy(indexTable[updatedCount].studentId, currentPtr->studentId);
                  indexTable[updatedCount].idx = updatedCount;
                  ++updatedCount;
                  currentPtr = currentPtr->next;
               }
               fclose(filePtr);
               recordCount = updatedCount;
               sortIndexTable(indexTable);
               for (int i = 0; i < updatedCount; ++i)
                  fwrite(indexTable + i, INDEX_SIZE, 1, indexFilePtr);
               fclose(indexFilePtr);
            }
            return;
         }
         foundStudent = false;
         currentPtr = *listHead;
         do
         {
            if (!stricmp(studentId, currentPtr->studentId))
            {
               foundStudent = true;
               break;
            }
            prevPtr = currentPtr;
            currentPtr = currentPtr->next;
         } while (currentPtr != NULL);

         if (!foundStudent)
         {
            textColor(11);
            printf("\nStudent %s not found", studentId);
            getch();
            system("cls");
            break;
         }

         textColor(11);
         printColumnHeaders();
         displayStudent(currentPtr);

         textColor(14);
         printf("\nAre you sure you want to delete this student (y/n)?: ");
         do
         {
            confirmDelete = getche();
            if (confirmDelete == 'y' || confirmDelete == 'n')
               break;
         } while (1);

         getch();
         if (confirmDelete == 'n')
         {
            system("cls");
            break;
         }

         if (currentPtr == *listHead)
            *listHead = (*listHead)->next;
         else
            prevPtr->next = currentPtr->next;
         free(currentPtr);
         textColor(14);
         printf("\nDeleted successfully");
         getch();
         system("cls");
      } while (1);

   } while (1);
}

void displayAllStudents(StudentPtr listHead)
{
   StudentPtr tempPtr = listHead;
   while (tempPtr != NULL)
   {
      if (isalnum(tempPtr->studentId[0]))
      {
         displayStudent(tempPtr);
         tempPtr = tempPtr->next;
      }
      else
      {
         tempPtr = tempPtr->next;
         continue;
      }
   }
}

void sortByScoreDescending(StudentPtr listHead)
{
   char tempId[ID_LENGTH], tempName[NAME_LENGTH];
   float tempScore;
   for (StudentPtr i = listHead; i->next != NULL; i = i->next)
   {
      if (!isalnum(i->studentId[0]))
         continue;
      for (StudentPtr j = i->next; j != NULL; j = j->next)
      {
         if (!isalnum(j->studentId[0]))
            continue;
         if (i->score < j->score)
         {
            tempScore = i->score;
            i->score = j->score;
            j->score = tempScore;
            strcpy(tempName, i->fullName);
            strcpy(i->fullName, j->fullName);
            strcpy(j->fullName, tempName);
            strcpy(tempId, i->studentId);
            strcpy(i->studentId, j->studentId);
            strcpy(j->studentId, tempId);
         }
      }
   }
}

void sortByScoreAscending(StudentPtr listHead)
{
   char tempId[ID_LENGTH], tempName[NAME_LENGTH];
   float tempScore;
   for (StudentPtr i = listHead; i->next != NULL; i = i->next)
   {
      if (!isalnum(i->studentId[0]))
         continue;
      for (StudentPtr j = i->next; j != NULL; j = j->next)
      {
         if (!isalnum(j->studentId[0]))
            continue;
         if (i->score > j->score)
         {
            tempScore = i->score;
            i->score = j->score;
            j->score = tempScore;
            strcpy(tempName, i->fullName);
            strcpy(i->fullName, j->fullName);
            strcpy(j->fullName, tempName);
            strcpy(tempId, i->studentId);
            strcpy(i->studentId, j->studentId);
            strcpy(j->studentId, tempId);
         }
      }
   }
}

void sortByNameAscending(StudentPtr listHead)
{
   char tempId[ID_LENGTH], tempName[NAME_LENGTH];
   float tempScore;
   for (StudentPtr i = listHead; i->next != NULL; i = i->next)
   {
      if (!isalnum(i->studentId[0]))
         continue;
      for (StudentPtr j = i->next; j != NULL; j = j->next)
      {
         if (!isalnum(j->studentId[0]))
            continue;
         if (stricmp(i->fullName, j->fullName) > 0)
         {
            tempScore = i->score;
            i->score = j->score;
            j->score = tempScore;
            strcpy(tempName, i->fullName);
            strcpy(i->fullName, j->fullName);
            strcpy(j->fullName, tempName);
            strcpy(tempId, i->studentId);
            strcpy(i->studentId, j->studentId);
            strcpy(j->studentId, tempId);
         }
      }
   }
   for (StudentPtr i = listHead; i->next != NULL; i = i->next)
   {
      if (!isalnum(i->studentId[0]))
         continue;
      for (StudentPtr j = i->next; j != NULL; j = j->next)
      {
         if (!isalnum(j->studentId[0]))
            continue;
         if (stricmp(strrchr(i->fullName, ' ') + 1, strrchr(j->fullName, ' ') + 1) > 0)
         {
            tempScore = i->score;
            i->score = j->score;
            j->score = tempScore;
            strcpy(tempName, i->fullName);
            strcpy(i->fullName, j->fullName);
            strcpy(j->fullName, tempName);
            strcpy(tempId, i->studentId);
            strcpy(i->studentId, j->studentId);
            strcpy(j->studentId, tempId);
         }
      }
   }
}

void sortByNameDescending(StudentPtr listHead)
{
   char tempId[ID_LENGTH], tempName[NAME_LENGTH];
   float tempScore;
   for (StudentPtr i = listHead; i->next != NULL; i = i->next)
   {
      if (!isalnum(i->studentId[0]))
         continue;
      for (StudentPtr j = i->next; j != NULL; j = j->next)
      {
         if (!isalnum(j->studentId[0]))
            continue;
         if (stricmp(i->fullName, j->fullName) < 0)
         {
            tempScore = i->score;
            i->score = j->score;
            j->score = tempScore;
            strcpy(tempName, i->fullName);
            strcpy(i->fullName, j->fullName);
            strcpy(j->fullName, tempName);
            strcpy(tempId, i->studentId);
            strcpy(i->studentId, j->studentId);
            strcpy(j->studentId, tempId);
         }
      }
   }
   for (StudentPtr i = listHead; i->next != NULL; i = i->next)
   {
      if (!isalnum(i->studentId[0]))
         continue;
      for (StudentPtr j = i->next; j != NULL; j = j->next)
      {
         if (!isalnum(j->studentId[0]))
            continue;
         if (stricmp(strrchr(i->fullName, ' ') + 1, strrchr(j->fullName, ' ') + 1) < 0)
         {
            tempScore = i->score;
            i->score = j->score;
            j->score = tempScore;
            strcpy(tempName, i->fullName);
            strcpy(i->fullName, j->fullName);
            strcpy(j->fullName, tempName);
            strcpy(tempId, i->studentId);
            strcpy(i->studentId, j->studentId);
            strcpy(j->studentId, tempId);
         }
      }
   }
}

void sortStudentList(StudentPtr listHead)
{
   char nameSort[NAME_LENGTH], scoreSort[NAME_LENGTH];
   char keyPressed;
   bool exitSort = false;
   printColumnHeaders();
   gotoxy(cursorX, cursorY);
   displayAllStudents(listHead);
   do
   {
      textColor(14);
      gotoxy(cursorX + ID_LENGTH, cursorY);
      fgets(nameSort, ID_LENGTH, stdin);
      removeNewline(nameSort);
      textColor(7);

      if (!stricmp(nameSort, "asc"))
      {
         sortByNameAscending(listHead);
         gotoxy(cursorX, cursorY);
         displayAllStudents(listHead);
      }
      else if (!stricmp(nameSort, "desc"))
      {
         sortByNameDescending(listHead);
         gotoxy(cursorX, cursorY);
         displayAllStudents(listHead);
      }

      do
      {
         textColor(14);
         gotoxy(cursorX + ID_LENGTH + NAME_LENGTH, cursorY);
         fgets(scoreSort, NAME_LENGTH, stdin);
         removeNewline(scoreSort);
         gotoxy(cursorX, cursorY);
         printf("%60s", " ");
         gotoxy(cursorX + ID_LENGTH + NAME_LENGTH, cursorY);
         printf("%s", scoreSort);
         if (!stricmp(scoreSort, "asc"))
            sortByScoreAscending(listHead);
         else if (!stricmp(scoreSort, "desc"))
            sortByScoreDescending(listHead);
         else if (stricmp(nameSort, "asc") && stricmp(nameSort, "desc") &&
                  stricmp(scoreSort, "asc") && stricmp(scoreSort, "desc"))
         {
            exitSort = true;
            break;
         }
         gotoxy(cursorX, cursorY);
         displayAllStudents(listHead);

         do
         {
            keyPressed = getch();
            if (keyPressed == 75 || keyPressed == 77)
               break;
         } while (1);

         if (keyPressed == 75)
            break;
         textColor(14);
         gotoxy(cursorX, cursorY);
         printf("%60s", " ");

      } while (1);
      if (exitSort)
      {
         system("cls");
         textColor(14);
         printf("Sort canceled");
         textColor(7);
         break;
      }
      gotoxy(cursorX, cursorY);
      printf("%60s", " ");
   } while (1);
}

void updateStudentInfo(StudentPtr studentList, IndexEntry *indexTable)
{
   textColor(11);
   printColumnHeaders();
   StudentPtr tempStudent;
   char choice, studentId[ID_LENGTH], fullName[NAME_LENGTH], scoreStr[5], *end;
   int binarySearchResult, indexPosition, newIndexPosition;
   bool isCancelled = false;
   FILE *filePtr = fopen(filePath, "rb+");

   do
   {
      do
      {
         textColor(7);
         gotoxy(cursorX, cursorY);
         printf("%15s", " ");
         gotoxy(cursorX, cursorY);
         textColor(14);
         fgets(studentId, ID_LENGTH, stdin);
         removeNewline(studentId);
         if (studentId[0] == '\0')
         {
            system("cls");
            isCancelled = true;
            textColor(14);
            printf("Update cancelled\n");
            fclose(filePtr);
            FILE *indexFilePtr = fopen(indexFilePath, "wb");
            for (int i = 0; i < recordCount; ++i)
               fwrite(indexTable + i, INDEX_SIZE, 1, indexFilePtr);
            fclose(indexFilePtr);
            textColor(7);
            break;
         }
         tempStudent = studentList;
         while (tempStudent != NULL)
         {
            if (!stricmp(tempStudent->studentId, studentId))
               break;
            tempStudent = tempStudent->next;
         }

         if (tempStudent == NULL)
         {
            textColor(12);
            printf("Student %s has dropped out", studentId);
            textColor(7);
         }
         else
         {
            gotoxy(cursorX, cursorY + 1);
            printf("%60s", " ");
            indexPosition = binarySearch(indexTable, tempStudent->studentId, &binarySearchResult);
            break;
         }
      } while (1);

      if (isCancelled)
         break;

      textColor(7);
      gotoxy(cursorX + 60, cursorY - 1);
      displayStudent(tempStudent);
      gotoxy(cursorX, cursorY + 2);
      printf("Enter new information");
      gotoxy(cursorX, cursorY + 4);
      textColor(11);
      printColumnHeaders();

      do
      {
         do
         {
            textColor(7);
            gotoxy(cursorX, cursorY + 5);
            fgets(studentId, ID_LENGTH, stdin);
            removeNewline(studentId);
            if (studentId[0] == '\0')
               break;
            else if (binarySearch(indexTable, studentId, &newIndexPosition) != -1)
            {
               gotoxy(cursorX, cursorY + 6);
               textColor(12);
               printf("Student with ID %s already exists", studentId);
               gotoxy(cursorX, cursorY + 5);
               printf("%15s", " ");
            }
            else
            {
               strcpy(tempStudent->studentId, studentId);
               strcpy(indexTable[binarySearchResult].studentId, studentId);
               sortIndexTable(indexTable);
               break;
            }
         } while (1);
         gotoxy(cursorX, cursorY + 6);
         printf("%60s", " ");
         gotoxy(cursorX + ID_LENGTH, cursorY + 5);
         fgets(fullName, NAME_LENGTH, stdin);
         removeNewline(fullName);
         if (fullName[0] != '\0')
            strcpy(tempStudent->fullName, fullName);
         gotoxy(cursorX + ID_LENGTH + NAME_LENGTH, cursorY + 5);
         fgets(scoreStr, 5, stdin);
         removeNewline(scoreStr);
         if (scoreStr[0] != '\0')
            tempStudent->score = strtof(scoreStr, &end);
         if (studentId[0] == '\0' && fullName[0] == '\0' && scoreStr[0] == '\0')
         {
            textColor(14);
            printf("\nUpdate cancelled\n");
            system("pause");
            system("cls");
            textColor(11);
            printColumnHeaders();
            textColor(7);
            break;
         }
         else
         {
            gotoxy(cursorX + 60, cursorY - 1);
            displayStudent(tempStudent);
            gotoxy(cursorX, cursorY + 7);
            printf("Information updated successfully");
            fseek(filePtr, STUDENT_SIZE * indexPosition, SEEK_SET);
            fwrite(tempStudent, STUDENT_SIZE, 1, filePtr);
         }

         textColor(14);
         printf("\nContinue updating this student's information (y/n)?: ");
         do
         {
            choice = getche();
            if (choice == 'y' || choice == 'n')
               break;
            printf("Enter again: ");
         } while (1);
         getch();

         if (choice == 'n')
         {
            printf("\nUpdate cancelled for this student\n");
            system("pause");
            system("cls");
            textColor(11);
            printColumnHeaders();
            break;
         }

         gotoxy(cursorX, cursorY + 5);
         printf("%100s", " ");
         gotoxy(cursorX, cursorY + 6);
         printf("%100s", " ");
         gotoxy(cursorX, cursorY + 7);
         printf("%100s", " ");
         gotoxy(cursorX, cursorY + 8);
         printf("%100s", " ");
      } while (1);
   } while (1);
}
