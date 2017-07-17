
/* Licensed Materials - Property of IBM                               */
/*                                                                    */
/* SAMPLE                                                             */
/*                                                                    */
/* (c) Copyright IBM Corp. 2017 All Rights Reserved                   */
/*                                                                    */
/* US Government Users Restricted Rights - Use, duplication or        */
/* disclosure restricted by GSA ADP Schedule Contract with IBM Corp   */
/*                                                                    */

#include "entry.h"

int putContainerString(char * channel, char * container, char * data, int dataLength);
int programLinkChannel(char * applid, char * program, char * channel);
char * getContainerString(char * channel, char * container);

int main()
{
	char applid[8] = "IYMYCICS";
	char program[8] = "EDUCHAN ";
	char channel[16] = "MYCHAN";
	char input_container[16] = "INPUTDATA       ";
	char output_container[16] = "OUTPUTDATA      ";
	char data[26] = "This is a test from EXCI C";
	int dataLength = 26;

  /* EXEC CICS PUT CONTAINER */
  int rcCont = putContainerString(channel, input_container, data, dataLength);
  printf("putContainerString rc=[%d]\n", rcCont);

  /* EXEC CICS LINK PROGRAM */
  int rcExec = programLinkChannel(applid, program, channel);
  printf("programLinkChannel rc=[%d]\n", rcExec);

  /* EXEC CICS GET CONTAINER */
  char * getCont = getContainerString(channel, output_container);
  printf("getContainerString data=[%s]\n", getCont);

  /* Free storage */
  free(applid);
  free(program);
  free(channel);
  free(input_container);
  free(output_container);
  free(data);
}

int putContainerString(char * channel, char * container, char * data, int dataLength)
{
  /* Standard return code */
  int rc = 0;

  /* Trace point */
  printf("About to call execCicsPutContainerString()\n");

  /* Call our 64-bit routine */
  rc = execCicsPutContainerString(container, channel, data, dataLength);

  /* Trace point */
  printf("Completed call to execCicsPutContainerString() rc=[%d]\n", rc);

  if(rc != 0)
  {
    printf("\nSomething failed\n");
  }

  /* Return from here */
  return rc;
}

int programLinkChannel(char * applid, char * program, char * channel)
{
  /* Standard return code */
  int rc = 0;

  /* Trace point */
  printf("About to call execCicsLinkChannel()\n");

  /* Call our 64-bit routine */
  rc = execCicsLinkChannel(applid, program, channel);

  /* Trace point */
  printf("Completed call to execCicsLinkChannel() rc=[%d]\n", rc);

  if(rc != 0)
  {
    printf("\nSomething failed\n");
  }

  /* Return from here */
  return rc;
}

char * getContainerString(char * channel, char * container)
{
  /* Return data */
  char * data;
  
  /* Trace point */
  printf("About to call execCicsGetContainerString()\n");
  
  /* Call our 64-bit routine */
  data = execCicsGetContainerString(container, channel, 1047);
  
  /* Trace point */
  printf("Completed call to execCicsGetContainerString() data=[%s]\n", data);
  
  /* Return from here */
  return data;
}
