/*----------------------------------------------------------------------
 * network_order.h -- macros to deal with byte order 
 *
 * January 2009, Bo Berry
 *
 * Copyright (c) 2009-2011 by Cisco Systems, Inc.
 * All rights reserved. 
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom
 * the Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT.  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *---------------------------------------------------------------------
 */

#ifndef __NETWORK_ORDER_H__
#define __NETWORK_ORDER_H__

#include <inttypes.h> 


/*
 * Network Order is the transmission of the most significant byte first,
 * or big-endian.
 * http://en.wikipedia.org/wiki/Endianness
 *
 * Little-Endian architectures, the low-order byte of a word is stored
 * in the lowest address in memory.
 * 
 * Big-Endian architectures, the high-order byte of a word is stored
 * in the lowest address in memory.
 * 
 * Porting, it may be necessary to modify these functions based upon your
 * platform. 
 * 
 *     16-bits  
 *         LSB ----------------> MSB   Big Endian 
 *             | Byte1 | Byte0 | 
 *         MSB <---------------- LSB   Little Endian 
 * 
 *     32-bits  
 *         LSB --------------------------------> MSB   Big Endian 
 *             | Byte3 | Byte2 | Byte1 | Byte0 |  
 *         MSB <-------------------------------- LSB   Little Endian 
 * 
 */ 

/*
 * Define the macro for processor order to network order 
 * 
 * PowerPC, 680x0
 * #define __BIG_ENDIAN__   
 * #define __BIG_ENDIAN_TO_FROM_NETWORK__   
 * 
 * x86, x64 
 * Note: MAC gcc defines the __LITTLE_ENDIAN__ macro 
 * #define __LITTLE_ENDIAN__   
 * #define __LITTLE_ENDIAN_TO_FROM_NETWORK__   
 */ 

#define __LITTLE_ENDIAN_TO_FROM_NETWORK__   


/* 8-bit moves */
#undef NETWORK_ORDER_GETCHAR 
#define NETWORK_ORDER_GETCHAR(ptr, value) { \
        (value) = *((char *)ptr); \
}

#undef NETWORK_ORDER_PUTCHAR 
#define NETWORK_ORDER_PUTCHAR(ptr, value) { \
        *((char *)ptr) = (char)(value); \
}


/* unsigned 8-bit moves */
#undef NETWORK_ORDER_GETBYTE 
#define NETWORK_ORDER_GETBYTE(ptr, value) { \
        (value) = *((uint8_t *)ptr); \
}

#undef NETWORK_ORDER_PUTBYTE
#define NETWORK_ORDER_PUTBYTE(ptr, value) { \
        *((uint8_t *)ptr) = (uint8_t)(value); \
}


#if defined  __LITTLE_ENDIAN_TO_FROM_NETWORK__   

/* 16-bit moves */ 
#undef NETWORK_ORDER_GETSHORT 
#define NETWORK_ORDER_GETSHORT(ptr, value) { \
        (value)  = *((uint8_t *)ptr + 0); \
        (value) <<= 8; \
        (value) |= *((uint8_t *)ptr + 1); \
}


#undef NETWORK_ORDER_PUTSHORT 
#define NETWORK_ORDER_PUTSHORT(ptr, value) { \
        *((uint8_t *)ptr + 0) = (uint8_t)((value) >> 8); \
        *((uint8_t *)ptr + 1) = (uint8_t)(value); \
}


/* 32-bit moves */ 
#undef NETWORK_ORDER_GETLONG  
#define NETWORK_ORDER_GETLONG(ptr, value) { \
        (value)  = *((uint8_t *)ptr + 0); (value) <<= 8; \
        (value) |= *((uint8_t *)ptr + 1); (value) <<= 8; \
        (value) |= *((uint8_t *)ptr + 2); (value) <<= 8; \
        (value) |= *((uint8_t *)ptr + 3); \
}

#undef NETWORK_ORDER_PUTLONG  
#define NETWORK_ORDER_PUTLONG(ptr, value) { \
        *((uint8_t *)ptr + 0) = (uint8_t) ((value) >> 24); \
        *((uint8_t *)ptr + 1) = (uint8_t) ((value) >> 16); \
        *((uint8_t *)ptr + 2) = (uint8_t) ((value) >> 8); \
        *((uint8_t *)ptr + 3) = (uint8_t) (value); \
}



