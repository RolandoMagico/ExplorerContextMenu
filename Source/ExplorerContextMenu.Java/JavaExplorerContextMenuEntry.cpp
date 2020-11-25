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
#include "JavaExplorerContextMenuEntry.h"

using namespace ContextQuickie;
/***********************************************************************************************************************
 DEFINES
***********************************************************************************************************************/

/***********************************************************************************************************************
 TYPES
***********************************************************************************************************************/

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
JavaExplorerContextMenuEntry::JavaExplorerContextMenuEntry(JNIEnv* environment, bool createInstance)
{
  this->javaClassName = string("explorercontextmenu/menu/ExplorerContextMenuEntry");
  this->javaEnvironment = environment;
  this->javaClass = this->javaEnvironment->FindClass(this->javaClassName.c_str());
  if (createInstance == true)
  {
    this->javaInstace = this->CreateInstance();
  }
  else
  {
    this->javaInstace = NULL;
  }
}

JavaExplorerContextMenuEntry::JavaExplorerContextMenuEntry(JNIEnv* environment, jobject javaInstance)
  : JavaExplorerContextMenuEntry(environment, false)
{
  this->javaInstace = javaInstance;
}

JavaExplorerContextMenuEntry::JavaExplorerContextMenuEntry(JNIEnv* environment, ExplorerContextMenuEntry* entry)
  : JavaExplorerContextMenuEntry(environment)
{
  if (entry->Text != nullptr)
  {
    this->SetText(*(entry->Text));
  }

  if (entry->HelpText != nullptr)
  {
    this->SetHelpText(*(entry->HelpText));
  }

  if (entry->BitmapHandle != nullptr)
  {
    this->SetImageHandle(entry->BitmapHandle);
  }

  if (entry->CommandString != nullptr)
  {
    this->SetCommandString(*(entry->CommandString));
  }

  this->SetNativeHandle(entry);
  this->SetCommandId(entry->CommandId);
  this->SetSeperator(entry->IsSeparator);
}

void JavaExplorerContextMenuEntry::CopyEntries(ExplorerContextMenuEntry& entry)
{
  for (size_t index = 0; index < entry.menuEntries.size(); index++)
  {
    JavaExplorerContextMenuEntry wrapper(this->javaEnvironment, entry.menuEntries[index]);
    this->AddEntry(wrapper);
    wrapper.CopyEntries(*(entry.menuEntries[index]));
  }
}

jobject JavaExplorerContextMenuEntry::CreateInstance()
{
  jmethodID constructor = this->javaEnvironment->GetMethodID(this->javaClass, "<init>", "()V");
  return this->javaEnvironment->NewObject(this->javaClass, constructor);
}

void JavaExplorerContextMenuEntry::SetText(wstring& value)
{
  this->InvokeStringSetterMethod("setText", value);
}

void JavaExplorerContextMenuEntry::SetHelpText(wstring& value)
{
  this->InvokeStringSetterMethod("setHelpText", value);
}

void JavaExplorerContextMenuEntry::SetCommandId(int32_t value)
{
  jmethodID setterMethod = this->javaEnvironment->GetMethodID(this->javaClass, "setCommandId", "(I)V");
  if (setterMethod != nullptr)
  {
    this->javaEnvironment->CallVoidMethod(this->javaInstace, setterMethod, value);
  }
}

void JavaExplorerContextMenuEntry::SetCommandString(wstring& value)
{
  this->InvokeStringSetterMethod("setCommandString", value);
}

void JavaExplorerContextMenuEntry::SetImageHandle(HBITMAP value)
{
  HDC deviceContextHandle;
  BITMAP bitmap = { 0 };
  if ((deviceContextHandle = GetDC(HWND_DESKTOP)) == NULL)
  {
    // GetDC failed
  }
  else if (GetObject(value, sizeof(bitmap), &bitmap) == 0)
  {
    // GetObject failed
  }
  else
  {
    BITMAPINFO bitmapInfo = { 0 };
    bitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bitmapInfo.bmiHeader.biWidth = bitmap.bmWidth;
    bitmapInfo.bmiHeader.biHeight = bitmap.bmHeight * -1;
    bitmapInfo.bmiHeader.biPlanes = bitmap.bmPlanes;
    bitmapInfo.bmiHeader.biBitCount = bitmap.bmBitsPixel;

    size_t arraySize = (size_t)bitmap.bmWidth * bitmap.bmHeight * 4;
    vector<int8_t> pixels(arraySize);
    if (GetDIBits(deviceContextHandle, value, 0, bitmap.bmHeight, &pixels[0], &bitmapInfo, DIB_RGB_COLORS) != 0)
    {
      this->InvokeIntSetterMethod("setImageDepth", bitmap.bmBitsPixel);
      this->InvokeIntSetterMethod("setImageHeigth", bitmap.bmHeight);
      this->InvokeIntSetterMethod("setImageWidth", bitmap.bmWidth);
      this->InvokeByteArraySetterMethod("setImageData", pixels);
    }
  }

  if (deviceContextHandle != NULL)
  {
    ReleaseDC(HWND_DESKTOP, deviceContextHandle);
  }
}

