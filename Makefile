CONTIKI_PROJECT = dconcrect-server dconcrect-client
all: $(CONTIKI_PROJECT)
CONTIKI = ../..
PROJECT_SOURCEFILES += traffic_classifier.c
include $(CONTIKI)/Makefile.include
