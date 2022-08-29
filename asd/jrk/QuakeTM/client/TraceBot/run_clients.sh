export TERM=xterm;

BASEDIR=`dirname $0`
BASEDIR=`cd $BASEDIR; pwd`
echo $BASEDIR
cd $BASEDIR

clear
echo "Script for starting Quake clients";
echo "---------------------------------------";
echo;


if test $# -eq 1 
then
  CNT=$1;
else
  CNT=1;
fi

echo "starting $CNT clients in the backend" ;
echo;

for ((i=1;i<=$CNT;i++))
do
  echo "client $i ... start";
  #ddd -x rungdb ./glqwclient &
  #screen -d -m gdb -x rungdb --args glqwclient -run_trace -name player$i >player$i
  $BASEDIR/glqwclient -run_trace -name player$i >output/player$i &
  sleep 0.1;
done;

exit 0;





