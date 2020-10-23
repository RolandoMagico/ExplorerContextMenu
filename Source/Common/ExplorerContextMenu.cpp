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
#include "ExplorerContextMenu.h"
#include "objbase.h"
#include "shlobj_core.h"
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
  ExplorerContextMenu::ExplorerContextMenu(vector<wstring> paths)
  {
    HRESULT result = S_OK;

    if (paths.empty())
    {
      /* Nothing to do if an empty list ist passed */
    }
    else if (paths.size() > UINT_MAX)
    {
      /* Too many items, cannot handle that */
    }
    else if (SUCCEEDED(result = CoInitialize(nullptr)))
    {
      ITEMIDLIST** itemIdList = new ITEMIDLIST * [paths.size()];
      for (size_t pathIndex = 0; (pathIndex < paths.size()) && SUCCEEDED(result); pathIndex++)
      {
        result = SHParseDisplayName(paths[pathIndex].c_str(), nullptr, &(itemIdList[pathIndex]), 0, NULL);
      }

      if (SUCCEEDED(result))
      {
        IShellFolder* desktop = nullptr;
        if (SUCCEEDED(result = SHGetDesktopFolder(&desktop)))
        {
          UINT flags = CMF_DEFAULTONLY;
          if ((GetKeyState(VK_SHIFT) & GET_KEY_STATE_KEY_PRESSED) == GET_KEY_STATE_KEY_PRESSED)
          {
            flags |= CMF_EXTENDEDVERBS;
          }

          HWND windowHandle = GetCurrentWindowHandle();
          if (windowHandle != nullptr)
          {
            DEFCONTEXTMENU defaultContextMenu = { 0 };
            defaultContextMenu.hwnd = windowHandle;
            defaultContextMenu.psf = desktop;
            defaultContextMenu.cidl = (UINT)paths.size();
            defaultContextMenu.apidl = (LPCITEMIDLIST*)itemIdList;

            IContextMenu* contextMenu;
            if (SUCCEEDED(result = SHCreateDefaultContextMenu(&defaultContextMenu, IID_IContextMenu3, (void**)&(contextMenu))))
            {
            }
            else if (SUCCEEDED(result = SHCreateDefaultContextMenu(&defaultContextMenu, IID_IContextMenu2, (void**)&(contextMenu))))
            {
            }
            else if (SUCCEEDED(result = SHCreateDefaultContextMenu(&defaultContextMenu, IID_IContextMenu, (void**)&(contextMenu))))
            {
            }

            if (SUCCEEDED(result))
            {
              this->GetMenuData(contextMenu, flags);
            }
          }

          IDataObject* dataObject;
          if (SUCCEEDED(result = desktop->GetUIObjectOf(windowHandle, (UINT)paths.size(), (LPCITEMIDLIST*)itemIdList, IID_IDataObject, NULL, (void**)&dataObject)))
          {
            LSTATUS registryResult;
            HKEY contextMenuHandlers = nullptr;
            wstring registryPath(L"*\\shellex\\ContextMenuHandlers");
            if ((registryResult = RegOpenKeyEx(HKEY_CLASSES_ROOT, registryPath.c_str(), 0, KEY_READ, &contextMenuHandlers)) == ERROR_SUCCESS)
            {
              DWORD numberOfSubKey = 0;
              DWORD subKeyMaxLength = 0;
              if ((registryResult = RegQueryInfoKey(contextMenuHandlers, NULL, NULL, NULL, &numberOfSubKey, &subKeyMaxLength, NULL, NULL, NULL, NULL, NULL, NULL)) == ERROR_SUCCESS)
              {
                for (DWORD keyIndex = 0; keyIndex < numberOfSubKey; keyIndex++)
                {
                  wchar_t* buffer = new wchar_t[subKeyMaxLength + 1];
                  DWORD numberOfReadBytes = subKeyMaxLength + 1;
                  if ((registryResult = RegEnumKeyEx(contextMenuHandlers, keyIndex, buffer, &numberOfReadBytes, NULL, NULL, NULL, NULL)) == ERROR_SUCCESS)
                  {
                    wprintf(L"%ls\n", buffer);
                    wchar_t buffer2[255];
                    DWORD bufferLength = sizeof(buffer2);
                    if ((registryResult = RegGetValue(contextMenuHandlers, buffer, NULL, RRF_RT_REG_SZ, NULL, buffer2, &bufferLength)) == ERROR_SUCCESS)
                    {
                      wprintf(L"%ls\n", buffer2);
                      IShellExtInit* shellExtInit;
                      GUID CLSID_NewMenu;
                      if (SUCCEEDED(result = CLSIDFromString(buffer2, &CLSID_NewMenu)))
                      {
                      }
                      else if (SUCCEEDED(result = CLSIDFromString(buffer, &CLSID_NewMenu)))
                      {
                      }
                      if (SUCCEEDED(result))
                      {
                        if (SUCCEEDED(result = CoCreateInstance(CLSID_NewMenu, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&shellExtInit))))
                        {
                          if (SUCCEEDED(result = shellExtInit->Initialize(NULL, dataObject, NULL)))
                          {
                            IContextMenu* contextMenu;
                            if (SUCCEEDED(result = shellExtInit->QueryInterface(IID_IContextMenu3, (void**)&(contextMenu))))
                            {
                            }
                            else if (SUCCEEDED(result = shellExtInit->QueryInterface(IID_IContextMenu2, (void**)&(contextMenu))))
                            {
                            }
                            else if (SUCCEEDED(result = shellExtInit->QueryInterface(IID_IContextMenu, (void**)&(contextMenu))))
                            {
                            }

                            if (SUCCEEDED(result))
                            {
                              this->GetMenuData(contextMenu, CMF_NORMAL);
                            }

                            shellExtInit->Release();
                          }
                        }
                      }
                    }
                  }
                }
              }

              RegCloseKey(contextMenuHandlers);
            }
          }

          desktop->Release();
        }
      }
    }
  }

  HWND ExplorerContextMenu::GetCurrentWindowHandle()
  {
    /* Try to get the current window handle, try to get the desktop handle first, if it doesn't work, try to get the console window handle. */
    HWND windowHandle = nullptr;
    windowHandle = GetDesktopWindow();
    if (windowHandle == nullptr)
    {
      windowHandle = GetConsoleWindow();
    }

    return windowHandle;
  }

  uint32_t ExplorerContextMenu::GetNumberOfEntries(HMENU menu)
  {
    uint32_t count = 0;
    for (int menuIndex = 0; menuIndex < GetMenuItemCount(menu); menuIndex++)
    {
      MENUITEMINFO menuInfo;

      menuInfo.fMask = MIIM_SUBMENU;
      menuInfo.cbSize = sizeof(MENUITEMINFO);
      if (GetMenuItemInfo(menu, menuIndex, true, &menuInfo) == false)
      {
        /* TODO: Error handling if the text cannot be retrieved */
      }
      else if (menuInfo.hSubMenu != nullptr)
      {
        count += GetNumberOfEntries(menuInfo.hSubMenu);
      }

      count++;
    }

    return count;
  }
}
/***********************************************************************************************************************
 EOF
***********************************************************************************************************************/