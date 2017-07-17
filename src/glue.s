*ASM     XOPTS(NOPROLOG NOEPILOG)
         SYSSTATE AMODE64=YES,ARCHLVL=2
*---------------------------------------------------------------------*
* Licensed Materials - Property of IBM                                *
*                                                                     *
* SAMPLE                                                              *
*                                                                     *
* (c) Copyright IBM Corp. 2017 All Rights Reserved                    *       
*                                                                     *
* US Government Users Restricted Rights - Use, duplication or         *
* disclosure restricted by GSA ADP Schedule Contract with IBM Corp    *
*---------------------------------------------------------------------*
SWFTGLUE TITLE 'Glue code to link from Swift to CICS'
         EJECT ,
*---------------------------------------------------------------------*
* Establish an LE environment with correct addressability             *
*---------------------------------------------------------------------*
SWFTGLUE CELQPRLG DSASIZE=DSASIZ,      Use calculated size of DSA      *
               BASEREG=NONE,           Use relative addressing         *
               PARMWRDS=3,             This routine needs 3 parms      *
               PARMREG=11,             Location of callers parms       *
               ENTNAME=swiftGlue,      Special entry point name        *
               PSECT=SWFTGLU1          PSECT name to use
swiftGlue ALIAS C'swiftGlue'
         USING AUTOSTG,R4              Our DSA
         USING MYPARMS,R11             Parameters supplied to us
         EJECT ,
*---------------------------------------------------------------------*
* Keep a copy of our environment across function calls                *
*---------------------------------------------------------------------*
SAVEENV  DS    0H
         LGR   R10,R5
         USING SWFTGLU1,R10
         EJECT ,
*---------------------------------------------------------------------*
* Check the supplied parameters for pre-allocated DSA storage         *
*---------------------------------------------------------------------*
PARMCHEK DS    0H
         LTGR  R12,R1                  First parm is pre-alloc DSA
         JNZ   SKIPMALL                Skip malloc if already provided
         EJECT ,
*---------------------------------------------------------------------*
* Grab some storage to build a 31-bit stack                           *
* Ultiimately, it would be good to use storage acquired here as the   *
* stack storage in swiftExci.c. Hence, the addition of the extra 1MB. *
*---------------------------------------------------------------------*
GETSTOR  DS    0H
         LARL  R1,=F'1048576'          Size of DSA for others
         L     R1,0(R1)
         AGHI  R1,STOR31L              Size of data to obtain for us
         STG   R1,PARM1                Parm for call to malloc31
*
         LMG   R5,R6,MALL31FD          Setup function descriptor
         BASR  R7,R6                   Call function
         NOPR  0
*
CEECWSA  LOCTR
C_WSA64  CATTR DEFLOAD,RMODE(64),PART(SWFTGLU1)
MALLOC31 ALIAS C'__malloc31'
MALLOC31 AMODE 64
MALLOC31 XATTR LINKAGE(XPLINK),SCOPE(IMPORT),REF(CODE)
MALL31FD DC    RD(MALLOC31)
         DC    VD(MALLOC31)
SWFTGLUE LOCTR
*
         DS    0H
         LGR   R5,10                   Restore our environment
         LTR   R3,R3                   Was call successful?
         JZ    NOMALL31                Terminate if no storage
*
         LGR   R12,R3                  Return from malloc into R12
         EJECT ,
*---------------------------------------------------------------------*
* Build a fully-working stack                                         *
*---------------------------------------------------------------------*
SKIPMALL DS    0H
         USING STOR31,R12              Address dedicated 31-bit stg
*
         LA    R13,SAVEAREA            Point R13 at the savearea ...
         XC    SAVEAREA,SAVEAREA       ... and clear
*
         LA    R1,C_PLIST              Point R1 at the PLIST
*
         LG    R2,FUNCID               Parm 1 is function ID
         ST    R2,C_FUNCID
         LG    R2,DATAPTR              Parm 2 is ptr to data
         ST    R2,C_DATAP
*
         XC    EXCIRC,EXCIRC           Clear RC placeholder
         EJECT ,
*---------------------------------------------------------------------*
* Call our EXCI program in the correct AMODE                          *
* Need to save the high halves of R2 through R14 because the CICS     *
* EXCI interface doesn't seem to preserve the top of 64-bit regs for  *
* the caller.                                                         *
*---------------------------------------------------------------------*
CALLPROG DS    0H
         LARL  R15,=V(CALLCICS)        Entry point to swiftExci routine
         L     R15,0(15)
         SAM31
         STMH  R2,R14,SAVEHIGH         Save the high halves
         BALR  R14,R15                 Off we go...
         LMH   R2,R14,SAVEHIGH         Restore the high halves
         SAM64
         ST    R15,EXCIRC              Save the return code
         EJECT ,
