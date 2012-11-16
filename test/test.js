var xattr = require("../build/Release/xattr.node");
var utils = require("./utils");

var startingRuns=false;
function runTest(runs){
	if (!startingRuns){
		startingRuns=runs;
	}
	if (runs<0){ console.log("complete"); return; }

	utils.createFile(runs, function(filename){
		console.log("***** RUN " + (startingRuns-runs) + "*********"); 
		console.log("\tSetting extended attributes on file: ", filename);
		var vals = {
			v0: utils.generateString(),
			v1: utils.generateString(),
			v2: utils.generateString(),
			v3: utils.generateString()
		}
		try {
			xattr.set(filename, "user.p0", vals.v0);
			xattr.set(filename, "user.p1", vals.v1 );
			xattr.set(filename, "user.p2", vals.v2); 
			xattr.set(filename, "user.p3", vals.v3);
		}catch(err){
			console.log("\tError: ", err);
			//console.log("\tVals: ", vals);
			return;	
		}


		var data = xattr.list(filename);

		for (var i = 0; i<4; i++){
			if (data["user.p"+i] != vals["v" + i]){
				console.log("Mismatch: ",data["user.p"+i],vals["v" +i]);
			}
		}

//		utils.removeFile(filename);
		console.log("******** END " + (startingRuns - runs) + "**********");
		runTest(runs-1);
	});
}

runTest(100);

