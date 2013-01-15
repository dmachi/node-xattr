var xattr = require("../build/Release/xattr.node");
var utils = require("./utils");

var startingRuns=false;
var errorCount=0;
var successCount=0;
//ifunction runTest(runs){
//	if (!startingRuns){
//		startingRuns=runs;
//	}
//	if (runs<1){ 
		//console.log("Completed " + startingRuns + " runs."); 
		//console.log("Success Count: ", successCount);
//		if (errorCount>0){
//			console.error("Error Count: ", errorCount);
//		}
//		return; 
//	}

//	utils.createFile(runs, function(filename){
//		console.log("***** RUN " + (startingRuns-runs) + "*********"); 
//		console.log("\tSetting extended attributes on file: ", filename);
//		var vals = {
//			v0: utils.generateString(),
//			v1: utils.generateString(),
//			v2: utils.generateString(),
//			v3: utils.generateString()
//		}
//		try {
//			xattr.set(filename, "user.p0", vals.v0);
//			xattr.set(filename, "user.p1", vals.v1 );
//			xattr.set(filename, "user.p2", vals.v2); 
//			xattr.set(filename, "user.p3", vals.v3);
//		}catch(err){
//			console.log("\tError Setting Attributes: ", vals, err);
			//console.log("\tVals: ", vals);
//			return;	
//		}

var fs = require('fs');
var array = fs.readFileSync('run1.txt').toString().split("\n");
for(i in array) {
	var parts=array[i].split(" ");
	var filename=parts[0];
	var attribute=parts[1];
	var assigned=parts[3];
	var data = xattr.list(filename);
	console.log(filename,attribute,data[attribute],assigned);
	if (data[attribute] != assigned){
		errorCount++;
		console.error("Mismatch: ",data[attribute],assigned, "on file ", filename);
	}else{
		successCount++;
	}	
}

//		utils.removeFile(filename);
//		console.log("******** END " + (startingRuns - runs) + "**********");
//		runTest(runs-1);
//	});
//}

//runTest(1000);

