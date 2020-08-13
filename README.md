# cics-exci-64bit-connector
This sample library for z/OS demonstrates how 64-bit LE applications on z/OS can use the external CICS interface (EXCI) to interact with CICS. The library is compiled and used in Unix System Services (USS) on z/OS.

To use this sample, you need to: 
1.	Set an environment variable called MY_CICS_HLQ
1. *Optionally*, customize the test 64-bit C application testProgram.c
1.	Run the build.sh script  
1.	Configure EXCI in your CICS region
1.	Set up USS for EXCI
1. *Optionally*, test the connector and EXCI set up using the sample test application testProgram.c
1.	Use the library

Instructions for all these steps are provided in this README.

## Description of architecture

* __src/entry.c__ (64-bit LE C) - this includes all the entry points exposed by the library. Within each entry point the data passed in is copied into 31-bit storage and packaged up (in C structs defined in __inc/entry.h__) to be passed to the 31-bit module (__src/exci.c__, described below), which is then used in calling CICS over EXCI.
* __src/glue.s__ (Assembly) - this Assembly module switches AMODE from 64-bit to 31-bit. AMODE(31) is required to use EXCI.  This module also switches from XPLINK to non-XPLINK linkage conventions. The data passed from __src/entry.c__ is then passed to the __src/exci.c__ module.
* __src/exci.c__ (31-bit Metal C) - this module takes the data passed from __src/glue.s__ and uses this to call into CICS over EXCI. The data is unpacked and used in the EXEC CICS API calls. Any data passed back from CICS is then put back in the C structs and passed back up the stack.

## EXCI API functions available
The sample connector has the following EXCI API functions:
* EXEC CICS LINK PROGRAM() CHANNEL()
* EXEC CICS LINK PROGRAM() COMMAREA() - *untested*
* EXEC CICS PUT CONTAINER - *currently limited to strings*
* EXEC CICS GET CONTAINER - *currently limited to strings. The length of the CONTAINER is currently limited to 2048. This can be changed in the __inc/entry.h__ and __src/exci.c__ files*

See the **Using the sample** section below for the entry points to each of these functions.

## Supporting files
* __Makefile__ - make file to compile the connector source code, package into a static archive, and link-edit as a DLL
* __build.sh__ - shell script to invoke the Makefile. The script also exports environment variables (for example, the STEPLIB) that are required for compilation and link-editing, and retrieves header files from CICS data sets.
* __testProgram.c__ - this is a sample 64-bit C program you can use to test the connector has built, and all setup has been completed successfully. See __3. Customize testProgram.c__ below to customize this file.
* __Cobol/EDUCHAN.cbl__ - used by __testProgram.c__ to verify the connector and EXCI has been set up correctly.
* __jcl/cobcomp.jcl__ - this is sample JCL that you can use to compile the EDUCHAN program, which is called by __testProgram.c__. Read the comments at the top of the file to customize as required.

## Pre-requisites
* A version of IBM CICS Transaction Server for z/OS (CICS) to use to build this sample: 
  * CICS TS V5.4 allows you to build this sample without any modifications to the code. However, you can edit the sample code to remove the channels and containers EXCI functions, which depend on CICS TS V5.4, and build against an earlier release.
  * References to CICS TS in this readme assume CICS TS V5.4 unless otherwise stated.
