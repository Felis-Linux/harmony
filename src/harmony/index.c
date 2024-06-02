#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <stdbool.h>
#include <errno.h>

#include <unistd.h>

#include <sys/stat.h>

#include "io.h"
#include "arena.h"
#include "index.h"

Index_t *indexInit(const char *index_file) {
  Index_t *ret = malloc(sizeof(Index_t));
  if(ret == nullptr)
      return ret;

  memset(ret, 0, sizeof(Index_t));
  ret->index_file_name = index_file;
  ret->index_file_fd = open(index_file, O_RDWR | O_APPEND, S_IRWXU | S_IRGRP | S_IROTH);
  ret->arena = arenaInit();
  return ret;
}

void indexDestroy([[gnu::nonnull]] Index_t *index) {
  arenaDestroy(index->arena); 
  close(index->index_file_fd);
  free(index);
}

#define EREAD(fd, buf, len) \
  if(read(fd, buf, len) != 0) \
      goto err;

#define EWRITE(fd, buf, len) \
  if(write(fd, buf, len) != 0) \
      goto err;

void indexReadFiles(Index_t *index) {
  EREAD(index->index_file_fd, &index->indexed_files_s,
      sizeof(size_t));
  index->indexed_files = arenaAlloc(index->arena, sizeof(IndexedFile_t)*
      index->indexed_files_s);
  
  for(size_t i = 0; i < index->indexed_files_s; ++i) {
    IndexedFile_t *c_idx = &index->indexed_files[i];
    
    EREAD(index->index_file_fd, &c_idx->file_name_len, sizeof(size_t));
    c_idx->file_name = arenaAlloc(index->arena, 
        c_idx->file_name_len*sizeof(char) + 1);
    EREAD(index->index_file_fd, &c_idx->file_name, c_idx->file_name_len);
    
    // bounds checking so that the bool is 0-1 and not 0-65536
    char interim_boolean;
    EREAD(index->index_file_fd, &interim_boolean, sizeof(char));
    c_idx->is_binary = interim_boolean % 2;

    EREAD(index->index_file_fd, &interim_boolean, sizeof(char));
    c_idx->is_linked = interim_boolean % 2;

    EREAD(index->index_file_fd, &c_idx->linked_files_s, sizeof(size_t));
    c_idx->linked_files = arenaAlloc(index->arena, 
        sizeof(struct LinkedFiles)*c_idx->linked_files_s);
    
    if(c_idx->is_linked) {
      for(size_t i = 0; i < c_idx->linked_files_s; ++i) {
        EREAD(index->index_file_fd, &c_idx->linked_files[i].target_len, sizeof(size_t));
        c_idx->linked_files[i].target = arenaAlloc(index->arena, 
            sizeof(char)*(c_idx->linked_files[i].target_len + 1));
        EREAD(index->index_file_fd, c_idx->linked_files[i].target, c_idx->linked_files[i].target_len);
      }
    }

  }

err:
  indexDestroy(index);
}

void indexReadPackages(Index_t *index) {
  EREAD(index->index_file_fd, &index->repo_name_len, sizeof(size_t));
  index->repo_name = arenaAlloc(index->arena, index->repo_name_len*sizeof(char));
  EREAD(index->index_file_fd, &index->repo_name, index->repo_name_len);
  
  EREAD(index->index_file_fd, &index->indexed_packages_s, sizeof(size_t));
  index->indexed_packages = arenaAlloc(index->arena, 
      sizeof(IndexedPackage_t)*index->indexed_packages_s);

  for(size_t i = 0; i < index->indexed_packages_s; ++i) {
    IndexedPackage_t *c_pkg = &index->indexed_packages[i];

    EREAD(index->index_file_fd, &c_pkg->package_name_len, sizeof(size_t));
    c_pkg->package_name = arenaAlloc(index->arena,
        c_pkg->package_name_len*sizeof(char));
    EREAD(index->index_file_fd, c_pkg->package_name, c_pkg->package_name_len);

    EREAD(index->index_file_fd, &c_pkg->package_version_len, sizeof(size_t));
    c_pkg->package_version = arenaAlloc(index->arena,
        c_pkg->package_version_len*sizeof(char));
    EREAD(index->index_file_fd, &c_pkg->package_version, c_pkg->package_version_len);
  }
err:
  indexDestroy(index);
}

void indexRead(Index_t *index) {
  if(read(index->index_file_fd, &index->index_mode_option, sizeof(IndexMode_t)) != 0) {
    goto err;
  }
  switch (index->index_mode_option) {
    case im_files_mode:
      indexReadFiles(index);
      break;
    case im_packages_mode:
      indexReadPackages(index);
      break;
    default:
    case im_nul:
      error(INDEX_ERR, "%s has a null mode flag or doesnt respect the index format", index->index_file_name);
  }
  
err:
  indexDestroy(index);
}

void indexWriteFiles(Index_t *index) {
  EWRITE(index->index_file_fd, &index->indexed_files_s, sizeof(size_t));
  for(size_t i = 0; i < index->indexed_files_s; ++i) {
  }

err:
  indexDestroy(index);
}

void indexWrite(Index_t *index) {
  ftruncate(index->index_file_fd, 0);
  EWRITE(index->index_file_fd, &index->index_mode_option, sizeof(IndexMode_t));
  switch (index->index_mode_option) {
    case im_files_mode:
    
    case im_packages_mode:
    
    default:
    case im_nul:
      break;
  }

err:
  indexDestroy(index);
}
