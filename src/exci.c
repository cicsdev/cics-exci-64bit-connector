/* Licensed Materials - Property of IBM                               */
/*                                                                    */
/* SAMPLE                                                             */
/*                                                                    */
/* (c) Copyright IBM Corp. 2017 All Rights Reserved                   */
/*                                                                    */
/* US Government Users Restricted Rights - Use, duplication or        */
/* disclosure restricted by GSA ADP Schedule Contract with IBM Corp   */
/*                                                                    */

#pragma XOPTS(EXCI)

#include <dfhxcplh.h>
#include <metal.h>
#include <stdlib.h>
#include <string.h>

#include "exci.h"

/*
 * Response and reason codes can be found here:
 * http://www.ibm.com/support/knowledgecenter/SSGMCP_5.3.0/com.ibm.cics.ts.doc/
 *       dfhtm/topics/dfhtmfb.html
 */


static int execCicsLinkCommarea(void * pParms);

static int execCicsLinkChannel(void * pParms);

static int execCicsPutContainerString(void * pParms);

static int execCicsGetContainerString(void * pParms);

#pragma map(main, "CALLCICS")
#pragma prolog(main, "EXCIPRLG")
#pragma epilog(main, "EXCIEPLG")
int main(int iFunctionId, void * pParms)
{
    switch ( iFunctionId ) {

    case FUNC_EXEC_CICS_LINK_COMM:
        return execCicsLinkCommarea(pParms);

    case FUNC_EXEC_CICS_LINK_CHAN:
		return execCicsLinkChannel(pParms);

    case FUNC_EXEC_CICS_PUT_CONT_STRING:
		return execCicsPutContainerString(pParms);

    case FUNC_EXEC_CICS_GET_CONT_STRING:
		return execCicsGetContainerString(pParms);

    default:
        return ERROR_INVALID_FUNCTION;
    }
}

static int execCicsLinkCommarea(void * pParms)
{
    /* Return code */
    int rc;

    /* Return codes */
    exci_exec_return_code link_retarea;

    /* Unpack the parm lists */
    execCicsLinkComm_t * pExecCicsLinkComm = pParms;

    /* Call CICS */
    EXEC CICS LINK PROGRAM(&pExecCicsLinkComm->program)
                   COMMAREA(pExecCicsLinkComm->pCommarea)
                   LENGTH(pExecCicsLinkComm->iCommareaLen)
                   APPLID(&pExecCicsLinkComm->applid)
                   RETCODE(&link_retarea);

    /* Return top-level value and set RESP values */
    pExecCicsLinkComm->RESP = link_retarea.exec_resp;
    pExecCicsLinkComm->RESP2 = link_retarea.exec_resp2;
    rc = link_retarea.exec_resp;

    /* Return to caller */
    return rc;
}

static int execCicsLinkChannel(void * pParms)
{
	/* Return code */
	int rc;

	/* Return codes */
	exci_exec_return_code link_retarea;

	/* Unpack the parm lists */
	execCicsLinkChan_t * pExecCicsLinkChan = pParms;

	/* Call CICS */
	EXEC CICS LINK PROGRAM(&pExecCicsLinkChan->program)
	               APPLID(&pExecCicsLinkChan->applid)
	               CHANNEL(&pExecCicsLinkChan->channel)
	               RETCODE(&link_retarea);

  /* Return top-level value and set RESP values */
  pExecCicsLinkChan->RESP = link_retarea.exec_resp;
  pExecCicsLinkChan->RESP2 = link_retarea.exec_resp2;
  rc = link_retarea.exec_resp;

	/* Return to caller */
	return rc;
}

static int execCicsPutContainerString(void * pParms)
{
    /* Return code */
    int rc;

    /* Return codes */
    exci_exec_return_code link_retarea;

    /* Unpack the parm lists */
    execCicsPutContString_t * pExecCicsPutContString = pParms;

    /* Call CICS */
    EXEC CICS PUT CONTAINER(&pExecCicsPutContString->container)
                  CHANNEL(&pExecCicsPutContString->channel)
                  FROM(pExecCicsPutContString->data)
                  FLENGTH(pExecCicsPutContString->iDataLen)
                  CHAR
                  RETCODE(&link_retarea);


    /* Return top-level value and set RESP values */
    pExecCicsPutContString->RESP = link_retarea.exec_resp;
    pExecCicsPutContString->RESP2 = link_retarea.exec_resp2;
    rc = link_retarea.exec_resp;

    return rc;
}

static int execCicsGetContainerString(void * pParms)
{
    /* Return code */
    int rc = 0;

    /* Return codes */
    exci_exec_return_code link_retarea;

    /* Unpack the parm lists */
    execCicsGetContString_t * pExecCicsGetContString = pParms;

    int flength = 2048;

    /* Call CICS */
    EXEC CICS GET CONTAINER(&pExecCicsGetContString->container)
                  CHANNEL(&pExecCicsGetContString->channel)
                  INTO(pExecCicsGetContString->data)
                  FLENGTH(flength)
                  INTOCCSID(pExecCicsGetContString->ccsid)
                  RETCODE(&link_retarea);

    /* Return top-level value and set RESP values */
    pExecCicsGetContString->RESP = link_retarea.exec_resp;
    pExecCicsGetContString->RESP2 = link_retarea.exec_resp2;
    rc = link_retarea.exec_resp;

    return rc;
}
