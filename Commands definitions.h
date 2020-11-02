#include "Common.h"
#include "JulesCPU/Execution.h"
#include "Macrofnc.h"

//UNWRAP_CMD(name, args, body)

/*
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
    cmd_popa	= 0x35,
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

*/

//
//---------------------------------------Arithmetic-----------------------------------------
//

#ifndef JMP_COMMON

#define JMP_COMMON(core, statement)     \
{                                       \
int newRIP = 0;                         \
if (isTapeEnd(core, sizeof(int))        \
    ||                                  \
    (newRIP = GET_ARGUMENT(core, int),  \
    newRIP >= core->program.size))      \
    {                                   \
        IRQ_InvalidParameters(core);    \
        return -1;                      \
    }                                   \
    if(statement) core->rip = newRIP;   \
}                                       \


#endif

#ifndef REJECT_STANDARD
UNWRAP_CMD(add, 2,

    {
        STACK_PUSH(core, parameters.first + parameters.second, -1)
    })

UNWRAP_CMD(sub, 2,

    {
        STACK_PUSH(core, parameters.first - parameters.second, -1)
    })

UNWRAP_CMD(mul, 2,

    {
        STACK_PUSH(core, parameters.first * parameters.second, -1)
    })

UNWRAP_CMD(div, 2,

    {
        if (parameters.second == 0)
        {
            IRQ_DivideByZero(core);
            return -1;
        }
        STACK_PUSH(core, parameters.first / parameters.second, -1)
        STACK_PUSH(core, parameters.first % parameters.second, -1)
    })

UNWRAP_CMD(cmp, 2,
    {
    long long value = parameters.first - parameters.second;
    if (!value) core->flags |= ((FLAG_ZF));
    else core->flags &= (~(FLAG_ZF));
    if (value >= 0) core->flags &= (~(FLAG_SF));
    else core->flags |= ((FLAG_SF));
    })

UNWRAP_CMD(fcmp, 2,
    {
    double value = DOUBLE(parameters.first) - DOUBLE(parameters.second);
    if (value == 0) core->flags |= ((FLAG_ZF));
    else core->flags &= (~(FLAG_ZF));
    if (value >= 0) core->flags &= (~(FLAG_SF));
    else core->flags |= ((FLAG_SF));
    })

UNWRAP_CMD(test, 2,
    {
    long long value = parameters.first & parameters.second;
    if (!value) core->flags |= ((FLAG_ZF));
    else core->flags &= (~(FLAG_ZF));
    if (value >= 0) core->flags &= (~(FLAG_SF));
    else core->flags |= ((FLAG_SF));
    })

UNWRAP_CMD(sin, 1,

    {
        double val = sin(DOUBLE(parameters.first));
        //printf("%016llx\n", val);
        STACK_PUSH(core, LL(val) , -1)
    })

    UNWRAP_CMD(meow, 1,

    {
        for (int i = 0; i != parameters.first; i++)
            cpu_printf(core, "meow\n");
    })

UNWRAP_CMD(cos, 1,

    {
        double val = cos(DOUBLE(parameters.first));
        //printf("%lf\n", val);
        STACK_PUSH(core, LL(val), -1)
    })

UNWRAP_CMD(sqrt, 1,

    {
        double val = sqrt(DOUBLE(parameters.first));
        printf("%lf\n", val);
        STACK_PUSH(core, LL(val), -1)
    })
UNWRAP_CMD(fadd, 2,

    {
        double val = DOUBLE(parameters.first) + DOUBLE(parameters.second);
        STACK_PUSH(core, LL(val), -1)
    })

UNWRAP_CMD(fsub, 2,

    {
        double val = DOUBLE(parameters.first) - DOUBLE(parameters.second);
        STACK_PUSH(core, LL(val), -1)
    })

UNWRAP_CMD(fmul, 2,

    {
        double val = DOUBLE(parameters.first) * DOUBLE(parameters.second);
        STACK_PUSH(core, LL(val), -1)
    })

UNWRAP_CMD(fdiv, 2,

    {
        double val = DOUBLE(parameters.first) / DOUBLE(parameters.second);
        STACK_PUSH(core, LL(val), -1)
    })
    
UNWRAP_CMD(ret, 0,
    {
        //printf("--------RET-------");
        long long newRip = 0;
        STACK_POP(core, newRip, -1)
        core->rip = newRip;
    })

UNWRAP_CMD(and, 2,

    {
        STACK_PUSH(core, parameters.first & parameters.second, -1)
    })

UNWRAP_CMD(xor, 2,

    {
        STACK_PUSH(core, parameters.first ^ parameters.second, -1)
    })

