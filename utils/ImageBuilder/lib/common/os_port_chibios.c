/**
 * @file os_port_chibios.c
 * @brief RTOS abstraction layer (ChibiOS/RT)
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

//Switch to the appropriate trace level
#define TRACE_LEVEL TRACE_LEVEL_OFF

//Dependencies
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "os_port.h"
#include "os_port_chibios.h"
#include "debug.h"

//Default task parameters
const OsTaskParameters OS_TASK_DEFAULT_PARAMS =
{
   NULL,       //Stack
   256,        //Size of the stack
   NORMALPRIO  //Task priority
};


/**
 * @brief Kernel initialization
 **/

void osInitKernel(void)
{
   //Kernel initialization
   chSysInit();
}


/**
 * @brief Start kernel
 **/

void osStartKernel(void)
{
   //Terminate the main thread
   chThdExit(MSG_OK);
}


/**
 * @brief Create a task
 * @param[in] name NULL-terminated string identifying the task
 * @param[in] taskCode Pointer to the task entry function
 * @param[in] arg Argument passed to the task function
 * @param[in] params Task parameters
 * @return Task identifier referencing the newly created task
 **/

OsTaskId osCreateTask(const char_t *name, OsTaskCode taskCode, void *arg,
   const OsTaskParameters *params)
{
   thread_t *handle;

   //Static allocation?
   if(params->stack != NULL)
   {
      //Create a new task
      handle = chThdCreateStatic(params->stack,
         params->stackSize * sizeof(uint32_t), params->priority,
         (tfunc_t) taskCode, arg);
   }
   else
   {
      //Create a new task
      handle = chThdCreateFromHeap(0, THD_WORKING_AREA_SIZE(params->stackSize),
         params->priority, taskCode, arg);
   }

   //Return the handle referencing the newly created thread
   return (OsTaskId) handle;
}


/**
 * @brief Delete a task
 * @param[in] taskId Task identifier referencing the task to be deleted
 **/

void osDeleteTask(OsTaskId taskId)
{
   //Delete the specified task
   if(taskId == OS_SELF_TASK_ID)
   {
      chThdExit(MSG_OK);
   }
   else
   {
      chThdTerminate((thread_t *) taskId);
   }
}


/**
 * @brief Delay routine
 * @param[in] delay Amount of time for which the calling task should block
 **/

void osDelayTask(systime_t delay)
{
   //Delay the task for the specified duration
   chThdSleep(OS_MS_TO_SYSTICKS(delay));
}


/**
 * @brief Yield control to the next task
 **/

void osSwitchTask(void)
{
   //Force a context switch
   chThdYield();
}


/**
 * @brief Suspend scheduler activity
 **/

void osSuspendAllTasks(void)
{
   //Suspend scheduler activity
   chSysLock();
}


/**
 * @brief Resume scheduler activity
 **/

void osResumeAllTasks(void)
{
   //Resume scheduler activity
   chSysUnlock();
}


/**
 * @brief Create an event object
 * @param[in] event Pointer to the event object
 * @return The function returns TRUE if the event object was successfully
 *   created. Otherwise, FALSE is returned
 **/

bool_t osCreateEvent(OsEvent *event)
{
   //Initialize the binary semaphore object
   chBSemObjectInit(event, TRUE);

   //Event successfully created
   return TRUE;
}


/**
 * @brief Delete an event object
 * @param[in] event Pointer to the event object
 **/

void osDeleteEvent(OsEvent *event)
{
   //No resource to release
}


/**
 * @brief Set the specified event object to the signaled state
 * @param[in] event Pointer to the event object
 **/

void osSetEvent(OsEvent *event)
{
   //Set the specified event to the signaled state
   chBSemSignal(event);
}


/**
 * @brief Set the specified event object to the nonsignaled state
 * @param[in] event Pointer to the event object
 **/

void osResetEvent(OsEvent *event)
{
   //Force the specified event to the nonsignaled state
   chBSemReset(event, TRUE);
}


/**
 * @brief Wait until the specified event is in the signaled state
 * @param[in] event Pointer to the event object
 * @param[in] timeout Timeout interval
 * @return The function returns TRUE if the state of the specified object is
 *   signaled. FALSE is returned if the timeout interval elapsed
 **/

bool_t osWaitForEvent(OsEvent *event, systime_t timeout)
{
   msg_t msg;

   //Wait until the specified event is in the signaled state or the timeout
   //interval elapses
   if(timeout == 0)
   {
      //Non-blocking call
      msg = chBSemWaitTimeout(event, TIME_IMMEDIATE);
   }
   else if(timeout == INFINITE_DELAY)
   {
      //Infinite timeout period
      msg = chBSemWaitTimeout(event, TIME_INFINITE);
   }
   else
   {
      //Wait until the specified event becomes set
      msg = chBSemWaitTimeout(event, OS_MS_TO_SYSTICKS(timeout));
   }

   //Check whether the specified event is set
   if(msg == MSG_OK)
      return TRUE;
   else
      return FALSE;
}


