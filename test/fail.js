var xattr = require("../build/Release/xattr.node");

var f = "/tmp/xattr.test"

console.log("Set xattr");
try {
	xattr.set(f, "system.testprop3", "Lorem Ipsum Dolor Sortis");
}catch(e){
	console.log("Caught Error: ", e);
}



