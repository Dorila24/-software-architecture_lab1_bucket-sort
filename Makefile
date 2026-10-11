PYTHON ?= python3
ECO_SDK ?=
VARIANT ?= detailed
PAIR ?= 3
BUILDER = AssemblyFiles/Mac/build.py
METHODS = variants/$(VARIANT)/pair-$(PAIR)/BucketMethods.c
OUTPUT = BuildFiles/$(VARIANT)-pair-$(PAIR)

.PHONY: demo test test-all benchmark verify-sources check-sdk

check-sdk:
	@test -n "$(ECO_SDK)" || { echo 'Set ECO_SDK to your separately obtained Eco.Framework directory.'; exit 1; }
	@test -d "$(ECO_SDK)" || { echo 'ECO_SDK directory does not exist.'; exit 1; }

demo: check-sdk
	$(PYTHON) $(BUILDER) --framework "$(ECO_SDK)" --methods "$(METHODS)" --output "$(OUTPUT)/Demo" --run

test: check-sdk
	$(PYTHON) $(BUILDER) --framework "$(ECO_SDK)" --methods "$(METHODS)" --output "$(OUTPUT)/Dependencies" --compile-only
	$(PYTHON) $(BUILDER) --framework "$(ECO_SDK)" --methods "$(METHODS)" --client checks/reference.c --output "$(OUTPUT)/Independent" --sanitize --run

test-all: check-sdk
	@set -e; for variant in short detailed; do for pair in 1 2 3; do $(MAKE) test VARIANT="$$variant" PAIR="$$pair" ECO_SDK="$(ECO_SDK)"; done; done

benchmark: check-sdk
	$(PYTHON) $(BUILDER) --framework "$(ECO_SDK)" --methods "$(METHODS)" --client checks/benchmark.c --output "$(OUTPUT)/Benchmark" --run

verify-sources:
	$(PYTHON) scripts/verify_sources.py
