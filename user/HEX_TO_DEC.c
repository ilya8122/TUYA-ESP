

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include "HEX_TO_DEC.h"

char *itoa(int number, char *destination, int base) {
  int count = 0;
  do {
    int digit = number % base;
    destination[count++] = (digit > 9) ? digit - 10 +'A' : digit + '0';
  } while ((number /= base) != 0);
  destination[count] = '\0';
  int i;
  for (i = 0; i < count / 2; ++i) {
    char symbol = destination[i];
    destination[i] = destination[count - i - 1];
    destination[count - i - 1] = symbol;
  }
  return destination;
}

 double pow(double number, double pow)
        {
int i11;
            double result = 1;
            for (i11 = 0; i11 < pow; i11++)
            {
                result *= number;
            }
            return result;
        }

int HEX_TO_DEC(char st[100])
{
    int i, s, k, p;
    s = 0;
    p = strlen(st) - 1;
    for (i = 0; st[i] != '\0'; i++)
    {
        switch (toupper(st[i]))
        {
            case 'A':
                k = 10;
                break;
            case 'B':
                k = 11;
                break;
            case 'C':
                k = 12;
                break;
            case 'D':
                k = 13;
                break;
            case 'E':
                k = 14;
                break;
            case 'F':
                k = 15;
                break;
            case '1':
                k = 1;
                break;
            case '2':
                k = 2;
                break;
            case '3':
                k = 3;
                break;
            case '4':
                k = 4;
                break;
            case '5':
                k = 5;
                break;
            case '6':
                k = 6;
                break;
            case '7':
                k = 7;
                break;
            case '8':
                k = 8;
                break;
            case '9':
                k = 9;
                break;
            case '0':
                k = 0;
                break;
        }
        s = s + k * pow(16, p);
        p--;
    }
  //  PR_DEBUG("\n %d", s) ;
    return s;
}




int get_value_3(int arg1,int arg2,int arg3)
{
char first[30];
char second[30];
char third[30];

char buf[100];
char result[100];

itoa (arg1, first, 16);
itoa (arg2, second, 16);
itoa (arg3, third, 16);


//PR_DEBUG(" %s %s %s ",first,second,third);
snprintf(buf, sizeof buf, "%s%s", first, second);
snprintf(result, sizeof result, "%s%s", buf, third);
//PR_DEBUG("\n %s \n %s",buf,result);

int n = HEX_TO_DEC(result);
//PR_DEBUG("\n %d",n);

return n;
}

int get_value_2(int arg1,int arg2)
{
char first[30];
char second[30];


char buf[100];


itoa (arg1, first, 16);
itoa (arg2, second, 16);

snprintf(buf, sizeof buf, "%s%s", first, second);

int n = HEX_TO_DEC(buf);
//PR_DEBUG("\n %d",n);

return n;
}





