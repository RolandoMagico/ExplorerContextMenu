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
#include <iostream>
#include "ExplorerContextMenuEntry.h"
#pragma unmanaged

/***********************************************************************************************************************
 DEFINES
***********************************************************************************************************************/
#define GET_KEY_STATE_KEY_PRESSED   (0x8000u)
#define MAX_STRING_LENGTH           (1024u)
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
namespace ContextQuickie
{
  using namespace std;

  ExplorerContextMenuEntry::ExplorerContextMenuEntry()
  {
  }

  ExplorerContextMenuEntry::ExplorerContextMenuEntry(IContextMenu* contextMenu, HMENU menu, int32_t index)
    : ExplorerContextMenuEntry()
  {
    this->ContextMenu = contextMenu;

    wchar_t buffer[MAX_STRING_LENGTH] = { 0 };

    // Get data from MENUITEMINFO
    MENUITEMINFO menuInfo = { 0 };
    memset(buffer, 0, sizeof(buffer));
    menuInfo.fMask = MIIM_BITMAP | MIIM_CHECKMARKS | MIIM_DATA | MIIM_FTYPE | MIIM_ID | MIIM_STATE | MIIM_STRING | MIIM_SUBMENU;
    menuInfo.cbSize = sizeof(MENUITEMINFO);
    menuInfo.dwTypeData = buffer;
    menuInfo.cch = sizeof(buffer);
    if ((GetMenuItemInfo(menu, index, true, &menuInfo) == TRUE))
    {
      if (menuInfo.cch != 0)
      {
        this->Text = new wstring(buffer);
      }

      if (menuInfo.fType == MFT_SEPARATOR)
      {
        this->IsSeparator = true;
      }
      else
      {
        this->IsSeparator = false;
        this->CommandId = menuInfo.wID;
      }

      if (menuInfo.hbmpItem != nullptr)
      {
        this->BitmapHandle = menuInfo.hbmpItem;
      }
      else if (menuInfo.hbmpUnchecked != nullptr)
      {
        this->BitmapHandle = menuInfo.hbmpUnchecked;
      }
      else if (menuInfo.hbmpChecked != nullptr)
      {
        this->BitmapHandle = menuInfo.hbmpChecked;
      }

      // Get data from IContextMenu
      char commandBuffer[MAX_STRING_LENGTH] = { 0 };
      if (SUCCEEDED(contextMenu->GetCommandString(this->CommandId, GCS_VERBA, NULL, commandBuffer, MAX_STRING_LENGTH)))
      {
        if (strlen(commandBuffer) != 0)
        {
          this->CommandString = new string(commandBuffer);
        }
      }

      memset(buffer, 0, sizeof(buffer));
      if (SUCCEEDED(contextMenu->GetCommandString(this->CommandId, GCS_HELPTEXTW, NULL, (CHAR*)buffer, MAX_STRING_LENGTH)))
      {
        if (wcslen(buffer) != 0)
        {
          this->HelpText = new wstring(buffer);
        }
      }

      if (menuInfo.hSubMenu != nullptr)
      {
        IContextMenu2* contextMenu2 = nullptr;
        if (SUCCEEDED(contextMenu->QueryInterface(IID_PPV_ARGS(&contextMenu2))))
        {
          contextMenu2->HandleMenuMsg(WM_INITMENUPOPUP, (WPARAM)menuInfo.hSubMenu, 0);
        }

        this->AddEntries(contextMenu, menuInfo.hSubMenu, true);
        DestroyMenu(menuInfo.hSubMenu);
      }
    }
    else
    {
      /* TODO: Error handling if the menu info cannot be retrieved */
    }
  }

  ExplorerContextMenuEntry::~ExplorerContextMenuEntry()
  {
    for (size_t entryIndex = 0; entryIndex < this->menuEntries.size(); entryIndex++)
    {
      delete this->menuEntries[entryIndex];
    }

    this->menuEntries.clear();

    if (this->BitmapHandle != nullptr)
    {
      // Do not delete the bitmap object. 
      // Some extensions (e.g. TortoiseHg) use a bitmap cache which will crash if the bitmap is deleted
      // DeleteObject(this->BitmapHandle);
    }

    if (this->Text != nullptr)
    {
      delete this->Text;
    }

    if (this->HelpText != nullptr)
    {
      delete this->HelpText;
    }

    if (this->CommandString != nullptr)
    {
      delete this->CommandString;
    }
  }

  void ExplorerContextMenuEntry::AddEntries(IContextMenu* contextMenu, HMENU menu, bool isDefaultMenu)
  {
    int menuItemCount = GetMenuItemCount(menu);

    vector<ExplorerContextMenuEntry*> createdEntries;
    for (uint32_t menuIndex = 0; menuIndex < (uint32_t)menuItemCount; menuIndex++)
    {
      ExplorerContextMenuEntry* entry = new ExplorerContextMenuEntry(contextMenu, menu, menuIndex);
      if (isDefaultMenu == true)
      {
        this->menuEntries.push_back(entry);
      }
      else
      {
        this->menuEntries.insert(this->menuEntries.begin() + this->NextInsertPosition, entry);
        this->NextInsertPosition++;
      }
    }

    if (isDefaultMenu == true)
    {
      // Assumption: Default entry is the first entry in the menu
      if (menuItemCount > 0)
      {
        this->NextInsertPosition++;
      }
    }
  }

