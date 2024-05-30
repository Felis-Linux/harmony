#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <stdbool.h>
#include <errno.h>

#include <unistd.h>

#include <sys/stat.h>

#include "io.h"
#include "index.h"

Index_t *indexInit(const char *index_file) {
  Index_t *ret = malloc(sizeof(Index_t));
  memset(ret, 0, sizeof(Index_t));
  ret->index_file_name = index_file;
  ret->index_file_fd = open(index_file, O_RDONLY, S_IRWXU | S_IRGRP | S_IROTH);
  return ret;
}

/* TODO: indexFree is not efficient at all, or is it? 
 * maybe use arena allocation, that would be significantly better.
 */

void indexFree(Index_t *index) {
  switch(index->index_mode_option){
    case im_files_mode:
      for(size_t i = 0; i < index->indexed_files_s; ++i) {
        IndexedFile_t c_idx = index->indexed_files[i];

        free(c_idx.file_name);
        
        for(size_t i = 0; i < c_idx.linked_files_s; ++i) {
          free(c_idx.linked_files[i].target);
        }
        free(c_idx.linked_files);
      }
      free(index->indexed_files);
      break;
    case im_packages_mode:
      free(index->repo_name);
      for(size_t i = 0; i < index->indexed_packages_s; ++i) {
        IndexedPackage_t c_pkg = index->indexed_packages[i];

        free(c_pkg.package_name);
        free(c_pkg.package_version);  
      }
      free(index->indexed_packages);
      break;
    case im_nul:
      break;
  }
  close(index->index_file_fd);
}

void indexReadFiles(Index_t *index) {
  eread(index->index_file_fd, &index->indexed_files_s,
      sizeof(size_t));
  index->indexed_files = malloc(sizeof(IndexedFile_t)*
      index->indexed_files_s);
  
  for(size_t i = 0; i < index->indexed_files_s; ++i) {
    IndexedFile_t *c_idx = &index->indexed_files[i];
    
    eread(index->index_file_fd, &c_idx->file_name_len, sizeof(size_t));
    c_idx->file_name = malloc(c_idx->file_name_len*sizeof(char) + 1);
    eread(index->index_file_fd, &c_idx->file_name, c_idx->file_name_len);
    
    // bounds checking so that the bool is 0-1 and not 0-65536
    char interim_boolean;
    eread(index->index_file_fd, &interim_boolean, sizeof(char));
    c_idx->is_binary = interim_boolean % 2;

    eread(index->index_file_fd, &interim_boolean, sizeof(char));
    c_idx->is_linked = interim_boolean % 2;

    eread(index->index_file_fd, &c_idx->linked_files_s, sizeof(size_t));
    c_idx->linked_files = malloc(sizeof(struct LinkedFiles)*c_idx->linked_files_s);
    
    if(c_idx->is_linked) {
      for(size_t i = 0; i < c_idx->linked_files_s; ++i) {
        eread(index->index_file_fd, &c_idx->linked_files[i].target_len, sizeof(size_t));
        c_idx->linked_files[i].target = malloc(sizeof(char)*(c_idx->linked_files[i].target_len + 1));
        eread(index->index_file_fd, c_idx->linked_files[i].target, c_idx->linked_files[i].target_len);
      }
    }

  } 
}

void indexReadPackages(Index_t *index) {
  eread(index->index_file_fd, &index->repo_name_len, sizeof(size_t));
  index->repo_name = malloc(index->repo_name_len*sizeof(char));
  eread(index->index_file_fd, &index->repo_name, index->repo_name_len);
  
  eread(index->index_file_fd, &index->indexed_packages_s, sizeof(size_t));
  index->indexed_packages = malloc(sizeof(IndexedPackage_t)*index->indexed_packages_s);

  for(size_t i = 0; i < index->indexed_packages_s; ++i) {
    IndexedPackage_t *c_pkg = &index->indexed_packages[i];

    eread(index->index_file_fd, &c_pkg->package_name_len, sizeof(size_t));
    c_pkg->package_name = malloc(c_pkg->package_name_len*sizeof(char));
    eread(index->index_file_fd, c_pkg->package_name, c_pkg->package_name_len);

    eread(index->index_file_fd, &c_pkg->package_version_len, sizeof(size_t));
    c_pkg->package_version = malloc(c_pkg->package_version_len*sizeof(char));
    eread(index->index_file_fd, &c_pkg->package_version, c_pkg->package_version_len);
  }
}

void indexRead(Index_t *index) {
  eread(index->index_file_fd, &index->index_mode_option, sizeof(IndexMode_t));
  switch (index->index_mode_option) {
    case im_files_mode:
    
    default:
    case im_nul:
      error(INDEX_ERR, "%s has a null mode flag or doesnt respect the index format", index->index_file_name);

  } 
}
