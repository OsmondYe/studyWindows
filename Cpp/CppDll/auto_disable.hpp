#pragma once

#include <windows.h>
#include <cassert>
#include <map>
#include <boost\utility.hpp>

class recursion_control : boost::noncopyable
{
public:

	recursion_control(void) :
		disabled_process(false),
		disabled_thread()
	{
		if (InitializeCriticalSectionAndSpinCount(&cs, 0x80004000) == FALSE)
		{
			InitializeCriticalSection(&cs);
		}
	}/* recursion_control */

	~recursion_control()
	{
		DeleteCriticalSection(&cs);
	}
	
	void thread_enable(void)
	{
		EnterCriticalSection(&cs);
		assert(disabled_thread[GetCurrentThreadId()] > 0);
		disabled_thread[GetCurrentThreadId()]--;
		LeaveCriticalSection(&cs);
	}/* thread_enable */
	 
	void thread_disable(void)
	{
		DWORD tid = GetCurrentThreadId();
		EnterCriticalSection(&cs);
		/* Initialize thread state to zero when this is the first state change. */
		if (disabled_thread.find(tid) == disabled_thread.end())
		{
			disabled_thread[tid] = 0;
		}
		disabled_thread[tid]++;
		LeaveCriticalSection(&cs);
	}/* thread_disable */

	
	void process_enable(void)
	{
		EnterCriticalSection(&cs);
		disabled_process = false;
		LeaveCriticalSection(&cs);
	}/* process_enable */
	 
	void process_disable(void)
	{
		EnterCriticalSection(&cs);
		disabled_process = true;
		LeaveCriticalSection(&cs);
	}/* process_disable */
	 
	bool is_process_disabled(void)
	{
		bool result;
		EnterCriticalSection(&cs);
		result = disabled_process;
		LeaveCriticalSection(&cs);
		return result;
	}/* is_process_disabled */
	 
	bool is_thread_disabled(void)
	{
		bool result = false;
		DWORD tid = GetCurrentThreadId();
		EnterCriticalSection(&cs);
		/* If the current thread ID is not in the map it's not been disabled */
		if (disabled_thread.find(tid) != disabled_thread.end())
		{
			if (disabled_thread[tid] > 0)
			{
				result = true;
			}
		}
		LeaveCriticalSection(&cs);
		return result;
	}/* is_thread_disabled */
	
	bool is_disabled(void)
	{
		bool result;
		EnterCriticalSection(&cs);
		result = is_thread_disabled() || is_process_disabled();
		LeaveCriticalSection(&cs);
		return result;
	}/* is_disabled */

private:

	std::map<DWORD, int>  disabled_thread;   /* Thread state is disabled? */
	bool                 disabled_process;  /* Process state is disabled? */
	CRITICAL_SECTION     cs;                /* Protect state variables */

};/* auto_control */

  /** recursion_control_auto
  *
  *  \brief Handle thread (current context) disablement automatically.  When an
  *         instance of this object is created it will disable recusion for the
  *         current thread until is is destroyed.  Typically this occurs when the
  *         stack is being unwound since an instance of this object would typically
  *         be local to the context.
  */
class recursion_control_auto : boost::noncopyable
{
public:

	recursion_control_auto(recursion_control& in_ac) : ac(in_ac)
	{
		ac.thread_disable();
	}

	~recursion_control_auto(void)
	{
		ac.thread_enable();
	}

private:
	recursion_control& ac;

};/* recursion_control_auto */