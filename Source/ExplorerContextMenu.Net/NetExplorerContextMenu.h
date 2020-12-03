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
#include "ExplorerContextMenu.h"
#include "NetExplorerContextMenuEntry.h"

/***********************************************************************************************************************
 CLASS
***********************************************************************************************************************/
namespace ContextQuickie
{
  using namespace System;
  using namespace System::Collections::Generic;
  using namespace ContextQuickie;

  public ref class NetExplorerContextMenu : public NetExplorerContextMenuEntry
  {
  private:
    /// <summary>
    /// Reference to the natvie entry of this instance.
    /// </summary>
    ExplorerContextMenu* nativeMenu;
  public:
    /// <summary>
    /// Copies the data from a native entry to this instance.
    /// </summary>
    /// <param name="entry">A list of paths to files or folders for which the menu will be created.</param>
    NetExplorerContextMenu(List<String^>^ paths);
  };
}

/***********************************************************************************************************************
 EOF
***********************************************************************************************************************/
