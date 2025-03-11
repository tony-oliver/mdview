TARGET =							mdview
BUILDDIR = 							build
TESTFILE =							README.md
GRAPHER =							fdp

.PHONY:								all clean graphviz run stripped install uninstall verbose ${BUILDDIR}/install_manifest.txt

all: 								lib/sundown ${BUILDDIR}
									@cmake --build ${BUILDDIR} -j 32

verbose:							lib/sundown ${BUILDDIR}
									@cmake --build ${BUILDDIR} -j 32 VERBOSE=1

${BUILDDIR}:
									@cmake -B ${BUILDDIR}

lib/sundown:
									cd lib && git clone https://github.com/vmg/sundown.git

stripped:							all
									@strip -s ${BUILDDIR}/app/${TARGET}

install: 							stripped
									@sudo cmake --install ${BUILDDIR}

uninstall:							${BUILDDIR}/install_manifest.txt
									@[[ -f $< ]] && sudo xargs -r rm -fv < $< && sudo rm -fv $< || true
									rm -rfv lib/sundown

run: 								all
									@${BUILDDIR}/app/${TARGET} ${TESTFILE}

clean:
									@rm -frv ${BUILDDIR} lib/sundown

graphviz: 							${BUILDDIR}/graphviz/${TARGET}.svg
									firefox $<

${BUILDDIR}/graphviz/${TARGET}.dot:
									cmake -B ${BUILDDIR} --graphviz=$@

${BUILDDIR}/graphviz/${TARGET}.svg:	${BUILDDIR}/graphviz/${TARGET}.dot
									${GRAPHER} -Tsvg $< >$@
