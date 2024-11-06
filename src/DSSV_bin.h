#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <stdbool.h>

#define MSSV 15
#define HT 25
#define DIEM 4

struct SinhVien;
typedef struct SinhVien *sv;

struct SinhVien
{
   char mssv[MSSV];
   char ht[HT];
   float diem;
   sv next;
};

#define sizeSV sizeof(struct SinhVien)

struct Index
{
   char mssv[MSSV];
   int idx;
};

typedef struct Index index;
#define sizeI sizeof(index)
#define MAX 100

char tenFile[MAX], tenFileIndex[MAX], tenFileTemp[MAX];
int x = 0, y = 1, n = 0;
bool ok = false, oge = true;
sv P = NULL;

void gotoxy(int x, int y)
{
   COORD toaDo;
   toaDo.X = x;
   toaDo.Y = y;
   SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), toaDo);
}

void textColor(int x)
{
   SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), x);
}

void ct(short *option, short end)
{
   do
   {
      scanf("%hi", option);
      if (*option >= 0 && *option <= end)
         break;
      textColor(14);
      printf("Nhap lai: ");
   } while (1);
}

void dinhDangCot()
{
   printf("%-15s%-25s%-4s", "MSSV", "Ho va ten", "Diem");
}

void xemSV(sv temp)
{
   textColor(7);
   printf("\n%-15s%-25s%-4.1f", temp->mssv, temp->ht, temp->diem);
}

void xoaXuongDong(char *s)
{
   int lens = strlen(s);
   if (s[lens - 1] == '\n')
      s[lens - 1] = '\0';
}

void giaiPhong(sv *L)
{
   sv temp = NULL;
   while (*L != NULL)
   {
      temp = *L;
      *L = (*L)->next;
      free(temp);
   }
}

void sapXep(index *a)
{
   bool check;
   index temp;
   for (int i = 0; i < n; ++i)
   {
      check = true;
      for (int j = 0; j < n - i - 1; ++j)
         if (stricmp(a[j].mssv, a[j + 1].mssv) > 0)
         {
            temp = a[j];
            a[j] = a[j + 1];
            a[j + 1] = temp;
            check = false;
         }
      if (check)
         break;
   }
}

void ghiFileText(sv L)
{
   char tenFileText[MAX];
   printf("Nhap ten file van ban muon ghi thong tin: ");
   fgets(tenFileText, MAX, stdin);
   xoaXuongDong(tenFileText);
   FILE *f = fopen(tenFileText, "w");
   fprintf(f, "%-15s%-25s%-4s", "MSSV", "Ho va ten", "Diem");
   sv temp = L;
   while (isalnum(temp->mssv[0]) && temp != NULL)
   {
      fprintf(f, "\n%-15s%-25s%-4.1f", temp->mssv, temp->ht, temp->diem);
      temp = temp->next;
   }
   fclose(f);
   textColor(14);
   printf("\nGhi thong tin thanh cong");
}

void ghiFileNhiPhan(sv L)
{
   char tenFileText[MAX];
   printf("Nhap ten file nhi phan muon ghi thong tin: ");
   fgets(tenFileText, MAX, stdin);
   xoaXuongDong(tenFileText);
   FILE *f = fopen(tenFileText, "wb");
   sv temp = L;
   while (isalnum(temp->mssv[0]) && temp != NULL)
   {
      fwrite(temp, sizeSV, 1, f);
      temp = temp->next;
   }
   fclose(f);
   textColor(14);
   printf("\nGhi thong tin thanh cong");
}

int timKiemNhiPhan(index *a, char *mssv, int *mid)
{
   int l = 0, r = n - 1, ss;
   while (l <= r)
   {
      *mid = (l + r) / 2;
      ss = stricmp(a[*mid].mssv, mssv);
      if (ss == 0)
         return a[*mid].idx;
      else if (ss > 0)
         r = *mid - 1;
      else
         l = *mid + 1;
   }
   return -1;
}

