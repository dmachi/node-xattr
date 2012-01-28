#node-xattr

node-xattr is a native module wrapping xattr to read and set extended attributes on files

## Build

node-waf configure && node-waf build && node-waf install

## usage

var xattr = require("xattr");

//get all the extended attributes on a file
var attrs = xattr.listxattr("/path/to/file");
/* returns object */

//set an extended attribute on a file.  Note at thie moment you can only set string values.
xattr.setxattr("/path/to/file", "someAttribute", "someValue");


 
