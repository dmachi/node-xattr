// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to permit
// persons to whom the Software is furnished to do so, subject to the
// following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN
// NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
// DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
// OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE
// USE OR OTHER DEALINGS IN THE SOFTWARE.

#include <node.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/file.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/xattr.h>
#include <iostream>

using namespace v8;
using namespace node;
using namespace std;

#define THROW_BAD_ARGS \
  ThrowException(Exception::TypeError(String::New("Bad argument")))

#define XATTR_SIZE 10000

#define REQ_STR_ARG(I, VAR)                                     \
    if (args.Length() <= (I) || !args[I]->IsString())           \
        return ThrowException(Exception::TypeError(             \
                String::New("Argument must be a string")));     \
    Local<String> VAR = Local<String>::Cast(args[I])

string ObjectToString(Local<Value> value) {
    String::Utf8Value utf8_value(value);
    return std::string(*utf8_value);
}

static Handle<Value> setxattr(const Arguments& args) {
	HandleScope scope;
	ssize_t res;

	//make sure we were passed a string
	REQ_STR_ARG(0, x);
	const char *filename= ObjectToString(x).c_str();

	//make sure we were passed a string
	REQ_STR_ARG(1, y);
	const char *attribute = ObjectToString(y).c_str();

	//make sure we were passed a string
	REQ_STR_ARG(2, z);
	const char *value = ObjectToString(z).c_str();

        //printf("Setting %s to %s on %s\n", attribute, value, filename);

	res = setxattr(filename,attribute,value,strlen(value)+1,0,0);

	if (res == -1){
		/* learn to generate errors */
		return Boolean::New(false);
	}
        return Boolean::New(true);
}

static Handle<Value> listxattr(const Arguments& args) {
	HandleScope scope;
	char list[XATTR_SIZE],value[XATTR_SIZE];
	ssize_t listLen,valueLen;
	int ns;

	//make sure we were passed a string
	REQ_STR_ARG(0, s);
	const char *filename= ObjectToString(s).c_str();
	listLen = listxattr(filename,list,XATTR_SIZE,0);

	if (listLen == -1){
		/* learn to generate errors */
		return (String::New("error"));
	}

	// create obj for return
	Handle<Object> result = Object::New();

	//for each of the attrs, do getxattr and add them as key/val to the obj
	for (ns=0; ns<listLen; ns+= strlen(&list[ns]) + 1){
		valueLen = getxattr(filename, &list[ns],value, XATTR_SIZE,0,0);
		if (valueLen > 0){
			result->Set(String::New(&list[ns]),String::New(&value[0],strlen(&value[0]+1)));
		}
	} 
	return result;
}

extern "C" {

	void init (Handle<Object> target)
	{
		HandleScope scope;
		NODE_SET_METHOD(target, "listxattr", listxattr);
		NODE_SET_METHOD(target, "setxattr", setxattr);
	}

	NODE_MODULE(xattr, init);
}
