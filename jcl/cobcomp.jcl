//COBCOMP JOB COBCOMP,,CLASS=M,MSGCLASS=H,
//  NOTIFY=&SYSUID,
//  MSGLEVEL=(1,1)
//*
//* Licensed Materials - Property of IBM
//*
//* SAMPLE
//*
//* (c) Copyright IBM Corp. 2017 All Rights Reserved
//*
//* US Government Users Restricted Rights - Use, duplication or
//* disclosure restricted by GSA ADP Schedule Contract with IBM Corp
//*
//* To use this sample build job for the EDUCHAN CICS COBOL program
//* replace the following symbols:
//*
//* @path_to_cobol@ - replace with the path on ZFS to the COBOL program
//*                   EDUCHAN.cbl
//*
//* @cics_hlq@ - replace with the high-level qualifier for CICS
//*
//* @cobol_comp_hlq@ - replace with the high-level qualifier for the
//*                    COBOL compiler SIGYCOMP library. For example
//*                    PP.COBOL390
//*
//* @output_loadlib@ - replace with output PDS for load library
//*
//* This sample job will call the standard DFHZITCL supplied procedure
//* in SDFHPROC to compile the COBOL.
//*
//*
//MYPROC   JCLLIB ORDER=(@cics_hlq@.SDFHPROC)
//APPLPROG EXEC DFHZITCL,INDEX=@cics_hlq@,
// LNGPRFX=@cobol_comp_hlq@,
// PROGLIB=@output_loadlib@,
// DSCTLIB=@cics_hlq@.SDFHCOB
//COBOL.SYSIN  DD PATHOPTS=(ORDONLY),PATH='@path_to_cobol@/EDUCHAN.cbl',
//            RECFM=FB,LRECL=80,FILEDATA=TEXT
//LKED.SYSIN DD *
   NAME EDUCHAN(R)
/*
