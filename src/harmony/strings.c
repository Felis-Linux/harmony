#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <regex.h>

#include "strings.h"
#include "hmndefines.h"

HMN_UNUSED_MALLOC char *hmnstrcut(char *str, size_t from, size_t to) {
  size_t cut_strlen = from - to;
  str += from;
  char *ret = nullptr;

  if(str[0] == '\0') 
      return ret;

  if((! (from < to)) && to > strlen(str)) {
    return nullptr;
  }
  
  ret = malloc(cut_strlen*sizeof(char));
  strncpy(ret, str, cut_strlen);
  return ret;
}

[[gnu::format(printf, 1, 2)]] HMN_UNUSED_MALLOC char *smprintf(const char *format, ...) {
  va_list args;
  va_start(args, format);
  
  size_t len = vsnprintf(NULL, 0, format, args);
  char *buf = malloc(sizeof(char)*(len + 1));
  if(buf == nullptr)
      return nullptr;

  vsnprintf(buf, len + 1, format, args);
  va_end(args);
  return buf;
}

StringsError_t pkgstringGetVersion(const char *pkgstring) {
  StringsError_t errret;
  regex_t regx;
  regmatch_t regx_matches[2];
  int ec;
  size_t errstrlen;
  
  if((ec = regcomp(&regx, "(?<=\\^)", REG_EXTENDED)) != 0)
      goto err_regx;

  if((ec = regexec(&regx, pkgstring, 2, regx_matches, 0)) != 0)
      goto err_regx;
  
  size_t regx_start = regx_matches[1].rm_so;
  size_t regx_end   = regx_matches[1].rm_eo;
  
  errret.str = malloc((regx_end - regx_start + 1)*sizeof(char));
  if(errret.str == nullptr)
      goto err_alloc;

  snprintf(errret.str, regx_end - regx_start + 1, "%s", pkgstring + regx_start);
  errret.option = strs_ok;

  return errret;

err_alloc:
  regfree(&regx);
  errret.option = strs_alloc_fail;
  return errret;

err_regx:
  errstrlen = regerror(ec, &regx, nullptr, 0);
  errret.option = strs_regx_fail;
  errret.errstr = malloc(errstrlen*sizeof(char));
  if(errret.errstr == nullptr)
      goto err_alloc;    
    
  regerror(ec, &regx, errret.errstr, errstrlen);
  return errret;
}

StringsError_t pkgstringGetName(const char *pkgstring) {
  StringsError_t errret;
  regex_t regx;
  regmatch_t regx_matches[2];
  int ec;
  size_t errstrlen;

  if((ec = regcomp(&regx, "(.+?)\\^", REG_EXTENDED)) != 0)
      goto err_regx;
  if((ec = regexec(&regx, pkgstring, 2, regx_matches, 0)) != 0)
      goto err_regx;
 
  size_t regx_start = regx_matches[1].rm_so;
  size_t regx_end   = regx_matches[1].rm_eo;

  errret.str = malloc((regx_end - regx_start + 1)*sizeof(char));
  if(errret.str == nullptr)
      goto err_alloc;

  snprintf(errret.str, regx_end - regx_start + 1, "%s", pkgstring + regx_start);
  errret.option = strs_ok;

  return errret;
err_alloc:
  errret.option = strs_alloc_fail;
  return errret;

err_regx:
  errstrlen = regerror(ec, &regx, nullptr, 0);
  errret.option = strs_regx_fail;
  errret.errstr = malloc(errstrlen*sizeof(char));
  if(errret.errstr == nullptr)
      goto err_alloc;
  regerror(ec, &regx, errret.errstr, errstrlen);
  return errret;
}
