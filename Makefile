CC = g++
CFLAGS = -Isrc -Isrc/Interpreter -Isrc/Catalog -Isrc/Record -Isrc/Buffer -Iinclude -O2
OBJSDIR = .objs
VPATH = src:src/Interpreter:src/Catalog:src/Record:src/Buffer:include
OBJS = main.o interpreter.o catalog.o record.o buffer.o
TARGET = minisql 
$(OBJSDIR):
	mkdir -p ./$@
$(TARGET):$(OBJSDIR) $(OBJS)
	$(CC) -o $(TARGET) $(OBJSDIR)/*.o $(CFLAGS) -g
$(OBJS):%.o:%.cpp
	$(CC) -c $(CFLAGS) $< -o $(OBJSDIR)/$@ -g
clean:
	-$(RM) $(TARGET)
	-$(RM) $(OBJSDIR)/*.o
