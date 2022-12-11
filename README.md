# RDRMenu2
A trainer for Red Dead Redemption 2.

## Building this project
You will need:
* Visual Studio 2019
* Git

```cmd
git clone --recurse-submodules git@github.com:Rahaboss/RDRMenu2.git
cd RDRMenu2
GenerateProjects.bat
```

## Debugging
In Visual Studio, right click on the project in the Solution Explorer. Click on Properties > Debugging. Set "Command" to the .exe path. Set "Attach" to Yes. Set your breakpoints, start the debugger and inject.

## Features
✔ Signature scanning
✔ Detour hooks
✔ Native invoker
✔ Job queue 
❌ JSON file support (settings file for example)
❌ YSC script global access
✔ D3D12 ImGui renderer
❌ Vulkan ImGui renderer
