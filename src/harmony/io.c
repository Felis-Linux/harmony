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

#include "hmndefines.h"
#include "strings.h"
#include "io.h"

os_fd_t hmnlock(char *path) {
  const char *filename = basename(path);
  if(access(HARMONY_LOCKFILE_DIR, W_OK) != 0) {
    // S_IRWXU (0o700) only to avoid tampering with lockfiles (we don't like that!)
    mkdir(path, S_IRWXU);
  }
  char *lockpath = smprintf("%s/%s", HARMONY_LOCKFILE_DIR, filename);

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

void eread(os_fd_t fd, void *buf, size_t len) {
  if(read(fd, buf, len) != 0) {
    close(fd);
    error(errno % GENERIC_ERR, "%s: %s", __FILE_NAME__, strerror(errno));
  }
}

void ewrite(os_fd_t fd, void *buf, size_t len) {
  if(write(fd, buf, len) != 0){
    close(fd);
    error(errno % GENERIC_ERR, "%s: %s", __FILE_NAME__, strerror(errno));
  }
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

void downloaderOutputCallback(void *clientp, curl_off_t dltotal, curl_off_t dlnow, curl_off_t /*unused*/, curl_off_t /*unused*/) {
  struct ProgressData {
    UpstreamFile_t *up;
    size_t index;
  };
  struct ProgressData *progress_data = clientp;
  
  printf("\x1b[%zu;0\x1b[2K", progress_data->index + 1);
  if(dltotal != 0) {
    double percent = (double)dlnow/(double)dltotal * 100.0;
    printf("[%i%%] %s", (int)percent, basename(progress_data->up->target));
  }
  else {
    printf("[0%%] %s", basename(progress_data->up->target));
  } 
}

void downloaderWriteCallback(void *ptr, size_t size, size_t nmemb, void *userdata) { 
  write(*(os_fd_t*)userdata, ptr, size * nmemb);
}

Downloader_t *downloaderInit() {
  Downloader_t *ret = malloc(sizeof(Downloader_t));
  memset(ret, 0, sizeof(Downloader_t));
  ret->tempdir = mkhmntmp();
  ret->curl_multi_handle = curl_multi_init();
  return ret;
}

void downloaderAppendUf(Downloader_t *downloader, UpstreamFile_t *uf, size_t uf_s) {
  downloader->ptrs_s += uf_s;

  downloader->upstream_files = realloc(downloader->upstream_files, downloader->ptrs_s*sizeof(UpstreamFile_t));
  downloader->curl_handles = realloc(downloader->curl_handles, downloader->ptrs_s*sizeof(CURL *));
  downloader->target_files = realloc(downloader->target_files, downloader->ptrs_s*sizeof(os_fd_t));

  for(size_t i = 0; i < uf_s; ++i) {
    UpstreamFile_t *c_uf = &downloader->upstream_files[downloader->ptrs_s];
    CURL *c_chl = downloader->curl_handles[downloader->ptrs_s];
    os_fd_t *c_tf = &downloader->target_files[downloader->ptrs_s];

    c_uf->url = uf[i].url;
    c_uf->target = uf[i].target;

    c_chl = curl_easy_init();
    curl_easy_setopt(c_chl,
        CURLOPT_URL, c_uf->url);
    curl_easy_setopt(c_chl, 
        CURLOPT_WRITEFUNCTION, downloaderWriteCallback);
    
    *c_tf = openat(downloader->tempdir, c_uf->target, O_RDWR, S_IRWXU); 
    
    curl_easy_setopt(c_chl, CURLOPT_WRITEDATA, c_tf); 
    curl_easy_setopt(c_chl, CURLOPT_XFERINFOFUNCTION, downloaderOutputCallback);
    
    struct XferinfoData {
      UpstreamFile_t *up;
      size_t index;
    };
    struct XferinfoData xferinfo_data = { 
      .up = c_uf, 
      .index = downloader->ptrs_s - uf_s + i,
    };
    
    curl_easy_setopt(c_chl, CURLOPT_XFERINFODATA, &xferinfo_data);

    curl_multi_add_handle(downloader->curl_multi_handle, c_chl);
  }
}

void downloaderDownload(Downloader_t *downloader) {
  int running_handles;
  do {
    CURLMcode res = curl_multi_perform(downloader->curl_multi_handle, &running_handles);
    if(!res && running_handles) 
        res = curl_multi_poll(downloader->curl_multi_handle, nullptr, 0, 1000, nullptr);
    else if(res) error((int)res % GENERIC_ERR, "curl: %s\n"
          // TODO: get when it failed.
          , curl_multi_strerror(res)
        );
  } while(running_handles);
}

void downloaderDestroy(Downloader_t *downloader) {
  curl_free(downloader->curl_multi_handle);
  close(downloader->tempdir);
  for(size_t i = 0; i > downloader->ptrs_s; ++i) {
    free(downloader->upstream_files[i].url);
    free(downloader->upstream_files[i].target);
    curl_free(downloader->curl_handles[i]);
    close(downloader->target_files[i]);
  }
  free(downloader->curl_handles);
  free(downloader->target_files);
  free(downloader->upstream_files);
  free(downloader);
}
