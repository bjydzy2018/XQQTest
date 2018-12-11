#by qqxiong


#业务			数目	序号
#airControl		11		1-11
#app			20		12-31
#carControl		3		32-34
#cmd			20		35-54										
#flight			12		55-66
#mapU			20		67-86
#musicX			20		87-106
#news			17		107-123
#radio			20		124-143
#scheduleX		7		144-150
#stock			19		151-169
#telephone		20		170-189
#train			3		190-192
#video			15		193-207
#weather		20		208-227
#weixin			13		228-240

import csv
import sys

# 默认参数，当没有命令行传参时 使用这些参数
strIn = 'TestISS-AIUI-eff-android-wifi.csv'
strOut = '某个业务响应时间.txt'
str="1-11"

# 读取命令行参数
if(len(sys.argv) != 1 and len(sys.argv) != 4):
	print('Invalid Arg! len(sys.argv)=%d' % (len(sys.argv)))
	print('Uasge: %s <strTestIssAIUILogFileName.log> <strVadInfoFileName.txt> <strTestIssAIUILogEffOutputFileName.csv>'
		% (sys.argv[0]))
	sys.exit(1)
if(len(sys.argv) == 4):
	strIn = sys.argv[1]
	strOut = sys.argv[3]
	str = sys.argv[2]

start = int(str.split('-')[0])
end = int(str.split('-')[1])
out=open(strOut,'w+')
Count = 0
with open(strIn, 'r', encoding="utf-8") as csvfile:
	reader = csv.reader(csvfile)
	for row in reader:
		column = row[4]
		#print(column)
		if(Count>=start and Count<=end):
			out.writelines(column)
			out.writelines('\n')
		Count = Count +1
	
out.close()




