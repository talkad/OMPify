export TERM=xterm;

BASEDIR=`dirname $0`
BASEDIR=`cd $BASEDIR; pwd`
echo $BASEDIR
cd $BASEDIR

clear
echo "Script for stoping Quake clients";
echo "---------------------------------------";
echo;


echo "Stopping all clients ...";
echo;
killall $BASEDIR/glqwclient;
#killall gdb;
exit 0;





