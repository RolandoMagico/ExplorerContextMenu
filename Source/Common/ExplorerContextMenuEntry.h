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
#include <string>
#include <vector>
#include "stdint.h"
#include "shlobj_core.h"
using namespace std;
/***********************************************************************************************************************
 CLASS
***********************************************************************************************************************/
namespace ContextQuickie
{
  class ExplorerContextMenuEntry
  {
  private:
    IContextMenu* ContextMenu = nullptr;

  public:
    wstring* Text;
    bool IsSeparator;
    uint32_t CommandId;
    uint32_t BitmapWidth;
    uint32_t BitmapHeight;
    uint32_t* BitmapHandle;
    vector<ExplorerContextMenuEntry*> menuEntries;
    ExplorerContextMenuEntry();
    ~ExplorerContextMenuEntry();
    size_t GetAllEntriesCount();
    void GetMenuData(HMENU menu);
    void GetMenuData(IContextMenu* contextMenu, uint32_t flags);
    void ExecuteCommand();
    void PrintMenu(uint32_t level = 0);
  };
}
/***********************************************************************************************************************
 EOF
***********************************************************************************************************************/

