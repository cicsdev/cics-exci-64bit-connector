/* Licensed Materials - Property of IBM                               */
/*                                                                    */
/* SAMPLE                                                             */
/*                                                                    */
/* (c) Copyright IBM Corp. 2017 All Rights Reserved                   */
/*                                                                    */
/* US Government Users Restricted Rights - Use, duplication or        */
/* disclosure restricted by GSA ADP Schedule Contract with IBM Corp   */
/*                                                                    */

#ifndef __exci_h
#define __exci_h

/*
 * Structure and type declarations.
 */


typedef struct {

  /* CICS applid to call */
  char applid[8];

  /* CICS program name to call */
  char program[8];

  /* Length of the commarea */
  int iCommareaLen;

  /* Pointer to the commarea */
  void * __ptr32 pCommarea;

  /* EXCI RESP codes */
  int RESP;
  int RESP2;

} execCicsLinkComm_t;

typedef struct {

  /* CICS applid to call */
  char applid[8];

  /* CICS program name to call */
  char program[8];

  /* Channel to pass to CICS */
	char channel[16];

  /* EXCI RESP codes */
  int RESP;
  int RESP2;

} execCicsLinkChan_t;

typedef struct {

  /* CICS applid to call */
  char container[16];

  /* CICS program name to call */
  char channel[16];

  /* Length of the commarea */
  int iDataLen;

  /* Pointer to the commarea */
  void * __ptr32 data;

  /* EXCI RESP codes */
  int RESP;
  int RESP2;

} execCicsPutContString_t;

typedef struct {

  /* CICS applid to call */
  char container[16];

  /* CICS program name to call */
  char channel[16];

  /* Container data */
  char data[2048];

  /* CCSID for returned data */
  int ccsid;

  /* EXCI RESP codes */
  int RESP;
  int RESP2;

} execCicsGetContString_t;


/*
 * Function selectors
 */

#define FUNC_EXEC_CICS_LINK_COMM       1
#define FUNC_EXEC_CICS_LINK_CHAN       2
#define FUNC_EXEC_CICS_PUT_CONT_STRING 3
#define FUNC_EXEC_CICS_GET_CONT_STRING 4


#define ERROR_INVALID_FUNCTION      -1
#define ERROR_OBTAINING_STORAGE     -2


/*
 * Function prototypes.
 */

#ifdef __cplusplus
extern "C" {
#endif


#ifdef __cplusplus
}
#endif
#endif
