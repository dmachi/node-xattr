var xattr = require("../build/Release/xattr.node");

var f = "/mnt/DataCollections/TEST";

console.log("List Attributes on: ", f);
console.log(xattr.list(f));
