#!/bin/sh
##################################################
validateout=`dirname $0`
validatetime=`date`
validated="0";
error=1

if [ -z $validateout ]
then
    validateout="."
fi
cd $validateout;
validateworkdir=`pwd`;

echo "*******************************************************" >> stdout;
echo "* AliRoot Validation Script V1.0                      *" >> stdout;
echo "* Time:    $validatetime " >> stdout;
echo "* Dir:     $validateout" >> stdout;
echo "* Workdir: $validateworkdir" >> stdout;
echo "* ----------------------------------------------------*" >> stdout;
ls -la ./ >> stdout;
echo "* ----------------------------------------------------*" >> stdout;

cp stdout stdout.log
cp stderr stderr.log

##################################################
sv=`grep -i  "Segmentation violation" *.log`
if [ "$sv" = "" ]
    then
    sf=`grep -i  "Segmentation fault" *.log`
    if [ "$sf" = "" ]
        then
        be=`grep -i  "Bus error" *.log`
        if [ "$be" = "" ]
	    then
	    ab=`grep -i "Break" *.log`
	    if [ "$ab" = "" ]
	        then
	        fp=`grep -i  "Floating point exception" *.log`
	        if [ "$fp" = "" ]
		    then
		    kl=`grep -i  "Killed" *.log`
		    if [ "$kl" = "" ]
		        then
		        bf=`grep -i "busy flag cleared" *.log`
		        if [ "$bf" = "" ]
                            then
			    cf=`grep -i "Compilation failed" *.log`
		            if [ "$cf" = "" ]
				then
				echo "* ----------------   Job Validated  ------------------*" >> stdout;
				error="0";
                            else
				echo "* #             Check Macro failed !                  #" >> stdout;
                            fi
                        fi
		    fi
	        fi
            fi
        fi
    fi
fi
if [ "$error" = "1" ] 
    then
    echo "* ################   Job not validated ################" >> stdout;
fi
echo "* ----------------------------------------------------*" >> stdout;
echo "*******************************************************" >> stdout;
sleep 15;
cd -
exit $error