/* 64-bit moves */ 
#undef NETWORK_ORDER_GETLLONG  
#define NETWORK_ORDER_GETLLONG(ptr, value) { \
        (value)  = *((uint8_t *)ptr + 0); (value) <<= 8; \
        (value) |= *((uint8_t *)ptr + 1); (value) <<= 8; \
        (value) |= *((uint8_t *)ptr + 2); (value) <<= 8; \
        (value) |= *((uint8_t *)ptr + 3); (value) <<= 8; \
        (value) |= *((uint8_t *)ptr + 4); (value) <<= 8; \
        (value) |= *((uint8_t *)ptr + 5); (value) <<= 8; \
        (value) |= *((uint8_t *)ptr + 6); (value) <<= 8; \
        (value) |= *((uint8_t *)ptr + 7); \
} 

#undef NETWORK_ORDER_PUTLLONG  
#define NETWORK_ORDER_PUTLLONG(ptr, value) { \
        *((uint8_t *)ptr + 0) = (uint8_t) ((value) >> 56); \
        *((uint8_t *)ptr + 1) = (uint8_t) ((value) >> 48); \
        *((uint8_t *)ptr + 2) = (uint8_t) ((value) >> 40); \
        *((uint8_t *)ptr + 3) = (uint8_t) ((value) >> 32); \
        *((uint8_t *)ptr + 4) = (uint8_t) ((value) >> 24); \
        *((uint8_t *)ptr + 5) = (uint8_t) ((value) >> 16); \
        *((uint8_t *)ptr + 6) = (uint8_t) ((value) >> 8); \
        *((uint8_t *)ptr + 7) = (uint8_t) (value); \
}


#elif defined  __BIG_ENDIAN_TO_FROM_NETWORK__   

/* 16-bit moves */
#undef NETWORK_ORDER_GETSHORT
#define NETWORK_ORDER_GETSHORT(ptr, value) { \
        (value)  = *((uint8_t *)ptr + 1); \
        (value) <<= 8; \
        (value) |= *((uint8_t *)ptr + 0); \
}


#undef NETWORK_ORDER_PUTSHORT
#define NETWORK_ORDER_PUTSHORT(ptr, value) { \
        *((uint8_t *)ptr + 1) = (uint8_t)((value) >> 8); \
        *((uint8_t *)ptr + 0) = (uint8_t)(value); \
}


/* 32-bit moves */
#undef NETWORK_ORDER_GETLONG
#define NETWORK_ORDER_GETLONG(ptr, value) { \
        (value)  = *((uint8_t *)ptr + 3); (value) <<= 8; \
        (value) |= *((uint8_t *)ptr + 2); (value) <<= 8; \
        (value) |= *((uint8_t *)ptr + 1); (value) <<= 8; \
        (value) |= *((uint8_t *)ptr + 0); \
}

#undef NETWORK_ORDER_PUTLONG
#define NETWORK_ORDER_PUTLONG(ptr, value) { \
        *((uint8_t *)ptr + 3) = (uint8_t) ((value) >> 24); \
        *((uint8_t *)ptr + 2) = (uint8_t) ((value) >> 16); \
        *((uint8_t *)ptr + 1) = (uint8_t) ((value) >> 8); \
        *((uint8_t *)ptr + 0) = (uint8_t) (value); \
}


/* 64-bit moves */
#undef NETWORK_ORDER_GETLLONG
#define NETWORK_ORDER_GETLLONG(ptr, value) { \
        (value)  = *((uint8_t *)ptr + 7); (value) <<= 8; \
        (value) |= *((uint8_t *)ptr + 6); (value) <<= 8; \
        (value) |= *((uint8_t *)ptr + 5); (value) <<= 8; \
        (value) |= *((uint8_t *)ptr + 4); (value) <<= 8; \
        (value) |= *((uint8_t *)ptr + 3); (value) <<= 8; \
        (value) |= *((uint8_t *)ptr + 2); (value) <<= 8; \
        (value) |= *((uint8_t *)ptr + 1); (value) <<= 8; \
        (value) |= *((uint8_t *)ptr + 0); \
}

#undef NETWORK_ORDER_PUTLLONG
#define NETWORK_ORDER_PUTLLONG(ptr, value) { \
        *((uint8_t *)ptr + 7) = (uint8_t) ((value) >> 56); \
        *((uint8_t *)ptr + 6) = (uint8_t) ((value) >> 48); \
        *((uint8_t *)ptr + 5) = (uint8_t) ((value) >> 40); \
        *((uint8_t *)ptr + 4) = (uint8_t) ((value) >> 32); \
        *((uint8_t *)ptr + 3) = (uint8_t) ((value) >> 24); \
        *((uint8_t *)ptr + 2) = (uint8_t) ((value) >> 16); \
        *((uint8_t *)ptr + 1) = (uint8_t) ((value) >> 8); \
        *((uint8_t *)ptr + 0) = (uint8_t) (value); \
}


#else 
#error CPU endianness not defined. 
#endif 

#endif 


