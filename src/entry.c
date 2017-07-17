/* Licensed Materials - Property of IBM                               */
/*                                                                    */
/* SAMPLE                                                             */
/*                                                                    */
/* (c) Copyright IBM Corp. 2017 All Rights Reserved                   */
/*                                                                    */
/* US Government Users Restricted Rights - Use, duplication or        */
/* disclosure restricted by GSA ADP Schedule Contract with IBM Corp   */
/*                                                                    */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "exci.h"
#include "entry.h"
#include "glue.h"

/*
 * Rounds a supplied value up to the next multiple of 8,
 * suitable for alignment with long or 64-bit pointers.
 */
#define ALIGN8(x)       ((((x)+7)/8)*8)

/*
 * Additional bytes to allocate for use later down the stack.
 */
#define DSA_SIZE        1048576

#define DEBUG           0

/*
 * Prototypes for functions only used in this compile unit.
 */

static size_t calculateOffsets(int * pLen, int nCount, int * pOffset);
static void calculateAddresses(void * pBase, int * pOffset, int nCount, void ** ppData);

/*
 * Calculates the required offsets when concatenating storage blocks together.
 * Returns the total amount of storage required.
 */
static size_t calculateOffsets(int * pLen, int nCount, int * pOffset)
{
    /* Total size required */
    size_t totalSize = 0;

    /* Loop counter */
    int i = 0;

    /* Loop over each of the areas, aligning correctly */
    for ( i = 0; i < nCount; i++ ) {

        /* Offset to this item is whatever has gone previously */
        pOffset[i] = totalSize;

        /* Calculate size of this item */
        totalSize += ALIGN8(pLen[i]);
    }

    /* Return the total size required */
    return totalSize;
}


static void calculateAddresses(void * pBase, int * pOffset, int nCount, void ** ppData)
{
    /* Loop counter */
    int i = 0;

    /* Loop over each of the areas, calculating their addresses
     * using ugly, but necessary, casting.
     */
    for ( i = 0; i < nCount; i++ ) {
        ppData[i] = (void *)(((char *) pBase) + pOffset[i]);
    }
}

int execCicsLinkCommarea(char * applid, char * program, void * commarea, int iCommareaLen)
{
    /*
     * Allocate sufficient 31-bit storage to keep us going.
     */

    /* Data areas to calculate offsets */
    int iLen[3] = { DSA_SIZE, sizeof(execCicsLinkComm_t), iCommareaLen };
    int iOffset[3] = { 0, 0, 0 };
    void * pData[3] = { NULL, NULL, NULL };

    /* Amount of 31-bit storage to allocate */
    size_t iBytes31 = calculateOffsets(iLen, 3, iOffset);

    /* Allocate the required storage */
    void * pStorage31 = __malloc31(iBytes31);
    if ( pStorage31 == NULL ) {
        return STORAGE_ERROR;
    }

    if (DEBUG > 0)
    {
        printf("Obtained 31-bit storage: %.16llX of length %d bytes\n", pStorage31, iBytes31);
    }

    /* Calculate storage locations */
    calculateAddresses(pStorage31, iOffset, 3, pData);

    /*
     * Initialise data structure for call to glue code.
     */

    /* Pointer to the DSA */
    void * pDSA = pData[0];

    /* Pointer to our data structure */
    execCicsLinkComm_t * pParms = (execCicsLinkComm_t *) pData[1];

    /* Initialise pList */
    memset(pParms->applid, ' ', 8);
    strncpy(pParms->applid, applid, strlen(applid));
    memset(pParms->program, ' ', 8);
    strncpy(pParms->program, program, strlen(program));
    pParms->iCommareaLen = iCommareaLen;
    pParms->pCommarea = pData[2];

    /* Copy 64-bit data to 31-bit area */
    memcpy(pParms->pCommarea, commarea, iCommareaLen);

    if (DEBUG > 0)
    {
        /* Status update */
        printf("About to call swiftGlue()\n");
        printf("  commarea    @ [%.16llX]\n", commarea);
        printf("  parms       @ [%.16llX]\n", pParms);
        printf("  commarea 31 @ [%.16llX]\n", pParms->pCommarea);
        printf("  applid      = [%.8s]\n", pParms->applid);
        printf("  program     = [%.8s]\n", pParms->program);
    }

    /*
     * Call glue code.
     */

    /* Invoke our glue code */
    int rc = swiftGlue(pDSA, FUNC_EXEC_CICS_LINK_COMM, pParms);

    /*
     * Cleanup parms on return.
     */

    if (DEBUG > 0)
    {
        /* Status update */
        printf("Back from swiftGlue rc=%d\n", rc);
        printf("  commarea    @ [%.16llX]\n", commarea);
        printf("  parms       @ [%.16llX]\n", pParms);
        printf("  commarea 31 @ [%.16llX]\n", pParms->pCommarea);
        printf("  applid      = [%.8s]\n", pParms->applid);
        printf("  program     = [%.8s]\n", pParms->program);
    }

    /* Copy the commarea back */
    memcpy(commarea, pParms->pCommarea, iCommareaLen);

    /* Free off the storage we acquired earlier */
    free(pStorage31);

    if (DEBUG > 0)
    {
        printf("Storage freed\n");
    }

    /* Return to caller */
    return rc;
}

