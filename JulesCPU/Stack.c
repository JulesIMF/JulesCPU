/*
Copyright (c) 2020  MIPT

Module Name:
    Stack

Abstract:
    Реализует класс стека для 64-битных переменных, а также имплементацию
    его методов и собственных "системных" функций.

Author:
    JulesIMF

Last Edit:
    15.10.2020 19:35

Edit Notes:
    1) Хеширование структуры
    2) stackCapacity
    3) Мелкие улучшения, дебаг и рефакторинг
    4) Временно удален _ReturnAddress()

    5) Dump

    6) Исправлен баг с хешированием
*/
#pragma once
#include "Stack.h"
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

//Макрос для создания копии
#define DUPLICATE(STACK, COPY)                                  \
Stack* COPY = _makeStackDuplicate(STACK);                       \
if (COPY == NULL)                                               \
    return STACK_DUPLICATION_ERROR;                             \
COPY += duplicateOffset;                                        \
stackDelete(STACK->duplicate - duplicateOffset);                \
STACK->duplicate = COPY;

//Макрос для возврата STACK_INVALID
#define CHECK_ERROR(EXPRESSION, ERROR)                          \
if(EXPRESSION)                                                  \
{                                                               \
    if(_STACK_PRINT_MESSAGES)                                   \
    {                                                           \
        printf("Check failed in %s: \"%s\", line %d\n",         \
               __FUNCTION__, #EXPRESSION, __LINE__);            \
        _dump(stack);                                           \
    }                                                           \
    return ERROR;                                               \
}

#define CHECK(EXPRESSION) CHECK_ERROR(EXPRESSION, STACK_INVALID)

#define STATIC_VALUES() if(!frontCanary) { if(!strcmp(__FUNCTION__, "stackNew")) _generateValues(); else return STACK_INVALID; }

#define _STACK_DUPLICATE_OFFSET
//#define _STACK_HASH
#define _STACK_RETURN_ADDRESS
//#define _STACK_DUPLICATE
#define _STACK_PRINT_MESSAGES 0
//#define _STACK_DEEP_VALIDATION
#define _STACK_NO_CHECK




static long long frontCanary = 0, backCanary = 0, poison = 0;
static int const maxAbsOffset = 10;
static int duplicateOffset = 0;

static void _stackRecalculate(Stack* stack);
static long long _getStructHash(Stack* stack);

static void _dump_(Stack* stack, int isDuplicate)
{
    if (stack == NULL)
    {
        printf("Null pointer passed into dump function\n");
        return;
    }

    printf(
        "[data]\t\t0x%p\n"
        "[capacity]\t%zu\n"
        "[size]\t\t%zu\n"
        "[hash]\t\t%llu\n"
        "[duplicate]\t0x%p\n"
        "[structHash]\t%llu\n",
        stack->data,
        stack->capacity,
        stack->size,
        stack->hash,
        stack->duplicate,
        stack->structHash);
#ifdef _STACK_HASH
    if (_getStructHash(stack) == stack->structHash)
    {
        if (stack->data && stack->capacity)
        {
            printf("[data listing]\n");
            long long* data = stack->data;
            for (size_t i = 0; i != stack->capacity + 2; i++)
            {
                printf("\t%zu:\t", i);
                if (data[i] == frontCanary)
                {
                    printf("frontCanary\n");
                    continue;
                }

                if (data[i] == poison)
                {
                    printf("poison\n");
                    continue;
                }

                if (data[i] == backCanary)
                {
                    printf("backCanary\n");
                    continue;
                }

                printf("%llu\n", data[i]);
            }
            printf("[~data listing]\n");
        }
        else
            printf("[data listing: unsecure parameters]\n");

        if (isDuplicate)
            return;

        if (stack->duplicate - duplicateOffset)
        {
            printf("[duplicate listing]\n");
            _dump_(stack->duplicate - duplicateOffset, 1);
            printf("[~duplicate listing]\n");
        }
        else
            printf("[duplicate listing: unsecure parameters]\n");
    }
    else
    #endif
        printf("[unsecure parameters]\n");
}

static void _dump(Stack* stack)
{
#if (_STACK_PRINT_MESSAGES != 0)
    printf("\n-----STACK \"0x%p\" DUMP BEGIN-----\n", stack);
    _dump_(stack, 0);
    printf("------STACK \"0x%p\" DUMP END------\n\n", stack);
#endif
}

static long long _getRandLL()
{
    static int set = 0;
    if (!set)
        srand(time(0)), set = 1;
    long long returned = 0;
    for (int i = 0; i != 64; i++)
        returned |= ((long long)(rand() & 1) << i);

    return returned;
}

/**
 * Генерирует системные значения.
 * \warning СИСТЕМНАЯ ФУНКЦИЯ
 */
static void _generateValues(void)
{
#ifdef    _STACK_RETURN_ADDRESS
    static int timesCalled = 0;

    timesCalled++;

    if (timesCalled > 1)
    {
    #ifdef    _STACK_PRINT_MESSAGES
        printf("\tFunction %s was unexpectedly called.\n\n", __FUNCTION__);
    #endif //!_STACK_PRINT_MESSAGES
        return;
    }
#endif //!_STACK_RETURN_ADDRESS

    srand(time(0)); //Прости, дорогой пользователь, я не нашел способа сохранить предыдущую начальную точку (((
    while (!frontCanary)
        frontCanary = _getRandLL();

    while (!backCanary)
        backCanary = _getRandLL();

    while (!poison)
        poison = _getRandLL();

#if ( defined(_STACK_DUPLICATE) && defined(_STACK_DUPLICATE_OFFSET) )
    while (!duplicateOffset)
        duplicateOffset = rand() % (maxAbsOffset * 2) - maxAbsOffset;
#endif

}

#ifdef _STACK_HASH
static long long _getHash(void* buffer, size_t size)
{
    static long long base = 0;
    char* data = (char*)buffer;

    //Установка base
    if (!base)
    {
        base = _getRandLL();
        base |= 1ll;
    }

    long long hash = 0;
    for (size_t i = 0; i != size; i++)
    {
        hash *= base;
        hash += (long long)*(data++);
    }

    return hash;
}

static long long _getDataHash(Stack* stack)
{
    //TODO Проверка адреса возврата
    assert(stack);

    return _getHash(stack->data + 1, sizeof(long long) * stack->size);
}

static long long _getStructHash(Stack* stack)
{
    //TODO Проверка адреса возврата
    assert(stack);
    static long long base = 0;
    if (!base)
    {
        base = _getRandLL();
        base |= 1ll;
    }


    //Ужас конечно
    return
        _getHash(&(stack->capacity), sizeof(stack->capacity)) +
        _getHash(&(stack->data), sizeof(stack->data)) +
        _getHash(&(stack->size), sizeof(stack->size)) +
        _getHash(&(stack->duplicate), sizeof(stack->duplicate)) +
        _getHash(&(stack->hash), sizeof(stack->hash));


}
#endif


StackStatus stackIsValid(Stack* stack)
{
    /*
        ЭТОТ БЛОК НЕЛЬЗЯ МЕНЯТЬ, Т К ЭТО ПРИВЕДЕТ К ИНВАЛИДАЦИИ ТАБЛИЦЫ СМЕЩЕНИЙ

        НАЧАЛО БЛОКА
    */
    STATIC_VALUES();
    if (stack == NULL)
        return STACK_NULL;


    //Дефолтные проверки
#ifdef _STACK_HASH
    CHECK(stack->structHash != _getStructHash(stack));
#endif

    CHECK(stack->data == NULL);

    CHECK(!stack->capacity);

    CHECK(stack->size > stack->capacity);

    //Канарейки и яд
    CHECK(stack->data[0] != frontCanary);

    CHECK(stack->data[stack->size + 1] != backCanary);

    size_t const realCapacity = stack->capacity + 2;
    long long* data = stack->data + stack->size + 2;
    for (int i = stack->size + 2; i != realCapacity; i++)
    {
        CHECK(*(data++) != poison);
    }

#ifdef _STACK_HASH
    CHECK(_getDataHash(stack) != stack->hash);
#endif

    /*
        ЭТОТ БЛОК НЕЛЬЗЯ МЕНЯТЬ, Т К ЭТО ПРИВЕДЕТ К ИНВАЛИДАЦИИ ТАБЛИЦЫ СМЕЩЕНИЙ

        КОНЕЦ БЛОКА
    */



#ifdef _STACK_DUPLICATE
    Stack* duplicate = stack->duplicate;

    CHECK(duplicate == NULL);
    duplicate -= duplicateOffset;
    CHECK(
        duplicate->capacity != stack->capacity ||
        duplicate->size != stack->size ||
        duplicate->hash != stack->hash ||
        duplicate->data == NULL ||
        duplicate->duplicate
    );

    long long* _data = duplicate->data;
    data = stack->data;

    for (int i = 0; i != realCapacity; i++)
        CHECK(*(data++) != *(_data++));
#endif


    return STACK_OK;
}


static Stack* _allocateStack(size_t capacity)
{
    Stack* stack = (Stack*)calloc(1, sizeof(Stack));
    if (stack == NULL)
        return NULL;

    stack->capacity = capacity;
    stack->size = 0;
    stack->duplicate = NULL;
    stack->hash = 0;

    size_t realCapacity = capacity + 2;
    stack->data = calloc(realCapacity, sizeof(long long));

    if (stack->data == NULL)
    {
        free(stack);
        return NULL;
    }

    return stack;
}


static Stack* _makeStackDuplicate(Stack* stack)
{
    if (stack == NULL || stack->data == NULL)
        return NULL;

    STATIC_VALUES();
    Stack* copy = _allocateStack(stack->capacity);

    if (copy == NULL)
        return NULL;

    copy->hash = stack->hash;
    copy->size = stack->size;

    size_t realCapacity = stack->capacity + 2;

    long long* copyData = copy->data;
    long long* stackData = stack->data;

    for (int i = 0; i != realCapacity; i++)
        *(copyData++) = *(stackData++);

#ifdef _STACK_HASH
    copy->structHash = _getStructHash(copy);
#endif
    return copy;
}


Stack* stackNew(size_t capacity)
{
    Stack* stack = _allocateStack(capacity);
    if (stack == NULL)
        return NULL;
#ifndef _STACK_NO_CHECK
    STATIC_VALUES();
#endif
    size_t realCapacity = capacity + 2;

    long long* data = stack->data;
    for (int i = 0; i != realCapacity; i++)
        *(data++) = poison;

    stack->data[0] = frontCanary;
    stack->data[1] = backCanary;
#ifdef _STACK_DUPLICATE
    Stack* duplicate = _makeStackDuplicate(stack);
    if (duplicate == NULL)
    {
        stackDelete(stack);
        return NULL;
    }
    stack->duplicate = duplicate + duplicateOffset;
#endif

    _stackRecalculate(stack);
    return stack;
}


void stackDelete(Stack* stack)
{
    if (stack == NULL)
        return;
    free(stack->data);
    if (stack->duplicate)
    {
        free((stack->duplicate - duplicateOffset)->data);
        free(stack->duplicate - duplicateOffset);
    }
    free(stack);
}


static void _stackRecalculate(Stack* stack)
{
    assert(stack);
#ifdef _STACK_HASH
    stack->hash = _getDataHash(stack);
#endif

#ifdef _STACK_DUPLICATE
    DUPLICATE(stack, duplicate);
#endif

#ifdef _STACK_HASH
    stack->structHash = _getStructHash(stack);
#endif
}


StackStatus stackResize(Stack* stack, size_t capacity)
{
    if (stack == NULL)
        return STACK_NULL;

    STATIC_VALUES();
    CHECK(stackIsValid(stack) != STACK_OK);

    if (capacity == NULL)
        return STACK_RESIZE_ERROR;

    if (capacity < stack->size)
        return STACK_OVERFLOW;

    long long* newData =
        (long long*)realloc(stack->data, (capacity + 2) * sizeof(long long));

    if (newData == NULL)
        return STACK_RESIZE_ERROR;

    size_t realCapacity = capacity + 2;

    for (size_t i = stack->capacity + 2; i < realCapacity; i++)
        newData[i] = poison;

    stack->data = newData;
    stack->capacity = capacity;

    _stackRecalculate(stack);

#ifdef _STACK_DEEP_VALIDATION
    return stackIsValid(stack);
#endif // _STACK_DEEP_VALIDATION
    return STACK_OK;
}


StackStatus stackPush(Stack* stack, long long value)
{
    /*
        ЭТОТ БЛОК НЕЛЬЗЯ МЕНЯТЬ, Т К ЭТО ПРИВЕДЕТ К ИНВАЛИДАЦИИ ТАБЛИЦЫ СМЕЩЕНИЙ

        НАЧАЛО БЛОКА
    */
    if (stack == NULL)
        return STACK_NULL;
#ifndef _STACK_NO_CHECK
    STATIC_VALUES();
    CHECK(stackIsValid(stack) != STACK_OK);
#endif

    if (stack->size == stack->capacity)
        return STACK_OVERFLOW;

    stack->data[1 + stack->size++] = value;
    stack->data[1 + stack->size] = backCanary;
    /*
        ЭТОТ БЛОК НЕЛЬЗЯ МЕНЯТЬ, Т К ЭТО ПРИВЕДЕТ К ИНВАЛИДАЦИИ ТАБЛИЦЫ СМЕЩЕНИЙ

        КОНЕЦ БЛОКА
    */
#ifndef _STACK_NO_CHECK
    _stackRecalculate(stack);
#endif

#ifdef _STACK_DEEP_VALIDATION
    return stackIsValid(stack);
#endif // _STACK_DEEP_VALIDATION
    return STACK_OK;
}


StackStatus stackPop(Stack* stack)
{
    /*
        ЭТОТ БЛОК НЕЛЬЗЯ МЕНЯТЬ, Т К ЭТО ПРИВЕДЕТ К ИНВАЛИДАЦИИ ТАБЛИЦЫ СМЕЩЕНИЙ

        НАЧАЛО БЛОКА
    */
    if (stack == NULL)
        return STACK_NULL;
#ifndef _STACK_NO_CHECK
    STATIC_VALUES();
    CHECK(stackIsValid(stack) != STACK_OK);
#endif

    if (!stack->size)
        return STACK_UNDERFLOW;

    stack->data[stack->size] = backCanary;
    stack->data[stack->size + 1] = poison;
    stack->size--;
    /*
        ЭТОТ БЛОК НЕЛЬЗЯ МЕНЯТЬ, Т К ЭТО ПРИВЕДЕТ К ИНВАЛИДАЦИИ ТАБЛИЦЫ СМЕЩЕНИЙ

        КОНЕЦ БЛОКА
    */
#ifndef _STACK_NO_CHECK
    _stackRecalculate(stack);
#endif

#ifdef _STACK_DEEP_VALIDATION
    return stackIsValid(stack);
#endif // _STACK_DEEP_VALIDATION
    return STACK_OK;
}


StackStatus stackSize(Stack* stack, size_t* value)
{
    if (stack == NULL)
        return STACK_NULL;

    if (value == NULL)
        return STACK_NULL;
#ifndef _STACK_NO_CHECK
    STATIC_VALUES();
    CHECK(stackIsValid(stack) != STACK_OK);
#endif

    * value = stack->size;

#ifdef _STACK_DEEP_VALIDATION
    return stackIsValid(stack);
#endif // _STACK_DEEP_VALIDATION
    return STACK_OK;
}

StackStatus stackCapacity(Stack* stack, size_t* value)
{
    if (stack == NULL)
        return STACK_NULL;

    if (value == NULL)
        return STACK_NULL;

    STATIC_VALUES();
    CHECK(stackIsValid(stack) != STACK_OK);
    *value = stack->capacity;

#ifdef _STACK_DEEP_VALIDATION
    return stackIsValid(stack);
#endif // _STACK_DEEP_VALIDATION
    return STACK_OK;
}


StackStatus stackTop(Stack* stack, long long* value)
{
    if (stack == NULL)
        return STACK_NULL;

    if (value == NULL)
        return STACK_NULL;

#ifndef _STACK_NO_CHECK
    STATIC_VALUES();
    CHECK(stackIsValid(stack) != STACK_OK);
#endif

    if (!stack->size)
        return STACK_UNDERFLOW;

    *value = stack->data[stack->size];

#ifdef _STACK_DEEP_VALIDATION
    return stackIsValid(stack);
#endif // _STACK_DEEP_VALIDATION
    return STACK_OK;
}

StackStatus stackClear(Stack* stack)
{
    /*
        ЭТОТ БЛОК НЕЛЬЗЯ МЕНЯТЬ, Т К ЭТО ПРИВЕДЕТ К ИНВАЛИДАЦИИ ТАБЛИЦЫ СМЕЩЕНИЙ

        НАЧАЛО БЛОКА
    */
    if (stack == NULL)
        return STACK_NULL;

    STATIC_VALUES();
    CHECK(stackIsValid(stack) != STACK_OK);

    stack->data[1] = backCanary;
    long long* data = stack->data + 2;
    size_t capacity = stack->capacity;
    for (int i = 0; i != capacity; i++)
        *(data++) = poison;

    stack->size = 0;
    /*
        ЭТОТ БЛОК НЕЛЬЗЯ МЕНЯТЬ, Т К ЭТО ПРИВЕДЕТ К ИНВАЛИДАЦИИ ТАБЛИЦЫ СМЕЩЕНИЙ

        КОНЕЦ БЛОКА
    */

    _stackRecalculate(stack);

#ifdef _STACK_DEEP_VALIDATION
    return stackIsValid(stack);
#endif // _STACK_DEEP_VALIDATION
    return STACK_OK;
}

Stack* stackCopy(Stack* stack)
{
    if (stack == NULL || stack->data == NULL)
        return NULL;

    STATIC_VALUES();

    Stack* copy = _makeStackDuplicate(stack);
    if (copy == NULL)
        return NULL;

#ifdef _STACK_DUPLICATE
    copy->duplicate = _makeStackDuplicate(stack->duplicate - duplicateOffset);

    if (copy->duplicate == NULL)
    {
        stackDelete(copy);
        return NULL;
    }

    copy->duplicate += duplicateOffset;
#endif

    _stackRecalculate(copy);

    return copy;
}


StackStatus stackRescue(Stack* stack)
{
    if (stack == NULL)
        return STACK_NULL;
    STATIC_VALUES();


#ifdef _STACK_DUPLICATE

#ifdef _STACK_HASH
    if ((stack->duplicate - duplicateOffset)->structHash != _getStructHash(stack->duplicate - duplicateOffset))
        return STACK_RESCUE_ERROR;
#endif // _STACK_HASH

    if (stack->duplicate == NULL)
        return STACK_RESCUE_ERROR;
    Stack* backup = stack->duplicate - duplicateOffset;
    backup->duplicate = _makeStackDuplicate(backup) + duplicateOffset;
    _stackRecalculate(backup);
    if (stackIsValid(backup) == STACK_INVALID)
    {
        stackDelete(backup->duplicate - duplicateOffset);
        backup->duplicate = NULL;
        return STACK_INVALID;
    }


    free(stack->data);
    *stack = *backup;
    return STACK_OK;
#else
    return STACK_RESCUE_ERROR;
#endif
}
