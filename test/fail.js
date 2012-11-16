var xattr = require("../build/Release/xattr.node");
var utils = require("./utils");

var f = "/tmp/xattr.test"

console.log("Set xattr");
try {
	xattr.set(f, "system.testprop3", "Lorem Ipsum Dolor Sortis");
}catch(e){
	console.log("Caught Error setting missing file (good): ", e);
}

utils.createFile(1,function(filename){
	try {
		xattr.set(filename, "system.testprop3", "Lorem Ipsum Dolor Sortis");
		utils.removeFile(filename);
	}catch(e){
		console.log("Caught Error setting attribute in the system namespace (good): ", e);
	}
	
});



