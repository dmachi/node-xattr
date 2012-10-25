#include <node.h>
#include <uv.h>
#include <sys/time.h>
#include <node_version.h>

struct AsyncBaton {
    uv_work_t request;
    v8::Persistent<v8::Function> callback;
    int error;

    v8::String filename;
    v8::String attribute;
    v8::String value;
};


