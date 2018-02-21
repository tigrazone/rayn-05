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

EXE      = rayn.exe
SRCS     =          \
	rayn.cpp    
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

# defines
CPPFLAGS = -MMD -Inr
CXXFLAGS = -Wall $(OPTFLAGS)
LDFLAGS  = -Lnr -lm
LIBS     = -lnr
VPATH    = rayn

# rules
$(EXE)	: $(OBJS) $(LIBS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) -o $(EXE) $(OBJS) $(LIBS) 

-lnr	:
	$(MAKE) -C nr

# clean
clean	:
	-$(RM) $(EXE) $(OBJS) $(OBJS:.o=.d) *~
	-$(MAKE) -C nr clean

# dependencies
-include $(OBJS:.o=.d)
