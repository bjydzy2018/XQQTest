#coding=utf-8

import re

line = "E:/project/SE_iFlytekSpeechSuite/Tags/Build1071/Project/07.Release/win32/bin/PcmRes/mvw/SceneId-8/接听/接听市区半开窗/M005_接听_009.wav"

line1 = "Ivw success!!场景是：8 ID是：0 得分是： 161"

match = re.match(".*(SceneId-./.*)", line)

# if match:
#     print match.group(1)

# fp = open('1.txt')
# for line in fp:
#     print line
#     print line.replace('\\', '/')

# import sys
# print len(sys.argv)
# for i in range(1, len(sys.argv)):
#     print i

print ('123'.ljust(10), 456)