void nhapTuBanPhim(sv *L, index *a)
{
   int z = 0;
   int ss = 1;
   if (oge)
   {
      FILE *f1;
      printf("Nhap ten file nhi phan can ghi thong tin: ");
      fgets(tenFile, MAX, stdin);
      xoaXuongDong(tenFile);
      ss = stricmp(tenFile, tenFileTemp);
      if (ss)
      {
         f1 = fopen(tenFile, "wb");
         fclose(f1);
         n = 0;
      }
      y = 2;
   }
   else
      y = 1;
   bool check;
   sv svMoi, temp, Q;
   temp = (sv)malloc(sizeSV);
   if (ok && ss)
      giaiPhong(L);
   textColor(11);
   dinhDangCot();
   do
   {
      svMoi = (sv)malloc(sizeSV);
      do
      {
         check = true;
         textColor(7);
         gotoxy(x, y);
         fgets(svMoi->mssv, MSSV, stdin);
         xoaXuongDong(svMoi->mssv);
         if (svMoi->mssv[0] == '\0')
         {
            textColor(14);
            printf("Dung nhap thong tin%30s", " ");
            gotoxy(x, y + 1);
            textColor(7);
            sapXep(a);
            strcpy(tenFileIndex, tenFile);
            strcpy(strstr(tenFileIndex, ".bin"), "_index.bin");
            FILE *fi = fopen(tenFileIndex, "wb");
            for (int i = 0; i < n; ++i)
               fwrite(a + i, sizeI, 1, fi);
            fclose(fi);
            free(temp);
            free(svMoi);
            if (!ss)
               oge = true;
            else
               oge = ok = false;
            y = 1;
            getch();
            return;
         }
         FILE *f2 = fopen(tenFile, "rb");
         while (!feof(f2))
         {
            fread(temp, sizeSV, 1, f2);
            if (!stricmp(temp->mssv, svMoi->mssv))
            {
               textColor(12);
               check = false;
               printf("Da co sinh vien ma %s trong danh sach    ", svMoi->mssv);
               gotoxy(x, y);
               printf("%15s", " ");
               gotoxy(x, y);
               break;
            }
         }

         if (check)
         {
            gotoxy(x, y + 1);
            printf("%50s", " ");
            break;
         }
         fclose(f2);
      } while (1);

      fflush(stdin);
      gotoxy(x + MSSV, y);
      fgets(svMoi->ht, HT, stdin);
      xoaXuongDong(svMoi->ht);
      gotoxy(x + MSSV + HT, y);
      scanf("%f", &svMoi->diem);
      getchar();
      gotoxy(x, y + 2);
      textColor(14);
      printf("Nhap thong tin sinh vien %s thanh cong", svMoi->mssv);
      FILE *f3 = fopen(tenFile, "ab");
      fwrite(svMoi, sizeSV, 1, f3);
      fclose(f3);
      z = n;
      strcpy(a[z].mssv, svMoi->mssv);
      a[z].idx = z;
      n++;
      ++y;
      gotoxy(x, y);
      if (*L == NULL)
      {
         svMoi->next = *L;
         P = *L = svMoi;
      }
      else
      {
         Q = P;
         svMoi->next = Q->next;
         Q->next = svMoi;
         P = P->next;
      }
   } while (1);
}

void nhapTuFile(sv *L, index *a)
{
   n = 0;
   oge = ok = true;
   if (*L != NULL)
      giaiPhong(L);
   printf("Nhap ten file nhi phan can doc thong tin: ");
   fgets(tenFile, MAX, stdin);
   xoaXuongDong(tenFile);
   FILE *f = fopen(tenFile, "rb");
   if (f == NULL)
   {
      textColor(12);
      printf("\nTep tin khong ton tai");
      return;
   }
   if (feof(f))
   {
      textColor(14);
      printf("\nfile %s chua co du lieu", tenFile);
      return;
   }
   sv Q, temp;
   do
   {
      temp = (sv)malloc(sizeSV);
      fread(temp, sizeSV, 1, f);
      if (*L == NULL)
      {
         temp->next = *L;
         P = *L = temp;
      }
      else
      {
         Q = P;
         temp->next = Q->next;
         Q->next = temp;
         P = P->next;
      }
   } while (!feof(f));
   fclose(f);
   textColor(14);
   printf("\nNhap du lieu tu file %s thanh cong", tenFile);
   strcpy(tenFileIndex, tenFile);
   strcpy(strstr(tenFileIndex, ".bin"), "_index.bin");
   FILE *fi = fopen(tenFileIndex, "rb");
   do
   {
      fread(a + n, sizeI, 1, fi);
      if (feof(fi))
         break;
      n++;
   } while (1);
   fclose(fi);
   strcpy(tenFileTemp, tenFile);
}

