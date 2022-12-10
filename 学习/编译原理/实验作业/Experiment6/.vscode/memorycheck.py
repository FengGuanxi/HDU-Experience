import sys
import argparse
import os
import filecmp
import subprocess
import platform

from comtool import *

if __name__ == "__main__":

    dsType, dsFileStr, resultFileType, userresultFileStr, resultFileStr = getProjInfo() 

    runProgCmdStr = getRunProgCmdStr(dsType, resultFileType, dsFileStr.format(1), userresultFileStr.format(1))
 
    runCommandStr = "valgrind --show-leak-kinds=all {0}".format(runProgCmdStr)
    if platform.system().lower() == 'windows':
        runCommandStr = "make && drmemory -ignore_kernel -leaks_only -brief -batch -- {0}".format(runProgCmdStr)
    
    runCmd(runCommandStr)


