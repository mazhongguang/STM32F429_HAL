C_INCLUDES = \
-ICORE \
-Idsp_lib/Include \
-IFATFS/exfuns \
-IFATFS/source \
-IHALLIB/Inc \
-IHALLIB/Inc/Legacy \
-IHARDWARE/IWDG \
-IHARDWARE/LCD \
-IHARDWARE/LED \
-IHARDWARE/REMOTE \
-IHARDWARE/sdio \
-IHARDWARE/TIM \
-IHARDWARE/WKUP \
-ISYSTEM/delay \
-ISYSTEM/sys \
-IUSER \
-IUSMART \
-Ilib/HAL_Drivers \
-Ilib/HAL_Drivers/l0 \
-IRT-Thread/3.1.4/components/drivers/include \
-IRT-Thread/3.1.4/components/drivers/include/drivers \
-IRT-Thread/3.1.4/components/drivers/include/ipc \


# C defines
C_DEFS =  \
-DUSE_HAL_DRIVER  \
-DUSE_FULL_LL_DRIVER \
-DSTM32L071xx \
-DRT_USING_NEWLIB \
# -DRT_USING_MINILIBC  \


# AS incluedes
AS_INCLUDES =


#以下是YCM配置文件需要复制的FLAG内容
#'-I',
#'CORE',
#'-I',
#'dsp_lib/Include',
#'-I',
#'FATFS/exfuns',
#'-I',
#'FATFS/source',
#'-I',
#'HALLIB/Inc',
#'-I',
#'HALLIB/Inc/Legacy',
#'-I',
#'HARDWARE/IWDG',
#'-I',
#'HARDWARE/LCD',
#'-I',
#'HARDWARE/LED',
#'-I',
#'HARDWARE/REMOTE',
#'-I',
#'HARDWARE/sdio',
#'-I',
#'HARDWARE/TIM',
#'-I',
#'HARDWARE/WKUP',
#'-I',
#'SYSTEM/delay',
#'-I',
#'SYSTEM/sys',
#'-I',
#'USER',
#'-I',
#'USMART',
#'-I',
#'lib/HAL_Drivers',
#'-I',
#'lib/HAL_Drivers/l0',
#'-I',
#'RT-Thread/3.1.4/components/drivers/include',
#'-I',
#'RT-Thread/3.1.4/components/drivers/include/drivers',
#'-I',
#'RT-Thread/3.1.4/components/drivers/include/ipc',