void traCuu(sv L)
{
   boolean checkma = true, checkht = true;
   sv temp;
   char ma[MSSV], ht[HT], maupr[MSSV], htupr[HT], c;
   do
   {
      system("cls");
      textColor(11);
      dinhDangCot();
      gotoxy(x, y);
      textColor(14);
      fgets(ma, MSSV, stdin);
      xoaXuongDong(ma);
      textColor(7);
      if (ma[0] != '\0')
      {
         checkma = true;
         temp = L;
         while (temp != NULL)
         {
            if (isalnum(temp->mssv[0]))
            {
               if (strstr(strlwr(strcpy(maupr, temp->mssv)), strlwr(ma)) != NULL)
               {
                  xemSV(temp);
                  checkma = false;
               }
               temp = temp->next;
            }
            else
            {
               temp = temp->next;
               continue;
            }
         }
         if (checkma)
         {
            gotoxy(x, y + 2);
            textColor(12);
            printf("Khong tim thay thong tin");
         }
      }
      do
      {
         textColor(14);
         gotoxy(x + MSSV, y);
         fgets(ht, HT, stdin);
         xoaXuongDong(ht);
         if (ma[0] == '\0' && ht[0] == '\0')
         {
            textColor(14);
            printf("\nThoat tra cuu");
            return;
         }
         else if (ma[0] != '\0' && ht[0] == '\0')
         {
            break;
         }
         if (checkma && ma[0] != '\0')
         {
            gotoxy(x, y);
            printf("%15s", " ");
         }
         else
         {
            system("cls");
            textColor(11);
            dinhDangCot();
            textColor(14);
            gotoxy(x + MSSV, y);
            printf("%-25s", ht);
         }

         gotoxy(x, y + 1);
         checkht = true;
         if (ht[0] != '\0')
         {
            temp = L;
            while (temp != NULL)
            {
               if (isalnum(temp->mssv[0]))
               {
                  if (strstr(strlwr(strcpy(htupr, temp->ht)), strlwr(ht)) != NULL)
                  {
                     xemSV(temp);
                     checkht = false;
                  }
                  temp = temp->next;
               }
               else
               {
                  temp = temp->next;
                  continue;
               }
            }
         }

         if (checkht && ht[0] != '\0')
         {
            gotoxy(x, y + 2);
            textColor(12);
            printf("Khong tim thay thong tin");
         }

         do
         {
            c = getch();
            if (c == 75 || c == 77)
               break;
         } while (1);

         system("cls");
         textColor(11);
         if (c == 75)
            break;
         dinhDangCot();
      } while (1);

   } while (1);
}

void locSV(sv *L, index *a)
{
   FILE *luuBan = fopen("NghiHoc.txt", "w");
   fprintf(luuBan, "%-15s%-25s%-4s", "MSSV", "Ho va ten", "Diem");
   sv temp = NULL;
   int count = 0;
   textColor(11);
   dinhDangCot();
   while (*L != NULL)
   {
      if ((*L)->diem < 4 && isalnum((*L)->mssv[0]))
      {
         ++count;
         temp = *L;
         xemSV(temp);
         fprintf(luuBan, "\n%-15s%-25s%-.1f", temp->mssv, temp->ht, temp->diem);
         *L = (*L)->next;
         free(temp);
      }
      else
         break;
   }

   if (*L == NULL)
      return;

   sv Q;
   P = Q = *L;
   while (P->next != NULL)
   {
      P = P->next;
      if (P->diem < 4 && isalnum(P->mssv[0]))
      {
         temp = P;
         Q->next = P->next;
         ++count;
         xemSV(temp);
         fprintf(luuBan, "\n%-15s%-25s%-.1f", temp->mssv, temp->ht, temp->diem);
         free(temp);
         P = Q;
      }
      else
         Q = P;
   }

   fclose(luuBan);

   if (!count)
   {
      textColor(14);
      system("cls");
      printf("Khong co sinh vien nao bi buoc thoi hoc");
   }
   else
   {
      count = 0;
      FILE *f = fopen(tenFile, "wb");
      FILE *fi = fopen(tenFileIndex, "wb");
      temp = *L;
      while (isalnum(temp->mssv[0]) && temp != NULL)
      {
         fwrite(temp, sizeSV, 1, f);
         strcpy(a[count].mssv, temp->mssv);
         a[count].idx = count;
         ++count;
         temp = temp->next;
      }
      fclose(f);
      n = count;
      sapXep(a);
      for (int i = 0; i < count; ++i)
         fwrite(a + i, sizeI, 1, fi);
      fclose(fi);
   }
}

