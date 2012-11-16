var 	fs = require("fs")

//var filename = "/gluster_mounts/polyomic/testdir/xattr.test";
//var filename = "/tmp/xattr.test";
//var filename = "/var/glusterfs/cid_share/cid_share_0/testdir/xattr.test";
//var filename = "/opt/solr_index/perfTest/xattr.test";
//var filename = "/var/glusterfs/galaxy/galaxy_0/testDir/xattr.test";
var filename = "/var/glusterfs/polyomic/polyomic_0/testDir/xattr.test";
//var filename = "/storage/testDir/xattr.test";

exports.createFile=function(index, cb){
	if (index){
		var f = filename + "." + index;
		fs.writeFile(f, "testFile Data: " + exports.generateString(500000), function(){
			cb(f);
		});
	}
}

exports.removeFile=function(f){
	fs.unlinkSync(f||filename);	
}

exports.generateString=function(size){
	var chars = "ABCDEFGHIJKLMNOPQRSTUVWXTZabcdefghiklmnopqrstuvwxyz";
	var string_length = Math.floor(Math.random() * (size || 1000));
	var randomstring = '';
	var charCount = 0;
	var numCount = 0;

	for (var i=0; i<string_length; i++) {
	if((Math.floor(Math.random() * 2) == 0) && numCount < 3 || charCount >= 5) {
		var rnum = Math.floor(Math.random() * 10);
		randomstring += rnum;
		numCount += 1;
	} else {
		// If any of the above criteria fail, go ahead and generate an alpha character from the chars string
		var rnum = Math.floor(Math.random() * chars.length);
		randomstring += chars.substring(rnum,rnum+1);
		charCount += 1;
	}
}

	return randomstring;
}
