//============================================================================
// 635 Linux Demonstration Code.
// test635,c: Ultra-simple 635 command-line communications example
// Copyright 2005, Crystalfontz America, Inc. Written by Brent A. Crosby
// www.crystalfontz.com, brent@crystalfontz.com
//============================================================================
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "typedefs.h"
#include "serial.h"
#include "cf_packet.h"
#include "show_packet.h"

//============================================================================
int main(int argc, char* argv[])
  {
  printf("Ultra-simple CFA-635 command-line communications example.\n");
  printf("Crystalfontz America, Inc. http://www.crystalfontz.com\n\n");
  printf("Usage:\n");
  printf("%s PORT BAUD\n",argv[0]);
  printf("PORT is something like \"/dev/ttyS0\" or \"/dev/usb/ttyUSB0\"\n");
  printf("BAUD is 19200 or 115200\n");
  printf("To clear the display, enter \"clear\" as an optional third parameter\n\n");


  //If only 0 or 1 parameter is entered, prompt for the missing parameter(s)
  if(argc < 3)
    {
      printf("\nMISSING A PARAMETER. Enter both PORT and BAUD.\n\n");
      return(0);
    }

  //Check for optional "clear" parameter and set flag if found
  int
    cleardisplay=0;
  if((argc > 3)&&(!strcmp(argv[3],"clear"))) cleardisplay=1;


  int
    baud;
  //default the baud to 115200
  if(strcmp(argv[2],"19200"))
    baud=115200;
  else
    baud=19200;

  if(Serial_Init(argv[1],baud))
    {
    printf("Could not open port \"%s\" at \"%d\" baud.\n",argv[1],baud);
    return(1);
    }
  else
    printf("\"%s\" opened at \"%d\" baud.\n\n",argv[1],baud);

  //For some reason, Linux seems to buffer up data from the LCD, and they are sometimes
  //dumped at the start of the program. Clear the serial buffer.
  while(BytesAvail())
    GetByte();

//Outgoing command packets. Either clear the screen
//or send our line information
//**********************************************
  if(cleardisplay)
  {
    outgoing_response.command = 6;
    outgoing_response.data_length = 0;
    send_packet();
  }
  else
  {
    //Send line 1 to the 635 using command 31
    outgoing_response.command = 31;
    outgoing_response.data[0]=0; //col
    outgoing_response.data[1]=0; //row
    memcpy(&outgoing_response.data[2],">>>This is line 1<<<",20);
    outgoing_response.data_length = 22;  //the col & row position + the 20 char data length
    send_packet();

		//CFA-635 communications protocol only allows
		//one outstanding packet at a time. Wait for the response
		//packet from the CFA-635 before sending another
		//packet.
		int
		k;
		int
		timed_out;
		timed_out = 1; //default timed_out is true
		for(k=0;k<=10000;k++)
		if(check_for_packet())
			{
			ShowReceivedPacket();
			timed_out = 0; //set timed_out to false
			break;
			}
		if(timed_out)
		printf("Timed out waiting for a response.\n");

    //Send line 2 to the 635 using command 31
    outgoing_response.command = 31;
    outgoing_response.data[0]=0; //col
    outgoing_response.data[1]=1; //row
    memcpy(&outgoing_response.data[2],">>>This is line 2<<<",20);
    outgoing_response.data_length = 22;
    send_packet();

		timed_out = 1; //default timed_out is true
		for(k=0;k<=10000;k++)
			if(check_for_packet())
				{
				ShowReceivedPacket();
				timed_out = 0; //set timed_out to false
				break;
				}
		if(timed_out)
			printf("Timed out waiting for a response.\n");

    //Send line 3 to the 635 using command 31
    outgoing_response.command = 31;
    outgoing_response.data[0]=0; //col
    outgoing_response.data[1]=2; //row
    memcpy(&outgoing_response.data[2],">>>This is line 3<<<",20);
    outgoing_response.data_length = 22;
    send_packet();

		timed_out = 1; //default timed_out is true
		for(k=0;k<=10000;k++)
			if(check_for_packet())
				{
				ShowReceivedPacket();
				timed_out = 0; //set timed_out to false
				break;
				}
		if(timed_out)
			printf("Timed out waiting for a response.\n");

    //Send line 4 to the 635 using command 31
    outgoing_response.command = 31;
    outgoing_response.data[0]=0; //col
    outgoing_response.data[1]=3; //row
    memcpy(&outgoing_response.data[2],">>>This is line 4<<<",20);
    outgoing_response.data_length = 22;
    send_packet();

		timed_out = 1; //default timed_out is true
		for(k=0;k<=10000;k++)
			if(check_for_packet())
				{
				ShowReceivedPacket();
				timed_out = 0; //set timed_out to false
				break;
				}
		if(timed_out)
			printf("Timed out waiting for a response.\n");
  }
//**********************************************

  printf("\nCtrl-C to exit.\n");
  printf("Updated display, now waiting for packets\n\n");

  while(!cleardisplay)
   {
     usleep(100000);  // 1/10 second
     if(check_for_packet())
       ShowReceivedPacket();
   }

  if(cleardisplay) printf("Display Cleared.\n");
  printf("Done.\n\n");
  Uninit_Serial();
  return 0;
  }
//============================================================================
