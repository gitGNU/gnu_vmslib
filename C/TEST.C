#include <stdio.h>

main (int argc, char **argv)
{
  int i;
  for (i = 0; i < argc; i++) {
    printf ("stdout: main line %3d :- argv[%2d] = %s\n", __LINE__, i, argv[i]);
  }
  hackargv (&argc, &argv);
  printf ("stdout: main line %3d :- argc == %d\n", __LINE__, argc);
  for (i = 0; i < argc; i++) {
    printf ("stdout: main line %3d :- argv[%2d] = %s\n", __LINE__, i, argv[i]);
    fprintf (stderr, "stderr: main line %3d :- argv[%2d] = %s\n", __LINE__, i, argv[i]);
  }
}