ExplorerContextMenuEntry* JavaExplorerContextMenuEntry::GetNativeHandle()
{
  ExplorerContextMenuEntry* result = nullptr;
  jmethodID getterMethod = this->javaEnvironment->GetMethodID(this->javaClass, "getNativeHandle", "()J");
  if (getterMethod != nullptr)
  {
    result = (ExplorerContextMenuEntry*)this->javaEnvironment->CallLongMethod(this->javaInstace, getterMethod);
  }

  return result;
}

void JavaExplorerContextMenuEntry::SetNativeHandle(ExplorerContextMenuEntry* value)
{
  this->InvokeLongSetterMethod("setNativeHandle", (int64_t)value);
}

void JavaExplorerContextMenuEntry::SetSeperator(bool value)
{
  jmethodID setterMethod = this->javaEnvironment->GetMethodID(this->javaClass, "setSeperator", "(Z)V");
  if (setterMethod != nullptr)
  {
    this->javaEnvironment->CallVoidMethod(this->javaInstace, setterMethod, value);
  }
}

void JavaExplorerContextMenuEntry::AddEntry(JavaExplorerContextMenuEntry& value)
{
  string setterMethodName = "(L" + this->javaClassName +";)V";
  jmethodID setterMethod = this->javaEnvironment->GetMethodID(this->javaClass, "addEntry", setterMethodName.c_str());
  if (setterMethod != nullptr)
  {
    this->javaEnvironment->CallVoidMethod(this->javaInstace, setterMethod, value.javaInstace);
  }
}

void JavaExplorerContextMenuEntry::InvokeByteArraySetterMethod(const char* methodName, vector<int8_t>& value)
{
  jsize arrayLength = static_cast<jsize>(value.size());
  jbyteArray javaArray = this->javaEnvironment->NewByteArray(arrayLength);
  this->javaEnvironment->SetByteArrayRegion(javaArray, 0, arrayLength, value.data());
  jmethodID setterMethod = this->javaEnvironment->GetMethodID(this->javaClass, methodName, "([B)V");
  if (setterMethod != nullptr)
  {
    this->javaEnvironment->CallVoidMethod(this->javaInstace, setterMethod, javaArray);
  }
}

void JavaExplorerContextMenuEntry::InvokeIntSetterMethod(const char* methodName, int32_t value)
{
  jmethodID setterMethod = this->javaEnvironment->GetMethodID(this->javaClass, methodName, "(I)V");
  if (setterMethod != nullptr)
  {
    this->javaEnvironment->CallVoidMethod(this->javaInstace, setterMethod, value);
  }
}

void JavaExplorerContextMenuEntry::InvokeLongSetterMethod(const char* methodName, int64_t value)
{
  jmethodID setterMethod = this->javaEnvironment->GetMethodID(this->javaClass, methodName, "(J)V");
  if (setterMethod != nullptr)
  {
    this->javaEnvironment->CallVoidMethod(this->javaInstace, setterMethod, value);
  }
}

void JavaExplorerContextMenuEntry::InvokeStringSetterMethod(const char* methodName, wstring& value)
{
  jmethodID setterMethod = this->javaEnvironment->GetMethodID(this->javaClass, methodName, "(Ljava/lang/String;)V");
  if (setterMethod != nullptr)
  {
    jstring jValue = this->javaEnvironment->NewString((jchar*)(value.c_str()), (jsize)(value.length()));
    this->javaEnvironment->CallVoidMethod(this->javaInstace, setterMethod, jValue);
  }
}
/***********************************************************************************************************************
 EOF
***********************************************************************************************************************/