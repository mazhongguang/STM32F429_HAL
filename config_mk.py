import os
import sys

f_src = open("src.mk","w")
f_path = open("cpppath.mk","w")

# 包含头文件的文件夹路径列表
ycm_flags = []
f_src.write('C_SOURCES = \\\n') 
f_path.write('C_INCLUDES = \\\n')
# 不使用的文件夹路径列表
# 前5个路径是需要手动处理的路径
remove_dir = ['.','lib/Src','lib/Src/Legacy','build', \
            'RT-Thread/3.1.4/components/cplusplus','RT-Thread/3.1.4/components/dfs', \
            'RT-Thread/3.1.4/libcpu/arm/common','lib/HAL_Drivers/config', \
            'lib/HAL_Drivers', \
            'RT-Thread/3.1.4/components/drivers','SYSTEM/usart', \
            'RT-Thread/3.1.4/components/libc','RT-Thread/3.1.4/components/net/at/at_socket', \
            'RT-Thread/3.1.4/components/net','RT-Thread/3.1.4/components/utilities', \
            'RT-Thread/3.1.4/components/vbus','RT-Thread/3.1.4/components/vmm' \
            ]

# 从第四个元素开始将所有包含的子目录路径加入到不使用的文件夹路径列表中
for m in remove_dir[4:] :
    for a,b,c in os.walk(m) :
        remove_dir.append(a)

# 遍历所有目录和子目录，并判断出包含头文件的文件夹路径和'.c'文件的全路径加文件名
for root,dirs,files in os.walk(os.getcwd()):
    root = os.path.relpath(root,os.getcwd())        # 从当前路径开始计算路径
    if not root in remove_dir :                     # 判断不在不使用文件夹路径中的文件，即正常使用中的文件位置
        for file in files :                         # 遍历所有文件
            if os.path.splitext(file)[1] == '.c' :  # 如果文件是'.c'文件，则将文件路径加文件名写入到src.mk文件中
                f_src.writelines(os.path.join(root, file) + ' \\\n')
            elif os.path.splitext(file)[1] == '.h' :#如果文件是头文件，则将所在路径加入到头文件所在文件夹列表中
                if not root in ycm_flags :          #头文件路径在ycm_flags列表中没有才加入ycm_flags，避免重复加入头文件夹列表
                    ycm_flags.append(root)

#根据需要增加 的C文件和 HAL库的 C文件
ycm_flags.append('lib/HAL_Drivers')
ycm_flags.append('lib/HAL_Drivers/l0')
ycm_flags.append('RT-Thread/3.1.4/components/drivers/include')
ycm_flags.append('RT-Thread/3.1.4/components/drivers/include/drivers')
ycm_flags.append('RT-Thread/3.1.4/components/drivers/include/ipc')
add_sources = '''\

C_SOURCES += 	 \\
		lib/Src/stm32l0xx_hal_rcc.c \\
		lib/Src/stm32l0xx_hal_rcc_ex.c \\
		lib/Src/stm32l0xx_hal_gpio.c \\
		lib/Src/stm32l0xx_hal_dma.c \\
		lib/Src/stm32l0xx_hal_pwr.c \\
		lib/Src/stm32l0xx_hal_pwr_ex.c \\
		lib/Src/stm32l0xx_hal_cortex.c \\
		lib/Src/stm32l0xx_hal.c \\
		lib/Src/stm32l0xx_hal_uart.c \\
		lib/Src/stm32l0xx_hal_uart_ex.c \\
		lib/Src/stm32l0xx_hal_tim.c \\
		lib/Src/stm32l0xx_hal_tim_ex.c \\
		lib/Src/stm32l0xx_hal_flash.c \\
		lib/Src/stm32l0xx_hal_flash_ex.c \\
		lib/Src/stm32l0xx_hal_adc.c \\
		lib/Src/stm32l0xx_hal_adc_ex.c \\
		lib/Src/stm32l0xx_hal_iwdg.c \\
		lib/Src/stm32l0xx_ll_gpio.c \\

C_SOURCES += \\
            lib/Hal_Drivers/drv_common.c \\
            lib/HAL_Drivers/drv_usart.c \\
            RT-Thread/3.1.4/components/drivers/serial/serial.c \\
            RT-Thread/3.1.4/components/drivers/src/completion.c \\
            RT-Thread/3.1.4/components/drivers/src/dataqueue.c \\

'''
f_src.write(add_sources)

#启动文件路径和其它汇编文件（线程切换文件）
startup_path = 'RT-Thread/3.1.4/libcpu/arm/cortex-m0/context_gcc.s \\\n'
asm_sources = 'core/gcc/startup_stm32l071xx.s'
f_src.write('\n\nASM_SOURCES =  \\\n' + startup_path + asm_sources)

#GCC编译器的路径（可以通过此选项进行指定版本GCC进行编译，一般用于系统中存在多个版本的GCC）
gcc_path = 'GCC_PATH = /opt/gcc-arm-none-eabi-6-2017-q2-update/bin'
f_src.write('\n\n\n' + gcc_path + '\n\n\n')

#C定义
c_define = '''

# C defines
C_DEFS =  \\
-DUSE_HAL_DRIVER  \\
-DUSE_FULL_LL_DRIVER \\
-DSTM32L071xx \\
-DRT_USING_NEWLIB \\
# -DRT_USING_MINILIBC  \\


# AS incluedes
AS_INCLUDES =


#以下是YCM配置文件需要复制的FLAG内容
'''
for a in ycm_flags :
    f_path.write('-I' + a + ' \\\n')
f_path.write(c_define)
for a in ycm_flags :
    f_path.write("#'-I',\n#'" + a + "',\n")

f_src.close()
f_path.close()

#在当前目录下生成下载脚本
shell_file = '''\
#!/bin/bash 
JLinkExe -CommandFile jlink.conf
'''
with open('download.sh','w') as sh :
    sh.writelines(shell_file)

#JLink下载配置文件内容前半部分    
jlink_conf_cache = '''\
si SWD 
Device STM32L071K8
Speed 4000
loadbin build/\
'''
#JLink下载配置文件后半部分
jlink_conf_cache1 = '''\
.bin,0x8000000
r
q

'''

#读取Makefile文件
with open('Makefile','r') as make_file :
    makefile_cache = make_file.readlines()

#提取读出的Makefile文件中的TARGET以拼接成需要烧录的文件全名
with open('jlink.conf','w') as jlink_conf :
    jlink_conf.writelines(jlink_conf_cache +  makefile_cache[16].strip() + jlink_conf_cache1 )


