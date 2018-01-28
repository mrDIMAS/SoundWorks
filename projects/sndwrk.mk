##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=sndwrk
ConfigurationName      :=Release
WorkspacePath          :=C:/Users/mrDIMAS/Desktop/SoundWorks/projects
ProjectPath            :=C:/Users/mrDIMAS/Desktop/SoundWorks/projects
IntermediateDirectory  :=./sndwrk/Release
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=mrDIMAS
Date                   :=21/01/2018
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
OutputFile             :=../lib/libsndwrk.dll
Preprocessors          :=$(PreprocessorSwitch)NDEBUG 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="sndwrk.txt"
PCHCompileFlags        :=
MakeDirCommand         :=makedir
RcCmpOptions           := 
RcCompilerName         :=C:/MinGW/bin/windres.exe
LinkOptions            :=  -Wl,--out-implib,../lib/libsndwrk.a -static-libgcc 
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). $(IncludeSwitch)../thirdparty/SDL2/include/ $(IncludeSwitch)../ 
IncludePCH             := 
RcIncludePath          := 
Libs                   := $(LibrarySwitch)mingw32 $(LibrarySwitch)dsound $(LibrarySwitch)vorbisfile $(LibrarySwitch)vorbis $(LibrarySwitch)ogg $(LibrarySwitch)pthread 
ArLibs                 :=  "libmingw32" "libdsound" "libvorbisfile" "libvorbis" "libogg.a" "libpthread" 
LibPath                := $(LibraryPathSwitch). 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := C:/MinGW/bin/ar.exe rcu
CXX      := C:/MinGW/bin/g++.exe
CC       := C:/MinGW/bin/gcc.exe
CXXFLAGS :=   $(Preprocessors)
CFLAGS   :=  -g -shared -O3 -Wall -pedantic -ansi  $(Preprocessors)
ASFLAGS  := 
AS       := C:/MinGW/bin/as.exe


##
## User defined environment variables
##
CodeLiteDir:=C:\Program Files (x86)\CodeLite
Objects0=$(IntermediateDirectory)/up_src_hiresclock.c$(ObjectSuffix) $(IntermediateDirectory)/up_src_hrtf.c$(ObjectSuffix) $(IntermediateDirectory)/up_src_dsp.c$(ObjectSuffix) $(IntermediateDirectory)/up_src_zone.c$(ObjectSuffix) $(IntermediateDirectory)/up_src_vec3.c$(ObjectSuffix) $(IntermediateDirectory)/up_src_util.c$(ObjectSuffix) $(IntermediateDirectory)/up_src_frustum.c$(ObjectSuffix) $(IntermediateDirectory)/up_src_effect.c$(ObjectSuffix) $(IntermediateDirectory)/up_src_device.c$(ObjectSuffix) $(IntermediateDirectory)/up_src_octree.c$(ObjectSuffix) \
	$(IntermediateDirectory)/up_src_decoder_ogg.c$(ObjectSuffix) $(IntermediateDirectory)/up_src_sound.c$(ObjectSuffix) $(IntermediateDirectory)/up_src_aabb.c$(ObjectSuffix) $(IntermediateDirectory)/up_src_log.c$(ObjectSuffix) $(IntermediateDirectory)/up_src_ray.c$(ObjectSuffix) $(IntermediateDirectory)/up_src_plane.c$(ObjectSuffix) $(IntermediateDirectory)/up_src_listener.c$(ObjectSuffix) $(IntermediateDirectory)/up_src_buffer.c$(ObjectSuffix) $(IntermediateDirectory)/up_src_mixer.c$(ObjectSuffix) $(IntermediateDirectory)/up_src_quat.c$(ObjectSuffix) \
	$(IntermediateDirectory)/up_src_mat4.c$(ObjectSuffix) $(IntermediateDirectory)/up_src_memory.c$(ObjectSuffix) 



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
	$(SharedObjectLinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)
	@$(MakeDirCommand) "C:\Users\mrDIMAS\Desktop\SoundWorks\projects/.build-release"
	@echo rebuilt > "C:\Users\mrDIMAS\Desktop\SoundWorks\projects/.build-release/sndwrk"

