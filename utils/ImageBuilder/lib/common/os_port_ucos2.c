/**
 * @file os_port_ucos2.c
 * @brief RTOS abstraction layer (Micrium uC/OS-II)
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
#include "os_port_ucos2.h"
#include "debug.h"

//Default task parameters
const OsTaskParameters OS_TASK_DEFAULT_PARAMS =
{
   NULL, //Stack
   0,    //Size of the stack
   0     //Task priority
};


/**
 * @brief Kernel initialization
 **/

void osInitKernel(void)
{
   //Scheduler initialization
   OSInit();
}


/**
 * @brief Start kernel
 **/

void osStartKernel(void)
{
   //Start the scheduler
   OSStart();
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
   INT8U err;
   OS_STK *stackTop;
   OsTaskId taskId;

   //Check parameters
   if(params->stack != NULL)
   {
      //Top of the stack
      stackTop = (OS_STK *) params->stack + (params->stackSize - 1);

      //Create a new task
      err = OSTaskCreateExt(taskCode, arg, stackTop, params->priority,
         params->priority, params->stack, params->stackSize, NULL,
         OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);

      //Check whether the task was successfully created
      if(err == OS_ERR_NONE)
      {
         taskId = (OsTaskId) params->priority;
      }
      else
      {
         taskId = OS_INVALID_TASK_ID;
      }
   }
   else
   {
      //Invalid parameters
      taskId = OS_INVALID_TASK_ID;
   }

   //Return the handle referencing the newly created task
   return taskId;
}


/**
 * @brief Delete a task
 * @param[in] taskId Task identifier referencing the task to be deleted
 **/

void osDeleteTask(OsTaskId taskId)
{
   //Delete the specified task
   OSTaskDel(taskId);
}


/**
 * @brief Delay routine
 * @param[in] delay Amount of time for which the calling task should block
 **/

void osDelayTask(systime_t delay)
{
   INT16U n;

   //Convert milliseconds to system ticks
   delay = OS_MS_TO_SYSTICKS(delay);

   //Delay the task for the specified duration
   while(delay > 0)
   {
      //The maximum delay is 65535 clock ticks
      n = MIN(delay, 65535);
      //Wait for the specified amount of time
      OSTimeDly(n);
      //Decrement delay value
      delay -= n;
   }
}


/**
 * @brief Yield control to the next task
 **/

void osSwitchTask(void)
{
   //Not implemented
}


/**
 * @brief Suspend scheduler activity
 **/

void osSuspendAllTasks(void)
{
   //Make sure the operating system is running
   if(OSRunning == OS_TRUE)
   {
      //Suspend scheduler activity
      OSSchedLock();
   }
}


/**
 * @brief Resume scheduler activity
 **/

void osResumeAllTasks(void)
{
   //Make sure the operating system is running
   if(OSRunning == OS_TRUE)
   {
      //Resume scheduler activity
      OSSchedUnlock();
   }
}


/**
 * @brief Create an event object
 * @param[in] event Pointer to the event object
 * @return The function returns TRUE if the event object was successfully
 *   created. Otherwise, FALSE is returned
 **/

bool_t osCreateEvent(OsEvent *event)
{
   INT8U err;

   //Create an event flag group
   event->p = OSFlagCreate(0, &err);

   //Check whether the event flag group was successfully created
   if(event->p != NULL && err == OS_ERR_NONE)
   {
      return TRUE;
   }
   else
   {
      return FALSE;
   }
}


/**
 * @brief Delete an event object
 * @param[in] event Pointer to the event object
 **/

void osDeleteEvent(OsEvent *event)
{
   INT8U err;

   //Make sure the operating system is running
   if(OSRunning == OS_TRUE)
   {
      //Properly dispose the event object
      OSFlagDel(event->p, OS_DEL_ALWAYS, &err);
   }
}


/**
 * @brief Set the specified event object to the signaled state
 * @param[in] event Pointer to the event object
 **/

void osSetEvent(OsEvent *event)
{
   INT8U err;

   //Set the specified event to the signaled state
   OSFlagPost(event->p, 1, OS_FLAG_SET, &err);
}


/**
 * @brief Set the specified event object to the nonsignaled state
 * @param[in] event Pointer to the event object
 **/

void osResetEvent(OsEvent *event)
{
   INT8U err;

   //Force the specified event to the nonsignaled state
   OSFlagPost(event->p, 1, OS_FLAG_CLR, &err);
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
   INT8U err;
   INT16U n;

   //Wait until the specified event is in the signaled state or the timeout
   //interval elapses
   if(timeout == 0)
   {
      //Non-blocking call
      OSFlagAccept(event->p, 1, OS_FLAG_WAIT_SET_ANY | OS_FLAG_CONSUME, &err);
   }
   else if(timeout == INFINITE_DELAY)
   {
      //Infinite timeout period
      OSFlagPend(event->p, 1, OS_FLAG_WAIT_SET_ANY | OS_FLAG_CONSUME, 0, &err);
   }
   else
   {
      //Convert milliseconds to system ticks
      timeout = OS_MS_TO_SYSTICKS(timeout);

      //Loop until the assigned time period has elapsed
      do
      {
         //The maximum timeout is 65535 clock ticks
         n = MIN(timeout, 65535);
         //Wait for the specified time interval
         OSFlagPend(event->p, 1, OS_FLAG_WAIT_SET_ANY | OS_FLAG_CONSUME, n, &err);
         //Decrement timeout value
         timeout -= n;

         //Check timeout value
      } while(err == OS_ERR_TIMEOUT && timeout > 0);
   }

   //Check whether the specified event is set
   if(err == OS_ERR_NONE)
   {
      return TRUE;
   }
   else
   {
      return FALSE;
   }
}


