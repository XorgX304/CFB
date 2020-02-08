# Canadian Furious Beaver


<img src="/Assets/img/logo/Logo_v1.svg" width="50%"/>



## Idea

Furious Beaver is a distributed tool for capturing IRPs sent to any Windows driver. It operates in 2 parts:

1. the "Broker" combines both a user-land agent and a self-extractable driver (`IrpDumper.sys`) that will install itself on the targeted system. 
  Once running it will expose (depending on the compilation options) a remote named pipe (reachable from 
  `\\target.ip.address\pipe\cfb`), or a TCP port listening on TCP/1337. The communication protocol was made to be simple by design 
  (i.e. not secure) allowing any [3rd party tool](https://github.com/hugsy/cfb-cli) to dump the driver IRPs from the same Broker 
  easily (via simple JSON messages).

2. the GUI is a Windows 10 UWP app made in a `ProcMon`-style: it will connect to wherever the broker is, and provide a convienent 
 GUI for manipulating the broker (driver enumeration, hooking and IRP capturing). It also offers facililties for forging/replaying 
 IRPs, auto-fuzzing (i.e. apply specific fuzzing policies on *each* IRP captured), or extract IRP in various formats (raw, as a 
 Python script, as a PowerShell script) for further analysis. The captured data can be saved on disk in an easily parsable format 
 (`*.cfb` = SQLite) for further analysis, and/or reload afterwards in the GUI.

Although the GUI obviously requires a Windows 10 environment (UWP App), the Broker itself can be deployed on any Windows 7+ host (x86 or x64). 
The target host must have `testsigning` BCD policy enabled, as the self-extracting driver is not WHQL friendly.


## Screenshots

### Intercepted IRP view

![Intercepted IRP view](https://i.imgur.com/xMOIIhC.png)

### IRP details

![IRP Metadata](https://i.imgur.com/zmh2QAw.png)
![IRP InputBuffer](https://i.imgur.com/j0W9ljL.png)


### IRP replay

![IRP Replay](https://i.imgur.com/9Ybq27G.png)


## Concept

`IrpDumper.sys` is the driver part of the CFB Broker that will auto-extract and install when launched. The driver will be responsible for hooking the IRP Major Function table of
the driver that is requested to be hooked, via an IOCTL passed from the Broker.
Upon success, the IRP table of the driver will then be pointing to `IrpDumper.sys` interception routine, as we can easily see with a debugger or tools like [`WinObjEx64`](https://github.com/hfiref0x/WinObjEx64).

![img](https://i.imgur.com/dYqHE6q.png)

`IrpDumper.sys` in itself then acts a rootkit, proxy-ing all calls to the targeted driver(s). When a `DeviceIoControl` is sent to a hooked driver, `IrpDumper` will simply capture the data if any, and push a message to the user-land agent (`Broker`), and yield the execution back to the legitimate drivers, allowing the intended code to continue as expected.
The `Broker` stores all this data in user-land waiting for a event to ask for them.


## Build

### GUI

Clone the repository, and build the `Broker` in the solution `CFB.sln` at the project root with Visual Studio (Debug - very verbose - or Release). Additionally, you can build the App GUI by building the `GUI (Universal Windows)` project.



### Command line

Clone the repository and in a VS prompt run

```
C:\cfb\> msbuild CFB.sln /p:Configuration=$Conf
```

Where `$Conf` can be set to `Release` to `Debug`.


## Setup

A Windows 7+ VM ([Windows 10 SDK VM](https://developer.microsoft.com/en-us/windows/downloads/virtual-machines) is recommended)

On this VM:
 - Enable kernel debug
 - Enable test signing

Install VS 2015/2017/2019 redist x86 or x64 depending on your VM architecture.

Follow the indications in the `Docs/` folder to improve your setup.


## Command-line client

Several command line tools (such as dumping all data to SQLite database, fuzzing IRP, etc.) can be found in the external repository
[CFB-cli](https://github.com/hugsy/CFB-cli).


## Why the name?

Because I had no idea for the name of this tool, so it was graciously generated by [a script of mine](https://github.com/hugsy/stuff/tree/master/random-word).


## Status
Project|Build Status
---|---
App|[![Build Status](https://dev.azure.com/blahcat/CFB/_apis/build/status/hugsy.CFB?branchName=master)](https://dev.azure.com/blahcat/CFB/_build/latest?definitionId=1&branchName=master)
Broker|[![Build Status](https://dev.azure.com/blahcat/CFB/_apis/build/status/hugsy.CFB?branchName=master)](https://dev.azure.com/blahcat/CFB/_build/latest?definitionId=1&branchName=master)
