##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=swtest
ConfigurationName      :=Release
WorkspacePath          :=C:/Users/mrDIMAS/Desktop/SoundWorks/projects
ProjectPath            :=C:/Users/mrDIMAS/Desktop/SoundWorks/projects
IntermediateDirectory  :=./swtest/Release
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=mrDIMAS
Date                   :=20/01/2018
CodeLitePath           :="C:/Program Files/CodeLite"
LinkerName             :=C:/MinGW/bin/g++.exe
SharedObjectLinkerName :=C:/MinGW/bin/g++.exe -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.i
DebugSwitch            :=-g 
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputFile             :=../test/sndwrk.exe
Preprocessors          :=$(PreprocessorSwitch)NDEBUG 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="swtest.txt"
PCHCompileFlags        :=
MakeDirCommand         :=makedir
RcCmpOptions           := 
RcCompilerName         :=C:/MinGW/bin/windres.exe
LinkOptions            :=  -static-libgcc 
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). $(IncludeSwitch)../thirdparty/SDL2/include/ $(IncludeSwitch)../ $(IncludeSwitch)../src/swtest/ 
IncludePCH             := 
RcIncludePath          := 
Libs                   := $(LibrarySwitch)sndwrk $(LibrarySwitch)mingw32 $(LibrarySwitch)opengl32 $(LibrarySwitch)glu32 $(LibrarySwitch)SDL2main $(LibrarySwitch)SDL2 
ArLibs                 :=  "libsndwrk" "libmingw32" "libopengl32" "libglu32" "libSDL2main" "libSDL2" 
LibPath                := $(LibraryPathSwitch). $(LibraryPathSwitch)../thirdparty/SDL2/lib/ $(LibraryPathSwitch)../lib/ 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := C:/MinGW/bin/ar.exe rcu
CXX      := C:/MinGW/bin/g++.exe
CC       := C:/MinGW/bin/gcc.exe
CXXFLAGS :=   $(Preprocessors)
CFLAGS   :=  -ggdb -O2 -Wall -ansi  $(Preprocessors)
ASFLAGS  := 
AS       := C:/MinGW/bin/as.exe


##
## User defined environment variables
##
CodeLiteDir:=C:\Program Files (x86)\CodeLite
Objects0=$(IntermediateDirectory)/up_src_swtest_swtest.c$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

MakeIntermediateDirs:
	@$(MakeDirCommand) "./swtest/Release"


$(IntermediateDirectory)/.d:
	@$(MakeDirCommand) "./swtest/Release"

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/up_src_swtest_swtest.c$(ObjectSuffix): ../src/swtest/swtest.c $(IntermediateDirectory)/up_src_swtest_swtest.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/Users/mrDIMAS/Desktop/SoundWorks/src/swtest/swtest.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_swtest_swtest.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_swtest_swtest.c$(DependSuffix): ../src/swtest/swtest.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_swtest_swtest.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_swtest_swtest.c$(DependSuffix) -MM ../src/swtest/swtest.c

$(IntermediateDirectory)/up_src_swtest_swtest.c$(PreprocessSuffix): ../src/swtest/swtest.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_swtest_swtest.c$(PreprocessSuffix) ../src/swtest/swtest.c


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./swtest/Release/