/**
 * @brief Set an event object to the signaled state from an interrupt service routine
 * @param[in] event Pointer to the event object
 * @return TRUE if setting the event to signaled state caused a task to unblock
 *   and the unblocked task has a priority higher than the currently running task
 **/

bool_t osSetEventFromIsr(OsEvent *event)
{
   INT8U err;

   //Set the specified event to the signaled state
   OSFlagPost(event->p, 1, OS_FLAG_SET, &err);

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
   //Create a semaphore
   semaphore->p = OSSemCreate(count);

   //Check whether the semaphore was successfully created
   if(semaphore->p != NULL)
   {
      return TRUE;
   }
   else
   {
      return FALSE;
   }
}


/**
 * @brief Delete a semaphore object
 * @param[in] semaphore Pointer to the semaphore object
 **/

void osDeleteSemaphore(OsSemaphore *semaphore)
{
   INT8U err;

   //Make sure the operating system is running
   if(OSRunning == OS_TRUE)
   {
      //Properly dispose the specified semaphore
      OSSemDel(semaphore->p, OS_DEL_ALWAYS, &err);
   }
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
   INT8U err;
   INT16U n;

   //Wait until the semaphore is available or the timeout interval elapses
   if(timeout == 0)
   {
      //Non-blocking call
      if(OSSemAccept(semaphore->p) > 0)
      {
         err = OS_ERR_NONE;
      }
      else
      {
         err = OS_ERR_TIMEOUT;
      }
   }
   else if(timeout == INFINITE_DELAY)
   {
      //Infinite timeout period
      OSSemPend(semaphore->p, 0, &err);
   }
   else
   {
      //Convert milliseconds to system ticks
      timeout = OS_MS_TO_SYSTICKS(timeout);

      //Loop until the assigned time period has elapsed
      do
      {
         //The maximum timeout is 65535 clock ticks
         n = MIN(timeout, 65535);
         //Wait for the specified time interval
         OSSemPend(semaphore->p, n, &err);
         //Decrement timeout value
         timeout -= n;

         //Check timeout value
      } while(err == OS_ERR_TIMEOUT && timeout > 0);
   }

   //Check whether the specified semaphore is available
   if(err == OS_ERR_NONE)
   {
      return TRUE;
   }
   else
   {
      return FALSE;
   }
}


/**
 * @brief Release the specified semaphore object
 * @param[in] semaphore Pointer to the semaphore object
 **/

void osReleaseSemaphore(OsSemaphore *semaphore)
{
   //Release the semaphore
   OSSemPost(semaphore->p);
}


/**
 * @brief Create a mutex object
 * @param[in] mutex Pointer to the mutex object
 * @return The function returns TRUE if the mutex was successfully
 *   created. Otherwise, FALSE is returned
 **/

bool_t osCreateMutex(OsMutex *mutex)
{
   INT8U err;

   //Create a mutex
   mutex->p = OSMutexCreate(OS_PRIO_MUTEX_CEIL_DIS, &err);

   //Check whether the mutex was successfully created
   if(mutex->p != NULL && err == OS_ERR_NONE)
   {
      return TRUE;
   }
   else
   {
      return FALSE;
   }
}


/**
 * @brief Delete a mutex object
 * @param[in] mutex Pointer to the mutex object
 **/

void osDeleteMutex(OsMutex *mutex)
{
   INT8U err;

   //Make sure the operating system is running
   if(OSRunning == OS_TRUE)
   {
      //Properly dispose the specified mutex
      OSMutexDel(mutex->p, OS_DEL_ALWAYS, &err);
   }
}


/**
 * @brief Acquire ownership of the specified mutex object
 * @param[in] mutex Pointer to the mutex object
 **/

void osAcquireMutex(OsMutex *mutex)
{
   INT8U err;

   //Obtain ownership of the mutex object
   OSMutexPend(mutex->p, 0, &err);
}


/**
 * @brief Release ownership of the specified mutex object
 * @param[in] mutex Pointer to the mutex object
 **/

void osReleaseMutex(OsMutex *mutex)
{
   //Release ownership of the mutex object
   OSMutexPost(mutex->p);
}


/**
 * @brief Retrieve system time
 * @return Number of milliseconds elapsed since the system was last started
 **/

systime_t osGetSystemTime(void)
{
   systime_t time;

   //Get current tick count
   time = OSTimeGet();

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

   //Enter critical section
   osSuspendAllTasks();
   //Allocate a memory block
   p = malloc(size);
   //Leave critical section
   osResumeAllTasks();

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

      //Enter critical section
      osSuspendAllTasks();
      //Free memory block
      free(p);
      //Leave critical section
      osResumeAllTasks();
   }
}
