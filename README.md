<!-- Written by Mohammad Ishrak Abedin-->
# ArgSpawn
---
### A commandline argument parser generator written in C++, for C++.
---
### The project relies on **[PREMAKE](https://premake.github.io/)** meta build system and can be compiled in Windows or Linux using preferred build system (Expected to use MSVC for Windows and GCC for Linux).

## *Tools Used*
---
- [Premake 5.0 Alpha](https://premake.github.io/)

*Premake5 is already provided for Windows in the `premake` folder.*

## *Libraries Used*
---
+ [fmtlib 7.1.3](https://github.com/fmtlib/fmt)
+ [yaml-cpp 0.6.3](https://github.com/jbeder/yaml-cpp)

## *Packages Required for Linux*
---
+ GCC
+ Make
+ Premake 5.0 alpha

---
**C++ 17 or Higher is Mandatory to compile, since the project uses *std::filesystem*. This is currently only written for x64 systems and not tested on others.**

---
## *Building for Windows (MSVC/ Visual Studio)*
---
*Powershell scripts are provided to make the build process faster. One might need to change **Execution Policy** to execute ps1 scripts. Alternatively the scripts can be converted to **bat** or **com** files to execute in **cmd**. The Windows version is targeted towards **MSVC** using **Microsoft Visual Studio**. Visual Studio version **2019 or Higher** recommended.*

Run the following commands in step to generate project files and build the project.
```powershell
.\makedir.ps1 # Generates output directories

.\premake\premake5.exe vs2019 # Pass respective Visual Studio version
```
*At this point, a **Visual Studio Solution (.sln)** will be generated. Use it to build the project.*

> ***VS 2017** solution by **Premake5** has some **bugs** and the Windows SDK is set to 8.1 or lower version. If using **VS 2017**, **manually set Windows SDK to the latest Windows 10 one**, by going to **Project > ArgSpawn Properties... > Configuration Properties > General > Windows SDK Version** and selecting the latest one.*

---
## *Building for Linux (GCC/ Make)*
---
*Shell Scripts are provided to make the build process faster. Install the aforementioned required packages before building.*

Run the following commands in step to generate project files and build the project.
```bash
#-- Project Generation --
sh ./makedir.sh # Generates output directories

premake5 gmake2 # Generates Makefile

#-- Building ---
# For building in Debug Mode
make config=debug
# or
# For building in Release Mode
make config=release
```
---
## Using the Application
---
After building, the executable is located in `bin/Release/` or `bin/Debug/` based on the build configuration. The application must be supplied with an `yaml` path and there are 2 other optional arguments. The output directory can be set with optional argument `--dir` or `-d` and flag `-v` or `--verbose` can be used to print out the generated files in the console. *Default output directory is the execution directory*. The output header and source files are not indent formatted and should be formatted using an IDE/Editor or any other tool if required.

*Example:*
```powershell
./ArgSpawn.exe -v ./yamls/ArgSpawnConfig.yaml --dir ./out/ 
```

### Optional arguments
+ `-d` or `--dir` *Output directory for the generated files (default is execution directory)*
+ `-v` or `--verbose` *Print the generated files in console*
---
## Example Yaml
-------
An example yaml with all possible keys are given below.
```yaml
Program Description: A Sample Program Description # Default is 'Program'
Angle Includes: [iostream, string, vector, memory] # string should be included if std::stoi, std::stod etc. are used for conversion
Quote Includes: [argParserUtil/ArgUtil.h, Vec3.h]
Namespace Name: argspawn # Optional, not using one would generate no namespace
Class Name: ArgumentParser # Default is 'ArgParser'
Positional Arguments:
  - Name: FilePath
    Description: Path to file
    Type: std::string
    Conversion: '' # Optional, default is considered as string
  - Name: Multiplier
    Description: Integer multiplier
    Type: int
    Conversion: std::stoi
  - Name: Coordinate
    Description: Coordinate of shape
    Type: Vec3 # Custom type is allowed by including custom header file
    Conversion: ArgUtil::StrToVec3 # Similarly, custom conversion is also allowed (should take std::string as input of the conversion function)
Optional Arguments:
  - Name: Width
    Description: Width of output
    Type: double
    Conversion: std::stod
    Symbols: [-w, --width]
    Default Value: 120.25
  - Name: Height
    Description: Height of output
    Type: double
    Conversion: std::stod
    Symbols: [-ht, --height]
    Default Value: 148.5566
  - Name: ShapeName
    Description: Name of the shape
    Type: std::string
    Symbols: [-s, --shape]
    Default Value: '"Rectangle"' # Remember to put quote for strings 
Flags:
  - Name: IsUniform
    Description: Force the output to be uniform
    Symbols: [-u, --uniform]
```
---
## Integration
---
Calling ArgSpawn with the yaml config file will generate `<Class Name>.h` and `<Class Name>.cpp` files. Integrate these files with your project. Arguments can then easily be parsed and accessed in the following manner.

```C++
auto parser = argspawn::ArgParser(argc, argv);
auto parsedArgs = parser.ParseArguments();
std::string yamlPath = std::filesystem::absolute(parsedArgs.YamlPath).string();
```
Parsed arguments are accessed as `<ArgumentsStruct>.<ArgumentName>`, where `<ArgumentName>` is same as provided in the `yaml` for each of the positional or optional arguments or flags. **Hence the name of arguments CANNOT contain any space, white space, character or number that are not allowed in variable naming in C++**.