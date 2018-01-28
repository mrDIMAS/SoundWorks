##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=hrtf
ConfigurationName      :=Release
WorkspacePath          :=C:/Users/mrDIMAS/Desktop/SoundWorks/projects
ProjectPath            :=C:/Users/mrDIMAS/Desktop/SoundWorks/projects
IntermediateDirectory  :=./hrtf/Release
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
OutputFile             :=../hrtfgen/hrtfgen.exe
Preprocessors          :=$(PreprocessorSwitch)NDEBUG 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="hrtf.txt"
PCHCompileFlags        :=
MakeDirCommand         :=makedir
RcCmpOptions           := 
RcCompilerName         :=C:/MinGW/bin/windres.exe
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). $(IncludeSwitch)../thirdparty/SDL2/include/ $(IncludeSwitch)../ 
IncludePCH             := 
RcIncludePath          := 
Libs                   := $(LibrarySwitch)mingw32 $(LibrarySwitch)opengl32 $(LibrarySwitch)glu32 $(LibrarySwitch)SDL2main $(LibrarySwitch)SDL2 $(LibrarySwitch)SDL2_ttf 
ArLibs                 :=  "libmingw32" "libopengl32" "libglu32" "libSDL2main" "libSDL2" "libSDL2_ttf" 
LibPath                := $(LibraryPathSwitch). $(LibraryPathSwitch). $(LibraryPathSwitch)../thirdparty/SDL2/lib/ 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := C:/MinGW/bin/ar.exe rcu
CXX      := C:/MinGW/bin/g++.exe
CC       := C:/MinGW/bin/gcc.exe
CXXFLAGS :=   $(Preprocessors)
CFLAGS   :=  -ggdb -O3 -Wall -pedantic -ansi -msse -msse2 -ffast-math  $(Preprocessors)
ASFLAGS  := 
AS       := C:/MinGW/bin/as.exe


##
## User defined environment variables
##
CodeLiteDir:=C:\Program Files (x86)\CodeLite
Objects0=$(IntermediateDirectory)/up_src_quat.c$(ObjectSuffix) $(IntermediateDirectory)/up_src_mat4.c$(ObjectSuffix) $(IntermediateDirectory)/up_src_util.c$(ObjectSuffix) $(IntermediateDirectory)/up_src_memory.c$(ObjectSuffix) $(IntermediateDirectory)/up_src_ray.c$(ObjectSuffix) $(IntermediateDirectory)/up_src_vec3.c$(ObjectSuffix) $(IntermediateDirectory)/up_src_hrtfgen_path.c$(ObjectSuffix) $(IntermediateDirectory)/up_src_log.c$(ObjectSuffix) $(IntermediateDirectory)/up_src_hrtfgen_hrtfgen.c$(ObjectSuffix) $(IntermediateDirectory)/up_src_octree.c$(ObjectSuffix) \
	$(IntermediateDirectory)/up_src_aabb.c$(ObjectSuffix) 



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
	@$(MakeDirCommand) "./hrtf/Release"


$(IntermediateDirectory)/.d:
	@$(MakeDirCommand) "./hrtf/Release"

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/up_src_quat.c$(ObjectSuffix): ../src/quat.c $(IntermediateDirectory)/up_src_quat.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/Users/mrDIMAS/Desktop/SoundWorks/src/quat.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_quat.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_quat.c$(DependSuffix): ../src/quat.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_quat.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_quat.c$(DependSuffix) -MM ../src/quat.c

$(IntermediateDirectory)/up_src_quat.c$(PreprocessSuffix): ../src/quat.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_quat.c$(PreprocessSuffix) ../src/quat.c

$(IntermediateDirectory)/up_src_mat4.c$(ObjectSuffix): ../src/mat4.c $(IntermediateDirectory)/up_src_mat4.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/Users/mrDIMAS/Desktop/SoundWorks/src/mat4.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_mat4.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_mat4.c$(DependSuffix): ../src/mat4.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_mat4.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_mat4.c$(DependSuffix) -MM ../src/mat4.c

$(IntermediateDirectory)/up_src_mat4.c$(PreprocessSuffix): ../src/mat4.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_mat4.c$(PreprocessSuffix) ../src/mat4.c

$(IntermediateDirectory)/up_src_util.c$(ObjectSuffix): ../src/util.c $(IntermediateDirectory)/up_src_util.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/Users/mrDIMAS/Desktop/SoundWorks/src/util.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_util.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_util.c$(DependSuffix): ../src/util.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_util.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_util.c$(DependSuffix) -MM ../src/util.c

$(IntermediateDirectory)/up_src_util.c$(PreprocessSuffix): ../src/util.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_util.c$(PreprocessSuffix) ../src/util.c