void xoaSV(sv *L, index *a)
{
   char ma[MSSV], yn;
   sv Q;
   int count = 0;
   bool check = false;
   do
   {
      do
      {
         textColor(7);
         printf("Nhap ma sinh vien muon xoa: ");
         textColor(14);
         fgets(ma, MSSV, stdin);
         xoaXuongDong(ma);
         if (ma[0] == '\0')
         {
            textColor(14);
            system("cls");
            printf("Huy xoa sinh vien");
            if (check)
            {
               count = 0;
               FILE *f = fopen(tenFile, "wb");
               FILE *fi = fopen(tenFileIndex, "wb");
               P = *L;
               while (isalnum(P->mssv[0]) && P != NULL)
               {
                  fwrite(P, sizeSV, 1, f);
                  strcpy(a[count].mssv, P->mssv);
                  a[count].idx = count;
                  ++count;
                  P = P->next;
               }
               fclose(f);
               n = count;
               sapXep(a);
               for (int i = 0; i < count; ++i)
                  fwrite(a + i, sizeI, 1, fi);
               fclose(fi);
            }
            return;
         }
         check = false;
         P = *L;
         do
         {
            if (!stricmp(ma, P->mssv))
            {
               check = true;
               break;
            }
            Q = P;
            P = P->next;
         } while (P != NULL);

         if (!check)
         {
            textColor(11);
            printf("\nKhong tim thay sinh vien %s", ma);
            getch();
            system("cls");
            break;
         }

         textColor(11);
         dinhDangCot();
         xemSV(P);

         textColor(14);
         printf("\nBan co chac muon xoa sinh vien nay khong (y/n)?: ");
         do
         {
            yn = getche();
            if (yn == 'y' || yn == 'n')
               break;
         } while (1);

         getch();
         if (yn == 'n')
         {
            system("cls");
            break;
         }

         if (P == *L)
            *L = (*L)->next;
         else
            Q->next = P->next;
         free(P);
         textColor(14);
         printf("\nXoa thanh cong");
         getch();
         system("cls");
      } while (1);

   } while (1);
}

void xemDSSV(sv L)
{
   sv temp = L;
   while (temp != NULL)
   {
      if (isalnum(temp->mssv[0]))
      {
         xemSV(temp);
         temp = temp->next;
      }
      else
      {
         temp = temp->next;
         continue;
      }
   }
}

void xepDiemGiam(sv L)
{
   char ma[MSSV], ht[HT];
   float d;
   for (sv i = L; i->next != NULL; i = i->next)
   {
      if (!isalnum(i->mssv[0]))
         continue;
      for (sv j = i->next; j != NULL; j = j->next)
      {
         if (!isalnum(j->mssv[0]))
            continue;
         if (i->diem < j->diem)
         {
            d = i->diem;
            i->diem = j->diem;
            j->diem = d;
            strcpy(ht, i->ht);
            strcpy(i->ht, j->ht);
            strcpy(j->ht, ht);
            strcpy(ma, i->mssv);
            strcpy(i->mssv, j->mssv);
            strcpy(j->mssv, ma);
         }
      }
   }
}

void xepDiemTang(sv L)
{
   char ma[MSSV], ht[HT];
   float d;
   for (sv i = L; i->next != NULL; i = i->next)
   {
      if (!isalnum(i->mssv[0]))
         continue;
      for (sv j = i->next; j != NULL; j = j->next)
      {
         if (!isalnum(j->mssv[0]))
            continue;
         if (i->diem > j->diem)
         {
            d = i->diem;
            i->diem = j->diem;
            j->diem = d;
            strcpy(ht, i->ht);
            strcpy(i->ht, j->ht);
            strcpy(j->ht, ht);
            strcpy(ma, i->mssv);
            strcpy(i->mssv, j->mssv);
            strcpy(j->mssv, ma);
         }
      }
   }
}

