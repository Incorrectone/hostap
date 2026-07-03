all: libwpavarlink.a

clean:
	rm -f *~ *.o *.d *.gcno *.gcda *.gcov
	rm -f libwpavarlink.a

install:
	@echo Nothing to be made.

ifndef CC
CC=gcc
endif

ifndef CFLAGS
CFLAGS = -MMD -O2 -Wall -g
endif

PKG_CONFIG ?= pkg-config
CFLAGS += -I../../src -I../../src/utils


Q=@
E=echo
ifeq ($(V), 1)
Q=
E=true
endif

%.o: %.c
	$(Q)$(CC) -c -o $@ $(CFLAGS) $<
	@$(E) "  CC " $<


ifdef CONFIG_WPS
CFLAGS += -DCONFIG_WPS
endif

CFLAGS += -DCONFIG_CTRL_IFACE_VARLINK

ifndef VARLINK_LIBS
VARLINK_LIBS := $(shell $(PKG_CONFIG) --libs libvarlink)
endif
ifndef VARLINK_INCLUDE
VARLINK_INCLUDE := $(shell $(PKG_CONFIG) --cflags libvarlink)
endif

CFLAGS += $(VARLINK_INCLUDE)

LIB_OBJS= \
	varlink_server.o \
	varlink_methods.o \
	varlink_events.o 

libwpavarlink.a: $(LIB_OBJS)
	$(AR) crT $@ $?

-include $(OBJS:%.o=%.d)
