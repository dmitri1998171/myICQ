#include "header.hpp"

void dieWithError(const char *device, const char *error_message) {
    printf("%s:: ERROR: %s\n", device, error_message);
    FILE *f = fopen("errors.log", "w+");
    fprintf(f, "%s - ERROR: %s\n", device, error_message);
    fclose(f);
    exit(1);
}