void xepTenTang(sv L)
{
   char ma[MSSV], ht[HT];
   float d;
   for (sv i = L; i->next != NULL; i = i->next)
   {
      if (!isalnum(i->mssv[0]))
         continue;
      for (sv j = i->next; j != NULL; j = j->next)
      {
         if (!isalnum(j->mssv[0]))
            continue;
         if (stricmp(i->ht, j->ht) > 0)
         {
            d = i->diem;
            i->diem = j->diem;
            j->diem = d;
            strcpy(ht, i->ht);
            strcpy(i->ht, j->ht);
            strcpy(j->ht, ht);
            strcpy(ma, i->mssv);
            strcpy(i->mssv, j->mssv);
            strcpy(j->mssv, ma);
         }
      }
   }
   for (sv i = L; i->next != NULL; i = i->next)
   {
      if (!isalnum(i->mssv[0]))
         continue;
      for (sv j = i->next; j != NULL; j = j->next)
      {
         if (!isalnum(j->mssv[0]))
            continue;
         if (stricmp(strrchr(i->ht, ' ') + 1, strrchr(j->ht, ' ') + 1) > 0)
         {
            d = i->diem;
            i->diem = j->diem;
            j->diem = d;
            strcpy(ht, i->ht);
            strcpy(i->ht, j->ht);
            strcpy(j->ht, ht);
            strcpy(ma, i->mssv);
            strcpy(i->mssv, j->mssv);
            strcpy(j->mssv, ma);
         }
      }
   }
}

void xepTenGiam(sv L)
{
   char ma[MSSV], ht[HT];
   float d;
   for (sv i = L; i->next != NULL; i = i->next)
   {
      if (!isalnum(i->mssv[0]))
         continue;
      for (sv j = i->next; j != NULL; j = j->next)
      {
         if (!isalnum(j->mssv[0]))
            continue;
         if (stricmp(i->ht, j->ht) < 0)
         {
            d = i->diem;
            i->diem = j->diem;
            j->diem = d;
            strcpy(ht, i->ht);
            strcpy(i->ht, j->ht);
            strcpy(j->ht, ht);
            strcpy(ma, i->mssv);
            strcpy(i->mssv, j->mssv);
            strcpy(j->mssv, ma);
         }
      }
   }
   for (sv i = L; i->next != NULL; i = i->next)
   {
      if (!isalnum(i->mssv[0]))
         continue;
      for (sv j = i->next; j != NULL; j = j->next)
      {
         if (!isalnum(j->mssv[0]))
            continue;
         if (stricmp(strrchr(i->ht, ' ') + 1, strrchr(j->ht, ' ') + 1) < 0)
         {
            d = i->diem;
            i->diem = j->diem;
            j->diem = d;
            strcpy(ht, i->ht);
            strcpy(i->ht, j->ht);
            strcpy(j->ht, ht);
            strcpy(ma, i->mssv);
            strcpy(i->mssv, j->mssv);
            strcpy(j->mssv, ma);
         }
      }
   }
}

void sapXepDSSV(sv L)
{
   char ht[HT], d[HT];
   char c;
   bool check = false;
   dinhDangCot();
   gotoxy(x, y);
   xemDSSV(L);
   do
   {

      textColor(14);
      gotoxy(x + MSSV, y);
      fgets(ht, MSSV, stdin);
      xoaXuongDong(ht);
      textColor(7);

      if (!stricmp(ht, "asc"))
      {
         xepTenTang(L);
         gotoxy(x, y);
         xemDSSV(L);
      }
      else if (!stricmp(ht, "desc"))
      {
         xepTenGiam(L);
         gotoxy(x, y);
         xemDSSV(L);
      }

      do
      {
         textColor(14);
         gotoxy(x + MSSV + HT, y);
         fgets(d, HT, stdin);
         xoaXuongDong(d);
         gotoxy(x, y);
         printf("%60s", " ");
         gotoxy(x + MSSV + HT, y);
         printf("%s", d);
         if (!stricmp(d, "asc"))
            xepDiemTang(L);
         else if (!stricmp(d, "desc"))
            xepDiemGiam(L);
         else if (stricmp(ht, "asc") && stricmp(ht, "desc") && stricmp(d, "asc") && stricmp(d, "desc"))
         {
            check = true;
            break;
         }
         gotoxy(x, y);
         xemDSSV(L);

         do
         {
            c = getch();
            if (c == 75 || c == 77)
               break;
         } while (1);

         if (c == 75)
            break;
         textColor(14);
         gotoxy(x, y);
         printf("%60s", " ");

      } while (1);
      if (check)
      {
         system("cls");
         textColor(14);
         printf("Huy sap xep");
         textColor(7);
         break;
      }
      gotoxy(x, y);
      printf("%60s", " ");
   } while (1);
}

