/******************************************************************************
Module:  EnsureCleanup.h
Notices: Copyright (c) 2007 Jeffrey Richter & Christophe Nasarre
Purpose: These classes ensure object cleanup when an object goes out of scope.
         See Appendix A.
******************************************************************************/


#pragma once   // Include this header file once per compilation unit


///////////////////////////////////////////////////////////////////////////////


#define WIN32_LEAN_AND_MEAN
#include <Windows.h>


///////////////////////////////////////////////////////////////////////////////


// Data type representing the address of the object's cleanup function.
// I used UINT_PTR so that this class works properly in 64-bit Windows.
typedef VOID (WINAPI* PFNENSURECLEANUP)(UINT_PTR);


///////////////////////////////////////////////////////////////////////////////


// Each template instantiation requires a data type, address of cleanup 
// function, and a value that indicates an invalid value.
template<class TYPE, PFNENSURECLEANUP pfn, UINT_PTR tInvalid = NULL> 
class CEnsureCleanup {
public:
   // Default constructor assumes an invalid value (nothing to cleanup)
   CEnsureCleanup() { m_t = tInvalid; }

   // This constructor sets the value to the specified value
   CEnsureCleanup(TYPE t) : m_t((UINT_PTR) t) { }

   // The destructor performs the cleanup.
   ~CEnsureCleanup() { Cleanup(); }

   // Helper methods to tell if the value represents a valid object or not..
   BOOL IsValid() { return(m_t != tInvalid); }
   BOOL IsInvalid() { return(!IsValid()); }

   // Re-assigning the object forces the current object to be cleaned-up.
   TYPE operator=(TYPE t) { 
      Cleanup(); 
      m_t = (UINT_PTR) t;
      return(*this);  
   }

   // Returns the value (supports both 32-bit and 64-bit Windows).
   operator TYPE() { 
      return (TYPE) m_t;
   }

   // Cleanup the object if the value represents a valid object
   void Cleanup() { 
      if (IsValid()) {
         // In 64-bit Windows, all parameters are 64-bits, 
         // so no casting is required
         pfn(m_t);         // Close the object.
         m_t = tInvalid;   // We no longer represent a valid object.
      }
   }
   
private:
   UINT_PTR m_t;           // The member representing the object
};


///////////////////////////////////////////////////////////////////////////////


// Macros to make it easier to declare instances of the template 
// class for specific data types.

#define MakeCleanupClass(className, tData, pfnCleanup) \
   typedef CEnsureCleanup<tData, (PFNENSURECLEANUP) pfnCleanup> className;

#define MakeCleanupClassX(className, tData, pfnCleanup, tInvalid) \
   typedef CEnsureCleanup<tData, (PFNENSURECLEANUP) pfnCleanup, \
   (INT_PTR) tInvalid> className;


///////////////////////////////////////////////////////////////////////////////


// Instances of the template C++ class for common data types.
MakeCleanupClass(CEnsureCloseHandle,        HANDLE,    CloseHandle);
MakeCleanupClassX(CEnsureCloseFile,         HANDLE,    CloseHandle, 
   INVALID_HANDLE_VALUE);
MakeCleanupClassX(CEnsureCloseFileFind,		HANDLE,    FindClose, 
   INVALID_HANDLE_VALUE);
MakeCleanupClass(CEnsureLocalFree,          HLOCAL,    LocalFree);
MakeCleanupClass(CEnsureGlobalFree,         HGLOBAL,   GlobalFree);
MakeCleanupClass(CEnsureRegCloseKey,        HKEY,      RegCloseKey);
MakeCleanupClass(CEnsureCloseServiceHandle, SC_HANDLE, CloseServiceHandle);
MakeCleanupClass(CEnsureCloseWindowStation, HWINSTA,   CloseWindowStation);
MakeCleanupClass(CEnsureCloseDesktop,       HDESK,     CloseDesktop);
MakeCleanupClass(CEnsureUnmapViewOfFile,    PVOID,     UnmapViewOfFile);
MakeCleanupClass(CEnsureFreeLibrary,        HMODULE,   FreeLibrary);
#ifdef	_WINSOCK2API_
MakeCleanupClassX(CEnsureCloseSocket,		SOCKET,    closesocket, 
   INVALID_SOCKET);
