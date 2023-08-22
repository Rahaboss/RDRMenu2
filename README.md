# RDRMenu2
A trainer for Red Dead Redemption 2.

## Building this project
You will need:
* Visual Studio 2019/2022
* Git
* [Vulkan SDK](https://sdk.lunarg.com/sdk/download/latest/windows/vulkan-sdk.exe)

```cmd
git clone --recurse-submodules git@github.com:Rahaboss/RDRMenu2.git
cd RDRMenu2
GenerateVS2019
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

## Screenshots
![ss2](https://user-images.githubusercontent.com/39026217/231996834-dbc09397-a0a8-4cad-b573-c7c36d304487.png)
![ss1](https://user-images.githubusercontent.com/39026217/231996787-314d0d50-9144-40f1-9ae8-1548ee12e369.png)
![ss3](https://user-images.githubusercontent.com/39026217/231996887-671c9cb2-1adf-44c4-b414-5c308daebebf.png)
![ss4](https://github.com/Rahaboss/RDRMenu2/assets/39026217/c9889213-ad6e-4fbd-944b-4c927b795c3a)
![ss7](https://github.com/Rahaboss/RDRMenu2/assets/39026217/6a0897e2-350d-4b37-86a3-fe0a4fca81c2)