void capNhat(sv L, index *a)
{
   textColor(11);
   dinhDangCot();
   sv temp;
   char c, ma[MSSV], ht[HT], d[5], *end;
   int k1, k2, k3;
   bool check = false;
   FILE *fwb = fopen(tenFile, "rb+");

   do
   {
      do
      {
         textColor(7);
         gotoxy(x, y);
         printf("%15s", " ");
         gotoxy(x, y);
         textColor(14);
         fgets(ma, MSSV, stdin);
         xoaXuongDong(ma);
         if (ma[0] == '\0')
         {
            system("cls");
            check = true;
            textColor(14);
            printf("Huy cap nhat thong tin\n");
            fclose(fwb);
            FILE *fi = fopen(tenFileIndex, "wb");
            for (int i = 0; i < n; ++i)
               fwrite(a + i, sizeI, 1, fi);
            fclose(fi);
            textColor(7);
            break;
         }
         temp = L;
         while (temp != NULL)
         {
            if (!stricmp(temp->mssv, ma))
               break;
            temp = temp->next;
         }

         if (temp == NULL)
         {
            textColor(12);
            printf("Sinh vien %s da bo hoc", ma);
            textColor(7);
         }
         else
         {
            gotoxy(x, y + 1);
            printf("%60s", " ");
            k2 = timKiemNhiPhan(a, temp->mssv, &k1);
            break;
         }
      } while (1);

      if (check)
         break;

      textColor(7);
      gotoxy(x + 60, y - 1);
      xemSV(temp);
      gotoxy(x, y + 2);
      printf("Nhap thong tin moi");
      gotoxy(x, y + 4);
      textColor(11);
      dinhDangCot();

      do
      {
         do
         {
            textColor(7);
            gotoxy(x, y + 5);
            fgets(ma, MSSV, stdin);
            xoaXuongDong(ma);
            if (ma[0] == '\0')
               break;
            else if (timKiemNhiPhan(a, ma, &k3) != -1)
            {
               gotoxy(x, y + 6);
               textColor(12);
               printf("Da ton tai sinh vien co ma la %s", ma);
               gotoxy(x, y + 5);
               printf("%15s", " ");
            }
            else
            {
               strcpy(temp->mssv, ma);
               strcpy(a[k1].mssv, ma);
               sapXep(a);
               break;
            }
         } while (1);
         gotoxy(x, y + 6);
         printf("%60s", " ");
         gotoxy(x + MSSV, y + 5);
         fgets(ht, HT, stdin);
         xoaXuongDong(ht);
         if (ht[0] != '\0')
            strcpy(temp->ht, ht);
         gotoxy(x + MSSV + HT, y + 5);
         fgets(d, 5, stdin);
         xoaXuongDong(d);
         if (d[0] != '\0')
            temp->diem = strtof(d, &end);
         if (ma[0] == '\0' && ht[0] == '\0' && d[0] == '\0')
         {
            textColor(14);
            printf("\nHuy cap nhat\n");
            system("pause");
            system("cls");
            textColor(11);
            dinhDangCot();
            textColor(7);
            break;
         }
         else
         {
            gotoxy(x + 60, y - 1);
            xemSV(temp);
            gotoxy(x, y + 7);
            printf("Cap nhat thong tin thanh cong");
            fseek(fwb, sizeSV * k2, SEEK_SET);
            fwrite(temp, sizeSV, 1, fwb);
         }

         textColor(14);
         printf("\nTiep tuc cap nhat thong tin cua sinh vien nay khong (y/n)?: ");
         do
         {
            c = getche();
            if (c == 'y' || c == 'n')
               break;
            printf("Nhap lai: ");
         } while (1);
         getch();

         if (c == 'n')
         {
            printf("\nHuy cap nhat thong tin cua sinh vien nay\n");
            system("pause");
            system("cls");
            textColor(11);
            dinhDangCot();
            break;
         }

         gotoxy(x, y + 5);
         printf("%100s", " ");
         gotoxy(x, y + 6);
         printf("%100s", " ");
         gotoxy(x, y + 7);
         printf("%100s", " ");
         gotoxy(x, y + 8);
         printf("%100s", " ");
      } while (1);
   } while (1);
}