*---------------------------------------------------------------------*
* Release our 31-bit storage                                          *
*---------------------------------------------------------------------*
RELEASE  DS    0H
         LTG   R3,DSAPTR               Were we supplied with storage?
         JNZ   SKIPFREE                Don't release if not ours
*
         STG   R12,PARM1               Pointer to data to free
*
         LMG   R5,R6,FREE_FD           Setup function descriptor
         BASR  R7,R6                   Call function
         NOPR  0
*
CEECWSA  LOCTR
C_WSA64  CATTR DEFLOAD,RMODE(64),PART(SWFTGLU1)
FREE     ALIAS C'free'
FREE     AMODE 64
FREE     XATTR LINKAGE(XPLINK),SCOPE(IMPORT),REF(CODE)
FREE_FD  DC    RD(FREE)
         DC    VD(FREE)
SWFTGLUE LOCTR
*
SKIPFREE DS    0H
         DROP  R12                     31-bit storage now gone
         LGR   R5,10                   Restore our environment
         EJECT ,
*---------------------------------------------------------------------*
* Set return code from called routine                                 *
*---------------------------------------------------------------------*
         DS    0H
         L     R3,EXCIRC               Restore RC from the EXCI call
         J     RETURN                  All finished
         EJECT ,
*---------------------------------------------------------------------*
* Failure has occurred allocating 31-bit storage                      *
*---------------------------------------------------------------------*
NOMALL31 DS    0H
         LHI   R3,-2
         J     RETURN
         EJECT ,
*---------------------------------------------------------------------*
* Set return code and terminate                                       *
* CELQEPLG will restore R4 through R15.                               *
*---------------------------------------------------------------------*
RETURN   DS    0H
         CELQEPLG ,
         EJECT ,
         DROP  R4                      No longer need DSA
         DROP  R10                     No longer need environment
         DROP  R11                     No longer need our parms
*---------------------------------------------------------------------*
* Program Constants and Local Static Variables                        *
*---------------------------------------------------------------------*
         LTORG ,
         EJECT ,
*---------------------------------------------------------------------*
* Dynamic storage                                                     *
*---------------------------------------------------------------------*
AUTOSTG  DSECT ,
         CEEDSA SECTYPE=XPLINK  Map CEE Dynamic Save Area
*
* Parameters used when calling other XPLINK routines
PARMLIST DS    0D
PARM1    DS    AD
PARM2    DS    AD
PARM3    DS    AD
PARM4    DS    AD
PARM5    DS    AD
PARM6    DS    AD
PARM7    DS    AD
PARM8    DS    AD
*
* Local storage
EXCIRC   DS    FD
*
DSASIZ   EQU   *-PARMLIST+CEEDSAHPSZ    Length of DSA required
         EJECT ,
*---------------------------------------------------------------------*
* Parameter list supplied to us on entry. Don't write to these        *
* locations - it isn't our storage to use.                            *
*---------------------------------------------------------------------*
MYPARMS  DSECT ,
         DS    0FD
DSAPTR   DS    AD                      Pre-allocated DSA
FUNCID   DS    FD                      Function identifier to call
DATAPTR  DS    AD                      Data in 31-bit storage
         EJECT ,
*---------------------------------------------------------------------*
* Storage we need to specifically acquire in 31-bit                   *
*---------------------------------------------------------------------*
STOR31   DSECT ,
         DS    0F
SAVEAREA DS    18F                     Savearea for called routine
SAVEHIGH DS    13F                     R2-R14 high half save area
*
C_PLIST  DS    0A                      Parm list for called function
C_FUNCID DS    A                       Function ID
C_DATAP  DS    A                       Ptr to data area
*
STOR31L  EQU *-STOR31
         EJECT ,
*---------------------------------------------------------------------*
* Includes                                                            *
*---------------------------------------------------------------------*
         PUSH  PRINT
         PRINT OFF
         CVT   DSECT=YES               Map CVT
         CEECAA ,                      Map LE Common Anchor Area
         ISBREGS ,                     Register equates
         POP   PRINT
         EJECT ,
*---------------------------------------------------------------------*
* End of program                                                      *
*---------------------------------------------------------------------*
         END   SWFTGLUE