  void ExplorerContextMenuEntry::GetMenuData(IContextMenu* contextMenu, uint32_t flags)
  {
    if ((GetKeyState(VK_SHIFT) & GET_KEY_STATE_KEY_PRESSED) == GET_KEY_STATE_KEY_PRESSED)
    {
      flags |= CMF_EXTENDEDVERBS;
    }
    HMENU menu = CreatePopupMenu();
    if (SUCCEEDED(contextMenu->QueryContextMenu(menu, 0, 0, UINT_MAX, flags)))
    {
      bool isDefaultMenu = ((flags & CMF_DEFAULTONLY) == CMF_DEFAULTONLY);
      this->AddEntries(contextMenu, menu, isDefaultMenu);
    }

    DestroyMenu(menu);
  }

  bool ExplorerContextMenuEntry::GetBitmapDimensions(uint32_t& width, uint32_t& height, uint32_t& planes, uint32_t& bitsPerPixel)
  {
    bool result = false;
    BITMAP bitmap = { 0 };
    if (this->BitmapHandle == nullptr)
    {
      // This entry doesn't have an image
    }
    else if (GetObject(this->BitmapHandle, sizeof(bitmap), &bitmap) == 0)
    {
      // GetObject failed
    }
    else
    {
      width = bitmap.bmWidth;
      height = bitmap.bmHeight;
      planes = bitmap.bmPlanes;
      bitsPerPixel = bitmap.bmBitsPixel;
      result = true;
    }

    return result;
  }

  bool ExplorerContextMenuEntry::GetBitmapPixelData(uint32_t width, uint32_t height, uint32_t planes, uint32_t bitsPerPixel, void* destination)
  {
    bool result = false;
    HDC deviceContextHandle = nullptr;

    BITMAPINFO bitmapInfo = { 0 };
    bitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bitmapInfo.bmiHeader.biWidth = width;
    bitmapInfo.bmiHeader.biHeight = height * -1;
    bitmapInfo.bmiHeader.biPlanes = planes;
    bitmapInfo.bmiHeader.biBitCount = bitsPerPixel;

    if (this->BitmapHandle == nullptr)
    {
      // This entry doesn't have an image
    }
    else if ((deviceContextHandle = GetDC(HWND_DESKTOP)) == NULL)
    {
      // GetDC failed
    }
    else if (GetDIBits(deviceContextHandle, this->BitmapHandle, 0, height, destination, &bitmapInfo, DIB_RGB_COLORS) == 0)
    {
      // GetDIBits failed
    }
    else
    {
      result = true;
    }

    if (deviceContextHandle != nullptr)
    {
      ReleaseDC(HWND_DESKTOP, deviceContextHandle);
    }

    return result;
  }

  int32_t ExplorerContextMenuEntry::ExecuteCommand(HWND windowHandle, bool executeSynchronous)
  {
    int32_t result = INT32_MIN;
    if (this->ContextMenu != nullptr)
    {
      CMINVOKECOMMANDINFO info = { 0 };
      info.cbSize = sizeof(info);
      info.hwnd = windowHandle;

      if (executeSynchronous == true)
      {
        info.fMask |= CMIC_MASK_NOASYNC;
      }

      if (this->CommandString != nullptr)
      {
        info.lpVerb = this->CommandString->c_str();
      }
      else
      {
        info.lpVerb = MAKEINTRESOURCEA(this->CommandId);
      }

      result = this->ContextMenu->InvokeCommand(&info);
    }

    return result;
  }

  size_t ExplorerContextMenuEntry::GetAllEntriesCount()
  {
    size_t count = 0;
    for (size_t entryIndex = 0; entryIndex < this->menuEntries.size(); entryIndex++)
    {
      count++;
      count += this->menuEntries[entryIndex]->GetAllEntriesCount();
    }

    return count;
  }

  void ExplorerContextMenuEntry::PrintMenu(uint32_t level)
  {
    wcout << wstring(4 * static_cast<__int64>(level), ' ');
    if (this->IsSeparator)
    {
      wcout << "-------------------" << std::endl;
    }
    else if (this->Text != nullptr)
    {
      std::wcout << (*this->Text) << std::endl;
    }

    for (size_t childIndex = 0; childIndex < this->menuEntries.size(); childIndex++)
    {
      this->menuEntries[childIndex]->PrintMenu(level + 1);
    }
  }

  void ExplorerContextMenuEntry::RemoveDuplicateSeparators()
  {
    for (size_t childIndex = 1; childIndex < this->menuEntries.size(); childIndex++)
    {
      if ((this->menuEntries[childIndex]->IsSeparator == true) &&
        (this->menuEntries[childIndex - 1]->IsSeparator == true))
      {
        delete this->menuEntries[childIndex];
        this->menuEntries.erase(this->menuEntries.begin() + childIndex);
      }
    }

    // Remove separator at the end if there is one
    if (this->menuEntries.empty() == false)
    {
      ExplorerContextMenuEntry* entry = this->menuEntries.at(this->menuEntries.size() - 1);
      if (entry->IsSeparator == true)
      {
        this->menuEntries.pop_back();
        delete entry;
      }
    }

    for (size_t childIndex = 0; childIndex < this->menuEntries.size(); childIndex++)
    {
      this->menuEntries[childIndex]->RemoveDuplicateSeparators();
    }
  }
}
