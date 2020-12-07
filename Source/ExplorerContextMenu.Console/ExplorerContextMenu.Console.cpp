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
#include "ExplorerContextMenu.h"

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
int wmain(int argc, wchar_t* argv[], wchar_t* envp[])
{
  vector<wstring> paths;

  // Argument at position 0 is the path to the executable, so start with index 1
  for (int argumentIndex = 1; argumentIndex < argc; argumentIndex++)
  {
    paths.push_back(argv[argumentIndex]);
  }

  set<wstring> whitelist;
  set<wstring> blacklist;

  // 7-Zip: 
  // whitelist.insert(L"{23170F69-40C1-278A-1000-000100020000}");

  // Notepad++: 
  // whitelist.insert(L"{B298D29A-A6ED-11DE-BA8C-A68E55D89593}");

  // TortoiseHg:
  // whitelist.insert(L"{46605027-5B8C-4DCE-BFE0-051B7972D64C}");

  // Modern Sharing
  // blacklist.insert(L"{e2bf9676-5f8f-435c-97eb-11607a5bedf7}");

  // Sharing
  // blacklist.insert(L"{f81e9010-6ea4-11ce-a7ff-00aa003ca9f6}");

  ExplorerContextMenu contextMenu(paths, true, whitelist, blacklist);
  contextMenu.PrintMenu();
  return 0;
}

/***********************************************************************************************************************
 EOF
***********************************************************************************************************************/
