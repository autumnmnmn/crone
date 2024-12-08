
#include <stdio.h>
#include <stdlib.h>

#define CRASH(x) do { fputs(x "\n", stderr); exit(1); } while (false)

#define __SCREAM(x) fputs(#x "\n", stderr)

#define FOO __SCREAM(FOO)
#define BAR __SCREAM(BAR)
#define BAZ __SCREAM(BAZ)


