CXXFLAGS = -O3 -std=c++11 -ltcmalloc -stdlib=libc++ 
LOGGING = -DLOGGING

.PHONEY: all clean 

all: game

clean:
	$(RM) $(OBJS)
	$(RM) $(TEST_OBJS)
	$(RM) $(PRECOMPILE_HEADERS)

game: model.cc
	$(CXX) $^ -o $@ $(CXXFLAGS)
