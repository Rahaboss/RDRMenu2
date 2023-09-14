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

## Screenshots
![SS1](https://github.com/Rahaboss/RDRMenu2/assets/39026217/64612c30-a37d-4f7c-92e0-406e92eb3d50)
![SS2](https://github.com/Rahaboss/RDRMenu2/assets/39026217/ac2fb102-7ac1-4f86-ae5f-6cf6c8bfdffe)
![SS3](https://github.com/Rahaboss/RDRMenu2/assets/39026217/73f269c1-30e9-4178-a1e3-07fa123d8471)
![SS4](https://github.com/Rahaboss/RDRMenu2/assets/39026217/3954e869-b06b-4f8c-b203-ed4ca68e5ef4)
