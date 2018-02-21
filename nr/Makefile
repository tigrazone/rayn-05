# 
#  make BUILD=release
#   optimized build (default)
#
#  make BUILD=profile
#   optimized build with profile info
#
#  make BUILD=debug
#   debug build
#

LIB      = libnr.a
SRCS     =                    \
	nrBasis.cpp           \
	nrBound.cpp           \
	nrChannel.cpp         \
	nrChannelColor.cpp    \
	nrChannelMarble.cpp   \
	nrCmdLine.cpp         \
	nrImage.cpp           \
	nrImagePCX.cpp        \
	nrImagePPM.cpp        \
	nrImageTGA.cpp        \
	nrLight.cpp           \
	nrLog.cpp             \
	nrMaterial.cpp        \
	nrMatrix.cpp          \
	nrModel.cpp           \
	nrModel3DS.cpp        \
	nrModelMTL.cpp        \
	nrModelOBJ.cpp        \
	nrParser.cpp          \
	nrPath.cpp            \
	nrPencil.cpp          \
	nrPixel.cpp           \
	nrScene.cpp           \
	nrStopWatch.cpp       \
	nrSurface.cpp         \
	nrSurfaceBox.cpp      \
	nrSurfaceBVH.cpp      \
	nrSurfaceRGS.cpp      \
	nrSurfaceSphere.cpp   \
	nrSurfaceTriangle.cpp \
	nrTimer.cpp           \
	nrView.cpp            
OBJS     = $(SRCS:.cpp=.o)

BUILD=release
ifeq ($(BUILD),release)
OPTFLAGS = -O3 -DNDEBUG
else
ifeq ($(BUILD),profile)
OPTFLAGS = -pg -O3 -DNDEBUG
else
OPTFLAGS = -g -D_DEBUG
endif
endif

CPPFLAGS = -MMD
CXXFLAGS = -Wall $(OPTFLAGS)
LDFLAGS  = -Lnr -lm
LIBS     = -lnr

# rules
$(LIB)	: $(OBJS)
	$(AR) cr $(LIB) $(OBJS)

# clean
clean	:
	-$(RM) $(LIB) $(OBJS) $(OBJS:.o=.d) *~

# dependencies
-include $(OBJS:.o=.d)
