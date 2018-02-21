# Microsoft Developer Studio Project File - Name="nr" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=nr - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "nr.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "nr.mak" CFG="nr - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "nr - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "nr - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "nr - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "nr - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "nr - Win32 Release"
# Name "nr - Win32 Debug"
# Begin Group "contain"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\nrArray.h
# End Source File
# Begin Source File

SOURCE=.\nrArray.inl
# End Source File
# Begin Source File

SOURCE=.\nrLibrary.h
# End Source File
# Begin Source File

SOURCE=.\nrLibrary.inl
# End Source File
# Begin Source File

SOURCE=.\nrList.h
# End Source File
# Begin Source File

SOURCE=.\nrList.inl
# End Source File
# End Group
# Begin Group "image"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\nrColor.h
# End Source File
# Begin Source File

SOURCE=.\nrColor.inl
# End Source File
# Begin Source File

SOURCE=.\nrColorRamp.cpp
# End Source File
# Begin Source File

SOURCE=.\nrColorRamp.h
# End Source File
# Begin Source File

SOURCE=.\nrImage.cpp
# End Source File
# Begin Source File

SOURCE=.\nrImage.h
# End Source File
# Begin Source File

SOURCE=.\nrImagePCX.cpp
# End Source File
# Begin Source File

SOURCE=.\nrImagePPM.cpp
# End Source File
# Begin Source File

SOURCE=.\nrImageTGA.cpp
# End Source File
# Begin Source File

SOURCE=.\nrPixel.cpp
# End Source File
# Begin Source File

SOURCE=.\nrPixel.h
# End Source File
# End Group
# Begin Group "io"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\nrCmdLine.cpp
# End Source File
# Begin Source File

SOURCE=.\nrCmdLine.h
# End Source File
# Begin Source File

SOURCE=.\nrLog.cpp
# End Source File
# Begin Source File

SOURCE=.\nrLog.h
# End Source File
# Begin Source File

SOURCE=.\nrParser.cpp
# End Source File
# Begin Source File

SOURCE=.\nrParser.h
# End Source File
# Begin Source File

SOURCE=.\nrPath.cpp
# End Source File
# Begin Source File

SOURCE=.\nrPath.h
# End Source File
# Begin Source File

SOURCE=.\nrPencil.cpp
# End Source File
# Begin Source File

SOURCE=.\nrPencil.h
# End Source File
# Begin Source File

SOURCE=.\nrProgress.h
# End Source File
# Begin Source File

SOURCE=.\nrProgress.inl
# End Source File
# End Group
# Begin Group "material"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\nrChannel.cpp
# End Source File
# Begin Source File

SOURCE=.\nrChannel.h
# End Source File
# Begin Source File

SOURCE=.\nrChannelColor.cpp
# End Source File
# Begin Source File

SOURCE=.\nrChannelColor.h
# End Source File
# Begin Source File

SOURCE=.\nrChannelMarble.cpp
# End Source File
# Begin Source File

SOURCE=.\nrChannelMarble.h
# End Source File
# Begin Source File

SOURCE=.\nrMaterial.cpp
# End Source File
# Begin Source File

SOURCE=.\nrMaterial.h
# End Source File
# End Group
# Begin Group "math"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\nrBasis.cpp
# End Source File
# Begin Source File

SOURCE=.\nrBasis.h
# End Source File
# Begin Source File

SOURCE=.\nrInterval.h
# End Source File
# Begin Source File

SOURCE=.\nrInterval.inl
# End Source File
# Begin Source File

SOURCE=.\nrMath.h
# End Source File
# Begin Source File

SOURCE=.\nrMath.inl
# End Source File
# Begin Source File

SOURCE=.\nrNoise.cpp
# End Source File
# Begin Source File

SOURCE=.\nrNoise.h
# End Source File
# Begin Source File

SOURCE=.\nrNoise.inl
# End Source File
# Begin Source File

SOURCE=.\nrVector2.h
# End Source File
# Begin Source File

SOURCE=.\nrVector2.inl
# End Source File
# Begin Source File

SOURCE=.\nrVector3.h
# End Source File
# Begin Source File

SOURCE=.\nrVector3.inl
# End Source File
# Begin Source File

SOURCE=.\nrView.cpp
# End Source File
# Begin Source File

SOURCE=.\nrView.h
# End Source File
# End Group
# Begin Group "model"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\nrModel.cpp
# End Source File
# Begin Source File

SOURCE=.\nrModel.h
# End Source File
# Begin Source File

SOURCE=.\nrModel3DS.cpp
# End Source File
# Begin Source File

SOURCE=.\nrModel3DS.h
# End Source File
# Begin Source File

SOURCE=.\nrModelMTL.cpp
# End Source File
# Begin Source File

SOURCE=.\nrModelMTL.h
# End Source File
# Begin Source File

SOURCE=.\nrModelOBJ.cpp
# End Source File
# Begin Source File

SOURCE=.\nrModelOBJ.h
# End Source File
# End Group
# Begin Group "ray"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\nrHit.h
# End Source File
# Begin Source File

SOURCE=.\nrHit.inl
# End Source File
# Begin Source File

SOURCE=.\nrLight.cpp
# End Source File
# Begin Source File

SOURCE=.\nrLight.h
# End Source File
# Begin Source File

SOURCE=.\nrRay.h
# End Source File
# Begin Source File

SOURCE=.\nrRay.inl
# End Source File
# End Group
# Begin Group "surface"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\nrBound.cpp
# End Source File
# Begin Source File

SOURCE=.\nrBound.h
# End Source File
# Begin Source File

SOURCE=.\nrScene.cpp
# End Source File
# Begin Source File

SOURCE=.\nrScene.h
# End Source File
# Begin Source File

SOURCE=.\nrSurface.cpp
# End Source File
# Begin Source File

SOURCE=.\nrSurface.h
# End Source File
# Begin Source File

SOURCE=.\nrSurfaceBox.cpp
# End Source File
# Begin Source File

SOURCE=.\nrSurfaceBox.h
# End Source File
# Begin Source File

SOURCE=.\nrSurfaceBVH.cpp
# End Source File
# Begin Source File

SOURCE=.\nrSurfaceBVH.h
# End Source File
# Begin Source File

SOURCE=.\nrSurfaceRGS.cpp
# End Source File
# Begin Source File

SOURCE=.\nrSurfaceRGS.h
# End Source File
# Begin Source File

SOURCE=.\nrSurfaceSphere.cpp
# End Source File
# Begin Source File

SOURCE=.\nrSurfaceSphere.h
# End Source File
# Begin Source File

SOURCE=.\nrSurfaceTriangle.cpp
# End Source File
# Begin Source File

SOURCE=.\nrSurfaceTriangle.h
# End Source File
# End Group
# Begin Group "time"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\nrStopWatch.cpp
# End Source File
# Begin Source File

SOURCE=.\nrStopWatch.h
# End Source File
# End Group
# End Target
# End Project
