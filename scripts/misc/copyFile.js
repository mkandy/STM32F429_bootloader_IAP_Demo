//Constants
var ForReading = 1;
var ForWriting = 2;

//File system object
var fso = new ActiveXObject("Scripting.FileSystemObject");
//Shell object
var shell = new ActiveXObject("WScript.Shell");

try
{
   var srcFile = "";
   var destFile = "";
   
   //Get command line arguments
	var args = WScript.Arguments;
	if(args.length < 2)
		throw("Error to few arguments!");
   
   //Get source file path
   srcFile = args.Item(0);
   
   //Get destination file path
   destFile = args.Item(1);
   
   //Unix to Windows path conversion
   srcFile = srcFile.replace(/\//g, "\\");
   destFile = destFile.replace(/\//g, "\\");
   
   //Copy file
   copyFile(fso, srcFile, destFile);
   
   //User message
   //shell.Popup("Done.");
}
catch (e)
{
   //User message
	shell.Popup("Error : " + e);
}


function copyFile(fso, srcFile, destFile) {
   if(!fso.FileExists(srcFile))
   {
      throw("File \"" + srcFile + "\" doesn't exist!");
	}
   else
   {
      fso.CopyFile(srcFile, destFile);
   }
}