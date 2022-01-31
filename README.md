# THIS PROJECT HAS BEEN REWRITTEN AND REBRANDED AS [RWLAZER64](https://github.com/x0reaxeax/rwlazer64) 

# RWLAZER x64 ALPHA

### DISCLAIMER
##### This is an alpha pre-release, meaning bugs are more than likely to be present.  
##### Since this tool can easily eff-up your computer, it is highly recommended to use it in a virtual environment, such as [Hyper-V](https://docs.microsoft.com/en-us/virtualization/hyper-v-on-windows/quick-start/enable-hyper-v) 

<p align="center">
  <img src="https://i.imgur.com/4PHf0pG.png" />
</p>

**Pre-release alpha binaries can be downloaded [HERE](https://github.com/x0reaxeax/rwlazer-alpha/releases)**

# General FAQ and instuctions

## What the heck is this?
RWLAZER is currently a memory editing tool, similar to Cheat Engine. It's R/W operations are done in privileged (ring0) mode via an EFI Driver. 

## What can RWLAZER do?
* Read/Write operations from privileged mode (ring0)
* Signature scans with wildcard support from privileged mode (ring0)
* Memory scans from privileged mode and user-space (ring0 & ring3)
* Read/Write Model-Specific Registers (MSRs)
* Convert values between hexadecimal/decimal/float
* Screw up your computer

## What are some planned features?
* x64 debugger [WIP]
* C API
* Configurable installer
* Trainer Engine (picture Cheat Engine Tables)
* x64 Assembler & Disassembler
* Scripting interface
* Loop operations
* Database of used R/W memory addresses and values
* More More More

## What do I need in order to run this tool ?
* UEFI compatible system running Windows 10 x64
* The tool has been tested on following Windows 10 versions:
```
version 1909
version 2004
version 20H2
```   
All of these are marked as COMPATIBLE

## Why the heck would you create this crappy tool, when you can just use Cheat Engine?
* Well, I couldn't get Cheat Engine installer to launch on my VM, to read obfuscated communication data from a car disagnostic tool at work. So.. yea.. I was also working on a [Call of Duty: Black Ops Cold War kernel cheat](https://github.com/x0reaxeax/c0wadlina), so this is a combined result.
* I only recently discovered that [RWEverything](http://rweverything.com) is a thing! ;_;

## How can I install this?
#### Prerequisites
* Make sure Secure Boot is disabled, or manually add `EFI\Boot\bootx64.efi` and `EFI\rwlazer\rwlazer.efi` to trusted entries.
* If you're using Hyper-V, make sure your **Generation2** machine's first entry in the boot order is your HDD with EFI System Partition, **NOT** `bootmgfw.efi` ![boot_order](https://i.imgur.com/JFclbtq.jpg)
#### Installing
* Download the [alpha pre-release](https://github.com/x0reaxeax/rwlazer-alpha/releases)
* Extract the archive to a folder
* Run `install_lazer64.exe` as administrator
* Mind the warning ![warning](https://i.imgur.com/smyk6OU.jpg)
* If the installation completed successfully, restart your computer ![install_success](https://i.imgur.com/EFCf2Mq.jpg)
* You should see RWLAZER logo before Windows 10 boots ![rwlazer_logo](https://i.imgur.com/NzaI7yA.jpg), if you don't, something went wrong
* Run `rwlazer.exe` as administrator ![rwlazer_premenu](https://i.imgur.com/xY4M888.jpg)
* Attach to a PID and go nuts ![rwlazer_menu](https://i.imgur.com/TXZ6AXZ.jpg)

## How can I Read/Write MSRs?
* These are unsafe operations (yes, I see the irony), which are hidden by default. To enable them, type `1337` in the main menu prompt and press ENTER.

## Where's the source, are you backdooring me??
* RWLAZER is malware free and the source code will be released as soon as I clean it up a little bit and move on to beta releases.
* In fact, I do NOT encourage you to use this program on any bare-metal. Please use a virtual machine.

## This program is shit!
* I know..

## [CREDITS] Is this all your work ?
Heeeeeeell NO! Special BIG thanks goes to:
* [Jamesits](https://github.com/Jamesits) for their [BGRT Injector](https://github.com/Jamesits/BGRTInjector) - this is responsible for chain-loading the `rwlazer.efi` driver and the cool RWLAZER boot logo!
* [TheCruZ](https://github.com/TheCruZ/EFI_Driver_Access) for their [CRZEFI](https://github.com/TheCruZ/EFI_Driver_Access) EFI loader, which is really the base foundation of this tool
* The mentioned ring3 memory scanner was taken from [HERE](https://progamercity.net/c-code/904-memory-scanner-console-c.html). I fixed and revamped it a lot, but the original credit goes to the forum member TEDSON.

## Can I use this to hack my anti-cheat secured games ?
* You can, but expect to be banned if the anti-cheat is at least semi-decent and/or runs with a driver. As I said, I used this to hack CoD: BOCW without issues and detection, but their anti-cheat has been updated long since doing so, and I'd expect an instant ban now. Do **NOT** rely on it. This is **NOT** a game hacking tool (~~anymore~~).

## What are all the files bundled in `rwlazer64.rar` ?
```
Files bundled in 'rwlazer64.rar':
 'install_lazer64.exe'     - RWLAZER Installer - Mounts EFI System Partition and copies RWLAZER files to it
 'rwlazer.exe'             - RWLAZERRRRRR
 'EFI\Boot\bootx64.efi'    - BGRTInjector - It takes care of all the things necessary to load the Windows 10 Boot Manager 'bootmgfw.efi', but instead passes the control over to 'rwlazer.efi'
 'EFI\rwlazer\rwlazer.efi' - This is the heart of RWLAZER. Better said, it's the EFI Driver responsible for all ring0 R/W, and other privileged instructions

Files present in the repo, which are not bundled in 'rwlazer64.rar':
 'DebugDummy.exe'           - A debugging dummy application to test RWLAZER on
 'src\installer_main.c'     - Source code of 'install_lazer64.exe'
```

# Feature Preview
```
[1]  - Change PID                                - Change current working PID
[2]  - Get Base Address                          - Get Process Base Address. Doing R/W operations on System 'processes' (like 'System' PID 4) will result in a BSOD. You can usually predict this, when this fuction fails to retrieve the base address
-----------------------------------------------
[3]  - Read Memory                               - Read `n` bytes at specified memory address
[4]  - Write Memory                              - Write `n` bytes at specified memory address
[5]  - Read String                               - Read NULL terminated string at specified memory address
[6]  - Read Addr+Offset                          - Read address at a specified memory address (pointer)
-----------------------------------------------
[8]  - MemScan                                   - Ring3 Memory Scanner
[9]  - SigScan                                   - Ring0 Signature Scanner (can be used as a ring0 memory scanner)
-----------------------------------------------
[30] - Re-read last address                      - Re-read value from last-read memory address
[40] - Re-write last address                     - Write to last used memory address
[41] - Re-write last value [DISABLED]            - Currently disabled
[42] - Zero Memory                               - Quick ZeroMemory()
-----------------------------------------------
[50] - Float2Hex Calculator                      - Float to Hex calculator (Little Endian)
[51] - Base Calculator                           - Hexadecimal <-> Decimal converter + Endianness calculator
[52] - Print DataType Size Chart                 - Displays size info of default C data types and their MIN/MAX values
-----------------------------------------------
[63] - Read MSR   [UNSAFE]                       - Read Model-Specific Register - rdmsr
[64] - Write MSR  [UNSAFE]                       - Write to Model-Specific Register - wrmsr
-----------------------------------------------
[70] - Enable Automatic Endian Conversion        - DEPRECATED - Automatically converts Big Endian to Little Endian. Smart ByteData system takes care of this
[71] - Disable Smart ByteData Input              - It is highly recommended to leave this enabled at all times. The Smart Input feature accepts user input in decimal and hexadecimal, while detecting machine endianness and automatically converting byte data according to the detected endianness 
-----------------------------------------------
[80] - Clear Last Address Info                   - When doing R/W operations, RWLAZER saves the respective memory addresses and displays them in main menu. Use this to clear them 
[81] - Clear Console                             - Clear console output
[82] - Fresh Start                               - Clear all saved addresses/values and current working PID
-----------------------------------------------
[99] - Exit

[1337]                                           - Toggle UNSAFE mode (rdmsr/wrmsr)
```

## Couple of things worth mentioning
* Smart Input accepts hex/dec input. In order to pass hex data to RWLAZER, use the `0x` prefix (`0xdeadbeef`). Without the prefix, RWLAZER will process the data as decimal values
* The ring3 memory scanner, despite being revamped, still holds some iffy/annoying quirks. Fixes incoming.

## Bugs
* If you find a bug, please create a new issue, with detailed information about the bug/problem. It'd help to include your Windows version from `winver` command. Thank you!