* Make v4.0 or higher - [available as an open source ported tool from Rocket Software](http://www.rocketsoftware.com/zos-open-source/tools)
* IBM z/OS with C and Assembler compiler
* Access to Unix System Services (USS) on z/OS

## Configuring the sample
### 1. Set up the environment

Before you can run the build.sh script, set the MY_CICS_HLQ environment variable. This is the high level qualifier for your CICS V5.4 installation. You can set this like this:

```bash
$ export MY_CICS_HLQ=CICSTS54.CICS
```

In the example above we set the CICS HLQ to `CICSTS54.CICS`, where the SDFHEXCI location would be `CICSTS54.CICS.SDFHEXCI`.

### 2.	Check permissions to run the build.sh script
To run the build.sh script, ensure that the user ID running the script has permission to execute the file. If you have cloned this repository from Git directly onto z/OS these should already be set. If not, run a command such as:

```shell
$ chmod 750 build.sh
```

You should now have all required permissions to execute the script. 

### 3. Customize testProgram.c
If you would like to use the test application __testProgram.c__ you need to customize before you run the build, as this application is built along with the connector.

To do this, open __testProgram.c__ and change the APPLID of the CICS region you will connect to:

```C
char applid[8] = "IYMYCICS";
```

### 4. Running a build
To run the script:

```bash
$ ./build.sh
```

Example output from this will look like this:

```bash
Running 64-bit EXCI connector sample build script

CICS HLQ      = [CICSTS54.CICS]
_CXX_L6SYSLIB = []
STEPLIB       = []

Setting _CXX_L6SYSLIB and STEPLIB to required values

_CXX_L6SYSLIB = [CICSTS54.CICS.SDFHEXCI:CEE.SCEEBND2:CBC.SCCNOBJ:SYS1.CSSLIB]
STEPLIB       = [CICSTS54.CICS.SDFHLOAD]

Getting EXCI header file DFHXCPLH
CICSTS54.CICS.SDFHC370(DFHXCPLH) -> inc/dfhxcplh.h: text

Running MakeFile

-----------------------------------------------------------------------

rm -f exci.s *.o
/bin/xlc -c -Wc,LP64 -qexportall -I./inc -o entry.o src/entry.c
/bin/as -mgoff -I./maclib -o glue.o src/glue.s
/bin/cxx -S -Wc,CICS,RENT,NOARGPARSE,GOFF,LONGNAME,NOSEARCH,METAL -I /usr/include/metal/ -I ./inc/ -o exci.s src/exci.c
/bin/as -mgoff -I./maclib -o exci.o exci.s
/bin/cxx -Wl,LP64,DLL -o cicsExci64Connector.so entry.o glue.o exci.o
ar -rc libCicsExci64Connector.a entry.o glue.o exci.o
/bin/xlc -Wc,LP64 -Wl,LP64 -I./inc -o test.o testProgram.c cicsExci64Connector.x

-----------------------------------------------------------------------

Returning _CXX_L6SYSLIB and STEPLIB to original values

_CXX_L6SYSLIB = []
STEPLIB       = []
```

This compiles all the required source, builds the static library archive, and creates the dynamically linked library (DLL).

## Using the sample

### Entry points

The entry points for the library can be found in the **inc/entry.h** file. The entry points are as follows:

```C
int execCicsLinkCommarea(char * applid, char * program, void * commarea, int iCommareaLen);

int execCicsLinkChannel(char * applid, char * program, char * channel);

int execCicsPutContainerString(char * container, char * channel, char * data, int iDataLength);

char * execCicsGetContainerString(char * container, char * channel, int ccsid);
```

### Configuring EXCI to work with the sample

*It is advised that you consult with your CICS Systems Programmer to determine how to setup EXCI in your CICS region, and verify that it is working correctly before you use this sample.*

#### EXCI setup instructions

You will need to configure the External CICS Interface (EXCI) in your CICS region. Instructions for how to do this can be found here: [Setting up EXCI for static routing](https://www.ibm.com/support/knowledgecenter/en/SSGMCP_5.4.0/configuring/interfaces/dfhtm_staticrouting.html). These instructions include installing the required CONNECTION and SESSION resources for EXCI.

Additionally, as with any EXCI application, you may need to create a custom EXCI options table. You can use this to, for example, override the CICS SVC if required. For more information on the using the DFHXCOPT EXCI options table see the CICS documentation in IBM Knowledge Center: [Using the EXCI options table, DFHXCOPT](https://www.ibm.com/support/knowledgecenter/en/SSGMCP_5.4.0/configuring/interfaces/dfhtmf0.html)

#### USS EXCI setup instructions

You will need the EXCI load library in the STEPLIB in USS. You can set this by executing the following command:

```bash
$ export STEPLIB=CICSTS54.CICS.SDFHEXCI:$STEPLIB
```

__Note:__ Substitute the `CICSTS54.CICS` high-level qualifier to match the location of the `SDFHEXCI` load library.

If you require a DFHXCOPT options table, once this has been built, you will need to include the data set containing the DFHXCOPT load module in the USS STEPLIB, in addition to the SDFHEXCI library. You can do this by executing the following command:

```bash
$ export STEPLIB=USERID.LOADLIB:$STEPLIB
```

Where *USERID.LOADLIB* is the data set containing the built DFHXCOPT member.

### Running the test application
To test that the connector has built successfully, and EXCI has been set up correctly, you can use the 64-bit C test application __testProgram.c__. To do this you have to:
1. Compile COBOL application and install
1. Run the test.o executable

#### Compile COBOL application and install

You will also need to compile and install the EDUCHAN COBOL program. You can use the sample JCL descibed above (be sure to customize as described in the file), or use your own custom COBOL compilation JCL.

Once compiled, you will need to define and install a CICS program resource definition. *Consult your local CICS Systems Programmer or similar CICS administrator for how to define and install program resources in your CICS region*.

#### Run test.o
The build process will have created the executable __test.o__. To verify the set up, run this application:

```bash
$ ./test.o
About to call execCicsPutContainerString()
Completed call to execCicsPutContainerString() rc=[0]
putContainerString rc=[0]
About to call execCicsLinkChannel()
Completed call to execCicsLinkChannel() rc=[0]
programLinkChannel rc=[0]
About to call execCicsGetContainerString()
Completed call to execCicsGetContainerString() data=[                                              C ICXE morf tset a si sihT]
getContainerString data=[                                              C ICXE morf tset a si sihT]
```

If you see the output above the connector, and EXCI are working correctly.

### Using the library
You can use the library as either statically linked or as dynamically linked (DLL).

#### Statically linked library
To use the static library compile the .a archive when you link-edit your application. Your application can include the **inc/entry.h** file.

When you link-edit an application that uses the statically linked library, set the following environment variable:

```bash
$ export LDFLAGS=\"-l//\'CICSTS54.CICS.SDFHEXCI\'\"
```

__Note:__ Substitute the `CICSTS54.CICS` high-level qualifier to match the location of the `SDFHEXCI` load library.

Alternatively, you can include when running the link edit command at command line.

#### Dynamically linked library (DLL)
To use the DLL, you can either:
* Open the DLL from your C code. An example of this can be found here [dlopen() — Gain access to a dynamic link library](https://www.ibm.com/support/knowledgecenter/en/SSLTBW_2.2.0/com.ibm.zos.v2r2.bpxbd00/dlopen.htm), or
* Compile your application with the sidedeck produced during compiled (.x file)

## License
This project is licensed under [Apache License Version 2.0](LICENSE).
