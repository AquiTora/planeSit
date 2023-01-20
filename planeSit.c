#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define PLANE 4
#define SEAT 12
#define NAME_SIZE 40

struct pass {
    char fName[NAME_SIZE];
    char sName[NAME_SIZE];
};
struct plane {
    bool st;
    bool conf;
    int id;
    struct pass name;
};

void freeSpaceCount(struct plane sam[][SEAT], int size, int num);    //Показать свободные места
void freeSpaceList(struct plane sam[][SEAT], int size, int num);     //Показывает список свободных мест
void occAlf(struct plane sam[][SEAT], int size, int num);            //Список забранированных мест в алфавитном порядке
void toBook(struct plane sam[][SEAT], int size, int num);            //Забранировать место для пассажира
void noBook(struct plane sam[][SEAT], int size, int num);            //Снять бронь
void confirmation(struct plane sam[][SEAT], int size, int num);      //Подтвердить бронь
void planeMenu(void);
void menu(void);
char * s_gets(char * st, int n);

int main(void)
{
    int f = 0;
    int s = 0;
    int pl;
    int flight;
    FILE * fp;
    char choice;
    struct plane ColAir[PLANE][SEAT] = {{{0, 0, 0, {"", ""}}}};
    int stSize = sizeof(struct plane);

    if((fp = fopen("14_9.dat", "r+b")) == NULL)
    {
        puts("Не удается открыть файл.");
        exit(1);
    }

    //for(f = 0; f < PLANE; f++)
    //while(s < PLANE && fread(&ColAir[s], stSize, 1, fp) == 1)
      //s++;
    fread(ColAir, stSize, PLANE * SEAT, fp);

    if(ColAir[f][1].id < 1)
      for(f = 0; f < PLANE; f++)
        for(s = 0; s < SEAT; s++)
          ColAir[f][s].id = s;

    puts("Выберите номер рейса(e, чтобы выйти):");
    planeMenu();
    while((scanf("%c", &choice)) > 0)
    {
        while(getchar() != '\n')
          continue;

        switch(choice)
        {
            case 'a':
              pl = 0;
              flight = 102;
              break;
            case 'b':
              pl = 1;
              flight = 311;
              break;
            case 'c':
              pl = 2;
              flight = 444;
              break;
            case 'd':
              pl = 3;
              flight = 519;
              break;
        }
        if(choice == 'e')
          break;

        puts("Для выбора функции введите ее буквенную метку:");
        menu();
        while((scanf("%c", &choice)) > 0 && choice != 'g')
        {
            while(getchar() != '\n')
              continue;

            switch(choice)
            {
                case 'a':
                  freeSpaceCount(ColAir, pl, flight);    //Показать свободные места
                  break;
                case 'b':
                  freeSpaceList(ColAir, pl, flight);     //Показывает список свободных мест
                  break;
                case 'c':
                  occAlf(ColAir, pl, flight);            //Список забранированных мест в алфавитном порядке
                  break;
                case 'd':
                  toBook(ColAir, pl, flight);           //Забранировать место для пассажира
                  break;
                case 'e':
                  noBook(ColAir, pl, flight);           //Снять бронь
                  break;
                case 'f':
                  confirmation(ColAir, pl, flight);     //Подтвердить бронь
                  break;
            }
            menu();
        }

        while(getchar() != '\n')
          continue;
        planeMenu();
    }

    rewind(fp);
    //for(f = 0; f < PLANE; f++)
    fwrite(ColAir, stSize, PLANE * SEAT, fp);

    puts("Всего доброго.");
    fclose(fp);

    return 0;
}

