#include <stdio.h>
main (int argc, char **argv)
{
  FILE *junk;
  int c;
  char line[513];

  hackargv (&argc, &argv);
  while (gets(line)) {
    printf("echo: ");
    puts(line);
  }
  printf ("echo completed\n");
}
