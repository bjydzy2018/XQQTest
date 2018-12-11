#by qqxiong
#业务的云端响应时间占比统计

#输出占比：<1.2s  1.2s-2s  2s-3s  >3s

import sys
# 默认参数，当没有命令行传参时 使用这些参数
strIn = '云端响应时间数据.txt'
strOut = '业务响应时间占比输出.txt'
# 读取命令行参数
if(len(sys.argv) != 1 and len(sys.argv) != 3):
	print('Invalid Arg! len(sys.argv)=%d' % (len(sys.argv)))
	sys.exit(1)
if(len(sys.argv) == 3):
	strIn = sys.argv[1]
	strOut = sys.argv[2]

f = open(strIn, 'r', encoding="utf-8")
out=open(strOut,'w+')

resultNum = 0
Sum = 0
count0 = 0
count1 = 0
count2 = 0
count3 = 0

for eachLine in f:
	line=eachLine.strip()
	if(line):
		resultNum = resultNum +1
		intLine = int(line)
		Sum = Sum + intLine
		if(intLine > 0 and intLine <= 1200):
			count0 = count0 + 1
		elif(intLine > 1200 and intLine <= 2000):
			count1 = count1 + 1
		elif(intLine > 2000 and intLine <= 3000):
			count2 = count2 + 1
		elif(3000<intLine):
			count3 = count3 + 1

#out.writelines('平均响应时间\t%.3f' % (Sum/resultNum))
#out.writelines('\n')

#out.writelines('<1.2s\t%.3f' % (count0/resultNum))
#out.writelines('\n')

#out.writelines('1.2s-2s\t%.3f' % (count1/resultNum))
#out.writelines('\n')

#out.writelines('2s-3s\t%.3f' % (count2/resultNum))
#out.writelines('\n')

#out.writelines('>3s\t%.3f' % (count3/resultNum))
#out.writelines('\n')

out.writelines('%.3f' % (Sum/resultNum))
out.writelines('\t%.3f' % (count0/resultNum))
out.writelines('\t%.3f' % (count1/resultNum))
out.writelines('\t%.3f' % (count2/resultNum))
out.writelines('\t%.3f' % (count3/resultNum))
out.writelines('\n')

out.close()

