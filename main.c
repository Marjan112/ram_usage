#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

int main() {
    const char *path = "/proc/meminfo";

    FILE *file = fopen(path, "r");
    if (!file) {
        perror(path);
        return 1;
    }

    int64_t total_kib = 0, avail_kib = 0;
    bool total_loaded = false, avail_loaded = false;

    char line[128];
    while (fgets(line, sizeof(line), file) != NULL) {
        if (total_loaded && avail_loaded) {
            fclose(file);
            break;
        }

        if (sscanf(line, "MemTotal: %lld", &total_kib) == 1) total_loaded = true;
        else if (sscanf(line, "MemAvailable: %lld", &avail_kib) == 1) avail_loaded = true;
    }

    const double total_gib = total_kib / (double)(1024 * 1024);

    const int64_t using_kib = total_kib - avail_kib;
    const double using_mib = using_kib / 1024.0;
    const double using_gib = using_mib / 1024.0;

    const double percentage = (using_kib / (double)total_kib) * 100;

    if (using_mib >= 1024) printf("RAM: %.1lf GiB/%.1lf GiB (%.1lf%)\n", using_gib, total_gib, percentage);
    else printf("RAM: %.1lf MiB/%.1lf GiB (%.1lf%)\n", using_mib, total_gib, percentage);

    return 0;
}
