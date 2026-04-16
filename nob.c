#define NOB_IMPLEMENTATION
#include "nob.h"

#define BUILD_FOLDER "build/"

int main(int argc, char **argv)
{
    GO_REBUILD_URSELF(argc, argv);

    bool run = false;

    const char *program_name = shift(argv, argc);
    while (argc > 0) {
        const char *flag = shift(argv, argc);
        if (strcmp(flag, "-run") == 0) {
            run = true;
        } else {
            nob_log(ERROR, "Usage: %s [-run]", program_name);
            nob_log(ERROR, "Unknown flag `%s`", flag);
            return 1;
        }
    }

    if (!mkdir_if_not_exists(BUILD_FOLDER)) return 1;

    Cmd cmd = {0};
    cmd_append(&cmd, "clang");
    cmd_append(&cmd, "-Wall");
    cmd_append(&cmd, "-Wextra");
    cmd_append(&cmd, "-Wswitch-enum");
    cmd_append(&cmd, "-fsanitize=memory,undefined");
    cmd_append(&cmd, "-ggdb");
    cmd_append(&cmd, "-o", BUILD_FOLDER"main");
    cmd_append(&cmd, "main.c");
    if (!cmd_run(&cmd)) return 1;

    if (run) {
        cmd_append(&cmd, BUILD_FOLDER"main");
        if (!cmd_run(&cmd)) return 1;
    }

    return 0;
}