PostBuild:
	@echo Executing Post Build commands ...
	cp ../lib/libsndwrk.dll ../test/
	@echo Done

MakeIntermediateDirs:
	@$(MakeDirCommand) "./sndwrk/Release"


$(IntermediateDirectory)/.d:
	@$(MakeDirCommand) "./sndwrk/Release"

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/up_src_hiresclock.c$(ObjectSuffix): ../src/hiresclock.c $(IntermediateDirectory)/up_src_hiresclock.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/Users/mrDIMAS/Desktop/SoundWorks/src/hiresclock.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_hiresclock.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_hiresclock.c$(DependSuffix): ../src/hiresclock.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_hiresclock.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_hiresclock.c$(DependSuffix) -MM ../src/hiresclock.c

$(IntermediateDirectory)/up_src_hiresclock.c$(PreprocessSuffix): ../src/hiresclock.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_hiresclock.c$(PreprocessSuffix) ../src/hiresclock.c

$(IntermediateDirectory)/up_src_hrtf.c$(ObjectSuffix): ../src/hrtf.c $(IntermediateDirectory)/up_src_hrtf.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/Users/mrDIMAS/Desktop/SoundWorks/src/hrtf.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_hrtf.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_hrtf.c$(DependSuffix): ../src/hrtf.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_hrtf.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_hrtf.c$(DependSuffix) -MM ../src/hrtf.c

$(IntermediateDirectory)/up_src_hrtf.c$(PreprocessSuffix): ../src/hrtf.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_hrtf.c$(PreprocessSuffix) ../src/hrtf.c

$(IntermediateDirectory)/up_src_dsp.c$(ObjectSuffix): ../src/dsp.c $(IntermediateDirectory)/up_src_dsp.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/Users/mrDIMAS/Desktop/SoundWorks/src/dsp.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_dsp.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_dsp.c$(DependSuffix): ../src/dsp.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_dsp.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_dsp.c$(DependSuffix) -MM ../src/dsp.c

$(IntermediateDirectory)/up_src_dsp.c$(PreprocessSuffix): ../src/dsp.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_dsp.c$(PreprocessSuffix) ../src/dsp.c

$(IntermediateDirectory)/up_src_zone.c$(ObjectSuffix): ../src/zone.c $(IntermediateDirectory)/up_src_zone.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/Users/mrDIMAS/Desktop/SoundWorks/src/zone.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_zone.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_zone.c$(DependSuffix): ../src/zone.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_zone.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_zone.c$(DependSuffix) -MM ../src/zone.c

$(IntermediateDirectory)/up_src_zone.c$(PreprocessSuffix): ../src/zone.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_zone.c$(PreprocessSuffix) ../src/zone.c

$(IntermediateDirectory)/up_src_vec3.c$(ObjectSuffix): ../src/vec3.c $(IntermediateDirectory)/up_src_vec3.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/Users/mrDIMAS/Desktop/SoundWorks/src/vec3.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_vec3.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_vec3.c$(DependSuffix): ../src/vec3.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_vec3.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_vec3.c$(DependSuffix) -MM ../src/vec3.c

$(IntermediateDirectory)/up_src_vec3.c$(PreprocessSuffix): ../src/vec3.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_vec3.c$(PreprocessSuffix) ../src/vec3.c

$(IntermediateDirectory)/up_src_util.c$(ObjectSuffix): ../src/util.c $(IntermediateDirectory)/up_src_util.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/Users/mrDIMAS/Desktop/SoundWorks/src/util.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_util.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_util.c$(DependSuffix): ../src/util.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_util.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_util.c$(DependSuffix) -MM ../src/util.c

$(IntermediateDirectory)/up_src_util.c$(PreprocessSuffix): ../src/util.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_util.c$(PreprocessSuffix) ../src/util.c

