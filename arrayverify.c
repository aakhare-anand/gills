#include <stdio.h>

typedef struct node_ {
   int *nums;
   char **strarray;
} node_t;

// node_t node = { {"abc"} , { "def" }, { "gfi" } };

 static char * const a = "abc";
 static char * const b = "def";
 static char * const c = "gfi";

static char *d[] = { "abc", "def", "gfi" };
static int nums[] = { 1, 2, 3 };

static node_t node = { nums, d };

main()
{
    int i;

    for (i = 0; i < 3; i++) {
        printf("%d  %s \n", node.nums[i], node.strarray[i]);
    }
}


