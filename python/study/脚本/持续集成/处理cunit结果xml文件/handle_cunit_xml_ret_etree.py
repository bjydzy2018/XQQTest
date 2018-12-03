# coding=utf-8

__author__ = 'hcliu'

try:
    import xml.etree.cElementTree as ET
except ImportError:
    import xml.etree.ElementTree as ET
from collections import defaultdict


def get_failure_records(tg_file):
    tree = ET.parse(tg_file)

    # 存储失败的断言信息
    failure_records = defaultdict(dict)

    for e, e1, e2, e3 in zip(tree.iterfind('.//CUNIT_RUN_TEST_FAILURE/TEST_NAME'), tree.iterfind('.//CUNIT_RUN_TEST_FAILURE/FILE_NAME'), tree.iterfind('.//CUNIT_RUN_TEST_FAILURE/LINE_NUMBER'), tree.iterfind('.//CUNIT_RUN_TEST_FAILURE/CONDITION')):
        failure_records[e.text.strip()]['FILE_NAME'] = e1.text.strip()

        if 'CONDITION' not in failure_records[e.text.strip()]:
            failure_records[e.text.strip()]['CONDITION'] = []

        failure_records[e.text.strip()]['CONDITION'].append(
            (e2.text.strip(), e3.text.strip()))

    return failure_records


def get_sum_records(tg_file):
    tree = ET.parse(tg_file)

    case_total = 0
    case_run = 0
    case_success = 0
    case_fail = 0

    assert_total = 0
    assert_run = 0
    assert_success = 0
    assert_fail = 0

    for e, e1, e2, e3, e4 in zip(tree.iterfind('.//CUNIT_RUN_SUMMARY_RECORD/TYPE'), tree.iterfind('.//CUNIT_RUN_SUMMARY_RECORD/TOTAL'), tree.iterfind('.//CUNIT_RUN_SUMMARY_RECORD/RUN'), tree.iterfind('.//CUNIT_RUN_SUMMARY_RECORD/SUCCEEDED'), tree.iterfind('.//CUNIT_RUN_SUMMARY_RECORD/FAILED')):
        if e.text.strip() == 'Test Cases':
            case_total = e1.text.strip()
            case_run = e2.text.strip()
            case_success = e3.text.strip()
            case_fail = e4.text.strip()

        elif e.text.strip() == 'Assertions':
            assert_total = e1.text.strip()
            assert_run = e2.text.strip()
            assert_success = e3.text.strip()
            assert_fail = e4.text.strip()

    return ((case_total, case_run, case_success, case_fail), (assert_total, assert_run, assert_success, assert_fail))

if __name__ == '__main__':
    import xlwt
    import argparse

    parser = argparse.ArgumentParser(description='从cunit结果xml文件中提取必要信息并进行规整')
    parser.add_argument('-f', '--file', dest='file',
                        nargs='+', help='需要处理的结果文件')
    args = parser.parse_args()

    tg_files = args.file
    file = xlwt.Workbook()

    for tg_file in tg_files:
        failure_records = get_failure_records(tg_file)
        sum_records = get_sum_records(tg_file)

        table = file.add_sheet(
            (tg_file.replace('\\', '/').split('/'))[-1].split('.')[0])
        row = 0

        table.write(row, 0, '统计')
        table.write(row, 1, '总数')
        table.write(row, 2, '运行')
        table.write(row, 3, '成功')
        table.write(row, 4, '失败')

        row += 1
        table.write(row, 0, '用例')
        table.write(row, 1, sum_records[0][0])
        table.write(row, 2, sum_records[0][1])
        table.write(row, 3, sum_records[0][2])
        table.write(row, 4, sum_records[0][3])

        row += 1
        table.write(row, 0, '断言')
        table.write(row, 1, sum_records[1][0])
        table.write(row, 2, sum_records[1][1])
        table.write(row, 3, sum_records[1][2])
        table.write(row, 4, sum_records[1][3])

        row += 2
        table.write(row, 0, '失败用例统计')

        row += 1
        table.write(row, 0, '用例')
        table.write(row, 1, '文件名')
        table.write(row, 2, '行号')
        table.write(row, 3, '断言条件')

        for failure_record in failure_records:
            row += 1

            table.write(row, 0, failure_record)
            table.write(row, 1, failure_records[failure_record]['FILE_NAME'])

            for line, cond in failure_records[failure_record]['CONDITION']:
                table.write(row, 2, line)
                table.write(row, 3, cond)

                row += 1

    file.save('接口测试结果.xls')
