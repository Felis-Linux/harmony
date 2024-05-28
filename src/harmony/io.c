#define _GNU_SOURCE
#include <fcntl.h>
#include <unistd.h>

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <libgen.h>
#include <string.h>
#include <errno.h>


#include <sys/io.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/random.h>

#include <curl/multi.h>
#include <curl/easy.h>
#include <curl/curl.h>

#include "io.h"

#define HARMONY_TEMPDIRS_DIR "/var/tmp/harmony"
#define HARMONY_LOCKFILE_DIR "/var/lock/harmony"

os_fd_t hmnlock(char *path) {
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
  if(access(lockpath, F_OK) != 0)
      error(GENERIC_ERR, "%s: file already locked", path);
  
  os_fd_t ret;
  if((ret = open(lockpath, O_RDWR | O_CREAT | O_TMPFILE, S_IRWXU)) != 0)
      error(errno % GENERIC_ERR, "system: %s", strerror(errno));
  free(lockpath);

  return ret;
}

os_fd_t mkhmntmp() {
  char random_bytes[32];
  getrandom(random_bytes, 32, GRND_RANDOM | GRND_NONBLOCK);
  char *dirp = smprintf("%s/art_%s", HARMONY_TEMPDIRS_DIR, random_bytes);
  mkdir(dirp, S_IRWXU);
  os_fd_t ret;
  if((ret = open(dirp, O_DIRECTORY | O_TMPFILE, S_IRWXU)) != 0)
      error(errno % GENERIC_ERR, "system: %s", strerror(errno));
  free(dirp);
  return ret; 
}

char *hmnstrcut(char *str, size_t from, size_t to, int *return_code) {
  size_t cut_strlen = from - to;
  str += from;
  char *ret = NULL;

  if(str[0] == '\0') 
      return ret;

  if((! (from < to)) && to > strlen(str)) {
    *return_code = HMN_STRING_TOO_LONG;
    return ret;
  }
  
  ret = malloc(cut_strlen*sizeof(char));
  strncpy(ret, str, cut_strlen);
  return ret;
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

[[gnu::format(printf, 1, 2)]] void ok(const char *format, ...) {
  va_list args;
  va_start(args, format);

  char *formatted_str = smprintf(format, args);
  printf("%s==%s>%s %s", COLOR_BOLD_MAGENTA, COLOR_BOLD_BLUE, COLOR_RESET, formatted_str);

  va_end(args);
}

[[gnu::format(printf, 1, 2)]] void warning(const char *format, ...) {
  va_list args;
  va_start(args, format);
  char *formatted_str = smprintf(format, args);
  fprintf(stderr, "%s[%sWRN%s]:\n\t %s==%s>%s %s",
      COLOR_BOLD_WHITE, COLOR_BOLD_ORANGE,
      COLOR_BOLD_WHITE, COLOR_BOLD_ORANGE,
      COLOR_BOLD_YELLOW, COLOR_RESET,
      formatted_str
      );
}

[[gnu::format(printf, 2, 3)]] void error(int ecode, const char *format, ...) {
  va_list args;
  va_start(args, format);
  char *formatted_str = smprintf(format, args);
  fprintf(stderr, "%s[%sERR%s]:\n\t %s==%s>%s %s", 
      COLOR_BOLD_WHITE, COLOR_BOLD_RED,
      COLOR_BOLD_WHITE, COLOR_BOLD_YELLOW, 
      COLOR_BOLD_ORANGE, COLOR_RESET, formatted_str
      );
  free(formatted_str);
  exit(ecode);
}
