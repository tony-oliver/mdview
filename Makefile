TARGET =							mdview
LIBRARY =							lib${TARGET}.so
TESTFILE =							README.md

#GENERATOR =						-G Ninja
BUILDDIR = 							build
INSTALLDIR =						/usr/local
LIBDIR =							lib64

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
									@cmake ${GENERATOR} -DCMAKE_INSTALL_LIBDIR=${LIBDIR} -B ${BUILDDIR}

#-----------------------------------------------------------------------------

.PHONY:								stripped
stripped:							all
									@strip -sv ${BUILDDIR}/app/${TARGET}
									@strip -sv ${BUILDDIR}/lib/${LIBRARY}

#-----------------------------------------------------------------------------

.PHONY:								install
install: 							stripped
									@sudo cmake --install ${BUILDDIR}
									@sudo ldconfig
									@echo "Don't forget> source /etc/bash_completion.d/mdview.bash"

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
									@xdg-open ${BUILDDIR}/docs/html/index.html 

#-----------------------------------------------------------------------------

.PHONY:								clean
clean:								# no dependencies
									@rm -frv ${BUILDDIR} lib/gdk-keynames.x-macros
