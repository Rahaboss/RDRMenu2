# RDRMenu2
A trainer for Red Dead Redemption 2.

## Building this project
You will need:
* [Visual Studio 2019/2022](https://visualstudio.microsoft.com/)
* [Git](https://git-scm.com/download/win)
* [Vulkan SDK](https://sdk.lunarg.com/sdk/download/latest/windows/vulkan-sdk.exe)

```cmd
git clone --recurse-submodules git@github.com:Rahaboss/RDRMenu2.git
cd RDRMenu2
GenerateVS2019.bat
```

## Debugging (Currently unavailable)
In Visual Studio, right click on the project in the Solution Explorer. Click on Properties > Debugging. Set "Command" to the .exe path. Set "Attach" to Yes. Set your breakpoints, start the debugger and inject.

## Features
* ✔ Signature scanning
* ✔ Detour hooks
* ✔ Native invoker
* ✔ Job queue
* ✔ JSON file support
* ✔ YSC script global access
* ✔ Entity pool access (peds, objects, vehicles, pickups)
* ✔ D3D12 ImGui renderer
* ❌ Vulkan ImGui renderer
* ❌ Debugging support