int execCicsLinkChannel(char * applid, char * program, char * channel)
{
    /*
     * Allocate sufficient 31-bit storage to keep us going.
     */

    /* Data areas to calculate offsets */
    int iLen[3] = { DSA_SIZE, sizeof(execCicsLinkChan_t), 0 };
    int iOffset[3] = { 0, 0, 0 };
    void * pData[3] = { NULL, NULL, NULL };

    /* Amount of 31-bit storage to allocate */
    size_t iBytes31 = calculateOffsets(iLen, 3, iOffset);

    /* Allocate the required storage */
    void * pStorage31 = __malloc31(iBytes31);
    if ( pStorage31 == NULL ) {
        return STORAGE_ERROR;
    }

    if (DEBUG > 0)
    {
        printf("Obtained 31-bit storage: %.16llX of length %d bytes\n", pStorage31, iBytes31);
    }

    /* Calculate storage locations */
    calculateAddresses(pStorage31, iOffset, 3, pData);

    /*
     * Initialise data structure for call to glue code.
     */

    /* Pointer to the DSA */
    void * pDSA = pData[0];

    /* Pointer to our data structure */
    execCicsLinkChan_t * pParms = (execCicsLinkChan_t *) pData[1];

    /* Initialise pList */
    memset(pParms->applid, ' ', 8);
    strncpy(pParms->applid, applid, strlen(applid));
    memset(pParms->program, ' ', 8);
    strncpy(pParms->program, program, strlen(program));
    memset(pParms->channel, ' ', 16);
    strncpy(pParms->channel, channel, strlen(channel));

    if (DEBUG > 0)
    {
        /* Status update */
        printf("About to call swiftGlue()\n");
        printf("  parms   @ [%.16llX]\n", pParms);
        printf("  applid  = [%.8s]\n", pParms->applid);
        printf("  program = [%.8s]\n", pParms->program);
        printf("  channel = [%.16s]\n", pParms->channel);
    }

    /*
     * Call glue code.
     */

    /* Invoke our glue code */
    int rc = swiftGlue(pDSA, FUNC_EXEC_CICS_LINK_CHAN, pParms);

    if (DEBUG > 0)
    {
        /* Status update */
        printf("Back from swiftGlue()\n");
        printf("  parms   @ [%.16llX]\n", pParms);
        printf("  applid  = [%.8s]\n", pParms->applid);
        printf("  program = [%.8s]\n", pParms->program);
        printf("  channel = [%.16s]\n", pParms->channel);
    }

    /*
     * Cleanup parms on return.
     */

    /* Free off the storage we acquired earlier */
    free(pStorage31);

    if (DEBUG > 0)
    {
        printf("Storage freed\n");
    }

    /* Return to caller */
    return rc;
}

