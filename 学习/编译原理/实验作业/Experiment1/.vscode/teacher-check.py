#!/bin/env python
# -*- coding: utf-8 -*-

import argparse
import difflib
import filecmp
import os
import platform
import subprocess
import sys
import webbrowser
from enum import Enum

from comtool import *

class CmpFileType:
    unknown = 0
    templatedatasource = 1
    templateresult = 2
    userprojresult = 3



def getFileCount(fileStr):
    fileCount = 1
    while 1:
        resultFile = fileStr.format(fileCount)
        if not os.path.isfile(resultFile):
            fileCount -= 1
            break
        fileCount += 1
    return fileCount

# 比较两个文件并把结果生成一份html文本
def compare_file(file1, file2, type):
    returnVal = 0
    bDat = False
    if file1 == "" or file2 == "":
        print('文件路径不能为空：第一个文件的路径：{0}, 第二个文件的路径：{1} .'.format(file1, file2))
        sys.exit()
    else:
        resultFileType = getResultFileType()
        dsFileType = getDataSourceFileType()
        if type == CmpFileType.userprojresult:
            promptInfo = "正在比较标准答案结果文件 {0} 和用户编写的应用程序结果文件 {1}"
            if resultFileType == ResultFileType.writedat:
                bDat = True
        elif type == CmpFileType.templateresult:
            promptInfo = "正在比较模板中的标准答案结果文件 {0} 和用户程序中的标准答案结果文件 {1}"
            if resultFileType == ResultFileType.writedat:
                bDat = True
        elif type == CmpFileType.templatedatasource:
            promptInfo = "正在比较模板中的数据源文件 {0} 和用户程序中的数据源文件 {1}"
            if dsFileType == DataSourceFileType.readdat:
                bDat = True
        print(promptInfo.format(file1, file2), end=': ')
        sys.stdout.flush() 

    if os.path.isfile(file1) and os.path.isfile(file2):
        comResult = 0
        if bDat:
            comResult = advanced_dat_file_compare(file1, file2)
        else:
            comResult = advanced_file_compare(file1, file2)
        if comResult == 1:
            print("文件相同")
            print()
            sys.stdout.flush() 
            returnVal = 1
        else:
            print("文件不同")
            print()
            sys.stdout.flush() 
    return returnVal

if __name__ == "__main__":

    dsType, dsFileStr, resultFileType, userresultFileStr, resultFileStr = getProjInfo()  

     # 检查数据源文件一致性
    dsFileCount = getFileCount(dsFileStr)
    for i in range(1, dsFileCount + 1):
        dsFile = dsFileStr.format(i)
        userresultFile = userresultFileStr.format(i)
        runProgCommand = getRunProgCmdStr(dsType, resultFileType, dsFile, userresultFile)
        
        runCmd(runProgCommand)
    
        print("正在检查{0}文件的一致性".format(dsFile))  
        sys.stdout.flush()

        if compare_file("template/{0}".format(dsFile), dsFile, CmpFileType.templatedatasource) == 0:
            runCmpCommand = "diff template/{0} {0} -b -B -y -i --suppress-common-lines".format(dsFile)
            runCmd(runCmpCommand)
            exit(1)
 
    # 检查输出文件的一致性
    resultFileCount = getFileCount(resultFileStr)
    for i in range(1, resultFileCount + 1):
        dsFile = dsFileStr.format(i)
        userresultFile = userresultFileStr.format(i)
        runProgCommand = getRunProgCmdStr(dsType, resultFileType, dsFile, userresultFile)
        runCmd(runProgCommand)
        resultFile = resultFileStr.format(i)
        print("正在检查{0}文件的一致性".format(resultFile)) 
        sys.stdout.flush() 

        if compare_file( "template/{0}".format(resultFile), resultFile, CmpFileType.templateresult) == 0:
            runCmpCommand = "diff template/{0} {0} -b -B -y -i --suppress-common-lines".format(resultFile)
            runCmd(runCmpCommand)
            exit(1)

    # 执行完成文件完整性检查
    os.system("fileidentity.sh")

    if sys.argv[2] != "":
        # 克隆教师检查项目，参数：${CODECODE_PROTOCOL}gitlab-ci-token:${CI_JOB_TOKEN}@${CODECODE_DOMAIN}/${TEACHERCHECK_REPO}
        runCloneCommand = "git clone {0} teacher_check".format(sys.argv[3])     
        runCmd(runCloneCommand)
       
        teaCheckCount = getFileCount("teacher_check/" + dsFileStr)
        for i in range(1, teaCheckCount + 1):
            print("正在检查附加算例case{0}".format(i))
            sys.stdout.flush() 
        #    runProgCommand = "./app.exe < teacher_check/input{0}.txt > user_output{0}.txt".format(i)
        #    execResult = os.system(runProgCommand) 
            dsFile = "teacher_check/{0}".format(dsFileStr.format(i))
            userresultFile = userresultFileStr.format(i)

            runProgCmdStr = getRunProgCmdStr(dsType, resultFileType, dsFile, userresultFile)
            runCmd(runProgCmdStr)
            
            resultFile = resultFileStr.format(i)

            if compare_file( "teacher_check/{0}".format(resultFile), userresultFile, CmpFileType.userprojresult) == 0: 
                runCmpCommand = "diff teacher_check/{0} {1} -b -B -y -i -W 100".format(resultFile, userresultFile)
                runCmd(runCmpCommand)
                exit(1)

        # 处理读取指定文件
        if teaCheckCount == 0:
            specifyfile = getSpecifyfileStr()
            if os.path.isfile("teacher_check/" + specifyfile):
                runCpCommand = "cp teacher_check/{0} {0}".format(specifyfile)
                runCmd(runCpCommand)
                print("正在检查附加算例")
                userresultFile = userresultFileStr.format(1)
                runProgCmdStr = getRunProgCmdStr(dsType, resultFileType, "", userresultFile)
                runCmd(runProgCmdStr)

                resultFile = resultFileStr.format(1)

                if compare_file( "teacher_check/{0}".format(resultFile), userresultFile, CmpFileType.userprojresult) == 0:                
                    runCmpCommand = "diff teacher_check/{0} {1} -b -B -y -i -W 100".format(resultFile, userresultFile)
                    runCmd(runCmpCommand)
                    exit(1)
        # 执行完成附加算例检查
        os.system("extracase.sh")
   