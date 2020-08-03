SET PATH=D:\Keil_v5\ARM\ARMCC\Bin;C:\Program Files (x86)\Common Files\Oracle\Java\javapath;C:\Windows\system32;C:\Windows;C:\Windows\System32\Wbem;C:\Windows\System32\WindowsPowerShell\v1.0\;C:\Windows\System32\OpenSSH\;D:\Program Files\Git\cmd;C:\Program Files\dotnet\;D:\Program Files\LLVM\bin;C:\Windows\system32\wbem;D:\Program Files (x86)\IVT Corporation\BlueSoleil\Mobile;C:\Users\MZG\AppData\Local\Microsoft\WindowsApps;D:\Program Files\Microsoft VS Code\bin;C:\Users\MZG\.dotnet\tools
SET CPU_TYPE=STM32F429IGTx
SET CPU_VENDOR=STMicroelectronics
SET UV2_TARGET=Target 1
SET CPU_CLOCK=0x00B71B00
"D:\Keil_v5\ARM\ARMCC\Bin\ArmAsm" --Via ".\objects\startup_stm32f429xx._ia"
"D:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\stm32f4xx_hal.__i"
"D:\Keil_v5\ARM\ARMCC\Bin\ArmLink" --Via ".\Objects\STM32F429.lnp"
