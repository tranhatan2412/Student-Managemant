#include "DSSV_bin.h"

int main()
{
   sv L = NULL;
   index *a = (index *)malloc(MAX * sizeI);

   short option;
   do
   {
      system("cls");
      textColor(11);
      printf("===============MENU===============\n");
      textColor(7);
      printf("1, Nhap thong tin sinh vien\n");
      printf("2, Xem danh sach sinh vien\n");
      printf("3, Tra cuu thong tin\n");
      printf("4, Loc ra nhung sinh vien thoi hoc\n");
      printf("5, Cap nhat thong tin sinh vien\n");
      printf("6, Sap xep danh sach sinh vien\n");
      printf("7, Xoa sinh vien bang ma\n");
      printf("8, Ghi thong tin ra file van ban\n");
      printf("9, Ghi thong tin ra file nhi phan\n");
      textColor(14);
      printf("0, Thoat chuong trinh\n");
      textColor(11);
      printf("Moi ban chon chuong trinh (0 - 9): ");
      ct(&option, 9);
      textColor(7);

      if (option == 0)
      {
         textColor(14);
         printf("\n\nThoat chuong trinh\n");
         system("pause");
         break;
      }
      system("cls");

      switch (option)
      {
      //>9, Ghi thong tin ra file nhi phan
      case 9:
      {
         if (L == NULL)
         {
            textColor(12);
            printf("Chua nhap thong tin sinh vien");
         }
         else
         {
            getchar();
            ghiFileNhiPhan(L);
         }
      }
      break;

      //>8, Ghi thong tin ra file van ban
      case 8:
      {
         if (L == NULL)
         {
            textColor(12);
            printf("Chua nhap thong tin sinh vien");
         }
         else
         {
            getchar();
            ghiFileText(L);
         }
      }
      break;
         //>5, Cap nhat thong tin sinh vien
      case 5:
      {
         if (L == NULL)
         {
            textColor(12);
            printf("Chua nhap thong tin sinh vien");
         }
         else
         {
            getchar();
            capNhat(L, a);
         }
      }
      break;
         //> 6, Sap xep danh sach sinh vien
      case 6:
      {
         if (L == NULL)
         {
            textColor(12);
            printf("Chua nhap thong tin sinh vien");
         }
         else
         {
            getchar();
            sapXepDSSV(L);
         }
      }
      break;
      //> 7, Xoa sinh vien bang ma
      case 7:
      {
         if (L == NULL)
         {
            textColor(12);
            printf("Chua nhap thong tin sinh vien");
         }
         else
         {
            getchar();
            xoaSV(&L, a);
         }
      }
      break;

      //> 4, Loc ra nhung sinh vien bi luu ban
      case 4:
      {
         if (L == NULL)
         {
            textColor(12);
            printf("Chua nhap thong tin sinh vien");
         }
         else
            locSV(&L, a);
      }
      break;

      //> 3, Tra cuu thong tin
      case 3:
      {
         if (L == NULL)
         {
            textColor(12);
            printf("Chua nhap thong tin sinh vien");
         }
         else
         {
            getchar();
            traCuu(L);
         }
      }
      break;

      //> 2, Xem danh sach sinh vien
      case 2:
      {
         if (L == NULL)
         {
            textColor(12);
            printf("Chua nhap thong tin sinh vien");
         }
         else
         {
            textColor(11);
            dinhDangCot();
            xemDSSV(L);
         }
      }
      break;

         //> 1, Nhap thong tin sinh vien
      case 1:
      {
         short select;
         do
         {
            system("cls");
            textColor(11);
            printf("NHAP THONG TIN SINH VIEN\n");
            textColor(7);
            printf("1, Nhap thong tin tu ban phim\n");
            printf("2, Nhap thong tin tu tep\n");
            printf("0, Quay lai menu chinh\n");
            textColor(11);
            printf("Moi ban chon chuong trinh (0 - 2): ");
            ct(&select, 2);
            textColor(7);

            if (select == 0)
            {
               textColor(14);
               printf("\n\nQuay lai menu chinh");
               break;
            }
            switch (select)
            {
            case 1:
            {
               system("cls");
               getchar();
               nhapTuBanPhim(&L, a);
            }
            break;
            case 2:
            {
               getchar();
               nhapTuFile(&L, a);
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
   free(a);
   giaiPhong(&L);
   return 0;
}