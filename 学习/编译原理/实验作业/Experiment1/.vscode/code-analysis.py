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

from comtool import *

if __name__ == "__main__":

    dsType, dsFileStr, resultFileType, userresultFileStr, resultFileStr = getProjInfo() 

    runProgCmdStr = getRunProgCmdStr(dsType, resultFileType, dsFileStr.format(1), userresultFileStr.format(1))

    runCommand = "make && valgrind --show-leak-kinds=all --xml=yes --xml-file=./build/valgrind-report.xml {0}".format(runProgCmdStr)      
    runCmd(runCommand)

 