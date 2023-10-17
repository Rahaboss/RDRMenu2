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

## Debugging (partial support)
In Visual Studio, right click on the project in the Solution Explorer. Click on Properties > Debugging. Set "Command" to the RDR2.exe path. Set "Attach" to Yes. Set your breakpoints, inject and then start the debugger.
