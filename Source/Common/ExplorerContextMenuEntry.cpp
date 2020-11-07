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
    this->BitmapHandle = nullptr;
    this->BitmapWidth = 0;
    this->BitmapHeight = 0;
    this->CommandId = 0;
    this->IsSeparator = false;
    this->menuEntries = vector<ExplorerContextMenuEntry*>();
    this->Text = nullptr;
  }

  ExplorerContextMenuEntry::ExplorerContextMenuEntry(IContextMenu* contextMenu, HMENU menu, int32_t index)
    : ExplorerContextMenuEntry()
  {
    this->ContextMenu = contextMenu;
    MENUITEMINFO menuInfo;

    menuInfo.fMask = MIIM_STRING;
    menuInfo.cbSize = sizeof(MENUITEMINFO);
    menuInfo.dwTypeData = nullptr;
    if ((GetMenuItemInfo(menu, index, true, &menuInfo) == TRUE) && (menuInfo.cch != 0))
    {
      menuInfo.cch++;
      wchar_t* buffer = new wchar_t[menuInfo.cch];
      menuInfo.dwTypeData = buffer;
      if (GetMenuItemInfo(menu, index, true, &menuInfo) == TRUE)
      {
        this->Text = new wstring(buffer);
      }
      else
      {
        /* TODO: Error handling if the text cannot be retrieved */
      }

      delete[] buffer;
    }
    else
    {
      /* TODO: Error handling if the text cannot be retrieved */
    }

    menuInfo.fMask = MIIM_FTYPE;
    menuInfo.cbSize = sizeof(MENUITEMINFO);
    if (GetMenuItemInfo(menu, index, true, &menuInfo) == false)
    {
      /* TODO: Error handling if the type cannot be retrieved */
    }
    else if (menuInfo.fType == MFT_SEPARATOR)
    {
      this->IsSeparator = true;
    }
    else
    {
      this->IsSeparator = false;
    }

    menuInfo.fMask = MIIM_ID;
    menuInfo.cbSize = sizeof(MENUITEMINFO);
    if (GetMenuItemInfo(menu, index, true, &menuInfo) == false)
    {
      /* TODO: Error handling if the type cannot be retrieved */
    }
    else
    {
      this->CommandId = menuInfo.wID;
    }

    menuInfo.fMask = MIIM_BITMAP;
    menuInfo.cbSize = sizeof(MENUITEMINFO);
    if (GetMenuItemInfo(menu, index, true, &menuInfo) == false)
    {
      /* TODO: Error handling if the text cannot be retrieved */
    }
    else if (menuInfo.hbmpItem != nullptr)
    {
      BITMAP bitMap;
      if (GetObject(menuInfo.hbmpItem, sizeof(BITMAP), &bitMap) != 0)
      {
        this->BitmapHandle = (uint32_t*)menuInfo.hbmpItem;
        this->BitmapWidth = bitMap.bmWidth;
        this->BitmapHeight = bitMap.bmHeight;
      }
    }

    menuInfo.fMask = MIIM_SUBMENU;
    menuInfo.cbSize = sizeof(MENUITEMINFO);
    if (GetMenuItemInfo(menu, index, true, &menuInfo) == false)
    {
      /* TODO: Error handling if the text cannot be retrieved */
    }
    else if (menuInfo.hSubMenu != nullptr)
    {
      this->AddEntries(contextMenu, menuInfo.hSubMenu, true);
    }
  }

  ExplorerContextMenuEntry::~ExplorerContextMenuEntry()
  {
    for (size_t entryIndex = 0; entryIndex < this->menuEntries.size(); entryIndex++)
    {
      delete this->menuEntries[entryIndex];
    }

    if (this->Text != nullptr)
    {
      delete this->Text;
    }
  }

  void ExplorerContextMenuEntry::AddEntries(IContextMenu* contextMenu, HMENU menu, bool isDefaultMenu)
  {
    int menuItemCount = GetMenuItemCount(menu);

    vector<ExplorerContextMenuEntry*> createdEntries;
    for (uint32_t menuIndex = 0; menuIndex < (uint32_t)menuItemCount; menuIndex++)
    {
      ExplorerContextMenuEntry* entry = new ExplorerContextMenuEntry(contextMenu, menu, menuIndex);
      createdEntries.push_back(entry);
    }

    if ((isDefaultMenu == true))
    {
      this->menuEntries.insert(this->menuEntries.cend(), createdEntries.begin(), createdEntries.end());
    }
    else if (createdEntries.empty() == false)
    {
      this->menuEntries.insert(this->menuEntries.begin() + 1, createdEntries.begin(), createdEntries.end());
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

  void ExplorerContextMenuEntry::ExecuteCommand()
  {
    if (this->ContextMenu != nullptr)
    {
      CMINVOKECOMMANDINFO info = { 0 };
      info.cbSize = sizeof(info);
      // TODO: info.hwnd = GetCurrentWindowHandle();
      info.lpVerb = MAKEINTRESOURCEA(this->CommandId);
      this->ContextMenu->InvokeCommand(&info);
    }
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
        this->menuEntries.erase(this->menuEntries.begin() + childIndex);
      }
    }

    for (size_t childIndex = 0; childIndex < this->menuEntries.size(); childIndex++)
    {
      this->menuEntries[childIndex]->RemoveDuplicateSeparators();
    }
  }
}

