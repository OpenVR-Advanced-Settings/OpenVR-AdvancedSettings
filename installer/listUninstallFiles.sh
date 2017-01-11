#!/bin/bash

# Execute this script in the root folder of the install dir
# Dont't forget to set OUTPUTFILE to a path that is writeable

OUTPUTFILE=../../installed_files.txt
INSTALLDIR=.

# Get command line arguments
if [ $# -eq 1 ]; then
	OUTPUTFILE=$1
elif [ $# -gt 1 ]; then
	OUTPUTFILE=$1
	INSTALLDIR=$2
fi

# Find all files
find "$INSTALLDIR" -iname '*' -type f -exec echo "Delete \$INSTDIR/{}" \; > $OUTPUTFILE

# Find all directories and print them out in reverse order
find "$INSTALLDIR" -iname '*' -type d -exec echo "RMdir \$INSTDIR/{}" \; | tac.exe >> $OUTPUTFILE

# Remove "/./" from the file paths
sed -i 's/\/\.//g' $OUTPUTFILE

# Replace "/" with "\"
sed -i 's/\//\\/g' $OUTPUTFILE
