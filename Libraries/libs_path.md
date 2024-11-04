## C/C++ GENERAL

-  ..\..\Libraries\glew-2.1.0\include
-  ..\..\Libraries\glfw-3.4.bin.WIN64\include
-  ..\..\Libraries\glm-master
-  %(AdditionalIncludeDirectories)

## LINKER GENERAL

-  ..\..\Libraries\glew-2.1.0\lib\Release\x64
-  ..\..\Libraries\glfw-3.4.bin.WIN64\lib-vc2022
-  %(AdditionalLibraryDirectories)

## LINKER INPUT

-  glew32.lib
-  opengl32.lib
-  glu32.lib
-  glfw3.lib
-  %(AdditionalDependencies)
