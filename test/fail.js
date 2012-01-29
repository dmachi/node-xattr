var xattr = require("../build/Release/xattr.node");

var f = "/mnt/DataCollections/Fubar";

console.log("Set xattr");
try {
	xattr.set(f, "user.testprop3", "Lorem Ipsum Dolor Sortis");
}catch(e){
	console.log("Caught Error: ", e);
}



