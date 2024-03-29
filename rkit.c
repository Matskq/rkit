#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

int udv = 0;
gen_string(int value);
gen_msg(void);
handle_clip(const char *x);
gen_iss();


int main (int argc, char **argv){
  
  int p;
  int c;

  opterr = 0;
  optopt = 0;

  while ((c = getopt (argc, argv, "dgix:")) != -1)
    switch (c){
      case 'd':
        udv = 1;
        gen_string(20);
        break;
      case 'g':
        udv = 2;
        gen_msg();
        break;
      case 'x':
        udv = 3;
        int gen_num = atoi(optarg);
        if(gen_num > 999999){
          printf("value cannot be over '999999'\n");
          return 1;
        }
        gen_string(gen_num);
        break;
      case 'i':
        udv = 4;
        gen_iss();
        break;
      case '?':
      if (isprint (optopt)){
          fprintf (stderr, ":-%c\n", optopt);
      } else{
          fprintf(stderr, ": `\\x%x'.\n", optopt);
      }
      return 1;
      default:abort();
    }
    for (p = optind; p < argc; p++){
      printf (": %s\n", argv[p]);
    }
    if(udv == 0){
      printf("Nothing to generate\n");
      return 0;
    }
  return 0;
}

const char set_a[] = "abcdefghijklmnopqrstuvwxyz0123456789";
const char set_b[] = "ABCEFGHIJKLMNOPQRSTUVWXYZ";
const char set_c[] = "0123456789";
int rset(int n) { return rand() % n; }

char *gen_r(int y) {
  char *x = malloc((y + 1) * sizeof(char));
  int i;
  for (i = 0; i < y; i++) {
    if (udv == 1){
      x[i] = set_a[rset(strlen(set_a))];
    } else if (udv == 2){
      x[i] = set_b[rset(strlen(set_b))];
    } else {
      x[i] = set_a[rset(strlen(set_a))];
    }
  }
  x[y] = '\0';
  return x;
}

int gen_string(int value){

  srand(time(NULL));
  char *x;
  x = gen_r(value);

  handle_clip(x);

  printf("%s\n", x);
  free(x);
}

char current_date[100];

int gen_msg(void){
  time_t now = time(NULL);
  struct tm *t = localtime(&now);
  strftime(current_date, sizeof(current_date)-1, "%y%m%d", t);

  char *final_string;

  srand(time(NULL));
  char *x;
  x = gen_r(4);

  if((final_string = malloc(strlen(x)+strlen(current_date)+1)) != NULL){
      final_string[0] = '\0';
      strcat(final_string, x);
      strcat(final_string, current_date);
  } else {
    return 1;
  }
  handle_clip(final_string);
  printf("%s\n", final_string);
  free(x);
  free(final_string);
}

int gen_iss(void){

    srand(time(NULL));

    char *y = "AC-";
    char *r;
    int x = 100 + rand() % 9000 - 1000;

    char z[5];
    sprintf(z, "%d", x);

    if((r = malloc(strlen(y)+strlen(z)+1)) != NULL){
        r[0] = '\0';
        strcat(r, y);
        strcat(r, z);
    } else {
        return 1;
    }

    handle_clip(r);
    printf("%s\n", r);
    free(r);
}

int handle_clip(const char *x) {

  const char s[] = "echo '%s' | pbcopy";
  char m[strlen(x) + strlen(s) - 1];
  sprintf(m ,s, x);

  return system(m);
}
