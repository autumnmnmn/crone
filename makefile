
_default: dirs
	@echo "\"make test\" for debug builds, \"make release\" to build a release"

.NOTINTERMEDIATE: # all

LINKS = -lX11 -lvulkan
HEADERS = -I code/headers
ERRORS = -Wfatal-errors -Wall -Werror=use-after-free -Wno-unused-variable
%_dev: VERSION_FLAGS = -Og -DDO_VALIDATION
%_release: VERSION_FLAGS = -O3
COMMON_FLAGS = -std=c23 $(ERRORS) $(VERSION_FLAGS) $(HEADERS)

GCC = gcc $(COMMON_FLAGS) -o

define MAKE_OBJECT
	@echo "making $@"
	@$(GCC) $@ $^ -c
endef

.object/_x11_window%: code/_x11/window.c
	$(MAKE_OBJECT)

.object/_vk_basics%: code/_vk/basics.c
	$(MAKE_OBJECT)

%_dev: SUFFIX = _dev
%_release: SUFFIX = _release
program/crone_%: code/crone.c .object/_x11_window_% .object/_vk_basics_%
	@echo "making $@"
	@$(GCC) program/crone$(SUFFIX) $^ $(LINKS)

.PHONY: _default dirs test clean release

dirs:
	@mkdir -p program .object

test: dirs program/crone_dev
	@echo -e "\nlaunching..."
	@program/crone_dev

DATE := $(shell date '+%d.%m.%Y')
release: dirs program/crone_release
	@cp program/crone_release program/crone_release_$(DATE)
	@echo -e "\nbuilt program/crone_release_$(DATE)"

clean: dirs
	@rm -f ./.object/*
	@rm -f ./program/*

