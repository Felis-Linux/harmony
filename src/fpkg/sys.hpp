/* felis.fpkg::sys
 *   This file is a part of the felis.fpkg project
 *   This file is licensed under the BSD-2 license
 *
 *   Copyright (c) 2024, Ika Sek
 *   All rights reserved. 
 *   Redistribution and use in source and binary forms, with or without 
 *   modification, are permitted provided that the following conditions are met:
 *   *  Redistributions of source code must retain the above copyright notice, 
 *      this list of conditions and the following disclaimer. 
 *   *  Redistributions in binary form must reproduce the above copyright 
 *      notice, this list of conditions and the following disclaimer in the 
 *      documentation and/or other materials provided with the distribution. 
 *   THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND ANY 
 *   EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
 *   WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
 *   DISCLAIMED. IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE FOR ANY 
 *   DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
 *   (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR 
 *   SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
 *   CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT 
 *   LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY 
 *   OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH 
 *   DAMAGE. 
 */


#include <string>
#include <fstream>
#include <ext/stdio_filebuf.h>
#include <sys/file.h>

#include <lua.hpp>
#include <LuaBridge/LuaBridge.h>

namespace fpkg::sys {
  class Locker {
    public:
      Locker(std::fstream &fs);
      Locker(Locker &);
      void lock();
      void unlock();
      ~Locker() = default;
    private:
      std::fstream &fs;
      __gnu_cxx::stdio_filebuf<char> *interm_buf;
  }; //Locker
  
  struct User {
    std::string name,
      password,
      home_directory;

    void ensureCreated();

  };  

  void inline moduleRegister(lua_State *L);
} //fpkg::fs
