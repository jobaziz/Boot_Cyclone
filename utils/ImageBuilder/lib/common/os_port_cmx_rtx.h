/**
 * @file os_port_cmx_rtx.h
 * @brief RTOS abstraction layer (CMX-RTX)
 *
 * @section License
 *
 * Copyright (C) 2010-2024 Oryx Embedded SARL. All rights reserved.
 *
 * This software is provided in source form for a short-term evaluation only. The
 * evaluation license expires 90 days after the date you first download the software.
 *
 * If you plan to use this software in a commercial product, you are required to
 * purchase a commercial license from Oryx Embedded SARL.
 *
 * After the 90-day evaluation period, you agree to either purchase a commercial
 * license or delete all copies of this software. If you wish to extend the
 * evaluation period, you must contact sales@oryx-embedded.com.
 *
 * This evaluation software is provided "as is" without warranty of any kind.
 * Technical support is available as an option during the evaluation period.
 *
 * @author Oryx Embedded SARL (www.oryx-embedded.com)
 * @version 3.0.4
 **/

#ifndef _OS_PORT_CMX_RTX_H
#define _OS_PORT_CMX_RTX_H

//Dependencies
#include "cxfuncs.h"

//Maximum number of semaphores that can be created
#ifndef OS_MAX_SEMAPHORES
   #define OS_MAX_SEMAPHORES 64
#elif (OS_MAX_SEMAPHORES < 1 || OS_MAX_SEMAPHORES > 255)
   #error OS_MAX_SEMAPHORES parameter is not valid
#endif

//Invalid semaphore identifier
#define OS_INVALID_SEMAPHORE_ID 255
//Invalid task identifier
#define OS_INVALID_TASK_ID 0
//Self task identifier
#define OS_SELF_TASK_ID 0

//Task priority (normal)
#ifndef OS_TASK_PRIORITY_NORMAL
   #define OS_TASK_PRIORITY_NORMAL 3
#endif

//Task priority (high)
#ifndef OS_TASK_PRIORITY_HIGH
   #define OS_TASK_PRIORITY_HIGH 2
#endif

//Milliseconds to system ticks
#ifndef OS_MS_TO_SYSTICKS
   #define OS_MS_TO_SYSTICKS(n) (n)
#endif

//System ticks to milliseconds
#ifndef OS_SYSTICKS_TO_MS
   #define OS_SYSTICKS_TO_MS(n) (n)
#endif

//Retrieve 64-bit system time (not implemented)
#ifndef osGetSystemTime64
   #define osGetSystemTime64() osGetSystemTime()
#endif

//Task prologue
#define osEnterTask()
//Task epilogue
#define osExitTask()
//Interrupt service routine prologue
#define osEnterIsr()
//Interrupt service routine epilogue
#define osExitIsr(flag)

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief System time
 **/

typedef uint32_t systime_t;


/**
 * @brief Task identifier
 **/

typedef uint8_t OsTaskId;


/**
 * @brief Task parameters
 **/

typedef struct
{
   CMX_FP fp;
   word32 *stack;
   size_t stackSize;
   uint_t priority;
} OsTaskParameters;


/**
 * @brief Event object
 **/

typedef struct
{
   uint8_t id;
} OsEvent;


/**
 * @brief Semaphore object
 **/

typedef struct
{
   uint8_t id;
} OsSemaphore;


/**
 * @brief Mutex object
 **/

typedef struct
{
   uint8_t id;
} OsMutex;


/**
 * @brief Task routine
 **/

typedef void (*OsTaskCode)(void *arg);


//Default task parameters
extern const OsTaskParameters OS_TASK_DEFAULT_PARAMS;

//Kernel management
void osInitKernel(void);
void osStartKernel(void);

//Task management
OsTaskId osCreateTask(const char_t *name, OsTaskCode taskCode, void *arg,
   const OsTaskParameters *params);

void osDeleteTask(OsTaskId taskId);
void osDelayTask(systime_t delay);
void osSwitchTask(void);
void osSuspendAllTasks(void);
void osResumeAllTasks(void);

//Event management
bool_t osCreateEvent(OsEvent *event);
void osDeleteEvent(OsEvent *event);
void osSetEvent(OsEvent *event);
void osResetEvent(OsEvent *event);
bool_t osWaitForEvent(OsEvent *event, systime_t timeout);
bool_t osSetEventFromIsr(OsEvent *event);

//Semaphore management
bool_t osCreateSemaphore(OsSemaphore *semaphore, uint_t count);
void osDeleteSemaphore(OsSemaphore *semaphore);
bool_t osWaitForSemaphore(OsSemaphore *semaphore, systime_t timeout);
void osReleaseSemaphore(OsSemaphore *semaphore);

//Mutex management
bool_t osCreateMutex(OsMutex *mutex);
void osDeleteMutex(OsMutex *mutex);
void osAcquireMutex(OsMutex *mutex);
void osReleaseMutex(OsMutex *mutex);

//System time
systime_t osGetSystemTime(void);

//Memory management
void *osAllocMem(size_t size);
void osFreeMem(void *p);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
