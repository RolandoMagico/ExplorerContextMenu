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
  /// <summary>
  /// A C++ wrapper for accessing the Java context menu entry using JNI.
  /// </summary>
  class JavaExplorerContextMenuEntry
  {
  private:
    /// <summary>
    /// The name of the java class which is wrapped.
    /// </summary>
    static const string javaClassName;

    /// <summary>
    /// The reference to the Java class which is wrapped.
    /// </summary>
    jclass javaClass = nullptr;

    /// <summary>
    /// The reference to the Java instance which is wrapped.
    /// </summary>
    jobject javaInstace = nullptr;

    /// <summary>
    /// The reference to the current Java environment. 
    /// </summary>
    JNIEnv* javaEnvironment = nullptr;

    /// <summary>
    /// Private constructor which only initializes the Java environment. It doesn't create a Java instance.
    /// </summary>
    /// <param name="environment">The reference to the current Java environment.</param>
    JavaExplorerContextMenuEntry(JNIEnv* environment);

    /// <summary>
    /// Invokes a setter method of the java instance which has a byte array as argument and void as return value.
    /// </summary>
    /// <param name="methodName">The name of the Java method.</param>
    /// <param name="value">The value which is passed to the method during the call.</param>
    void InvokeByteArraySetterMethod(const char* methodName, vector<int8_t>& value);
    
    /// <summary>
    /// Invokes a setter method of the java instance which has an integer value as argument and void as return value.
    /// </summary>
    /// <param name="methodName">The name of the Java method.</param>
    /// <param name="value">The value which is passed to the method during the call.</param>
    void InvokeIntSetterMethod(const char* methodName, int32_t value);
    
    /// <summary>
    /// Invokes a setter method of the java instance which has a long value as argument and void as return value.
    /// </summary>
    /// <param name="methodName">The name of the Java method.</param>
    /// <param name="value">The value which is passed to the method during the call.</param>
    void InvokeLongSetterMethod(const char* methodName, int64_t value);
    
    /// <summary>
    /// Invokes a setter method of the java instance which has a string value as argument and void as return value.
    /// </summary>
    /// <param name="methodName">The name of the Java method.</param>
    /// <param name="value">The value which is passed to the method during the call.</param>
    void InvokeStringSetterMethod(const char* methodName, wstring& value);

    /// <summary>
    /// Sets the text in the Java instance to the specific value.
    /// </summary>
    /// <param name="value">The value to set.</param>
    void SetText(wstring& value);
    
    /// <summary>
    /// Sets the help text in the Java instance to the specific value.
    /// </summary>
    /// <param name="value">The value to set.</param>
    void SetHelpText(wstring& value);
    
    /// <summary>
    /// Sets the command ID in the Java instance to the specific value.
    /// </summary>
    /// <param name="value">The value to set.</param>
    void SetCommandId(int32_t value);
    
    /// <summary>
    /// Sets the command string in the Java instance to the specific value.
    /// </summary>
    /// <param name="value">The value to set.</param>
    void SetCommandString(wstring& value);
    
    /// <summary>
    /// Copies the image data from the bitmap handle to the java instance.
    /// </summary>
    /// <param name="entry">The context menu entry from which the data are copied to the Java instance.</param>
    void SetImageHandle(ExplorerContextMenuEntry& entry);
    
    /// <summary>
    /// Sets the separator value in the Java instance to the specific value.
    /// </summary>
    /// <param name="value">The value to set.</param>
    void SetSeperator(bool value);
    
    /// <summary>
    /// Adds the specific entry as child entry of the java instance.
    /// </summary>
    /// <param name="value">The child entry.</param>
    void AddEntry(JavaExplorerContextMenuEntry& value);
  public:
    
    /// <summary>
    /// Constructor for creating a C++ instance which wraps a new Java instance.
    /// The Java instance is created during the constructor call.
    /// </summary>
    /// <param name="environment">The reference to the current Java environment.</param>
    /// <param name="entry">The context menu entry from which the data are copied to the Java instance.</param>
    JavaExplorerContextMenuEntry(JNIEnv* environment, ExplorerContextMenuEntry& entry);

    /// <summary>
    /// Constructor for creating a C++ instnace which wraps an existing Java instnace.
    /// </summary>
    /// <param name="environment">The reference to the current Java environment.</param>
    /// <param name="entry">The context menu entry from which the data are copied to the Java instance.</param>
    /// <param name="javaInstance">The Java instance which will be wrapped.</param>
    JavaExplorerContextMenuEntry(JNIEnv* environment, jobject javaInstance);

    /// <summary>
    /// Copies the entries of an existing context menu to the Java environment.
    /// The required Java instances are created during the the copy operation.
    /// </summary>
    /// <param name="entry">The source entry for the copy operation.</param>
    void CopyEntries(ExplorerContextMenuEntry& entry);
    
    /// <summary>
    /// Gets the address of the native instance from the Java instance.
    /// </summary>
    /// <returns>The entry with the address of natvie instance.</returns>
    ExplorerContextMenuEntry* GetNativeHandle();

    /// <summary>
    /// Sets the address of the natvie instance in the Java instnace.
    /// </summary>
    /// <param name="value">The native instance.</param>
    void SetNativeHandle(ExplorerContextMenuEntry* value);
  };
}
/***********************************************************************************************************************
 EOF
***********************************************************************************************************************/
