/*
Copyright (c) 2020  MIPT

Module Name:
    Common.c

Abstract:
    ����� �����������

Author:
    JulesIMF

Last Edit:
    17.10.2020 0:26

Edit Notes:

*/

#ifndef _JCPU_COMMON
#define _JCPU_COMMON

#define _JCPU_ASM_VERSION           20201102u
#define _JCPU_MIN_ASM_VERSION       20201030u
#define _JCPU_SIGNATURE_SIZE        sizeof(jbcSignature)

typedef char                        byte;
#define LABEL_SYMBOL                ':'
#define STACKSET_SYMBOL             '@'
#define INTSET_SYMBOL               '#'
#define DATA_SYMBOL                 '~'

typedef enum
{
    OUTF_LLD = 0,
    OUTF_LF,
    OUTF_016LLX,
} OutFormat;

typedef enum
{
    //Arithmetic
    cmd_add	    = 0x00,
    cmd_sub	    = 0x01,
    cmd_mul	    = 0x02,
    cmd_div	    = 0x03,
    cmd_sin	    = 0x04,
    cmd_cos	    = 0x05,
    cmd_sqrt	= 0x06,
    cmd_inc	    = 0x07,
    cmd_dec	    = 0x08,
    cmd_fadd	= 0x0A,
    cmd_fsub	= 0x0B,
    cmd_fmul	= 0x0C,
    cmd_fdiv	= 0x0D,

    //Control
    cmd_jmp	    = 0x10,
    cmd_call	= 0x11,
    cmd_ret	    = 0x12,
    cmd_je      = 0x15,
    cmd_jne     = 0x16,
    cmd_jl      = 0x17,
    cmd_jle     = 0x18,
    cmd_jg      = 0x19,
    cmd_jge     = 0x1A,

    //Logic
    cmd_and     = 0x20,
    cmd_xor     = 0x21,
    cmd_or      = 0x22,
    cmd_test	= 0x23,
    cmd_cmp	    = 0x24,
    cmd_shl	    = 0x25,
    cmd_shr	    = 0x26,
    cmd_not     = 0x29,
    cmd_fcmp    = 0x2A,

    //Stack
    cmd_mov	    = 0x30,
    cmd_push	= 0x31,
    cmd_pop	    = 0x32,
    cmd_pusha	= 0x33,
    cmd_pushf   = 0x34,
    cmd_popa	= 0x35,
    cmd_popf    = 0x36,
    cmd_pushr   = 0x37,
    cmd_pushs   = 0x38,
    cmd_pushl   = 0x39,

    //System
    cmd_out	    = 0x40,
    cmd_hlt	    = 0x41,
    cmd_in	    = 0x42,
    cmd_dump	= 0x43,
    cmd_cls     = 0x46,
    cmd_scr     = 0x47,
    cmd_tran    = 0x48,
    cmd_putch   = 0x49,
    cmd_getch   = 0x4A,
    cmd_meow    = 0x4B,
    
} Cmd;

typedef struct
{
    unsigned signature;
    unsigned asmVersion;
    long long checksum;
    long long stackSize;
    unsigned INT[8];
} jbcSignature;


typedef enum 
{ 
    AST_RAX = 0, 
    AST_RBX, 
    AST_RCX, 
    AST_RDX,
    AST_STACKTOP, 
    AST_CONST 
} AST;

enum ConsoleColor
{
    Black,
    Blue,
    Green,
    Cyan,
    Red,
    Magenta,
    Brown,
    LightGray,
    DarkGray,
    LightBlue,
    LightGreen,
    LightCyan,
    LightRed,
    LightMagenta,
    Yellow,
    White
};


#endif // !_JCPU_COMMON
