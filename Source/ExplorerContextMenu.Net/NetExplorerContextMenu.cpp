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
#include <stdlib.h>
#include "ExplorerContextMenu.h"
#include "NetExplorerContextMenu.h"

namespace ContextQuickie
{
    using namespace System::Runtime::InteropServices;

    NetExplorerContextMenu::NetExplorerContextMenu(List<String^>^ paths)
    {
#if 0
      wchar_t** localPaths = static_cast<wchar_t**>(malloc(sizeof(wchar_t*) * paths->Count));
      for (int i = 0; i < paths->Count; i++)
      {
        localPaths[i] = static_cast<wchar_t*>(Marshal::StringToHGlobalUni(paths[i]).ToPointer());
      }

      int entriesCount = ContextMenuProvider_CreateContextMenu((const wchar_t**)localPaths, paths->Count);

      this->Entries = gcnew List<ShellContextMenuEntry^>();
      for (int entryIndex = 0; entryIndex < entriesCount; entryIndex++)
      {
        ContextMenuProvider_MenuEntry entry;
        if (ContextMenuProvider_GetContextMenuEntry(entryIndex, &entry) != 0)
        {
          ShellContextMenuEntry^ entry = gcnew ShellContextMenuEntry();
          entry->Text = gcnew String(entry->Text);
          this->Entries->Add(entry);
        }
      }
#endif
    }
}

/***********************************************************************************************************************
 EOF
***********************************************************************************************************************/