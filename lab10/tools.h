//tools.h
#ifndef TOOLS_H
#define TOOLS_H

typedef unsigned char byte;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long uint64;

typedef int bool;

#define true 0x0001
#define false 0x0000

#define EMPTY (byte)0x00
#define FULL (byte)0xFF

typedef int(Comparision)(const void *a, const void *b);
typedef void(Action)(void *target);
typedef bool(Condition)(const void *target);

#endif