
# 
# It's also a good idea to add target dependencies for header files.
# Otherwise, make will think that everything is up-to-date even if
# you've changed the header file definitions.
# 
# Credit for this Makefile goes to Mark Royer
# http://www.gnu.org/software/make/manual/make.html

CFLAGS = -std=c99 -O2 -g -Wall -fmessage-length=0

# Make sure to modify these three variables for the project submission!
LASTNAME = Stevenson
ASSIGNMENT = Project1
VERSION =  0001
TARGET = Project1.out
FNAME = $(LASTNAME)-$(ASSIGNMENT)-$(VERSION)
SRCS = $(wildcard *.c)
HEADERS = $(wildcard *.h)
OBJS = $(SRCS:%.c=%.o)
DOCDIR = docs

# The content between these dashes is automatically created in the project 
# named interp.
# --------------------------------------------

all: $(TARGET) 

-include $(OBJS:%.o=%.d)


$(TARGET): $(OBJS) $(HEADERS)
	$(CC) -o $(TARGET) $(OBJS)

# --------------------------------------------

clean:
	rm -rf *~ $(OBJS) $(OBJS:%.o=%.d) $(TARGET) $(DOCDIR) $(FNAME).tgz
docs: $(TARGET) README.dox
	doxygen Doxyfile

submit: clean
	cp -r $(PWD) /tmp/$(FNAME)
	tar -cvz -C /tmp/ -f $(FNAME).tgz $(FNAME)
	rm -r /tmp/$(FNAME)