int execCicsPutContainerString(char * container, char * channel, char * data, int iDataLength)
{
	/*
	 * Allocate sufficient 31-bit storage to keep us going.
	 */

	/* Data areas to calculate offsets */
	int iLen[3] = { DSA_SIZE, sizeof(execCicsPutContString_t), iDataLength };
	int iOffset[3] = { 0, 0, 0 };
	void * pData[3] = { NULL, NULL, NULL };

	/* Amount of 31-bit storage to allocate */
	size_t iBytes31 = calculateOffsets(iLen, 3, iOffset);

	/* Allocate the required storage */
	void * pStorage31 = __malloc31(iBytes31);
	if ( pStorage31 == NULL ) {
		return STORAGE_ERROR;
	}

	if (DEBUG > 0)
	{
	    printf("Obtained 31-bit storage: %.16llX of length %d bytes\n", pStorage31, iBytes31);
	}

	/* Calculate storage locations */
	calculateAddresses(pStorage31, iOffset, 3, pData);

	/*
	 * Initialise data structure for call to glue code.
	 */

	/* Pointer to the DSA */
	void * pDSA = pData[0];

	/* Pointer to our data structure */
	execCicsPutContString_t * pParms = (execCicsPutContString_t *) pData[1];

	/* Initialise pList */
	memset(pParms->container, ' ', 16);
	strncpy(pParms->container, container, strlen(container));
	memset(pParms->channel, ' ', 16);
	strncpy(pParms->channel, channel, strlen(channel));
	pParms->iDataLen = iDataLength;
	pParms->data = pData[2];

	/* Copy 64-bit data to 31-bit area */
	memcpy(pParms->data, data, iDataLength);

	if (DEBUG > 0)
	{
        /* Status update */
        printf("About to call swiftGlue()\n");
        printf("  data      @ [%.16llX]\n", data);
        printf("  parms     @ [%.16llX]\n", pParms);
        printf("  data 31   @ [%.16llX]\n", pParms->data);
        printf("  channel   = [%.16s]\n", pParms->channel);
        printf("  container = [%.16s]\n", pParms->container);
	}

	/*
	 * Call glue code.
	 */

	/* Invoke our glue code */
    int rc = swiftGlue(pDSA, FUNC_EXEC_CICS_PUT_CONT_STRING, pParms);

    if (DEBUG > 0)
    {
        /* Status update */
        printf("Back from swiftGlue()\n");
        printf("  data      @ [%.16llX]\n", data);
        printf("  parms     @ [%.16llX]\n", pParms);
        printf("  data 31   @ [%.16llX]\n", pParms->data);
        printf("  channel   = [%.16s]\n", pParms->channel);
        printf("  container = [%.16s]\n", pParms->container);
    }

	/* Free off the storage we acquired earlier */
	free(pStorage31);

	if (DEBUG > 0)
	{
	    printf("Storage freed\n");
	}

	/* Return to caller */
	return rc;
}

char * execCicsGetContainerString(char * container, char * channel, int ccsid)
{
	/*
	 * Allocate sufficient 31-bit storage to keep us going.
	 */

	/* Data areas to calculate offsets */
	int iLen[3] = { DSA_SIZE, sizeof(execCicsGetContString_t), 0 };
	int iOffset[3] = { 0, 0, 0 };
	void * pData[3] = { NULL, NULL, NULL };

	/* Amount of 31-bit storage to allocate */
	size_t iBytes31 = calculateOffsets(iLen, 3, iOffset);

	/* Allocate the required storage */
	void * pStorage31 = __malloc31(iBytes31);
	if ( pStorage31 == NULL ) {
		/*return STORAGE_ERROR;*/
		return "STORAGE_ERROR";
	}

	if (DEBUG > 0)
	{
		printf("Obtained 31-bit storage: %.16llX of length %d bytes\n", pStorage31, iBytes31);
	}

	/* Calculate storage locations */
	calculateAddresses(pStorage31, iOffset, 3, pData);

	/*
	 * Initialise data structure for call to glue code.
	 */

	/* Pointer to the DSA */
	void * pDSA = pData[0];

	/* Pointer to our data structure */
	execCicsGetContString_t * pParms = (execCicsGetContString_t *) pData[1];

	/* Initialise pList */
	memset(pParms->container, ' ', 16);
	strncpy(pParms->container, container, strlen(container));
	memset(pParms->channel, ' ', 16);
	strncpy(pParms->channel, channel, strlen(channel));
  pParms->ccsid = ccsid;

	if (DEBUG > 0)
	{
		/* Status update */
		printf("About to call swiftGlue()\n");
		/*printf("  data      @ [%.16llX]\n", data);*/
		printf("  parms     @ [%.16llX]\n", pParms);
		printf("  data 31   @ [%.16llX]\n", pParms->data);
		printf("  channel   = [%.16s]\n", pParms->channel);
		printf("  container = [%.16s]\n", pParms->container);
	}

	/*
	 * Call glue code.
	 */

	/* Invoke our glue code */
	int rc = swiftGlue(pDSA, FUNC_EXEC_CICS_GET_CONT_STRING, pParms);

	if (DEBUG > 0)
	{
		/* Status update */
		printf("Back from swiftGlue()\n");
		/*printf("  data      @ [%.16llX]\n", data);*/
		printf("  parms     @ [%.16llX]\n", pParms);
		printf("  data 31   @ [%.16llX]\n", pParms->data);
		printf("  channel   = [%.16s]\n", pParms->channel);
		printf("  container = [%.16s]\n", pParms->container);
	}

	char * output;

	if (rc > 0)
	{
		output = "error";
	}
	else
	{
	    if (DEBUG > 0)
	    {
			printf("data: [%s]\n", pParms->data);
			printf("data length: [%d]\n", strlen(pParms->data));
	    }
		output = pParms->data;
	}

	/* Free off the storage we acquired earlier */
	free(pStorage31);

	if (DEBUG > 0)
	{
		printf("Storage freed\n");
	}

	/* Return to caller */

	return output;
}
