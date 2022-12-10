#!/bin/env python
# -*- coding: utf-8 -*-

import os
import platform
import sys
from enum import Enum

execProgStr = sys.argv[1]

class DataSourceFileType(Enum):
    noinput = 0
    input = 1
    readtxt = 2
    readdat = 3

class ResultFileType(Enum):
    nooutput = 0
    output = 1
    writetxt = 2
    writedat = 3

class PlatformType(Enum):
    other = 0
    windows = 1
    linux = 2

# 读取文件
def read_file(file_name):
    try:
        file_desc = open(file_name, 'r')
        # 读取后按行分割
        text = file_desc.read().splitlines()
        file_desc.close()
        return text
    except IOError as error:
        print('Read input file Error: {0}'.format(error))
        sys.exit()

# 比较时忽略行末的空格和文件末尾的回车换行
def advanced_file_compare(file1, file2):
    f1 = open(file1)
    f2 = open(file2)
   
    returnVal = 1

    str1 = []
    for line1 in f1:
        line1 = line1.rstrip()
        line1 = line1.replace('\n', '')
        if len(line1) != 0:
            str1.append(line1)
    str2 = []
    for line2 in f2:
        line2 = line2.rstrip()
        line2 = line2.replace('\n', '')
        if len(line2) != 0:
            str2.append(line2)
    
    count = 0
    if len(str1) == len(str2):
        for line1 in str1:
            if line1 != str2[count]: #文件不同
                returnVal = 0
                break
            else:
                count = count + 1
    else:
        returnVal = 0
           
    f1.close()
    f2.close()
   
    return returnVal

def advanced_dat_file_compare(file1, file2):
    returnVal = 1
    runCommand = "diff {0} {1} -b -B -y -i -W 100".format(file1, file2)                
    execResult = os.system(runCommand)
    if execResult != 0:
        returnVal = 0
    return returnVal

def getPlatformType():
    platformType = PlatformType.other
    if platform.system().lower() == 'windows':
        platformType = PlatformType.windows
    elif platform.system().lower() == 'linux':
        platformType = PlatformType.linux
    else:
        platformType = PlatformType.other
    return platformType

def getProjInfo():
    dsType = getDataSourceFileType()
    dsFileStr = getDataSourceFileStr(dsType)
    resultFileType = getResultFileType()  
    resultFileStr = getResultFileStr(resultFileType)  
    userresultFileStr = getUserResultFileStr(resultFileType)  
    return dsType, dsFileStr, resultFileType, userresultFileStr, resultFileStr

def getDataSourceFileType():
    dsType = DataSourceFileType.noinput
    if os.path.isfile("input1.txt"):
        dsType = DataSourceFileType.input
    elif os.path.isfile("readfile1.txt"):
        dsType = DataSourceFileType.readtxt
    elif os.path.isfile("readfile1.dat"):
        dsType = DataSourceFileType.readdat
    else:
        dsType = DataSourceFileType.noinput
    return dsType

def getDataSourceFileStr(dataSourceFileType):
    dsType = dataSourceFileType
    dsFileStr = ""
    if dsType == DataSourceFileType.readtxt:
        dsFileStr = "readfile{0}.txt"
    elif dsType == DataSourceFileType.readdat:
        dsFileStr = "readfile{0}.dat"
    elif dsType == DataSourceFileType.input:
        dsFileStr = "input{0}.txt"
    return dsFileStr

def getResultFileType():
    resultFileType = 0
    if os.path.isfile("output1.txt"):
        resultFileType = ResultFileType.output
    elif os.path.isfile("writefile1.txt"):
        resultFileType = ResultFileType.writetxt
    elif os.path.isfile("writefile1.dat"):
        resultFileType = ResultFileType.writedat   
    else:
        resultFileType = ResultFileType.nooutput
    return resultFileType

def getResultFileStr(resultFileType):
    resultFileStr = ""
    type = resultFileType
    if type == ResultFileType.output:
        resultFileStr = "output{0}.txt"
    elif type == ResultFileType.writetxt:
        resultFileStr = "writefile{0}.txt"
    elif type == ResultFileType.writedat:
        resultFileStr = "writefile{0}.dat"
    return resultFileStr

def getUserResultFileStr(resultFileType):
    userresultFileStr = ""
    type = resultFileType
    if type == ResultFileType.output:
        userresultFileStr = "user_output{0}.txt"
    elif type == ResultFileType.writetxt:
        userresultFileStr = "user_writefile{0}.txt"
    elif type == ResultFileType.writedat:
        userresultFileStr = "user_writefile{0}.dat"
    return userresultFileStr

def getSpecifyfileStr():
    specifyFile = ""
    if os.path.isfile("file.dat"):
        specifyFile = "file.dat"
    elif os.path.isfile("file.txt"):
        specifyFile = "file.txt"
    return specifyFile

def getRunProgCmdStr(dsType, resultFileType, dsFile, userresultFile):
    global execProgStr
    inresultFileStr = ""
    if dsType == DataSourceFileType.input:
        if resultFileType == ResultFileType.output:
            inresultFileStr = " < {0} > {1} ".format(dsFile, userresultFile)
        elif resultFileType == ResultFileType.writedat or resultFileType == ResultFileType.writetxt:
            inresultFileStr = " {1} < {0} ".format(dsFile, userresultFile)
    elif dsType == DataSourceFileType.readtxt or dsType == DataSourceFileType.readdat:
        if resultFileType == ResultFileType.output:
            inresultFileStr = " {0} > {1} ".format(dsFile, userresultFile)
        elif resultFileType == ResultFileType.writedat or resultFileType == ResultFileType.writetxt:
            inresultFileStr = " {0} {1} ".format(dsFile, userresultFile)
    else:
        if resultFileType == ResultFileType.output:
            inresultFileStr = " > {0} ".format(userresultFile)   
        elif resultFileType == ResultFileType.writedat or resultFileType == ResultFileType.writetxt:
            inresultFileStr = " {0} ".format(userresultFile)          

    runCommandStr = execProgStr
    runCommandStr += inresultFileStr
    return runCommandStr

def runCmd(cmdStr):
    print("正在执行命令: {0}".format(cmdStr))
    sys.stdout.flush() 
    execResult = os.system(cmdStr) 
    if execResult != 0:
        exit(1)    
