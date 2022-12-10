#!/bin/env python
# -*- coding: utf-8 -*-

import difflib
import sys
import argparse
import webbrowser
import os
import filecmp
import subprocess
import platform

from enum import Enum

from comtool import *
from gittool import *

class Color(Enum):
    white = 0
    green = 1
    red = 2
    yellow = 3
    

bCI = False
if len(sys.argv) > 2 and sys.argv[2] == "ci":
    bCI = True

if bCI == False:
    print(
        "************************************************************\n"
        "如果下面报告类似 ImportError: No module named 'xxx' 的错误，\n"
        "说明 python 运行环境中缺少必要的包。\n"
        "此时可以启动\"安装python包\"任务来修复 python 运行环境(需要连接互联网)。\n"
        "************************************************************\n"
    )

from colorama import Fore, init, AnsiToWin32
init(wrap=False)
stream = AnsiToWin32(sys.stderr).stream

red = lambda text: '\033[0;31;1m' + text + '\033[0m'
green = lambda text: '\033[0;32;1m' + text + '\033[0m'
yellow = lambda text: '\033[0;33;1m' + text + '\033[0m'

    
# 比较两个文件并把结果生成一份html文本
def compare_file(file1, file2, seqNum, caseCount, bCI, resultFileType, dsType):
    returnVal = 0
    if file1 == "" or file2 == "":
        print('文件路径不能为空：第一个文件的路径：{0}, 第二个文件的路径：{1} .'.format(file1, file2))
        sys.stdout.flush()
        sys.exit()
    else:
        print("正在比较标准答案结果文件 {0} 和用户编写的应用程序结果文件 {1}".format(file1, file2), end=': ')
        sys.stdout.flush()

    if os.path.isfile(file1) and os.path.isfile(file2):
        comResult = 0
        if resultFileType == ResultFileType.writedat:
            comResult = advanced_dat_file_compare(file1, file2)
        else:
            comResult = advanced_file_compare(file1, file2)

        if comResult:   
            print("文件相同")
            score = 40
            if seqNum == caseCount:
                score = 100
            else:
                score = score + 60 / caseCount * seqNum

            if bCI :
                promptInfo = "Case{0} 验证成功".format(seqNum)
                outputPromptInfo(bCI, promptInfo, Color.green)
                print("exec-score", int(score))
                print()
                sys.stdout.flush() 
            else:
                promptInfo = "Case{0} 验证成功, 分数: {1}".format(seqNum, int(score))
                outputPromptInfo(bCI, promptInfo, Color.green)

            if seqNum == caseCount:
                promptInfo = "恭喜你通过了所有测试！"
                outputPromptInfo(bCI, promptInfo, Color.green)

            returnVal = 1
            return returnVal
        else:
            print("文件不同")
            promptInfo = "Case{0} 验证失败".format(seqNum)
            outputPromptInfo(bCI, promptInfo, Color.green)

            if bCI == False and resultFileType != ResultFileType.writedat:
                text1_lines = read_file(file1)
                text2_lines = read_file(file2)
                diff = difflib.HtmlDiff()    # 创建HtmlDiff 对象
                result = diff.make_file(text1_lines, text2_lines)  # 通过make_file 方法输出 html 格式的对比结果
                # 将结果写入到result_comparation.html文件中
                try:
                    with open('result_comparation.html', 'a+', encoding="utf-8") as result_file:
                        dsFile = getDataSourceFileStr(dsType).format(seqNum)
                        resultfile = getResultFileStr(resultFileType).format(seqNum)
                        userresultfile = getUserResultFileStr(resultFileType).format(seqNum)
                        platformurl = getPlatformURL() + "/engintime/codecode/publicmanual/blob/master/comp-file-desc.md"
                        promptContent = "<p>第一次查看此页面吗？<a href={0} target='_blank'>查看页面说明</a></p>".format(platformurl)
                        promptContent += "<p>Case {0} 验证失败。使用的数据源文件是 {1}。</br>标准答案结果文件 {2}（左边）与用户编写的应用程序结果文件 {3}（右边）的比较结果：</p>".format(seqNum, dsFile, resultfile, userresultfile)
                        result = promptContent + result
                        result_file.write(result)
                except IOError as error:
                    print('写入html文件错误：{0}'.format(error))
                finally:
                    return returnVal
            else:
                return returnVal


# 1表示绿色,2表示红色,3表示黄色
def outputPromptInfo(bCI, promptInfo, color):

    if bCI :
        if color == Color.green:
            print(green(promptInfo))
        elif color == Color.red:
            print(red(promptInfo))
        elif color == Color.yellow:
            print(yellow(promptInfo))
        else:
            print(promptInfo)
        sys.stdout.flush()
    else:
        if color == Color.green:
            print(Fore.GREEN + promptInfo, file = stream)
        elif color == Color.red:
            print(Fore.RED + promptInfo, file = stream)
        elif color == Color.yellow:
            print(Fore.YELLOW + promptInfo, file = stream)  
        else:
            print(promptInfo)     
        print(Fore.WHITE, file = stream)
    sys.stdout.flush()

