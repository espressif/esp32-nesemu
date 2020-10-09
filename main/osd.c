/* vim: set tabstop=3 expandtab:
**
** This file is in the public domain.
**
** osd.c
**
** $Id: osd.c,v 1.2 2001/04/27 14:37:11 neil Exp $
**

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <signal.h>
#include <unistd.h>

#include "log.h"
#include "nofrendo.h"
#include "noftypes.h"
#include "osd.h"
#include "version.h"

#include "nofconfig.h"

char configfilename[] = "na";

/* This is os-specific part of main() */
int osd_main(int argc, char *argv[])
{
   config.filename = configfilename;

   return main_loop(ROM_FILE, system_autodetect);
}

/* File system interface */
void osd_fullname(char *fullname, const char *shortname)
{
   strncpy(fullname, shortname, PATH_MAX);
}

/* This gives filenames for storage of saves */
char *osd_newextension(char *string, char *ext)
{
   // dirty: assume extension is 3 characters
   size_t l = strlen(string);
   string[l - 3] = ext[1];
   string[l - 2] = ext[2];
   string[l - 1] = ext[3];

   return string;
}

/* This gives filenames for storage of PCX snapshots */
int osd_makesnapname(char *filename, int len)
{
   return -1;
}
