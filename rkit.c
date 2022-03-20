#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

int udv = 0;

int main (int argc, char **argv){
  
  int p;
  int c;

  opterr = 0;
  optopt = 0;

  while ((c = getopt (argc, argv, "dgx:")) != -1)
    switch (c){
      case 'd':
        udv = 1;
        gen_string(20);
        break;
      case 'g':
        udv = 2;
        gen_string(10);
        break;
      case 'x':
        udv = 3;
        int gen_num = atoi(optarg);
        gen_string(gen_num);
        break;
      case '?':
      if (isprint (optopt)){
          fprintf (stderr, "( -%c ) Unexpected\n", optopt);
      } else{
          fprintf(stderr, "Unexpected option `\\x%x'.\n", optopt);
      }
      return 1;
      default:abort();
    }
    for (p = optind; p < argc; p++){
      printf ("Non-option argument %s\n", argv[p]);
    }
    if(udv == 0){
      printf("Nothing to generate\n");
      return 0;
    }
  return 0;
}

const char set_a[] = "abcdefghijklmnopqrstuvwxyz0123456789";
const char set_b[] = "ABCEFGHIJKLMNOPQRSTUVWXYZ0123456789";
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

int handle_clip(const char *x) {

  const char s[] = "echo '%s' | pbcopy";
  char m[strlen(x) + strlen(s) - 1];
  sprintf(m ,s, x);

  return system(m);
}