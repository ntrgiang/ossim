OMNETPP_CONFIGFILE=/home/giang/workspace/422-omnet-inet-donet/omnetpp-4.2.2/Makefile.inc
#OMNETPP_CONFIGFILE=/home/giang/workspace/ebitsim-inet-omnet42/omnetpp-4.2.1/Makefile.inc

all: checkmakefiles
	cd src && $(MAKE)

clean: checkmakefiles
	cd src && $(MAKE) clean

cleanall: checkmakefiles
	cd src && $(MAKE) MODE=release clean
	cd src && $(MAKE) MODE=debug clean
	rm -f src/Makefile

makefiles:
	cd src && opp_makemake -f --deep

checkmakefiles:
	@if [ ! -f src/Makefile ]; then \
	echo; \
	echo '======================================================================='; \
	echo 'src/Makefile does not exist. Please use "make makefiles" to generate it!'; \
	echo '======================================================================='; \
	echo; \
	exit 1; \
	fi
