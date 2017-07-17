/* Licensed Materials - Property of IBM                               */
/*                                                                    */
/* SAMPLE                                                             */
/*                                                                    */
/* (c) Copyright IBM Corp. 2017 All Rights Reserved                   */       
/*                                                                    */
/* US Government Users Restricted Rights - Use, duplication or        */
/* disclosure restricted by GSA ADP Schedule Contract with IBM Corp   */
/*                                                                    */

#ifndef __glue_h
#define __glue_h

/*
 * Structure and type declarations.
 */


#ifdef __cplusplus
extern "C" {
#endif

/*
 * Function prototypes.
 */

int swiftGlue(void * __ptr32 pDSA,
              int iFunction,
              void * __ptr32 pParms);

#ifdef __cplusplus
}
#endif
#endif
