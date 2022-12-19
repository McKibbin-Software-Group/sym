/*--------------------------------------------------------------------*
 *  default.c
 *  Dec 04 (PJW)
 *
 *  Default_setup() will be called automatically just before the
 *  actual language is initialized.  The actual language only 
 *  needs to override methods that need to be changed from their
 *  default behavior.
 *--------------------------------------------------------------------*/

#include "error.h"
#include "lang.h"
#include "output.h"
#include "str.h"
#include "sym.h"
#include "codegen.h"
#include "nodes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define now(arg) (cur->type == arg)

void Default_begin_file(char *basename)
{
}

void Default_end_file()
{
}

void Default_declare(void *sym)
{
}

void Default_begin_block(void *eq)
{
}

void Default_begin_eqn(void *eq)
{
}

void Default_end_eqn(void *eq)
{
   fprintf(code," ;\n\n");
}

char *Default_begin_func(char *func, char *arg)
{
   if( arg )return concat(4,func,"(",arg,",");
   return concat(2,func,"("); 
}

char *Default_end_func()
{
   return strdup(")");
}

/*-------------------------------------------------------------------*
 *  Default spprint
 *
 *  Added by Geoff Shuetrim in December 2022 to facilitate more
 *  fine-grain control over how nodes are printed in equations.
 * 
 *  This default implementation is lifted from the original 
 *  spprint.c file.
 *-------------------------------------------------------------------*/
char *Default_spprint(Nodetype prevtype, Node *cur, char *indent)
{

   int parens, wrap_right;
   char *comma, *cr;
   char *lstr, *rstr, *chunk;

   if (cur == 0)
      return strdup("");

   parens = 0;
   comma = "";
   switch (prevtype)
   {
   case nul:
   case add:
   case sub:
      if (now(neg))
         parens = 1;
      break;

   case mul:
      if (now(add) || now(sub))
         parens = 1;
      if (now(dvd) || now(neg))
         parens = 1;
      break;

   case neg:
      parens = 1;
      if (now(nam) || now(num) || now(mul))
         parens = 0;
      if (now(log) || now(exp) || now(pow))
         parens = 0;
      if (now(sum) || now(prd))
         parens = 0;
      break;

   case dvd:
      parens = 1;
      if (now(nam) || now(num) || now(pow))
         parens = 0;
      if (now(sum) || now(prd))
         parens = 0;
      if (now(log) || now(exp))
         parens = 0;
      break;

   case pow:
      parens = 1;
      if (now(nam) || now(num) || now(log) || now(exp))
         parens = 0;
      if (now(sum) || now(prd))
         parens = 0;
      break;

   case log:
   case exp:
   case lag:
   case led:
      parens = 1;
      break;

   case equ:
   case sum:
   case prd:
   case dom:
      break;

   case nam:
   case num:
      if (now(nam) || now(num))
         comma = ",";
      break;

   default:
      fatal_error("%s", "invalid state reached in spprint");
   }

   switch (cur->type)
   {
   case sum:
   case prd:
      lstr = codegen_spprint(cur->type, cur->l, indent);
      rstr = codegen_spprint(cur->type, cur->r, indent);
      chunk = concat(6, cur->str, "(", lstr, ",", rstr, ")");
      free(lstr);
      free(rstr);
      break;

   case lst:
      lstr = strdup("(");
      for (cur = cur->r; cur; cur = cur->r)
      {
         if (cur->r)
            rstr = concat(3, lstr, cur->str, ",");
         else
            rstr = concat(2, lstr, cur->str);
         free(lstr);
         lstr = rstr;
      }
      chunk = concat(2, lstr, ")");
      free(lstr);
      break;

   default:
      lstr = codegen_spprint(cur->type, cur->l, indent);
      rstr = codegen_spprint(cur->type, cur->r, indent);

      cr = "";
      if (indent)
         if (strlen(lstr) + strlen(rstr) > 70 || strlen(lstr) > 40 || strlen(rstr) > 40)
            cr = concat(2, "\n", indent);

      wrap_right = 0;
      if (cur->type == sub)
         if (cur->r->type == add || cur->r->type == sub)
            wrap_right = 1;

      if (parens)
         if (wrap_right)
            chunk = concat(8, "(", lstr, comma, cr, cur->str, "(", rstr, "))");
         else
            chunk = concat(7, "(", lstr, comma, cr, cur->str, rstr, ")");
      else if (wrap_right)
         chunk = concat(7, lstr, comma, cr, cur->str, "(", rstr, ")");
      else
         chunk = concat(5, lstr, comma, cr, cur->str, rstr);

      if (strlen(cr))
         free(cr);
      free(lstr);
      free(rstr);
   }

   return chunk;

}

//
//  Connect up the public routines.
//

void Default_setup(void)
{
   lang_begin_file ( Default_begin_file  ); 
   lang_end_file   ( Default_end_file    );
   lang_declare    ( Default_declare     );
   lang_begin_block( Default_begin_block ); 
   lang_begin_eqn  ( Default_begin_eqn   ); 
   lang_end_eqn    ( Default_end_eqn     );
   lang_begin_func ( Default_begin_func  );
   lang_end_func   ( Default_end_func    );   
   lang_show_symbol( 0                   );
   lang_spprint    ( Default_spprint     );
}

