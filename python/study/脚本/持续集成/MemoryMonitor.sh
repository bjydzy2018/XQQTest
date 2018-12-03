#Description:	Cross platform performance monitoring
#Author:		xinli3
#Version:		1.0  
#CreateTime:	2017-03-17

MemoryMonitor()
{
	echo $1 $2 $3
	if [ $# -lt 2 ];then
		echo "Usage: MemoryMonitor.sh argv[1] argv[2] [argv[3]]";
		echo "       argv[1]: Support Plantform: qnx, android, arm-linux, intel64.";
		echo "       argv[2]: The process that you want to monitor.";
		echo "       argv[3]: Optional, The output file name.";
		exit 0;
	fi
	
	if [ $# -eq 3 ];then
		outName=$3;
	else
		outName="./memory.csv";
	fi
	
	#is process alive
	if [ $1 == "android" ];then
		pid=`ps | grep -v grep | grep $2 | busybox awk NR==1'{print $2}'`;
	else
		pid=`ps -A | grep -v grep | grep $2 | awk NR==1'{print $1}'`;
	fi
	echo $pid
	
	if [ $? -ne 0 ];then
		echo "process does not exist: "$2
		exit 0
	else
		echo "Memory Monitor start..."
	fi
	
	#get plantform
	if [ "$1" == "qnx" ];then
		echo 'Total,Code,Data,Heap,Stack,Other,Pid,Command' > "$outName";
		echo 'Total,Code,Data,Heap,Stack,Other,Pid,Command';
		while true;do
			tmp=`showmem -P $pid | grep $2`
			if [ $? -ne 0 ];then
				echo "process does not exist: "$2;
				exit 0;
			else
				sleep 1;
			fi
			echo $tmp | sed "s/[ ][ ]*/,/g" >> "$outName";
			echo $tmp;
		done
	elif [ $1 == "arm-linux" ];then
		echo 'PID,USER,PR,NI,VIRT,RES,SHR,S,%CPU,%MEM,TIME+,COMMAND' > "$outName";
		echo 'PID,USER,PR,NI,VIRT,RES,SHR,S,%CPU,%MEM,TIME+,COMMAND';
		while true;do
			tmp=`/usr/bin/top -bcn 1 -p $pid | grep $2`
			if [ $? -ne 0 ];then
				echo "process does not exist: "$2;
				exit 0;
			else
				sleep 1;
			fi
			echo $tmp | sed "s/[ ][ ]*/,/g" | sed "s/,//" >> "$outName";
			echo $tmp;
		done
	elif [ $1 == "android" ];then
		echo 'PID,PR,CPU%,S,#THR,VSS,RSS,UID,Name' > "$outName";
		echo 'PID,PR,CPU%,S,#THR,VSS,RSS,UID,Name';
		while true;do
			tmp=`top -n 1 | grep "$pid" | grep $2`
			if [ $? -ne 0 ];then
				echo "process does not exist: "$2;
				exit 0;
			else
				sleep 1;
			fi
			echo $tmp | busybox sed "s/[ ][ ]*/,/g" >> "$outName";
			echo $tmp;
		done
	elif [ $1 == "intel64" ];then
		echo 'PID,PPID,USER,STAT,VSZ,%VSZ,CPU,%CPU,COMMAND' > "$outName";
		echo 'PID,PPID,USER,STAT,VSZ,%VSZ,CPU,%CPU,COMMAND';
		while true;do
			tmp=`top -n 1 | grep $pid | grep $2`
			if [ $? -ne 0 ];then
				echo "process does not exist: "$2;
				exit 0;
			else
				sleep 1;
			fi
			echo $tmp | sed "s/[ ][ ]*/,/g" >> "$outName"
			echo $tmp
		done
	else
		echo "plantform does not support."
		echo "Support Plantform: qnx, android, arm-linux, intel64"
	fi
}

MemoryMonitor $@