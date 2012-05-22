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
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/xattr.h>
#include <iostream>
using namespace v8;
using namespace node;
using namespace std;

#define XATTR_SIZE 10000

#define REQ_STR_ARG(I, VAR)                                     \
    if (args.Length() <= (I) || !args[I]->IsString())           \
        return ThrowException(Exception::TypeError(             \
                String::New("Argument must be a string")));     \
    Local<String> VAR = Local<String>::Cast(args[I])

#define REQ_ASCII_ARG(I, VAR)                                     \
    if (args.Length() <= (I) || !args[I]->IsString())           \
        return ThrowException(Exception::TypeError(             \
                String::New("Argument must be a string")));     \
    String::AsciiValue VAR(args[I]);



string ObjectToString(Local<Value> value) {
    String::AsciiValue ascii_value(value);
    return std::string(*ascii_value);
}

static Handle<Value> set(const Arguments& args) {
	HandleScope scope;
	ssize_t res;
	int valLen;

        REQ_ASCII_ARG(0,filename);
        REQ_ASCII_ARG(1,attribute);
        REQ_ASCII_ARG(2,val);
        valLen = val.length();
  #ifdef __APPLE__
    res = setxattr(*filename, *attribute, *val, valLen,0,0);
  #else
	  res = setxattr(*filename, *attribute, *val, valLen,0);
	#endif
        //printf("Setting file: %s, attribute: %s, value: %s, length: %d\n", *filename, *attribute, *val,val.length());


	//Error
	if (res == -1){
                return ThrowException(Exception::TypeError(             \
                        String::Concat(String::New("Error setting extended attribue: "), String::New(strerror(errno)))));
	}
	
        return Boolean::New(true);
}

static Handle<Value> list(const Arguments& args) {
	HandleScope scope;
	char list[XATTR_SIZE],value[XATTR_SIZE];
	const char *filename;
	ssize_t listLen,valueLen;
	int ns;

	//make sure we were passed a string
	REQ_STR_ARG(0, s);
	filename= ObjectToString(s).c_str();

	//get all the extended attributes on filename
	#ifdef __APPLE__
	  listLen = listxattr(filename,list,XATTR_SIZE,0);
	#else
	  listLen = listxattr(filename,list,XATTR_SIZE);
	#endif

	// create obj for return
	Handle<Object> result = Object::New();

	//for each of the attrs, do getxattr and add them as key/val to the obj
	for (ns=0; ns<listLen; ns+= strlen(&list[ns])+1){
	  #ifdef __APPLE__
		  valueLen = getxattr(filename, &list[ns],value, XATTR_SIZE, 0, 0);
		#else
		  valueLen = getxattr(filename, &list[ns],value, XATTR_SIZE);
		#endif
		if (valueLen > 0){
			result->Set(String::New(&list[ns]),String::New(value, valueLen));
		}
	} 
	return result;
}

extern "C" {

	void init (Handle<Object> target)
	{
		NODE_SET_METHOD(target, "list", list);
		NODE_SET_METHOD(target, "set", set);
	}

	NODE_MODULE(xattr, init);
}
