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
#include "ExplorerContextMenuEntry.h"

/***********************************************************************************************************************
 CLASS
***********************************************************************************************************************/
namespace ContextQuickie
{
  using namespace System;
  using namespace System::Collections::Generic;
  using namespace System::Drawing;

  public ref class NetExplorerContextMenuEntry
  {
  private:
    Image^ icon;
    ExplorerContextMenuEntry* nativeEntry;
    List<NetExplorerContextMenuEntry^>^ childs = gcnew List<NetExplorerContextMenuEntry^>();

  internal:
    NetExplorerContextMenuEntry(ExplorerContextMenuEntry* entry);
    void CopyNativeData(ExplorerContextMenuEntry* entry);

  public:
    NetExplorerContextMenuEntry();
    void ExecuteCommand();
    property String^ Text;
    property Boolean IsSeperator
    {
      Boolean get();
    }

    property IEnumerable<NetExplorerContextMenuEntry^>^ Childs
    {
      IEnumerable<NetExplorerContextMenuEntry^>^ get();
    }

    property Image^ Icon
    {
      Image^ get();
    }
  };
}

/***********************************************************************************************************************
 EOF
***********************************************************************************************************************/
