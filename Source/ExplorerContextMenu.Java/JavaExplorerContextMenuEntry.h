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
#pragma once
#include "jni.h"
#include "ExplorerContextMenuEntry.h"
/***********************************************************************************************************************
 CLASS
***********************************************************************************************************************/
namespace ContextQuickie
{
  class JavaExplorerContextMenuEntry
  {
  private:
    string javaClassName;
    jclass javaClass;
    jobject javaInstace;
    JNIEnv* javaEnvironment;
    jobject CreateInstance();
  public:
    JavaExplorerContextMenuEntry(JNIEnv* environment, bool createInstance = true);
    JavaExplorerContextMenuEntry(JNIEnv* environment, jobject javaInstance);
    JavaExplorerContextMenuEntry(JNIEnv* environment, ExplorerContextMenuEntry* entry);
    void CopyEntries(ExplorerContextMenuEntry& entry);
    void SetText(wstring& value);
    void SetHelpText(wstring& value);
    void SetCommandId(int32_t value);
    void SetImageHandle(uint32_t* value);
    ExplorerContextMenuEntry* GetNativeHandle();
    void SetNativeHandle(ExplorerContextMenuEntry* value);
    void SetSeperator(bool value);
    void AddEntry(JavaExplorerContextMenuEntry& value);
  };
}
/***********************************************************************************************************************
 EOF
***********************************************************************************************************************/