/**
 * @brief Set an event object to the signaled state from an interrupt service routine
 * @param[in] event Pointer to the event object
 * @return TRUE if setting the event to signaled state caused a task to unblock
 *   and the unblocked task has a priority higher than the currently running task
 **/

bool_t osSetEventFromIsr(OsEvent *event)
{
   //Set the specified event to the signaled state
   chBSemSignalI(event);

   //The return value is not relevant
   return FALSE;
}


/**
 * @brief Create a semaphore object
 * @param[in] semaphore Pointer to the semaphore object
 * @param[in] count The maximum count for the semaphore object. This value
 *   must be greater than zero
 * @return The function returns TRUE if the semaphore was successfully
 *   created. Otherwise, FALSE is returned
 **/

bool_t osCreateSemaphore(OsSemaphore *semaphore, uint_t count)
{
   //Initialize the semaphore object
   chSemObjectInit(semaphore, count);

   //Semaphore successfully created
   return TRUE;
}


/**
 * @brief Delete a semaphore object
 * @param[in] semaphore Pointer to the semaphore object
 **/

void osDeleteSemaphore(OsSemaphore *semaphore)
{
   //No resource to release
}


/**
 * @brief Wait for the specified semaphore to be available
 * @param[in] semaphore Pointer to the semaphore object
 * @param[in] timeout Timeout interval
 * @return The function returns TRUE if the semaphore is available. FALSE is
 *   returned if the timeout interval elapsed
 **/

bool_t osWaitForSemaphore(OsSemaphore *semaphore, systime_t timeout)
{
   msg_t msg;

   //Wait until the semaphore is available or the timeout interval elapses
   if(timeout == 0)
   {
      //Non-blocking call
      msg = chSemWaitTimeout(semaphore, TIME_IMMEDIATE);
   }
   else if(timeout == INFINITE_DELAY)
   {
      //Infinite timeout period
      msg = chSemWaitTimeout(semaphore, TIME_INFINITE);
   }
   else
   {
      //Wait until the specified semaphore becomes available
      msg = chSemWaitTimeout(semaphore, OS_MS_TO_SYSTICKS(timeout));
   }

   //Check whether the specified semaphore is available
   if(msg == MSG_OK)
      return TRUE;
   else
      return FALSE;
}


/**
 * @brief Release the specified semaphore object
 * @param[in] semaphore Pointer to the semaphore object
 **/

void osReleaseSemaphore(OsSemaphore *semaphore)
{
   //Release the semaphore
   chSemSignal(semaphore);
}


/**
 * @brief Create a mutex object
 * @param[in] mutex Pointer to the mutex object
 * @return The function returns TRUE if the mutex was successfully
 *   created. Otherwise, FALSE is returned
 **/

bool_t osCreateMutex(OsMutex *mutex)
{
   //Initialize the mutex object
   chMtxObjectInit(mutex);

   //Mutex successfully created
   return TRUE;
}


/**
 * @brief Delete a mutex object
 * @param[in] mutex Pointer to the mutex object
 **/

void osDeleteMutex(OsMutex *mutex)
{
   //No resource to release
}


/**
 * @brief Acquire ownership of the specified mutex object
 * @param[in] mutex Pointer to the mutex object
 **/

void osAcquireMutex(OsMutex *mutex)
{
   //Obtain ownership of the mutex object
   chMtxLock(mutex);
}


/**
 * @brief Release ownership of the specified mutex object
 * @param[in] mutex Pointer to the mutex object
 **/

void osReleaseMutex(OsMutex *mutex)
{
   //Release ownership of the mutex object
#if (CH_KERNEL_MAJOR < 3)
   chMtxUnlock();
#else
   chMtxUnlock(mutex);
#endif
}


/**
 * @brief Retrieve system time
 * @return Number of milliseconds elapsed since the system was last started
 **/

systime_t osGetSystemTime(void)
{
   systime_t time;

   //Get current tick count
   time = chVTGetSystemTime();

   //Convert system ticks to milliseconds
   return OS_SYSTICKS_TO_MS(time);
}


/**
 * @brief Allocate a memory block
 * @param[in] size Bytes to allocate
 * @return A pointer to the allocated memory block or NULL if
 *   there is insufficient memory available
 **/

__weak_func void *osAllocMem(size_t size)
{
   void *p;

   //Allocate a memory block
   p = chHeapAlloc(NULL, size);

   //Debug message
   TRACE_DEBUG("Allocating %" PRIuSIZE " bytes at 0x%08" PRIXPTR "\r\n",
      size, (uintptr_t) p);

   //Return a pointer to the newly allocated memory block
   return p;
}


/**
 * @brief Release a previously allocated memory block
 * @param[in] p Previously allocated memory block to be freed
 **/

__weak_func void osFreeMem(void *p)
{
   //Make sure the pointer is valid
   if(p != NULL)
   {
      //Debug message
      TRACE_DEBUG("Freeing memory at 0x%08" PRIXPTR "\r\n", (uintptr_t) p);

      //Free memory block
      chHeapFree(p);
   }
}


/**
 * @brief Idle loop hook
 **/

void osIdleLoopHook(void)
{
   //Not implemented
}
