import sys
import os, glob
       
if(len(sys.argv) != 4):
	print "usage: readfolder.py <location> <extension> <search-string>"
	exit(0)

path = sys.argv[1]
extension=sys.argv[2]

for infile in glob.glob(os.path.join(path, extension)):
#for infile in glob.glob( os.path.join(path, '*.fasta') ):
	file=open(infile, "r")
	line=file.readline()

	linenum=1
	while line:
		if((line.find(sys.argv[3])) != -1):
			print infile+":"+str(linenum)+"\t"+line
		line=file.readline()
		linenum=linenum+1;