UNWRAP_CMD(or, 2,

    {
        STACK_PUSH(core, parameters.first || parameters.second, -1)
    })

UNWRAP_CMD(shl, 2,

    {
        if (parameters.second < 0 || parameters.second > 64)
        {
            IRQ_InvalidParameters(core); 
            return; 
        }
        STACK_PUSH(core, parameters.first << parameters.second, -1)
    })

UNWRAP_CMD(shr, 2,

    {
        if (parameters.second < 0 || parameters.second > 64)
        {
            IRQ_InvalidParameters(core);
            return;
        }
        STACK_PUSH(core, parameters.first >> parameters.second, -1)
    })
    
UNWRAP_CMD(push, 1,
    {
        STACK_PUSH(core, parameters.first, -1)
    })

UNWRAP_CMD(pop, 1,
    {
        core->rip -= 9;
        SecondByte secondByte = getSecondByte(core);
        if (secondByte.isPtr_1 == -1)
            return -1;
        if (popBySecondByte(core, secondByte))
            return -1;
    })

UNWRAP_CMD(mov, 2,
    {
        core->rip -= 17;
        SecondByte secondByte = getSecondByte(core);
        if (secondByte.isPtr_1 == -1)
        return -1;
        //По спецификации popBySecondByte, его интересует только первые четыре бита
        secondByte.isPtr_1 = secondByte.isPtr_2;
        secondByte.AST_1   = secondByte.AST_2;
        STACK_PUSH(core, parameters.first, -1);
        core->rip += 8;
        if (popBySecondByte(core, secondByte))
            return -1;
    })

UNWRAP_CMD(pusha, 0,
    {
        for (int reg = 0; reg != 4; reg++)
        {
            STACK_PUSH(core, core->rx[reg], -1);
        }
    }
)

UNWRAP_CMD(pushf, 0,
    {
        STACK_PUSH(core, core->flags, -1)
    })

UNWRAP_CMD(popa, 0,
    {
        for (int reg = 0; reg != 4; reg++)
        {
            STACK_POP(core, core->rx[3 - reg], -1);
        }
    }
)

UNWRAP_CMD(popf, 0,
    {
        long long val = 0;
        STACK_POP(core, val, -1);
        core->flags = val;
    }
)

UNWRAP_CMD(pushr, 0,
    {
        STACK_PUSH(core, core->rip, -1)
    })

UNWRAP_CMD(pushs, 0,
    {
        STACK_PUSH(core, core->coreStack->size, -1)
    })


UNWRAP_CMD(in, 0,
    {
        long long value = 0;
        int procN = 0;
        char buffer[128];
        cpu_printf(core, "Enter value: ");
        scanf("%127s", buffer);
        //core->currentX = 0; core->currentY = 0;
        int isSet = 0;
        if (!isSet && sscanf(buffer, "%lld%n", &value, &procN) > 0 && (!buffer[procN])) isSet = 1;
        if (!isSet && sscanf(buffer, "%llx%n", &value, &procN) > 0 && (!buffer[procN])) isSet = 1;
        if (!isSet && sscanf(buffer, "%lf%n",  &value, &procN) > 0 && (!buffer[procN])) isSet = 1;
        if(!isSet) return -1;
        STACK_PUSH(core, value, -1);

        long long* dst = core->ram + core->currentY * core->videoX + 13;
        for (int i = 0; i != 128 && buffer[i]; i++)
            dst[i] = buffer[i];

        core->currentY++;
        core->currentX = 0;
        GotoXY(core->coord.X, core->coord.Y + core->currentY);
    })

UNWRAP_CMD(dump, 0,
   {
        cpu_cls(core);
    printf(
        "\n-----CORE DUMP BEGIN-----\n"
        "[RAX]          \t%lld\n"
        "[RBX]          \t%lld\n"
        "[RCX]          \t%lld\n"
        "[RDX]          \t%lld\n"
        "[RIP]          \t%d\n"
        "[isWorking]    \t%d\n"
        "[INT]          \t%d\n"
        "[programm.size]\t%d\n"
        "[capacity]     \t%d\n"
        "[size]         \t%d\n\n",
        core->rx[0], core->rx[1],
        core->rx[2], core->rx[3],
        core->rip, core->isWorking,
        core->interruption,
        core->program.size,
        core->coreStack->capacity,
        core->coreStack->size);

    printf(
        "[CF]           \t%d\n"
        "[PF]           \t%d\n"
        "[AF]           \t%d\n"
        "[ZF]           \t%d\n"
        "[SF]           \t%d\n"
        "[TF]           \t%d\n"
        "[IF]           \t%d\n"
        "[DF]           \t%d\n"
        "[OF]           \t%d\n"
        "[NT]           \t%d\n"
        "------CORE DUMP END------\n\n",
        !!(core->flags & FLAG_CF),
        !!(core->flags & FLAG_PF),
        !!(core->flags & FLAG_AF),
        !!(core->flags & FLAG_ZF),
        !!(core->flags & FLAG_SF),
        !!(core->flags & FLAG_TF),
        !!(core->flags & FLAG_IF),
        !!(core->flags & FLAG_DF),
        !!(core->flags & FLAG_OF),
        !!(core->flags & FLAG_NT)
    );
    core->currentX = 0;
    core->currentY = 22;
    })

