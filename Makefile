CONTIKI_PROJECT = dconcrect-server dconcrect-client
all: $(CONTIKI_PROJECT)
CONTIKI = ../..
PROJECT_SOURCEFILES += traffic_classifier.c dynamic_threshold.c
include $(CONTIKI)/Makefile.include
