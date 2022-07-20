**Linux系统及应用**

-   Linux系统管理

    -   系统启动与配置

        -   BIOS与MBR

            -   读取磁盘扇区 dd -i -o -b -count

        -   Grub2

            -   stage 1.0

            -   stage 1.5

            -   stage 2.0

    -   系统部署

        -   系统安装

            -   安装前准备

            -   安装方式

            -   安装过程

        -   系统更新

        -   桌面环境

        -   配置日期和时间

        -   配置网络

        -   控制台与终端

        -   虚拟机技术

            -   VMWare Workstation

            -   QEMU/KVM

            -   其他虚拟机

            -   云服务器与虚拟私有主机

            -   容器技术

        -   Linux帮助系统

            -   man

            -   info

            -   命令自带帮助

    -   系统资源状态监控

        -   内存

        -   CPU

        -   磁盘

        -   /proc

        -   vmstate

        -   top

        -   ps -ef aux

        -   kill -9 -15

    -   用户和组管理

        -   用户管理

        -   用户组管理

        -   权限管理

    -   存储管理

        -   文件系统

            -   ext2/3/4

            -   xfs

        -   根文件系统

        -   系统目录与FHS

        -   /proc虚拟文件系统

        -   磁盘空间管理\*

            -   fdisk

            -   gpart

        -   设备管理\*

        -   计划任务

            -   at命令

            -   cron

            -   anacron

        -   服务管理

        -   网络管理\*

            -   网络基础概述

            -   网络配置

    -   日志管理

-   程序开发基础

    -   程序编译概述

        -   GNU工具链

    -   gcc编译器

    -   gdb调试器

    -   make和makefile

        -   文件依赖关系

        -   默认规则

        -   宏变量

    -   版本控制

        -   git概述

        -   本地库初始化

            -   git init

            -   git clone

        -   add和commit

        -   git日志

        -   reset命令

        -   diff命令

        -   协作和托管中心

            -   创建远程库

            -   远程库操作

                -   fork

                -   pull request

        -   分支管理\*

        -   SSH免密登录\*

-   Shell编程

    -   Shell概述

    -   编辑器

        -   编辑器概述

        -   Vim基本用法

        -   Vim配置管理

        -   Vim插件管理

    -   变量

        -   数据类型

        -   普通变量

        -   环境变量

        -   位置变量

        -   特殊变量

    -   输入和输出

        -   输入命令read

        -   输出命令echo

    -   表达式

        -   算术表达式

        -   逻辑表达式

        -   命令

            -   expr

            -   bc

            -   test

            -   双小括号 (( ))

            -   中括号 [ ]

        -   复合

            -   \-a -o 表达式复合

            -   \&amp;&amp; \|\| 命令复合

    -   分支结构

        -   if分支结构

        -   case分支结构

            -   \$变量名

    -   循环结构

        -   for循环结构

            -   变量名

        -   whle循环结构

        -   until循环结构

        -   select循环结构

            -   break结束循环

    -   函数

        -   定义和调用

        -   点命令载入

        -   参数传递

-   Linux应用管理

    -   常见服务器应用概述

    -   FTP服务器

        -   工作模式

        -   端口

    -   WEB服务器

        -   apache

        -   nginx

        -   虚拟主机

    -   DNS服务器\*

    -   SSH服务器\*

-   Linux运维技术

    -   LANMP开发服务器

        -   Tomcat

        -   MySQL

        -   PHP

    -   基于容器的应用部署

        -   Docker

        -   iSulad

-   Linux操作系统概述

    -   操作系统概述

        -   操作系统定义

            -   硬件

            -   内核

            -   Shell

            -   管理软硬件资源

            -   构建高效虚拟机

        -   功能

            -   处理机/进程管理

            -   内存管理

            -   设备管理

            -   文件系统

    -   Unix操作系统

        -   Unix的历史

        -   Unix哲学

    -   Linux操作系统

        -   Linux发展历史

    -   自由软件

        -   运动产生的背景

        -   自由的定义

            -   自由度0：自由运行软件自由运行软件

            -   自由度1：自由学习和修改软件源代码

            -   自由度2：自由再发布软件拷贝

            -   自由度3：自由发布修改后的软件版本

        -   GNU项目

        -   GPL许可证

    -   开源软件

        -   软件开发方法

        -   OSI

        -   与自由软件的区别

        -   代码托管

            -   svn

            -   git

                -   github

                -   gitlab

                -   gitee

    -   国产操作系统

        -   UOS

        -   openEuler

        -   Harmony OS

        -   红旗Linux

        -   AliOS

        -   Deepin Linux

    -   Linux发展现状与展望

        -   Linux与人工智能

        -   Linux与大数据

        -   Linux与云计算

        -   Linux与物联网

    -   Linux内核

        -   版本号

            -   2.6.0之前

            -   2.6.0之后

            -   uname -r

            -   cat /proc/version

    -   Linux发行版

        -   常见发行版举例

        -   选择发行版

        -   查看发行版信息

            -   cat /ets/os-release

-   Linux常用命令

    -   启动、重启和关机

    -   文件和目录相关的命令

        -   cat、more、less、zless命令

            -   cat -n

            -   cat创建文件

        -   cd、pwd和ls命令

            -   ls -a

            -   ls -l

            -   ls -i

        -   cp、touch和mv命令

        -   mkdir、rmdir和rm命令

            -   mkdir -p

            -   rm -rf

        -   ln命令

            -   \-s 软链接

            -   硬链接

        -   file、type和stat命令

            -   时间戳

            -   文件类型

        -   find命令

            -   \-name

            -   \-size

            -   \-exec和-ok command {} \\;

    -   字符/文件处理

        -   grep命令

            -   正则表达式 . \^ \$ ...

            -   \-E 扩展正则表达式 \| \&amp;

        -   xargs命令

            -   \-I

        -   cut命令

            -   \-d -f

        -   wc和sor命令

            -   wc -l -c

            -   sort -u -r -n -s

        -   sed命令

            -   sed -i 's/\\"//g' 删除双引号

        -   awk命令

            -   awk '{print\$1}'

    -   文本比较命令

        -   diff

        -   comp

        -   comm

    -   压缩和打包

        -   gzip

        -   tar zxf/zcf

    -   校验工具

        -   md5sum

        -   sha1sum

    -   命令组合

        -   管道“\|”

        -   分号“;”

        -   与“&amp;&amp;”

        -   或“\|\|”

        -   重定向

            -   输出 \> \>\> \&amp;\> 2\>&amp;1

            -   输入 \< \<\<