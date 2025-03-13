TARGET =							mdview
BUILDDIR = 							build
TESTFILE =							README.md
GRAPHER =							fdp

.PHONY:								all clean graphviz run stripped install uninstall verbose ${BUILDDIR}/install_manifest.txt

all: 								${BUILDDIR}
									@cmake --build ${BUILDDIR} -j 32

verbose:							${BUILDDIR}
									@cmake --build ${BUILDDIR} -v -j 32

${BUILDDIR}:						# no dependencies
									@cmake -B ${BUILDDIR}

stripped:							all
									@strip -s ${BUILDDIR}/app/${TARGET}

install: 							stripped
									@sudo cmake --install ${BUILDDIR}

uninstall:							${BUILDDIR}/install_manifest.txt
									@[[ -f $< ]] && sudo xargs -r rm -fv < $< && sudo rm -fv $< || true

run: 								all
									@${BUILDDIR}/app/${TARGET} ${TESTFILE}

clean:								# no dependencies
									@rm -frv ${BUILDDIR}

graphviz: 							${BUILDDIR}/graphviz/${TARGET}.svg
									firefox $<

${BUILDDIR}/graphviz/${TARGET}.dot:	# no dependencies
									cmake -B ${BUILDDIR} --graphviz=$@

${BUILDDIR}/graphviz/${TARGET}.svg:	${BUILDDIR}/graphviz/${TARGET}.dot
									${GRAPHER} -Tsvg $< >$@
