#include "main.h"

int MatchExp (char *str_request ,char *str_regex )
{
   int err;
   regex_t preg;

   err = regcomp (&preg, str_regex, REG_NOSUB | REG_EXTENDED );
   if (err == 0)
   {
      int match;

      match = regexec (&preg, str_request, 0, NULL, 0);
      regfree (&preg);
      if (match == 0)
      {
         return 1;
      }
      else if (match == REG_NOMATCH)
      {
         return 0;
      }
   }
   return 2;
}