$(IntermediateDirectory)/up_src_memory.c$(ObjectSuffix): ../src/memory.c $(IntermediateDirectory)/up_src_memory.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/Users/mrDIMAS/Desktop/SoundWorks/src/memory.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_memory.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_memory.c$(DependSuffix): ../src/memory.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_memory.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_memory.c$(DependSuffix) -MM ../src/memory.c

$(IntermediateDirectory)/up_src_memory.c$(PreprocessSuffix): ../src/memory.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_memory.c$(PreprocessSuffix) ../src/memory.c

$(IntermediateDirectory)/up_src_ray.c$(ObjectSuffix): ../src/ray.c $(IntermediateDirectory)/up_src_ray.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/Users/mrDIMAS/Desktop/SoundWorks/src/ray.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_ray.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_ray.c$(DependSuffix): ../src/ray.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_ray.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_ray.c$(DependSuffix) -MM ../src/ray.c

$(IntermediateDirectory)/up_src_ray.c$(PreprocessSuffix): ../src/ray.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_ray.c$(PreprocessSuffix) ../src/ray.c

$(IntermediateDirectory)/up_src_vec3.c$(ObjectSuffix): ../src/vec3.c $(IntermediateDirectory)/up_src_vec3.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/Users/mrDIMAS/Desktop/SoundWorks/src/vec3.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_vec3.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_vec3.c$(DependSuffix): ../src/vec3.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_vec3.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_vec3.c$(DependSuffix) -MM ../src/vec3.c

$(IntermediateDirectory)/up_src_vec3.c$(PreprocessSuffix): ../src/vec3.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_vec3.c$(PreprocessSuffix) ../src/vec3.c

$(IntermediateDirectory)/up_src_hrtfgen_path.c$(ObjectSuffix): ../src/hrtfgen/path.c $(IntermediateDirectory)/up_src_hrtfgen_path.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/Users/mrDIMAS/Desktop/SoundWorks/src/hrtfgen/path.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_hrtfgen_path.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_hrtfgen_path.c$(DependSuffix): ../src/hrtfgen/path.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_hrtfgen_path.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_hrtfgen_path.c$(DependSuffix) -MM ../src/hrtfgen/path.c

$(IntermediateDirectory)/up_src_hrtfgen_path.c$(PreprocessSuffix): ../src/hrtfgen/path.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_hrtfgen_path.c$(PreprocessSuffix) ../src/hrtfgen/path.c

$(IntermediateDirectory)/up_src_log.c$(ObjectSuffix): ../src/log.c $(IntermediateDirectory)/up_src_log.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/Users/mrDIMAS/Desktop/SoundWorks/src/log.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_log.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_log.c$(DependSuffix): ../src/log.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_log.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_log.c$(DependSuffix) -MM ../src/log.c

$(IntermediateDirectory)/up_src_log.c$(PreprocessSuffix): ../src/log.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_log.c$(PreprocessSuffix) ../src/log.c

$(IntermediateDirectory)/up_src_hrtfgen_hrtfgen.c$(ObjectSuffix): ../src/hrtfgen/hrtfgen.c $(IntermediateDirectory)/up_src_hrtfgen_hrtfgen.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/Users/mrDIMAS/Desktop/SoundWorks/src/hrtfgen/hrtfgen.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_hrtfgen_hrtfgen.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_hrtfgen_hrtfgen.c$(DependSuffix): ../src/hrtfgen/hrtfgen.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_hrtfgen_hrtfgen.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_hrtfgen_hrtfgen.c$(DependSuffix) -MM ../src/hrtfgen/hrtfgen.c

$(IntermediateDirectory)/up_src_hrtfgen_hrtfgen.c$(PreprocessSuffix): ../src/hrtfgen/hrtfgen.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_hrtfgen_hrtfgen.c$(PreprocessSuffix) ../src/hrtfgen/hrtfgen.c

$(IntermediateDirectory)/up_src_octree.c$(ObjectSuffix): ../src/octree.c $(IntermediateDirectory)/up_src_octree.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/Users/mrDIMAS/Desktop/SoundWorks/src/octree.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_octree.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_octree.c$(DependSuffix): ../src/octree.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_octree.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_octree.c$(DependSuffix) -MM ../src/octree.c

$(IntermediateDirectory)/up_src_octree.c$(PreprocessSuffix): ../src/octree.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_octree.c$(PreprocessSuffix) ../src/octree.c

$(IntermediateDirectory)/up_src_aabb.c$(ObjectSuffix): ../src/aabb.c $(IntermediateDirectory)/up_src_aabb.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/Users/mrDIMAS/Desktop/SoundWorks/src/aabb.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_aabb.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_aabb.c$(DependSuffix): ../src/aabb.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_aabb.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_aabb.c$(DependSuffix) -MM ../src/aabb.c

$(IntermediateDirectory)/up_src_aabb.c$(PreprocessSuffix): ../src/aabb.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_aabb.c$(PreprocessSuffix) ../src/aabb.c


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./hrtf/Release/