$(IntermediateDirectory)/up_src_frustum.c$(ObjectSuffix): ../src/frustum.c $(IntermediateDirectory)/up_src_frustum.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/Users/mrDIMAS/Desktop/SoundWorks/src/frustum.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_frustum.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_frustum.c$(DependSuffix): ../src/frustum.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_frustum.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_frustum.c$(DependSuffix) -MM ../src/frustum.c

$(IntermediateDirectory)/up_src_frustum.c$(PreprocessSuffix): ../src/frustum.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_frustum.c$(PreprocessSuffix) ../src/frustum.c

$(IntermediateDirectory)/up_src_effect.c$(ObjectSuffix): ../src/effect.c $(IntermediateDirectory)/up_src_effect.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/Users/mrDIMAS/Desktop/SoundWorks/src/effect.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_effect.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_effect.c$(DependSuffix): ../src/effect.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_effect.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_effect.c$(DependSuffix) -MM ../src/effect.c

$(IntermediateDirectory)/up_src_effect.c$(PreprocessSuffix): ../src/effect.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_effect.c$(PreprocessSuffix) ../src/effect.c

$(IntermediateDirectory)/up_src_device.c$(ObjectSuffix): ../src/device.c $(IntermediateDirectory)/up_src_device.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/Users/mrDIMAS/Desktop/SoundWorks/src/device.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_device.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_device.c$(DependSuffix): ../src/device.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_device.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_device.c$(DependSuffix) -MM ../src/device.c

$(IntermediateDirectory)/up_src_device.c$(PreprocessSuffix): ../src/device.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_device.c$(PreprocessSuffix) ../src/device.c

$(IntermediateDirectory)/up_src_octree.c$(ObjectSuffix): ../src/octree.c $(IntermediateDirectory)/up_src_octree.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/Users/mrDIMAS/Desktop/SoundWorks/src/octree.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_octree.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_octree.c$(DependSuffix): ../src/octree.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_octree.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_octree.c$(DependSuffix) -MM ../src/octree.c

$(IntermediateDirectory)/up_src_octree.c$(PreprocessSuffix): ../src/octree.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_octree.c$(PreprocessSuffix) ../src/octree.c

$(IntermediateDirectory)/up_src_decoder_ogg.c$(ObjectSuffix): ../src/decoder_ogg.c $(IntermediateDirectory)/up_src_decoder_ogg.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/Users/mrDIMAS/Desktop/SoundWorks/src/decoder_ogg.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_decoder_ogg.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_decoder_ogg.c$(DependSuffix): ../src/decoder_ogg.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_decoder_ogg.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_decoder_ogg.c$(DependSuffix) -MM ../src/decoder_ogg.c

$(IntermediateDirectory)/up_src_decoder_ogg.c$(PreprocessSuffix): ../src/decoder_ogg.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_decoder_ogg.c$(PreprocessSuffix) ../src/decoder_ogg.c

$(IntermediateDirectory)/up_src_sound.c$(ObjectSuffix): ../src/sound.c $(IntermediateDirectory)/up_src_sound.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/Users/mrDIMAS/Desktop/SoundWorks/src/sound.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_sound.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_sound.c$(DependSuffix): ../src/sound.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_sound.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_sound.c$(DependSuffix) -MM ../src/sound.c

$(IntermediateDirectory)/up_src_sound.c$(PreprocessSuffix): ../src/sound.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_sound.c$(PreprocessSuffix) ../src/sound.c

$(IntermediateDirectory)/up_src_aabb.c$(ObjectSuffix): ../src/aabb.c $(IntermediateDirectory)/up_src_aabb.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/Users/mrDIMAS/Desktop/SoundWorks/src/aabb.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_aabb.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_aabb.c$(DependSuffix): ../src/aabb.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_aabb.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_aabb.c$(DependSuffix) -MM ../src/aabb.c

$(IntermediateDirectory)/up_src_aabb.c$(PreprocessSuffix): ../src/aabb.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_aabb.c$(PreprocessSuffix) ../src/aabb.c

