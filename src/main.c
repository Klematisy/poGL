#include <stdio.h>
#include "vladlib/vladlib.h"
#include "game.h"

int main(void)
{
    vl_init_lib(640, 640, "VLAd");

    game_run();

    vl_deinit_lib();
    return 0;
}