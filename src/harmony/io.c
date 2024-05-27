#define _GNU_SOURCE
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#include <sys/io.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/random.h>

#include "io.h"

#define HARMONY_LOCKFILE_DIR "/var/lock/harmony"

os_fd_t *leased_fds = NULL;
size_t  leased_fds_s = 0;

os_fd_t lockByPath(const char *path) {
  const char *filename = basename(path);
  if(access(HARMONY_LOCKFILE_DIR, W_OK) != 0) {
#ifdef DEBUG2
    ok("DEBUG: creating directory %s with permissions 700", HARMONY_LOCKFILE_DIR);
#endif
    // S_IRWXU (0o700) only to avoid tampering with lockfiles (we don't like that!)
    mkdir(path, S_IRWXU);
  }
  char *lockpath = smprintf("%s/%s", HARMONY_LOCKFILE_DIR, filename);

#ifdef DEBUG2
  ok("DEBUG: creating file %s with permissions 700", lockpath);
#endif
  os_fd_t ret = open(lockpath, O_TMPFILE | O_RDONLY, S_IRWXU);
  free(lockpath);

  leased_fds = realloc(leased_fds, ++leased_fds_s*(sizeof(os_fd_t)));
  leased_fds[leased_fds_s] = ret;
  return ret;
}

void unlockAll() {
  for(size_t i = 0; i < leased_fds_s; ++i) {
    close(leased_fds[i]);
  }
  free(leased_fds);
}

[[gnu::format(printf, 1, 2)]] char *smprintf(const char *format, ...) {
  va_list args;
  va_start(args, format);
  
  size_t len = vsnprintf(NULL, 0, format, args);
  char *buf = malloc(sizeof(char)*(len + 1));
  vsnprintf(buf, len + 1, format, args);
  va_end(args);
  return buf;
}

[[gnu::format(printf, 2, 3)]] void error(int ecode, const char *format, ...) {
  unlockAll();
  va_list args;
  va_start(args, format);
  char *formatted_str = smprintf(format, args);
  fprintf(stderr, "%s[%sERR%s]:\n %s==%s>%s %s", 
      COLOR_BOLD_WHITE, COLOR_BOLD_RED,
      COLOR_BOLD_WHITE, COLOR_BOLD_YELLOW, 
      COLOR_BOLD_ORANGE, COLOR_RESET, formatted_str
      );
  free(formatted_str);
  exit(ecode);
}
