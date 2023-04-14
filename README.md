# RDRMenu2
A trainer for Red Dead Redemption 2.

## Building this project
You will need:
* Visual Studio 2017/2019/2022
* Git

```cmd
git clone --recurse-submodules git@github.com:Rahaboss/RDRMenu2.git
cd RDRMenu2
generate_vs2019.bat
```

## Debugging
In Visual Studio, right click on the project in the Solution Explorer. Click on Properties > Debugging. Set "Command" to the .exe path. Set "Attach" to Yes. Set your breakpoints, start the debugger and inject.

## Features
* ✔ Signature scanning
* ✔ Detour hooks
* ✔ Native invoker
* ✔ Job queue
* ✔ JSON file support
* ✔ YSC script global access
* ✔ D3D12 ImGui renderer
* ❌ Vulkan ImGui renderer

## Screenshots
![ss2](https://user-images.githubusercontent.com/39026217/231996834-dbc09397-a0a8-4cad-b573-c7c36d304487.png)
![ss1](https://user-images.githubusercontent.com/39026217/231996787-314d0d50-9144-40f1-9ae8-1548ee12e369.png)
![ss3](https://user-images.githubusercontent.com/39026217/231996887-671c9cb2-1adf-44c4-b414-5c308daebebf.png)
