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
#include "node_xattr.h"
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


void setAttribute(uv_work_t *req) {
	AsyncBaton *baton = static_cast<AsyncBaton*>(req->data);
	ssize_t res;

#ifdef __APPLE__
//	res = setxattr(baton->filename, baton->attribute, baton->value, baton->value.Length(),0,0);
#else
//	res = setxattr(baton->filename, baton->attribute, baton->value, baton->value.Length(),0);
#endif
	res = 0;
        printf("Setting file: %s, attribute: %s, value: %s, length: %d\n", baton->filename, baton->attribute, baton->value,baton->value.Length());

	//Error
	if (res == -1){
		baton->error=errno;
	}
}

void setAfter(uv_work_t *req) {
	AsyncBaton *baton = static_cast<AsyncBaton*>(req->data);

	if (baton->error){
	        Local<Object> error = Object::New();
		error->Set(String::New("code"), Integer::New(baton->error));
		error->Set(String::New("message"), ErrorToString(baton->error));
		Handle<Value> argv[] = { error, Null() };
		baton->callback->Call(Context::GetCurrent()->Global(), 2, argv);
	}else{
		Handle<Value> argv[] = {};
		baton->callback->Call(Context::GetCurrent->Global(),0,argv);
	}

	baton->callack.Dispose()
	delete baton;
}

v8::Handle<v8::Value> set(const v8::Arguments &args) {
	HandleScope scope;

	if (args.Length() < 1) {
		return ThrowException(String::New("Expected filename as the first argument"));
	}

	if (args[0]->IsString()) {
		if (args.Length() < 4) { 
			if (!args[1]->IsString())
				return ThrowException(String::New("Expected second argument to be a string (attribute name)"));
			if (!args[2]->IsString())
				return ThrowException(String::New("Expected third argument to be a string (attribute value)"));
			if (!args[3]->IsFunction())
				return ThrowException(String::New("Expected fourth (last) argument to be a callback function"));
		}
	}else {
		return ThrowException(String::New("Expected string or buffer as first argument"));
	}

	AsyncBaton *baton = new AsyncBaton;
	baton->request.data = baton;
	baton->filename=String::New(args[0]->ToString());

	//String::Utf8Value attribute(args[1]->ToString());
	baton->attribute=String::New(args[1]->ToString());

	//String::Utf8Value value(args[2]->ToString());
	baton->value=String::New(args[2]->ToString());
	
	uv_queue_work(uv_default_loop(), &baton->request, setAttribute, setAfter);

	return Undefined();
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
