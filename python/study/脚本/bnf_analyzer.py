#!/usr/bin/python
#-*-coding:utf-8-*-

"""
BNF文件分析自动生成语料脚本工具
@author: feixu@iflytek.com
@version: 1.0.3
"""

import codecs
import re
import itertools
import os
import sys
import optparse

class Bnf(object):
	'''
	bnf文件对象类
	'''
	def __init__(self,bnf_file):
		self.bnf_file = bnf_file
		fo = codecs.open(bnf_file,'r','gbk')
		self.file_text = fo.read()
		fo.close()
		self.collect_all_tags()
		self.init_all_tags()
		self.gen_start_tag_regular_exp()

	def collect_all_tags(self):
		'''
		收集所有的标签
		<node1>:xxxxxxxxx这就是一个标签
		将这些标签收集到实例变量tags_dict字典中，key是标签名，value就是冒号后面的内容
		'''
		self.tags_dict = {}
		tag_pattern = re.compile(r'<([^>]+)>:(.*?)<[^>]+>:',re.M|re.S)
		end_tag_pattern = re.compile(r'<([^>]+)>:(.*)',re.M|re.S)	#注意不能使用懒惰匹配
		start_tag_pattern = re.compile(r'!start\s+<([^>]+)>;',re.M|re.S)
		start_tag_match = start_tag_pattern.search(self.file_text)
		self.start_tag = start_tag_match.group(1)
		pos = 0
		tag_match = tag_pattern.search(self.file_text,pos)
		while True:
			if not tag_match:
				tag_match = end_tag_pattern.search(self.file_text,pos)
				if not tag_match:
					break
			self.tags_dict[tag_match.group(1)] = tag_match.group(2)
			pos = tag_match.end(2)
			tag_match = tag_pattern.search(self.file_text,pos)

	def init_all_tags(self):
		'''
		处理所有的标签
		collect_all_tags收集完所有的标签后，存储在tags_dict字典中
		有些标签存在嵌套关系，如下：
		<CmdAllStartp>: <patch> | <CmdVehicStartp> | <CmdCommStartp>
		<CmdVehicStartp>: <cmdvehiclep>|([<actemprep>]<actemp>)|(<fanlevprep><fanlevelp>);
		<CmdCommStartp>: <cmdcommp>;
		以及标签中后面都带有分号，以及还有换行，本方法用来将嵌套标签合并，去除分号，去除换行等初始化操作
		顺便将中括号替换成括号，为了方便转换成正则表达式用
		'''
		
		#用来存储标签的类型，有些标签的值是子标签，有些标签的值是语料信息
		self.tag_type_dict = {}
		tag_pattern = re.compile(r'.*<([^>]+)>.*',re.M|re.S)
		for key,value in self.tags_dict.items():
			if tag_pattern.match(value):
				if key==self.start_tag:
					self.tag_type_dict[key] = 'start'
				else:
					self.tag_type_dict[key] = 'sub_tag'
			else:
				self.tag_type_dict[key] = 'solt'

		sub_tag_arr = self.get_all_sub_tag()
		start_tag_value = self.tags_dict[self.start_tag]

		while True:
			'''
			这里防止出现有多层标签嵌套的情况，所以使用while循环进行多次检测
			'''
			finished = True
			for sub_tag in sub_tag_arr:
				start_tag_value = re.sub('<'+sub_tag+'>','('+self.tags_dict[sub_tag]+')',start_tag_value)
			for sub_tag in sub_tag_arr:
				if sub_tag in  start_tag_value:
					finished = False
			if finished:
				break
		self.tags_dict[self.start_tag] = start_tag_value

		#统一进行清理与替换
		for key,value in self.tags_dict.items():
			self.tags_dict[key] = strings.clear_comments(value)
			self.tags_dict[key] = strings.clear_space_char(self.tags_dict[key])
			self.tags_dict[key] = strings.clear_semicolon_char(self.tags_dict[key])

			#在bnf文件的语法中，中括号里的内容可有可无
			if not key==self.start_tag:
				self.tags_dict[key] = strings.replace_middle_bracket(self.tags_dict[key])
			self.tags_dict[key] = strings.clear_id_info(self.tags_dict[key])

	def get_all_sub_tag(self):
		sub_tag_arr = []
		for key,value in self.tag_type_dict.items():
			if value=='sub_tag':
				sub_tag_arr.append(key)
		return sub_tag_arr

	def get_all_matched_yf_exp(self):
		'''
		返回所有能否匹配表达式的语法表达式
		'''
		match_dict = {}
		tag_arr = self.get_regular_exp_all_tag()
		comb_arr = Combination.get_arr_combinations(tag_arr)
		regular_exp = self.start_tag_regular_exp
		for comb in comb_arr:
			if re.match(regular_exp,comb):
				match_dict[comb] = 1
		return match_dict.keys()

	def get_all_yl_by_yf(self,yf_str,filter_dict):
		'''
		通过语法字符串<play><fm><suff>来获取所有组合的语料
		'''
		yf_tags = strings.get_string_tags(yf_str)
		yf_tag_arr = []
		for yf_tag in yf_tags:
			if yf_tag in filter_dict.keys():
				yf_tag_arr.append(self.tags_dict[yf_tag].split('|')[:filter_dict[yf_tag]])
			else:
				yf_tag_arr.append(self.tags_dict[yf_tag].split('|'))
		return Combination.get_arrs_product(yf_tag_arr)

	def gen_start_tag_regular_exp(self):
		#将表达式整理成有效的正则表达式
		self.start_tag_regular_exp = self.tags_dict[self.start_tag]
		#self.start_tag_regular_exp = re.sub('\)',')?',self.start_tag_regular_exp)
		#self.start_tag_regular_exp = '^('+self.start_tag_regular_exp+')?$'
		self.start_tag_regular_exp = re.sub('\[','(',self.start_tag_regular_exp)
		self.start_tag_regular_exp = re.sub('\]',')?',self.start_tag_regular_exp)
		self.start_tag_regular_exp = '^('+self.start_tag_regular_exp+')$'


	def get_regular_exp_all_tag(self):
		'''
		根据正则表达式中出现的所有的节点的顺序，将所有的节点取出来【需要有顺序】
		'''
		tag_pattern = re.compile(r'(<[^>]+>)',re.M|re.S)
		return tag_pattern.findall(self.start_tag_regular_exp)

