#include <stdio.h>
#include "console.h"

int main(int argc, char*argv[]) {

    if (screen_init(40, 80)) {
        screen_refresh();
        getchar();
        screen_fini();
    }
    else
    {
        printf("Screen Fail to Initialize\n");
    }

    return 0;
}
