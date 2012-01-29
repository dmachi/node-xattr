var xattr = require("../build/Release/xattr.node");

var f = "/mnt/DataCollections/TEST";

console.log("Setting extended attributes.");
xattr.set(f, "user.testprop3", "Lorem Ipsum Dolor Sortis");
xattr.set(f, "user.testpropb", "Diff Value");
xattr.set(f, "user.testpropa", "ValueForA");
console.log("Done.");

console.log("Attributes ", f, ":");
console.log(xattr.list(f));


