
_default: dirs
	@echo "\"make test\" for debug builds, \"make release\" to build a release"

OBJECT_DIR = .temp/object
PROJECT_NAME = crone

.NOTINTERMEDIATE: # all

LINKS = -lX11 -lvulkan
HEADERS = -I $(PROJECT_NAME)/interface
ERRORS = -Wfatal-errors -Wall -Werror=use-after-free -Wno-unused-variable -Wpedantic
%_dev: VERSION_FLAGS = -Og -DDO_VALIDATION -fsanitize=address,undefined
%_release: VERSION_FLAGS = -O3
FLAGS = -std=c23 $(ERRORS) $(VERSION_FLAGS) $(HEADERS) -DUSE_X11

GCC = gcc $(FLAGS) -o

define MAKE_OBJECT
	@echo "[makefile] making $@"
	@$(GCC) $@ $^ -c
endef

.temp/make: $(shell find $(PROJECT_NAME) -type f -name '*.c')
	@find $(PROJECT_NAME) -type f -name '*.c' > .temp/cfiles
	@sed 's/$(PROJECT_NAME)\/\(.*\)\.c/\1/g;s/\//_/' .temp/cfiles > .temp/object_names
	@paste -d " " .temp/cfiles .temp/object_names > .temp/build_plan
	@sed 's/\(.*\) \(.*\)/$$(OBJECT_DIR)\/\2_%: \1\n\t$$(MAKE_OBJECT)/g' .temp/build_plan > $@
	@echo -n ".temp/lib_%: " >> $@
	@sed 's/.* \(.*\)/$$(OBJECT_DIR)\/\1_%/g' .temp/build_plan | tr "\n" " " >> $@
	@echo -n -e "\n\t@echo \"[.temp/make] linking \$$@\"" >> $@
	@echo -n -e "\n\t@ld -r -o \$$@ $$^" >> $@

ifneq ("$(wildcard .temp)","")
include .temp/make
else
.temp/lib_%: dirs
	make $@
endif

%_dev: SUFFIX = _dev
%_release: SUFFIX = _release
%_dev: %
%_release: %
program/$(PROJECT_NAME)_%: program.c .temp/lib_%
	@echo "[makefile] making $@"
	@$(GCC) $@ $^ $(LINKS)

.PHONY: _default dirs test clean release self objects immaculate

dirs:
	@mkdir -p program $(OBJECT_DIR)

test: objects program/$(PROJECT_NAME)_dev
	@echo -e "\n[makefile] launching..."
	@program/$(PROJECT_NAME)_dev

DATE := $(shell date '+%d.%m.%Y')
release: objects program/$(PROJECT_NAME)_release
	@cp program/$(PROJECT_NAME)_release program/$(PROJECT_NAME)_release_$(DATE)
	@echo -e "\n[makefile] built program/$(PROJECT_NAME)_release_$(DATE)"

clean: dirs
	@rm -rf ./.temp/*
	@rm -f ./program/*
	@mkdir -p program $(OBJECT_DIR)

immaculate:
	@rm -rf .temp
	@rm -rf program

objects: dirs .temp/make

