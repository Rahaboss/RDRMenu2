# DLLTemplate
A template made for DLL injection.

## Building this project
You will need:
* Visual Studio 2019
* Git

```
git clone --recurse-submodules git@github.com:Rahaboss/DLLTemplate.git
cd DLLTemplate
GenerateProjects.bat
```

## Debugging
In Visual Studio, right click on the project in the Solution Explorer. Click on Properties > Debugging. Set "Command" to the .exe path. Set "Attach" to Yes. Set your breakpoints, start the debugger and inject.
