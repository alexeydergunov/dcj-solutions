import sys
import os

source = sys.argv[1]
library = sys.argv[2]
nodes = sys.argv[3]

targetDir = "__target"

if not os.path.exists(source):
	print "Source file", source, "doesn't exist"
	exit()

if not os.path.exists(library):
	print "Library file", library, "doesn't exist"
	exit()

if source[-4:] != ".cpp":
	print "Source file is not cpp:", source
	exit()

problemName = source[:-4]
print "Problem name is", problemName

if not problemName in library:
	"Library", library, "doesn't contain problem name", problemName
	exit()

if not os.path.exists(targetDir):
	os.mkdir(targetDir)

sourceFile = open(source, "r")

hackedSource = targetDir + "/" + "__" + source
hackedSourceFile = open(hackedSource, "w")

includeLineToTest = "#include \"../" + library + "\""
includeLineToSubmit = "#include \"" + problemName + ".h\""

containsIncludeLineToSubmit = False
for line in sourceFile:
	if line == includeLineToSubmit + "\n":
		containsIncludeLineToSubmit = True
		hackedSourceFile.write(includeLineToTest + "\n")
		continue
	hackedSourceFile.write(line)

sourceFile.close()
hackedSourceFile.close()

if not containsIncludeLineToSubmit:
	print "Source", source, "doesn't contain include line '", includeLineToSubmit, "'"
	exit()

print "Everything is ready, start testing..."

commandLine = "python ../dcj.py test --source " + hackedSource + " --nodes " + nodes
print "Executing command:", commandLine

os.system(commandLine)