void freeSpaceCount(struct plane sam[][SEAT], int size, int num)
{
    int f;
    int count = 0;

    for(f = 0; f < SEAT; f++)
      if(sam[size][f].st < 1)
        count++;

    printf("На рейс №%d\n", num);
    printf("Количество свободных мест = %d\n", count);
}
void freeSpaceList(struct plane sam[][SEAT], int size, int num)
{
    int f;
    int count;

    printf("Список свободных мест на рейсе №%d:\n", num);
    for(f = 0; f < SEAT; f++)
      if(sam[size][f].st < 1)
        printf("Место %d\n", sam[size][f].id);
}
void occAlf(struct plane sam[][SEAT], int size, int num)
{
    int f;
    int s;
    struct plane buf;

    for(f = 0; f < SEAT; f++)
    {
        if(sam[size][f].st < 1)
          continue;

        for(s = f + 1; s < SEAT; s++)
        {
            if(sam[size][s].st < 1)
              continue;

            if((strcmp(sam[size][f].name.sName, sam[size][s].name.sName)) > 1)
            {
                buf = sam[size][f];
                sam[size][f] = sam[size][s];
                sam[size][s] = buf;
            }
        }
    }

    puts("Список занятых мест в алфавитном порядке:");
    for(f = 0; f < SEAT; f++)
    {
        if(sam[size][f].st > 0)
        {
            printf("Место %d. Занято: %s %s. ", sam[size][f].id, sam[size][f].name.sName, sam[size][f].name.fName);
            if(sam[size][f].conf > 0)
              puts("Подтверждено.");
            else
              puts("Не подтверждено.");
        }
    }
}
void toBook(struct plane sam[][SEAT], int size, int num)
{
    int f;
    int choice;

    puts("Выберите место от 0 до 11(или введите q, чтобы выйти в меню):");
    while((scanf("%d", &choice)) > 0)
    {
        while(getchar() != '\n')
          continue;

        if(choice > 12 || choice < 0)
        {
            puts("Это место недоступно.");
            continue;
        }
        else if(sam[size][choice].st > 0)
        {
            puts("Это место занято. Выберите другое.");
            continue;
        }

        puts("Введите имя:");
        s_gets(sam[size][choice].name.fName, NAME_SIZE);
        puts("Введите фамилию:");
        s_gets(sam[size][choice].name.sName, NAME_SIZE);
        sam[size][choice].st = 1;
        sam[size][choice].id = choice;
        puts("Забронированно и ждет подтверждения.");

        break;
    }
}
void noBook(struct plane sam[][SEAT], int size, int num)
{
    int f;
    int choice;

    puts("Введите номер места с которого нужно снять бронь.");
    scanf("%d", &choice);

    for(f = 0; f < SEAT; f++)
    {
        if(sam[size][f].id == num)
        {
            sam[size][f].st = 0;
            sam[size][f].conf = 0;
        }
    }
}
void confirmation(struct plane sam[][SEAT], int size, int num)
{
    int f;
    int choice;

    puts("Выберите номер места для подтверждения(или q, чтобы выйти):");
    while((scanf("%d", &choice)) > 0)
    {
        if(choice > 12 || choice < 0)
          puts("Это место недоступно.");
        else if(sam[size][choice].conf > 0)
          puts("Это место уже подтверждено.");
        else if(sam[size][choice].st < 1)
          puts("Это место никем не занято.");
        else
        {
            puts("Место подтверждено.");
            sam[size][choice].conf = 1;
            break;
        }
    }
}

void planeMenu(void)
{
    puts("a) 102\n"
         "b) 311\n"
         "c) 444\n"
         "d) 519\n"
         "e) Выход");
}
void menu(void)
{
    puts("a) Показать количество свободных мест\n"
         "b) Показать список свободных мест\n"
         "c) Показать список забронированных мест в алфавитном порядке\n"
         "d) Забранировать место для пассажира\n"
         "e) Снять бронь с места\n"
         "f) Подтвердить бронь\n"
         "g) К выбору рейса");
}

char * s_gets(char * st, int n)
{
    char * ret_val;
    char * find;

    ret_val = fgets(st, n, stdin);

    if(ret_val)
    {
        find = strchr(st, '\n');
        if(find)
          *find = '\0';
        else
          while(getchar() != '\n')
            continue;
    }

    return ret_val;
}