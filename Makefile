.PHONY: all algo sim gm clean

all: algo sim gm

algo:
	$(MAKE) -C Algorithm

sim:
	$(MAKE) -C Simulator

gm:
	$(MAKE) -C GameManager

clean:
	$(MAKE) -C Algorithm clean
	$(MAKE) -C Simulator clean
	$(MAKE) -C GameManager clean

