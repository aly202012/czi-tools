
#include "config.h"

#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <err.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

#include "types.h"
#include "macros.h"
#include "mapfile.h"

#define DEV_ZERO "/dev/zero"

/* portable (if hacky) function which does something similar to
 * posix_fallocate(3). for best results, truncate the file named by fd to zero
 * bytes beforehand by opening with O_TRUNC or using ftruncate(2) */
int xfallocate(int fd, size_t sz) {
    static int zerofd = -1;
    struct map_ctx *ctx;

    if (sz == 0)
        return fwarnx1("invalid argument, size parameter is set to zero"), -1;
    
    if (zerofd == -1) {
        if ((zerofd = open(DEV_ZERO, O_RDONLY)) == -1)
            return fwarn1("could not open " DEV_ZERO " for reading"), -1;
    }

    if ((ctx = map_mopen(DEV_ZERO, zerofd, sz, PROT_READ)) == NULL)
        return fwarnx1("could not map " DEV_ZERO " into memory"), -1;

    if (map_dwrite(ctx, fd, sz) == -1) {
        fwarnx1("could not write to output file");
        map_mclose(ctx);
        return -1;
    }

    map_mclose(ctx);
    return 0;
}