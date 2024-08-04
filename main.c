#include <stdio.h>
#include "sprider-chatapp.h"

int main(int argc, char** argv) {
    sprider_t sprider_app = {0};
    sprider_init();
    sprider_createapp(&sprider_app);
    sprider_run(&sprider_app);
    sprider_destroyapp(&sprider_app);
    return 0;
}