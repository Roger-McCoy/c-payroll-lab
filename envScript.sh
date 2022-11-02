#!/bin/bash
# The first line, called 'shebang - #!' indicates use the bash shell to evaluate the script.
#!bin/perl would have requested the use of Perl to evaluate the script.
# Usually lines beginning with a pound sign are comments.

echo "Welcome to COSC2347 C executing on host $HOSTNAME running $OSTYPE."
echo "Your user id is $USER in home directory $HOME."
echo "Your system is executing version $BASH_VERSION."
echo "Your current path is $PATH, present working directory $PWD."
echo "Your previous working directory was $OLDPWD."
echo "The process ID of the interpreter (or script) $PPID "
# "with $# arguments accessible as $0, $1, $2, ... "

sleep 20
echo "After a 20 second nap, you have been running $SECONDS seconds."
exit

#Execute in current directory: ./envScript.sh #BUT, WON'T WORK INITIALLY.
#To actually execute the script, you must change it's permissions.
#First, check it's initial permissions with:
# ls -l *.sh #This lists every .sh file in the directory's permissions (*wildcard operator)
#Second, actually modify permissions with(+x think +executable):
#chmod +x envScript.sh