#endif	// _WINSOCK2API_

#ifdef	_INC_WINSNMP
MakeCleanupClassX(CEnsureCloseSnmpSession, HSNMP_SESSION, SnmpClose, SNMPAPI_FAILURE);
MakeCleanupClassX(CEnsureCloseSnmpEntity, HSNMP_ENTITY, SnmpFreeEntity, SNMPAPI_FAILURE);
MakeCleanupClassX(CEnsureCloseSnmpContext, HSNMP_CONTEXT, SnmpFreeContext, SNMPAPI_FAILURE);
MakeCleanupClassX(CEnsureCloseSnmpVbl, HSNMP_VBL, SnmpFreeVbl, SNMPAPI_FAILURE);
MakeCleanupClassX(CEnsureCloseSnmpPdu, HSNMP_PDU, SnmpFreePdu, SNMPAPI_FAILURE);
#endif	// _INC_WINSNMP
///////////////////////////////////////////////////////////////////////////////

// Special class for freeing a block from a heap
// Special class is required because SnmpFreeDescriptor requires 2 parameters
#ifdef	_INC_WINSNMP
class CEnsureSnmpFreeDescriptor {
public:
	CEnsureSnmpFreeDescriptor(smiUINT32 syntax=SNMP_SYNTAX_NULL, smiLPOPAQUE descriptor=NULL) 
		: m_syntax(syntax), m_descriptor(descriptor) { }
	~CEnsureSnmpFreeDescriptor() { Cleanup(); }

	smiLPOPAQUE operator=(smiLPOPAQUE pv) { 
		Cleanup(); 
		m_descriptor = pv; 
		return(m_descriptor); 
	}
	operator smiLPOPAQUE() { return(m_descriptor); }
	void Cleanup() { 
		if (m_syntax != SNMP_SYNTAX_NULL) { 
			SnmpFreeDescriptor(m_syntax, m_descriptor);
			m_syntax = SNMP_SYNTAX_NULL; 
		} 
	}

private:
	smiUINT32	m_syntax;
	smiLPOPAQUE	m_descriptor;
};
#endif	// _INC_WINSNMP

///////////////////////////////////////////////////////////////////////////////


// Special class for releasing a reserved region.
// Special class is required because VirtualFree requires 3 parameters
class CEnsureReleaseRegion {
public:
   CEnsureReleaseRegion(PVOID pv = NULL) : m_pv(pv) { }
   ~CEnsureReleaseRegion() { Cleanup(); }

   PVOID operator=(PVOID pv) { 
      Cleanup(); 
      m_pv = pv; 
      return(m_pv); 
   }
   operator PVOID() { return(m_pv); }
   void Cleanup() { 
      if (m_pv != NULL) { 
         VirtualFree(m_pv, 0, MEM_RELEASE); 
         m_pv = NULL; 
      } 
   }
   
private:
   PVOID m_pv;
};


///////////////////////////////////////////////////////////////////////////////


// Special class for freeing a block from a heap
// Special class is required because HeapFree requires 3 parameters
class CEnsureHeapFree {
public:
   CEnsureHeapFree(PVOID pv = NULL, HANDLE hHeap = GetProcessHeap()) 
      : m_pv(pv), m_hHeap(hHeap) { }
   ~CEnsureHeapFree() { Cleanup(); }

   PVOID operator=(PVOID pv) { 
      Cleanup(); 
      m_pv = pv; 
      return(m_pv); 
   }
   operator PVOID() { return(m_pv); }
   void Cleanup() { 
      if (m_pv != NULL) { 
         HeapFree(m_hHeap, 0, m_pv); 
         m_pv = NULL; 
      } 
   }
   
private:
   HANDLE m_hHeap;
   PVOID m_pv;
};


///////////////////////////////// End of File /////////////////////////////////
