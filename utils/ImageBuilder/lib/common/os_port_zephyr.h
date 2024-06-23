/**
 * @file os_port_zephyr.h
 * @brief RTOS abstraction layer (Zephyr)
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

#ifndef _OS_PORT_ZEPHYR_H
#define _OS_PORT_ZEPHYR_H

//Dependencies
#include <zephyr/kernel.h>

//Invalid task identifier
#define OS_INVALID_TASK_ID NULL
//Self task identifier
#define OS_SELF_TASK_ID NULL

//Task priority (normal)
#ifndef OS_TASK_PRIORITY_NORMAL
   #define OS_TASK_PRIORITY_NORMAL (CONFIG_NUM_PREEMPT_PRIORITIES - 1)
#endif

//Task priority (high)
#ifndef OS_TASK_PRIORITY_HIGH
   #define OS_TASK_PRIORITY_HIGH (CONFIG_NUM_PREEMPT_PRIORITIES - 2)
#endif

//Milliseconds to system ticks
#ifndef OS_MS_TO_SYSTICKS
   #define OS_MS_TO_SYSTICKS(n) (n)
#endif

//System ticks to milliseconds
#ifndef OS_SYSTICKS_TO_MS
   #define OS_SYSTICKS_TO_MS(n) (n)
#endif

//Task prologue
#define osEnterTask()
//Task epilogue
#define osExitTask()

//Interrupt service routine prologue
#ifndef osEnterIsr
   #define osEnterIsr()
#endif

//Interrupt service routine epilogue
#ifndef osExitIsr
   #define osExitIsr(flag) (void) flag
#endif

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

typedef k_tid_t OsTaskId;


/**
 * @brief Task parameters
 **/

typedef struct
{
   struct k_thread *tcb;
   k_thread_stack_t *stack;
   size_t stackSize;
   uint_t priority;
} OsTaskParameters;


/**
 * @brief Event object
 **/

typedef struct k_sem OsEvent;


/**
 * @brief Semaphore object
 **/

typedef struct k_sem OsSemaphore;


/**
 * @brief Mutex object
 **/

typedef struct k_mutex OsMutex;


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
uint64_t osGetSystemTime64(void);

//Memory management
void *osAllocMem(size_t size);
void osFreeMem(void *p);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
