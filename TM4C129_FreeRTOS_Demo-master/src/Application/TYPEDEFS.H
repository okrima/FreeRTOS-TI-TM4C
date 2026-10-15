/*
 * TYPEDEFS.H
 *
 *  Created on: 2 lug 2024
 *      Author: albanese
 */

#ifndef SRC_APPLICATION_TYPEDEFS_H_
#define SRC_APPLICATION_TYPEDEFS_H_


#pragma pack(push)  /* push current alignment to stack */
#pragma pack(1)     /* set alignment to 1 byte boundary */
union  TBitWord                      // typedef TBitWord
{
  unsigned short vWORD;
  struct { unsigned char L, H;} BYTE;
  struct { unsigned D0:1, D1:1, D2:1, D3:1, D4:1, D5:1, D6:1, D7:1;
           unsigned D8:1, D9:1, DA:1, DB:1, DC:1, DD:1, DE:1, DF:1;} BIT;
} ;
#pragma pack(pop)   /* restore original alignment from stack */


#endif /* SRC_APPLICATION_TYPEDEFS_H_ */
