SOURCES = main.c alarm.c bankedText.c burtle_jsf32.c common.c damage.c emplacement.c hiring_hall.c insystem.c jump.c jump-map.c maneuver-map.c menu.c name.c panel.c passengers.c ship.c shipyard.c sprite.c starport.c survey.c trade.c wilderness.c world.c

PROGRAM = TRADER

CC65_TARGET = cx16

CC	= cl65 
CFLAGS 	= --cpu 65c02 -t $(CC65_TARGET) --create-dep $(<:.c=.d) -Ors 
LDFLAGS	= -t $(CC65_TARGET) -m $(PROGRAM).map
OBJDIR  = .obj

#############################################
OBJECTS = $(SOURCES:%.c=$(OBJDIR)/%.o)

.SUFFIXES: 
all: $(PROGRAM)

ifneq (($MAKECMDGOALS),clean)
-include $(SOURCES:.c=.d)
endif

clean:
	$(RM) $(OBJECTS) $(SOURCES:.c=.d) $(PROGRAM) $(PROGRAM).map

.PHONY: all clean

$(PROGRAM): $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $^

$(OBJECTS): $(OBJDIR)/%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<


