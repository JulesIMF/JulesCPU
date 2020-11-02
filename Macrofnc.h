#ifndef _JCPU_MACROFNC
#define _JCPU_MACROFNC

#define IS_TAPE_END(core, type, ret)    \
if (isTapeEnd(core, sizeof(type))){     \
IRQ_TapeError(core);                    \
return ret;                             \
}


#define STACK_TOP(core, dst, ret)                               \
if (stackTop(core->coreStack, &(dst)) != STACK_OK)              \
{                                                               \
    IRQ_StackError(core);                                       \
    return ret;                                                 \
}


#define STACK_POP(core, dst, ret)                               \
if (stackTop(core->coreStack, &(dst)) != STACK_OK               \
||                                                              \
stackPop(core->coreStack) != STACK_OK)                          \
{                                                               \
    IRQ_StackError(core);                                       \
    return ret;                                                 \
}                                                               


#define STACK_PUSH(core, src, ret)                              \
if (stackPush(core->coreStack, src) != STACK_OK)                \
{                                                               \
    IRQ_StackError(core);                                       \
    return ret;                                                 \
}


#define DOUBLE(value) ( *(double*)(&(value)) )
#define LL(value) ( *(long long*)(&(value)) )


#define GET_ARGUMENT(CORE, TYPE) ((CORE->rip += sizeof(TYPE)),  *( TYPE*)( (CORE)->program.tape + CORE->rip - sizeof(TYPE) ) )

#endif