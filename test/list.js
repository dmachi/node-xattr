var xattr = require("../build/Release/xattr.node");

var f = "/tmp/xattr.test"

console.log("List Attributes on: ", f);
console.log(xattr.list(f));
