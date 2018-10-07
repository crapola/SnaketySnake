warnings=-Wall -Weffc++ -pedantic -Wextra
CXXFLAGS=-std=c++17 $(warnings) -O2 -DNDEBUG
LDFLAGS=-mwindows -static -s
LDLIBS=-lgdi32 -luser32 -lkernel32 -lwinmm
src_folders=src
src_files=$(foreach p,$(src_folders),$(wildcard $(p)/*.cpp))
objdir=temp
objs=$(addprefix $(objdir)/,$(addsuffix .o,$(notdir $(basename $(src_files)))))
# Target.
all: $(objdir)/snaketysnake.exe
	@echo Done. Output file is ./$(objdir)/snaketysnake.exe
# Link.
$(objdir)/snaketysnake.exe: $(objs)
	$(CXX) -o $(objdir)/snaketysnake.exe $(objs) $(LDFLAGS) $(LDLIBS)
# Clean temp folder.
clean:
	del $(objdir)\*.o
# Compile object files.
$(objdir)/%.o: src/%.cpp | $(objdir)
	$(CXX) $(CXXFLAGS) -o $@ -c $<
# Create temp/ folder.
$(objdir):
	mkdir $@