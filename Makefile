TARGET =				mdview
BUILDDIR = 				build
TESTFILE =				README.md

.PHONY:					all verbose cmake clean run install uninstall ${BUILDDIR}/install_manifest.txt

all: ${BUILDDIR};		@cd ${BUILDDIR} && make -j4

verbose: ${BUILDDIR};	@cd ${BUILDDIR} && make -j4 VERBOSE=1

${BUILDDIR}:;			@cmake -B ${BUILDDIR}

cmake:;					@cmake -B ${BUILDDIR}

clean:;					@rm -rvf ${BUILDDIR}

run: all;				@LD_LIBRARY_PATH=${BUILDDIR}/lib ${BUILDDIR}/src/${TARGET} ${TESTFILE}

install: all;			@sudo cmake --install ${BUILDDIR}

uninstall:				${BUILDDIR}/install_manifest.txt
						@[[ -f $< ]] && sudo xargs -r rm -fv <$< && sudo rm -fv $< || true
