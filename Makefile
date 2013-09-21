CC = g++
CFLAGS = -Isrc -Isrc/Interpreter -Iinclude -O2
OBJSDIR = .objs
VPATH = src:src/Interpreter:include
OBJS = main.o interpreter.o
TARGET = minisql 
$(OBJSDIR):
	mkdir -p ./$@
$(TARGET):$(OBJSDIR) $(OBJS)
	$(CC) -o $(TARGET) $(OBJSDIR)/*.o $(CFLAGS)
$(OBJS):%.o:%.cpp
	$(CC) -c $(CFLAGS) $< -o $(OBJSDIR)/$@
clean:
	-$(RM) $(TARGET)
	-$(RM) $(OBJSDIR)/*.o
