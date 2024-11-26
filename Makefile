TARGET =				mdview
BUILDDIR = 				build
TESTFILE =				README.md

.PHONY:					all verbose cmake clean run strip install uninstall ${BUILDDIR}/install_manifest.txt

all: ${BUILDDIR};		@cd ${BUILDDIR} && make -j

verbose: ${BUILDDIR};	@cd ${BUILDDIR} && make -j VERBOSE=1

${BUILDDIR}:;			@cmake -B ${BUILDDIR}

cmake:;					@cmake -B ${BUILDDIR}

clean:;					@rm -rvf ${BUILDDIR}

run: all;				@${BUILDDIR}/src/${TARGET} ${TESTFILE}

strip: all;				@strip -s ${BUILDDIR}/src/${TARGET}

install: strip;			@sudo cmake --install ${BUILDDIR}

uninstall:				${BUILDDIR}/install_manifest.txt
						@[[ -f $< ]] && sudo xargs -r rm -fv < $< && sudo rm -fv $< || true