class Combination(object):
	'''
	排列组合类
	封装各种排列组合的方法
	生成笛卡儿积等
	'''
	def __init__(self):
		pass

	@staticmethod
	def get_arr_combinations(arr):
		'''
		获取给定数组从1位到最大位的各种组合
		'''
		combinations_res = []
		for i in range(1,len(arr)+1):
			combinations_res.extend(map(lambda x:''.join(x),list(itertools.combinations(arr,i))))
		return combinations_res

	@staticmethod
	def get_arrs_product(arrs):
		'''
		获取多个数组的笛卡儿积
		'''
		product_arr = []
		method_p_str = ','.join(map(lambda x:'arrs[{0}]'.format(x),range(len(arrs))))
		for x in eval('itertools.product({0})'.format(method_p_str)):
			product_arr.append(''.join(x))
		return product_arr

class strings(object):

	def __init__(self):
		pass

	@staticmethod
	def get_last_line_str_len(str):
		'''
		获取多行字符串中最后一行的字符串长度
		'''
		return len(str.split('\n')[-1])

	@staticmethod
	def clear_space_char(str):
		'''
		清除字符串中的换行与空白
		'''
		return re.sub('\s','',str)

	@staticmethod
	def clear_semicolon_char(str):
		'''
		清除所有分号字符
		'''
		return re.sub(';','',str)

	@staticmethod
	def clear_id_info(str):
		'''
		清除id信息
		'''
		return re.sub('!id\(\d+\)','',str)

	@staticmethod
	def clear_comments(str):
		'''
		清除所有注释
		'''
		ret_str = ""
		count = 0
		for s in str.splitlines():
			if re.match(r'\/\/.*',s):
				continue
			else:
				ret_str+=s
		return ret_str

	@staticmethod
	def get_string_tags(str):
		'''
		获取一个字符串中的所有的标签
		'''
		tag_pattern = re.compile(r'<([^>]+)>',re.M|re.S)
		return tag_pattern.findall(str)

	@staticmethod
	def replace_middle_bracket(str):
		'''
		替换中括号为小括号
		'''
		str = re.sub('\[','(',str)
		str = re.sub('\]',')',str)
		return str

