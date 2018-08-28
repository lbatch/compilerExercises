#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

int main()
{
  FILE *fptr;
  FILE *nptr;
  char *read;
  size_t n = 0;
  int c;
  int state;

  fptr = fopen("commented.c", "r");
  nptr = fopen("capCommented.c", "w");
 
  if(fptr == NULL || nptr == NULL)
  {
    perror("File not found");
    return 1;
  }

  read = calloc(1000, sizeof(char));
  state = 1;

  while((c = fgetc(fptr)) != EOF && state >= 1 && state <= 4)
  {
    switch(state) {

      case 1:

        read[n] = c;
        n++;

        if(n > 999)
        {
          fputs(read, nptr);
          memset(read, 0, strlen(read));
          n = 0;
        }

        if(c == '/')
        {
          state = 2;
        }
        else state = 1;

        break;

      case 2:
          
        read[n] = c;
        n++;

        if(n > 999)
        {
          fputs(read, nptr);
          memset(read, 0, strlen(read));
          n = 0;
        }

        if(c == '*')
        {
          state = 3;
        }
        else state = 1;
      
        break;
     
      case 3:
        c = toupper(c);

        read[n] = c;
        n++;

        if(n > 999)
        {
          fputs(read, nptr);
          memset(read, 0, strlen(read));
        }

        if(c == '*')
        {
          state = 4;
        }
        else state = 1;
      case 4:
        c = toupper(c);
        if(c == '/')
        {
          read[n] = c;
          n++;

          if(n > 999)
          {
            fputs(read, nptr);
            memset(read, 0, strlen(read));
            n = 0;
          }
          state = 1;
        }
        else if(c == '*')
        {
          state = 4;
        }
        else state = 3;
        break;
      }
    }
    fputs(read, nptr);
    free(read);
    fclose(nptr);
}
