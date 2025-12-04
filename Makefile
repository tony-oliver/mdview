TARGET =							mdview
LIBRARY =							lib${TARGET}.so

BUILDDIR = 							build
INSTALLDIR =						/usr/local

TESTFILE =							README.md

#===========================================================================#
#																			#
#	Targets and pseudo-targets ('all' should always be listed first).		#
#																			#
#===========================================================================#

.PHONY:								all
all: 								${BUILDDIR}
									@cmake --build ${BUILDDIR} -j

#-----------------------------------------------------------------------------

.PHONY:								verbose
 verbose:							${BUILDDIR}
									@cmake --build ${BUILDDIR} -v
									
#-----------------------------------------------------------------------------

${BUILDDIR}:						# no dependencies
									@cmake -DCMAKE_INSTALL_LIBDIR=lib64 -B ${BUILDDIR}

#-----------------------------------------------------------------------------

.PHONY:								stripped
stripped:							all
									@strip -sv ${BUILDDIR}/app/${TARGET}
									@strip -sv ${BUILDDIR}/lib/${LIBRARY}

#-----------------------------------------------------------------------------

.PHONY:								install
install: 							stripped
									@sudo cmake --install ${BUILDDIR}
	                               	@sudo ldconfig ${INSTALLDIR}/lib64

#-----------------------------------------------------------------------------

.PHONY:								uninstall ${BUILDDIR}/install_manifest.txt
uninstall:							${BUILDDIR}/install_manifest.txt
									@[[ -f $< ]] && sudo xargs -r rm -rfv < $< && sudo rm -rfv $< || true

#-----------------------------------------------------------------------------

.PHONY:								run-test
run-test: 							all
									@${BUILDDIR}/app/${TARGET} ${TESTFILE}

#-----------------------------------------------------------------------------

.PHONY:								view-docs
view-docs:							all
									xdg-open ${BUILDDIR}/docs/html/index.html 

#-----------------------------------------------------------------------------

.PHONY:								clean
clean:								# no dependencies
									@rm -frv ${BUILDDIR}
