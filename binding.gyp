{
  "targets": [
    {
	"target_name": "xattr",
	"sources": [ "src/node_xattr.cc" ],
	'cflags': ["-g", "-D_FILE_OFFSET_BITS=64", "-D_LARGEFILE_SOURCE", "-Wall"],
    }
  ]
}

