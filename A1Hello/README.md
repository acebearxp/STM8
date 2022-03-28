# A1Hello
利用最简单的CPU循环来闪烁LED灯
测试编译及烧录环境是否一切正常

## Hardware
+ 一块STM8的开发板
+ ST-LINK 下图中任意一款皆可

![st-link](st-link.jpg)

记得安装USB驱动,以及把固件升级到最新版本
[STSW-LINK007](https://www.st.com/en/development-tools/stsw-link007.html)

## Software
+ [STVD-STM8](https://www.st.com/en/development-tools/stvd-stm8.html) 官方的开发环境
+ [STVP-STM8](https://www.st.com/en/development-tools/stvp-stm8.html) 官方的烧录程序
+ [STM8S/A Standard peripheral library](https://www.st.com/en/embedded-software/stsw-stm8069.html) 官方提供的标准外设库
+ [Cosmic STM8 Compiler](https://www.cosmicsoftware.com/stm8.php) Cosmic提供的STM8编译器([特别免费许可](https://www.cosmicsoftware.com/download_stm8_free.php))

## Program
 + 设置使用LSI(内部低频时钟128kHz),预分频设置为128,使CPU运行在1kHz频率上
 + 设置GPIO-E5为慢速推挽输出,E5连接到了开发板上的一个内置LED上
 + 在一个循环中每隔一定时间改变GPIO-E5的电平

## Build
可以用STVD打开项目直接编译,或者使用makefile编译(microsoft nmake)
+ main.c里使用了时钟和GPIO
+ 因此需要把STM8S_StdPeriph_Driver里的 stm8s_clk.c & stm8s_gpio.c 加入编译
+ 使用编译器参数指定芯片类型, 如`-dSTM8S105`
+ stm8s_conf.h由STM8S_StdPeriph_Driver的示例工程里提供,通过宏定义关闭芯片不支持的功能
+ stm8_interrupt_vector.c 虽然没有使用中断向量,但必须提供中断向量的定义.使用`__VSCODE__`以便在vscode中屏蔽不支持的语法
+ *.lkf 是STM8链接器描述文件,这个文件是用STVD生成的

## Flash
使用STVP打开生成的s19文件,烧录至flash中

烧录完成后,程序不会自动启用,需要通过ST-LINK发送reset信号重启程序