$(IntermediateDirectory)/up_src_log.c$(ObjectSuffix): ../src/log.c $(IntermediateDirectory)/up_src_log.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/Users/mrDIMAS/Desktop/SoundWorks/src/log.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_log.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_log.c$(DependSuffix): ../src/log.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_log.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_log.c$(DependSuffix) -MM ../src/log.c

$(IntermediateDirectory)/up_src_log.c$(PreprocessSuffix): ../src/log.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_log.c$(PreprocessSuffix) ../src/log.c

$(IntermediateDirectory)/up_src_ray.c$(ObjectSuffix): ../src/ray.c $(IntermediateDirectory)/up_src_ray.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/Users/mrDIMAS/Desktop/SoundWorks/src/ray.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_ray.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_ray.c$(DependSuffix): ../src/ray.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_ray.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_ray.c$(DependSuffix) -MM ../src/ray.c

$(IntermediateDirectory)/up_src_ray.c$(PreprocessSuffix): ../src/ray.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_ray.c$(PreprocessSuffix) ../src/ray.c

$(IntermediateDirectory)/up_src_plane.c$(ObjectSuffix): ../src/plane.c $(IntermediateDirectory)/up_src_plane.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/Users/mrDIMAS/Desktop/SoundWorks/src/plane.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_plane.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_plane.c$(DependSuffix): ../src/plane.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_plane.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_plane.c$(DependSuffix) -MM ../src/plane.c

$(IntermediateDirectory)/up_src_plane.c$(PreprocessSuffix): ../src/plane.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_plane.c$(PreprocessSuffix) ../src/plane.c

$(IntermediateDirectory)/up_src_listener.c$(ObjectSuffix): ../src/listener.c $(IntermediateDirectory)/up_src_listener.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/Users/mrDIMAS/Desktop/SoundWorks/src/listener.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_listener.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_listener.c$(DependSuffix): ../src/listener.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_listener.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_listener.c$(DependSuffix) -MM ../src/listener.c

$(IntermediateDirectory)/up_src_listener.c$(PreprocessSuffix): ../src/listener.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_listener.c$(PreprocessSuffix) ../src/listener.c

$(IntermediateDirectory)/up_src_buffer.c$(ObjectSuffix): ../src/buffer.c $(IntermediateDirectory)/up_src_buffer.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/Users/mrDIMAS/Desktop/SoundWorks/src/buffer.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_buffer.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_buffer.c$(DependSuffix): ../src/buffer.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_buffer.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_buffer.c$(DependSuffix) -MM ../src/buffer.c

$(IntermediateDirectory)/up_src_buffer.c$(PreprocessSuffix): ../src/buffer.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_buffer.c$(PreprocessSuffix) ../src/buffer.c

$(IntermediateDirectory)/up_src_mixer.c$(ObjectSuffix): ../src/mixer.c $(IntermediateDirectory)/up_src_mixer.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/Users/mrDIMAS/Desktop/SoundWorks/src/mixer.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_mixer.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_mixer.c$(DependSuffix): ../src/mixer.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_mixer.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_mixer.c$(DependSuffix) -MM ../src/mixer.c

$(IntermediateDirectory)/up_src_mixer.c$(PreprocessSuffix): ../src/mixer.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_mixer.c$(PreprocessSuffix) ../src/mixer.c

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

$(IntermediateDirectory)/up_src_memory.c$(ObjectSuffix): ../src/memory.c $(IntermediateDirectory)/up_src_memory.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/Users/mrDIMAS/Desktop/SoundWorks/src/memory.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_memory.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_memory.c$(DependSuffix): ../src/memory.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_memory.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_memory.c$(DependSuffix) -MM ../src/memory.c

$(IntermediateDirectory)/up_src_memory.c$(PreprocessSuffix): ../src/memory.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_memory.c$(PreprocessSuffix) ../src/memory.c


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./sndwrk/Release/


