/***********************************************************************************************************************
 MIT License

 Copyright(c) 2020 Roland Reinl

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files(the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions :

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
***********************************************************************************************************************/

/***********************************************************************************************************************
 INCLUDES
***********************************************************************************************************************/
#include "explorercontextmenu_menu_ProcessMonitor.h"
#include <windows.h>
#include <tlhelp32.h>
#include <string>
#include <vector>

using namespace std;
/***********************************************************************************************************************
 DEFINES
***********************************************************************************************************************/

/***********************************************************************************************************************
 TYPES
***********************************************************************************************************************/
typedef struct ProcessInfoType
{
  DWORD Id;
  wstring Name;

  ProcessInfoType()
  {
    this->Id = 0;
  }
}ProcessInfoType;
/***********************************************************************************************************************
 LOCAL CONSTANTS
***********************************************************************************************************************/

/***********************************************************************************************************************
 LOCAL VARIABLES
***********************************************************************************************************************/

/***********************************************************************************************************************
 LOCAL FUNCTION DECLARATIONS
***********************************************************************************************************************/

/***********************************************************************************************************************
 IMPLEMENTATION
***********************************************************************************************************************/
JNIEXPORT jobjectArray JNICALL Java_explorercontextmenu_menu_ProcessMonitor_getCurrentChildProcesses(JNIEnv* env, jobject instance)
{
  jobjectArray result = nullptr;
  
  DWORD currentProcessId = GetCurrentProcessId();
  
  vector<ProcessInfoType*> processes;

  HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
  if (snapshot != INVALID_HANDLE_VALUE)
  {
    PROCESSENTRY32 processEntry = { 0 };
    processEntry.dwSize = sizeof(PROCESSENTRY32);

    while (Process32Next(snapshot, &processEntry) == TRUE)
    {
      if (processEntry.th32ParentProcessID == currentProcessId)
      {
        ProcessInfoType* processInfo = new ProcessInfoType();
        processInfo->Id = processEntry.th32ProcessID;
        processInfo->Name = processEntry.szExeFile;
        processes.push_back(processInfo);
      }
    }

    jclass javaClass = env->FindClass("explorercontextmenu/menu/ProcessInfo");
    if (javaClass != nullptr)
    {
      jmethodID constructor = env->GetMethodID(javaClass, "<init>", "()V");
      jfieldID processIdField = env->GetFieldID(javaClass, "processId", "J");
      jfieldID processNameField = env->GetFieldID(javaClass, "name", "Ljava/lang/String;");
      if ((constructor != nullptr) && (processIdField != nullptr) && (processNameField != nullptr))
      {
        result = env->NewObjectArray((jsize)processes.size(), javaClass, nullptr);
        for (uint32_t processIndex = 0; processIndex < processes.size(); processIndex++)
        {
          jobject instance = env->NewObject(javaClass, constructor);

          env->SetLongField(instance, processIdField, processes[processIndex]->Id);

          jstring javaString = env->NewString((jchar*)processes[processIndex]->Name.c_str(), (jsize)processes[processIndex]->Name.length());
          env->SetObjectField(instance, processNameField, javaString);

          env->SetObjectArrayElement(result, processIndex, instance);
        }
      }
    }
  }

  return result;
}
/***********************************************************************************************************************
 EOF
***********************************************************************************************************************/