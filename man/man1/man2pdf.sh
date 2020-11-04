
# Create Document Names Based on $1 and $2 if present for man page number.
#
FOLDER=.
if [ ! $2 ] ; then 
  PAGE=""
  CMD="$1"
# Full Name of Post Script File to be used
  PSFN="$FOLDER""/$CMD""_manual.ps"
# Full Name of PDF File to be used
  PDFN="$FOLDER""/$CMD""_manual.pdf"
else
 PAGE="$1" 
 CMD="$2"
# Full Name of Post Script File to be used
 PSFN="$FOLDER""/$CMD($PAGE)""_manual.ps"
# Full Name of PDF File to be used
 PDFN="$FOLDER""/$CMD($PAGE)""_manual.pdf"
fi

#
# Display Error if PDF File Already Exists & Do not create new PDF File
# Else create PDF Document based on command and page number if supplied.
#

if [ ! -e $PDFN ] ; then
# Create .ps file from MAN Page
  man -Tps $PAGE $CMD >> $PSFN 
# Convert .ps file to pdf File
  ps2pdf $PSFN $PDFN
# Remove .ps file when done
  rm $PSFN
  tput setf 7
  tput setb 1
  tput bold
  echo $PDFN " File Has Been Created For You..."
  tput sgr0
  echo
else
  tput setf 7
  tput setb 4
  tput bold
  echo $PDFN " File Already Exists..."
  tput sgr0
  echo
fi

exit 0
# End Of Script