if __name__ == "__main__":
    
    compResultFile = "result_comparation.html"
    if os.path.isfile(compResultFile):
        os.remove(compResultFile)

    promptInfo = "正在使用 makefile 文件生成项目"
    outputPromptInfo(bCI, promptInfo, Color.green)
    execResult = os.system("make")

    if execResult != 0:
        errorInfo = "生成项目失败"
        outputPromptInfo(bCI, errorInfo, Color.red)
        exit(1) 
    else:
        score = 40
        if not os.path.isfile("output1.txt") and not os.path.isfile("writefile1.txt") and not os.path.isfile("writefile1.dat"):
            score = 100
        if bCI:
            promptInfo = "生成项目成功"
            outputPromptInfo(bCI, promptInfo, Color.green)
            promptInfo = "exec-score {0}".format(score)
            print(promptInfo)
            print()
            sys.stdout.flush() 
        else:
            promptInfo = "生成项目成功, 分数 {0}".format(score)
            outputPromptInfo(bCI, promptInfo, Color.green)

    dsType, dsFileStr, resultFileType, userresultFileStr, resultFileStr = getProjInfo() 

    if resultFileType == ResultFileType.nooutput:
        exit(0) 

    # 获取case的数量
    caseCount = 1
    while 1:
        resultFile = resultFileStr.format(caseCount)
        if not os.path.isfile(resultFile):
            caseCount -= 1
            break
        caseCount += 1

    if caseCount > 0:
        if bCI :
            print(yellow("标准答案结果文件和用户输出结果文件的比较原则是：比较时忽略行尾空白字符和文件末尾的空白行。"))
        else:
            print(Fore.YELLOW + "标准答案结果文件和用户输出结果文件的比较原则是：比较时忽略行尾空白字符和文件末尾的空白行。", file = stream, end='')
            print(Fore.WHITE, file = stream)

    seqNum = 1
    while 1:
        dsFile = dsFileStr.format(seqNum)     
        resultFile = resultFileStr.format(seqNum)
        userresultFile = userresultFileStr.format(seqNum)
        if seqNum == 1 and not os.path.isfile(resultFile):
            promptInfo = "该项目未提供自动化验证功能"
            if bCI :
                print(red(promptInfo))
            else:
                print(Fore.RED + "该项目未提供自动化验证功能", file = stream, end='')
                print(Fore.WHITE, file = stream)
            break
        if not os.path.isfile(resultFile):
            break
            
        if dsType == DataSourceFileType.noinput:
            print("正在验证 case{0}".format(seqNum))
        else:
            print("正在使用数据源文件 {0} 验证 case{1}".format(dsFile, seqNum))

        runCmdStr =getRunProgCmdStr(dsType, resultFileType, dsFile, userresultFile)
        
        promptInfo = "正在执行命令: {0}".format(runCmdStr)
        outputPromptInfo(bCI, promptInfo, Color.white)

        if bCI == False:
            promptInfo = "提示：如果验证程序长时间未结束，说明应用程序中可能存在死循环。请停止验证程序(Ctrl+c)，修改应用程序后再验证。"
            outputPromptInfo(bCI, promptInfo, Color.yellow)

        execResult = os.system(runCmdStr)
        if execResult != 0:
            errorInfo = "应用程序执行异常，返回值：{0}。".format(execResult)
            outputPromptInfo(bCI, errorInfo, Color.red)
            exit(1)
 
        if os.path.isfile(resultFile) and os.path.isfile(userresultFile):
            if compare_file(resultFile, userresultFile, seqNum, caseCount, bCI, resultFileType, dsType) == 0:
                if bCI :
                    if dsType != DataSourceFileType.noinput:
                        print("使用的数据源文件是 {0}。".format(dsFile))
                    
                    print("标准答案结果文件 {0}（左边）与用户编写的应用程序结果文件 {1}（右边）的比较结果：".format(resultFile, userresultFile))
                    sys.stdout.flush()    
                    runCommand = "diff {0} {1} -b -B -y -i -W 100".format(resultFile, userresultFile)                
                    execResult = os.system(runCommand)
                    if execResult != 0:
                        print()
                        exit(1)
                else:
                    if resultFileType != ResultFileType.writedat:
                        if os.path.exists('/.dockerenv'):
                            promptInfo = "查看文件比较结果可帮助你查找验证失败的原因。方法是：\n选择 View 菜单中的 Explorer 打开文件列表，右键点击 result_comparation.html 文件，在弹出的菜单中选择 Open Preview"     
                        else:
                            promptInfo = "已经使用浏览器打开了结果比较文件result_comparation.html"
                            webbrowser.open("result_comparation.html")    
                    else:
                        promptInfo = "选择 View 菜单中的 Explorer 打开文件列表，右键点击 writefile*.dat文件或user_writefile*.dat文件，在弹出的菜单中选择 Open With...，再选择 Hex Editor "
                    outputPromptInfo(bCI, promptInfo, Color.red)
                exit(1)
        seqNum = seqNum + 1
