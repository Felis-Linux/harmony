#include "fpkg.hpp"
#include "fs.hpp"

#include <cstdio>
#include <fstream>
#include <format>

namespace fpkg::fs {
  // possible segfault RIGHT OVER HERE \/
  Locker::Locker(std::fstream &fs): fs(fs), interm_buf((__gnu_cxx::stdio_filebuf<char> *)fs.rdbuf()) {}
  void Locker::lock() {
    debug(std::format("Locking file (by fd) {}", interm_buf->fd()));
    if(ftrylockfile(interm_buf->file()) != 0) {
      error("Failed to lock a file (locked by another process, am i alone?)");
    }
  }

  void Locker::unlock() {
    debug(std::format("Unlocking file (by fd) {}", interm_buf->fd()));
    funlockfile(interm_buf->file());
  }
}
