// #include "MutexHelper.h"
// #include "StringHelper.h"

// #ifdef _WIN32
//     #include <windows.h>
//     #include <time.h>
//     #include <io.h>
//     #include <direct.h>
// #endif

// namespace aigc
// {

// MutexHelper::MutexHelper(const std::string& name)
// {
//     m_mutex = NULL;
//     if (StringHelper::IsEmpty(name))
//         Creat(name);
// }

// MutexHelper::~MutexHelper()
// {
//     Close();
// }

// bool MutexHelper::Creat(const std::string& name)
// {
//     HANDLE newMutex = NULL;
//     if (StringHelper::IsEmpty(name))
//         newMutex = CreateMutex(NULL, FALSE, (LPCWSTR)NULL);
//     else
//         newMutex = CreateMutex(NULL, FALSE, (LPCWSTR)name.c_str());

//     if (newMutex == NULL)
//         return false;
    
//     m_mutex = newMutex;
//     return true;
// }

// bool MutexHelper::Open(const std::string &name)
// {
//     HANDLE newMutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, (LPCWSTR)name.c_str());
//     if (newMutex == NULL)
//         return false;

//     m_mutex = newMutex;
//     return true;
// }

// bool MutexHelper::Close()
// {
//     if (m_mutex == NULL)
//         return true;
//     CloseHandle(m_mutex);
//     m_mutex == NULL;
//     return true;
// }

// bool MutexHelper::Lock(int msecond)
// {
//     if (m_mutex == NULL)
//         return false;
//     DWORD ret = WaitForSingleObject(m_mutex, msecond <= 0 ? INFINITE : msecond);
//     if (ret == WAIT_TIMEOUT)
//         return false;
//     return true;
// }   

// bool MutexHelper::Release()
// {
//     if (m_mutex == NULL)
//         return false;
//     ReleaseMutex(m_mutex);
//     return true;
// }

// }