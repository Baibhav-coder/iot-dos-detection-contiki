CONTIKI_PROJECT = dconcrect-server dconcrect-server-phase2 dconcrect-server-baseline dconcrect-client
all: $(CONTIKI_PROJECT)
CONTIKI = ../..
PROJECT_SOURCEFILES += traffic_classifier.c dynamic_threshold.c
include $(CONTIKI)/Makefile.include
