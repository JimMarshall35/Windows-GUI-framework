- C++ framework for developing guis on windows
- Based on WPF
- GUIs defined declaratively by xml
- MVVM pattern + data binding
- No usage of .rc file defined dialogs, resources will be used only for string tables
- Pure win32 - no MFC dependency and there never will be
- wide character strings ONLY

future work
- WPF style value converters - needed to have proper MVVM pattern usage
- WPF style data validation
- refine and finish data binding, hierarchical data contexts, binding to arrays / lists, ect ect
- WPF style DSL for bindings "{Binding Property, Convertor MyConverter}", ect
- WPF style static resources
- dockable panels
- toolbar / ribbon bar
- menus
- MFC / Win32 accelerator table equivalent / abstraction
- DX context canvas widget
- OpenGL context canvas widget
- GDI / GDI+ context canvas widget
- GDI abstraction layer to allow cross platform ports
- build in satellite DLL support for translations
- show view as a modal window
- Command arguments (as in the Command class used by view models)
- customizable file dialog like MFC
- Support libs: Logging, command line arg parsing
- Mac or linux native API port
- Tooling: Translation tool - translate resource DLLS from a csv file. Widget inspecter - Like unreal engine widget inspector. Live edit of UI like WPF - WPF overlay showing unresolved bindings, opens widget inspector
- Always think about how code could be reused in an in game UI with custom drawn widgets

  out of scope:
  - a complex reflection system
  - dependency injection container
  - any OpenGL or DX code beyond creating a context
  - support libraries beyond those two listed above
  - ever supporting narrow strings
