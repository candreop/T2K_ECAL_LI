#ifndef _ECALLI_BUILD_INFO_H_ 
#define _ECALLI_BUILD_INFO_H_ 
/* 
 * Header file automatically generated by the configure script 
*/ 

/* 
 * These macros can be used in the following way (as the ones at ROOT's RVersion.h): 
 * #if __ECALLI_RELEASE_CODE__ >= ECALLI_RELCODE(2,4,11) 
 * #include <newheader.h>
 * #else
 * #include <oldheader.h>
 * #endif
*/ 
#define ECALLI_RELCODE(a,b,c) (((a) << 16) + ((b) << 8) + (c)) 

#define __ECALLI_RELEASE__      "1.0.2"
#define __ECALLI_RELEASE_CODE__ ECALLI_RELCODE(1,0,2) 



#endif

