# coding=utf-8

__author__ = 'hcliu'

from bs4 import BeautifulSoup
from collections import defaultdict


def get_failure_records(tg_file):
    soup = BeautifulSoup(open(tg_file), "lxml-xml")

    # 存储失败的断言信息
    failure_records = defaultdict(dict)

    for failure_record in soup.find_all('CUNIT_RUN_TEST_FAILURE'):
        failure_records[failure_record.TEST_NAME.string][
            'FILE_NAME'] = failure_record.FILE_NAME.string.strip()
        if 'CONDITION' not in failure_records[failure_record.TEST_NAME.string]:
            failure_records[failure_record.TEST_NAME.string]['CONDITION'] = []
        failure_records[failure_record.TEST_NAME.string]['CONDITION'].append(
            (failure_record.LINE_NUMBER.string.strip(), failure_record.CONDITION.string.strip()))

    return failure_records


def get_sum_records(tg_file):
    soup = BeautifulSoup(open(tg_file), "lxml-xml")

    case_total = 0
    case_run = 0
    case_success = 0
    case_fail = 0

    assert_total = 0
    assert_run = 0
    assert_success = 0
    assert_fail = 0

    for sum_record in soup.find_all('CUNIT_RUN_SUMMARY_RECORD'):
        if sum_record.TYPE.string.strip() == 'Test Cases':
            case_total = sum_record.TOTAL.string.strip()
            case_run = sum_record.RUN.string.strip()
            case_success = sum_record.SUCCEEDED.string.strip()
            case_fail = sum_record.FAILED.string.strip()

        elif sum_record.TYPE.string.strip() == 'Assertions':
            assert_total = sum_record.TOTAL.string.strip()
            assert_run = sum_record.RUN.string.strip()
            assert_success = sum_record.SUCCEEDED.string.strip()
            assert_fail = sum_record.FAILED.string.strip()

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