UNWRAP_CMD(tran, 0,
    {
            long long src = 0;
            long long dst = 0;
            long long length = 0;

            STACK_POP(core, length, -1);
            STACK_POP(core, dst, -1);
            STACK_POP(core, src, -1);

            int capacity = core->capacity;
            int size = core->program.size;

            byte* tape = core->program.tape;
            long long* ram = core->ram;

            if (src + length > size || dst + length > capacity)
            {
                IRQ_HeapOverflow(core);
                return;
            }

            for (int i = 0; i != length; i++)
            {
                ram[dst++] = tape[src++];
            }
    })

UNWRAP_CMD(cls, 0,
    {
        cpu_cls(core);
    })

UNWRAP_CMD(scr, 0,
    {
        cpu_PrnBuf(core);
    })

UNWRAP_CMD(hlt, 0,
    {
        core->isWorking = 0;
    })

UNWRAP_CMD(getch, 0,
    {
        long long c = _getch();
        STACK_PUSH(core, c, -1);
        
    })

UNWRAP_CMD(putch, 0,
    {
        long long c = 0;
        STACK_TOP(core, c, -1);
        _putch(c);
        core->currentX++;
        cpu_normalize(core);
    })
#endif        

#ifndef REJECT_SPECIAL
UNWRAP_CMD(inc, 0,
    {
        IS_TAPE_END(core, byte, -1)
        int rxIndex = (GET_ARGUMENT(core, byte) & 3); 
        core->rx[rxIndex]++;
    })

UNWRAP_CMD(dec, 0,
    {
        IS_TAPE_END(core, byte, -1)
        int rxIndex = (GET_ARGUMENT(core, byte) & 3); 
        core->rx[rxIndex]--;
    })

UNWRAP_CMD(out, 0,
    {
        long long value = 0;
        STACK_TOP(core, value, -1);
        IS_TAPE_END(core, byte, -1);
        byte secondByte = GET_ARGUMENT(core, byte);
        char buf[128];
        long long* dst = core->ram + core->currentY * core->videoX + core->currentX;
        switch (secondByte)
        {
        case OUTF_016LLX:
            sprintf(buf, "     %016llx", value);
            break;
        case OUTF_LF:
            sprintf(buf, "     %lf", value);
            break;
        default:
            sprintf(buf, "     %lld", value);
        }
        for (int i = 0; i != 128 && buf[i]; i++)
            dst[i] = buf[i];

        core->currentY = (core->currentY + 1) % core->videoY;

        cpu_PrnBuf(core);
    })
#endif


#ifndef REJECT_JMP
UNWRAP_CMD(jmp, 0, 
    {
        JMP_COMMON(core, 
        1)
    })

UNWRAP_CMD(call, 0, 
    {
        //printf("--------CALL-------");
        STACK_PUSH(core, core->rip + 4, -1)
        JMP_COMMON(core, 
        1)
    })

UNWRAP_CMD(je, 0, 
    {
        JMP_COMMON(core, 
        core->flags & FLAG_ZF)
    })

UNWRAP_CMD(jne, 0, 
    {
        JMP_COMMON(core, 
        !(core->flags & FLAG_ZF))
    })

UNWRAP_CMD(jl, 0, 
    {
        JMP_COMMON(core, 
        (core->flags & FLAG_SF) && !(core->flags & FLAG_ZF))
    })

UNWRAP_CMD(jle, 0, 
    {
        JMP_COMMON(core, 
        (core->flags & FLAG_ZF) || (core->flags & FLAG_SF))
    })

UNWRAP_CMD(jg, 0, 
    {
        JMP_COMMON(core, 
        !(core->flags & FLAG_SF) && !(core->flags & FLAG_ZF))
    })

UNWRAP_CMD(jge, 0, 
    {
        JMP_COMMON(core, 
        (core->flags & FLAG_ZF) || !(core->flags & FLAG_SF))
    })

UNWRAP_CMD(pushl, 0, 
    {
        IS_TAPE_END(core, int, -1)
        STACK_PUSH(core, GET_ARGUMENT(core, int), -1)
    })

#endif
