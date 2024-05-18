/* felis.fpkg::config 
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

#pragma once

#include <string>
#include <vector>
#include <filesystem>

namespace felis::fpkg::config {
  using namespace std::string_literals;
  
  struct Repo {
    std::string name, url,
      index_file;
  };

  struct Package {
    std::string repo,
      name,
      version;
  };

  struct Config {
    std::filesystem::path system_root = "/";
    std::filesystem::path fpkg_root = system_root.string() + "/var/felis/fpkg"s;
    std::filesystem::path fpkg_repo_dictionary = fpkg_root.string() + "/repos"s;
    std::filesystem::path fpkg_hooks = fpkg_root.string() + "/hooks"s;
    std::filesystem::path fpkg_cache = fpkg_root.string() + "/cache"s;
    std::filesystem::path fpkg_config_directory = system_root.string() + "/etc/felis/fpkg"s;

    // 1: Name, 
    // 2: Url
    std::vector<Repo> repos;
    std::vector<Package> declared_packages;
  };

  Config parseConfig(std::filesystem::path path);
} // fpkg::config
