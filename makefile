warnings=-Wall -Weffc++ -pedantic -Wextra
CXXFLAGS=-std=c++17 $(warnings) -O2 -DNDEBUG
LDFLAGS=-mwindows -static -s
LDLIBS=-lgdi32 -luser32 -lkernel32 -lwinmm
src_folders=src
src_files=$(foreach p,$(src_folders),$(wildcard $(p)/*.cpp))
objdir=temp
objs=$(addprefix $(objdir)/,$(addsuffix .o,$(notdir $(basename $(src_files)))))
target=$(objdir)/snaketysnake.exe
# Target.
all: $(target)
	@echo Done. Output file is ./$(target)
# Link.
$(target): $(objs)
	$(CXX) -o $(target) $(objs) $(LDFLAGS) $(LDLIBS)
# Compile object files.
$(objdir)/%.o: src/%.cpp | $(objdir)
	$(CXX) $(CXXFLAGS) -o $@ -c $<
# Create temp/ folder.
$(objdir):
	mkdir $@
# Clean temp folder.
clean:
	del $(objdir)\*.o