class fs(object):
	'''
	文件操作类
	'''

	def __init__(self):
		pass

	@staticmethod
	def dir_exists(dir_path):
		'''
		判断目录是否存在
		'''
		return os.path.exists(dir_path)

	@staticmethod
	def make_dir(dir_path):
		'''
		文件夹不存在就创建，如果存在就不管
		'''
		if fs.dir_exists(dir_path):
			return
		else:
			os.makedirs(dir_path)

	@staticmethod
	def get_file_name_without_ext(file_path):
		'''
		获取文件路径中的文件名，不包含扩展名
		'''
		basename = os.path.basename(file_path)
		return os.path.splitext(basename)[0]

def main(args=sys.argv[1:]):
	args=sys.argv[1:]
	parser = optparse.OptionParser()
	parser.add_option("-n","--noyl",
						dest="noyl",
						default=False,
						action="store_true",
						help=u'不获取具体的语料信息')

	parser.add_option("-m","--matched",
						dest="matched",
						default=False,
						action="store_true",
						help=u'获取所有匹配的语法表达式')

	parser.add_option("-f", "--filter",
						dest="filter",
						default=None,
						action='append',
						help=u'使用条件来进行过滤，如：-f song-100 -f name-20  意思就是song中随机获取100条记录，name中随机获取20条记录,为了方式数量过多导致内存撑爆')

	parser.add_option("-i","--info",
						dest="info",
						default=False,
						action="store_true",
						help=u'打印bnf文件中基本信息，各节点的元素数量')

	parser.add_option("-b","--bnffile",
						dest="bnffile",
						default=None,
						type="string",
						help=u'指定需要分析的bnf文件')

	options, args = parser.parse_args(args)

	if not options.bnffile:
		print u"bnf文件没有指定".encode('gbk')
		exit(-1)

	filter_dict = {}
	if options.filter:
		for filter_info in options.filter:
			filter_dict[filter_info.split('-')[0]] = int(filter_info.split('-')[1])

	dir_name = fs.get_file_name_without_ext(options.bnffile)
	fs.make_dir(dir_name)
	report_file = os.path.join(dir_name,'report.txt')
	bnf_obj = Bnf(options.bnffile)
	match_yf_arr = bnf_obj.get_all_matched_yf_exp()

	report_dict = {}		#定义报告字典容器，用来收集结果信息，便于后期报告整理

	if options.info:
		for key,value in bnf_obj.tags_dict.items():
			print "{0}---{1}".format(key,len(value.split('|')))

	if options.matched:
		for match_yf in match_yf_arr:
			print match_yf
		exit(-2)

	if options.noyl:
		exit(-3)

	rep_fo = codecs.open(report_file,'w','gbk')

	for match_yf in match_yf_arr:
		print u"正在处理语法表达式:[{0}]".encode('gbk').format(match_yf)
		out_file_name = os.path.join(dir_name,'{0}.txt'.format('-'.join(strings.get_string_tags(match_yf))))
		fo = codecs.open(out_file_name,'w','gbk')
		all_yl = bnf_obj.get_all_yl_by_yf(match_yf,filter_dict)
		report_dict[match_yf] = len(all_yl)				#获取语料的组合数量存到报告字典中
		for line in all_yl:			
			fo.write(line+'\n')		#将所有的语料写入到文档中
		fo.close()		#关闭文件
		print u"表达式:[{0}]---处理完成".encode('gbk').format(match_yf)

	yl_sum = 0		#用来统计所有语料的总条数

	#遍历report字典来整理报告文件
	print u"开始生成结果报告……".encode('gbk')
	for key,value in report_dict.items():
		rep_fo.write(u"语法:{0}====组合数量:{1}\n".format(key,value))
		yl_sum+=value
	rep_fo.write(u"语料总共数量:{0}".format(yl_sum))
	rep_fo.close()
	print u"生成结果报告完成".encode('gbk')



if __name__ == "__main__":
	main()


