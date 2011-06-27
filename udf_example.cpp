/* Copyright (C) 2002 MySQL AB

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; version 2 of the License.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA */


#include <iostream>
#include <vector>

#include <my_global.h>
#include <my_sys.h>
#include <m_string.h>		//* To get strmov() *
#include <string.h>
#include <mysql.h>
#include <ctype.h>

extern "C" {
	my_bool execute_js_init(UDF_INIT *initid, UDF_ARGS *args, char *message);
	void execute_js_deinit(UDF_INIT *initid);
	char *execute_js(UDF_INIT *initid, UDF_ARGS *args, char *result, unsigned long *length, char *is_null, char *error);
}

/*************************************************************************
** Example of init function
** Arguments:
** initid	Points to a structure that the init function should fill.
**		This argument is given to all other functions.
**	my_bool maybe_null	1 if function can return NULL
**				Default value is 1 if any of the arguments
**				is declared maybe_null.
**	unsigned int decimals	Number of decimals.
**				Default value is max decimals in any of the
**				arguments.
**	unsigned int max_length  Length of string result.
**				The default value for integer functions is 21
**				The default value for real functions is 13+
**				default number of decimals.
**				The default value for string functions is
**				the longest string argument.
**	char *ptr;		A pointer that the function can use.
**
** args		Points to a structure which contains:
**	unsigned int arg_count		Number of arguments
**	enum Item_result *arg_type	Types for each argument.
**					Types are STRING_RESULT, REAL_RESULT
**					and INT_RESULT.
**	char **args			Pointer to constant arguments.
**					Contains 0 for not constant argument.
**	unsigned long *lengths;		max string length for each argument
**	char *maybe_null		Information of which arguments
**					may be NULL
**
** message	Error message that should be passed to the user on fail.
**		The message buffer is MYSQL_ERRMSG_SIZE big, but one should
**		try to keep the error message less than 80 bytes long!
**
** This function should return 1 if something goes wrong. In this case
** message should contain something usefull!
**************************************************************************/

#include <v8.h>

using namespace v8;


//std::vector<AllContextData> contexts;
struct JsContext {
	Isolate *isolate;
	Persistent<Context> context;
	Handle<Script> script;
	Handle<Function> function;
};

my_bool execute_js_init(UDF_INIT *initid, UDF_ARGS *args, char *message)
{
  if (args->arg_count < 1 || args->arg_type[0] != STRING_RESULT)
  {
    strcpy(message,"execute_js: Wrong number of arguments");
    return 1;
  }
  for (int i=0; i<args->arg_count; i++) if (args->arg_type[i]!=STRING_RESULT) {
    strcpy(message,"execute_js: Wrong argument type");
    return 1;
  }
  initid->max_length=1024;

  JsContext *ctx = new JsContext();
  initid->ptr = (char*)ctx;
  ctx->isolate = Isolate::New();
  ctx->isolate->Enter();
  {
  		HandleScope handle_scope;
		//V8::GetCurrentThreadId();
		//Isolate *t = Isolate::GetCurrent();
		//internal::Isolate * i = (internal::Isolate *) t;
		//fprintf(stderr,itoa(i->thread_id().ToInteger()));

		fprintf(stderr,"ASDASDADS - i2\n");

		// Create a new context.
		ctx->context = Context::New();
		
		Handle<Context> hc = ctx->context;
		hc->Enter();
		
		//Context::Scope context_scope(ctx->context);

		fprintf(stderr,"ASDASDADS - i3\n");

		// Create a string containing the JavaScript source code.
		Handle<String> source = String::New(args->args[0],args->lengths[0]);
		ctx->script = Script::Compile(source);
		if (args->arg_count>1) {
			ctx->function = Handle<Function>::Cast(ctx->context->Global()->Get(String::New(args->args[1],args->lengths[1])));
		}
		
		fprintf(stderr,"ASDASDADS - i5\n");

  }
  return 0;
}

/****************************************************************************
** Deinit function. This should free all resources allocated by
** this function.
** Arguments:
** initid	Return value from xxxx_init
****************************************************************************/


void execute_js_deinit(UDF_INIT *initid)
{
	JsContext *ctx = (JsContext *)initid->ptr;
	Handle<Context> hc = ctx->context;
	hc->Exit();
	ctx->context.Dispose();
	ctx->isolate->Exit();
	ctx->isolate->Dispose();

}

/***************************************************************************
** UDF string function.
** Arguments:
** initid	Structure filled by xxx_init
** args		The same structure as to xxx_init. This structure
**		contains values for all parameters.
**		Note that the functions MUST check and convert all
**		to the type it wants!  Null values are represented by
**		a NULL pointer
** result	Possible buffer to save result. At least 255 byte long.
** length	Pointer to length of the above buffer.	In this the function
**		should save the result length
** is_null	If the result is null, one should store 1 here.
** error	If something goes fatally wrong one should store 1 here.
**
** This function should return a pointer to the result string.
** Normally this is 'result' but may also be an alloced string.
***************************************************************************/

char *execute_js(UDF_INIT *initid __attribute__((unused)),
               UDF_ARGS *args, char *result, unsigned long *length,
               char *is_null, char *error __attribute__((unused)))
{
	JsContext *ctx = (JsContext *)initid->ptr;
	HandleScope handle_scope;

	fprintf(stderr,"ASDASDADS - e1\n");

//	Context::Scope context_scope(ctx->context);	
	// Create a string containing the JavaScript source code.
	Handle<String> source = String::New(args->args[0],args->lengths[0]);
	ctx->script = Script::Compile(source);
	
	Handle<Value> jsresult;
	if (args->arg_count>1) {
		fprintf(stderr,"ASDASDADS - e2\n");
		Handle<Value> fargs[args->arg_count-2];
		for (int i=2;i<args->arg_count;i++) {
			fargs[i-2] = String::New(args->args[i],args->lengths[i]);
		}
		fprintf(stderr,"ASDASDADS - e3\n");
		jsresult = ctx->script->Run();
		fprintf(stderr,"ASDASDADS - e4\n");
		ctx->function = Handle<Function>::Cast(ctx->context->Global()->Get(String::New(args->args[1],args->lengths[1])));
		fprintf(stderr,"ASDASDADS - e5\n");
		jsresult = ctx->function->Call(ctx->function,args->arg_count-2, fargs);
	} else {
		jsresult = ctx->script->Run();
	}

	fprintf(stderr,"ASDASDADS - e8\n");

	// Convert the result to an ASCII string and print it.
	String::Utf8Value res(jsresult);
	fprintf(stderr,"ASDASDADS - e9\n");
	strncpy(result,*res,res.length());
	*length = res.length();


	return result;
}

