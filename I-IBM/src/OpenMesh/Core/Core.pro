################################################################################
#
################################################################################

# include( $$TOPDIR/qmake/all.include )

TEMPLATE = lib  # Set this for library projects

TARGET = OpenMeshCore

contains( OPENFLIPPER , OpenFlipper ){
	DESTDIR = $${TOPDIR}/OpenMesh/lib
} else {
	DESTDIR = $${TOPDIR}/lib
}


DIRECTORIES = . Geometry IO IO/exporter IO/importer IO/reader IO/writer \
		Mesh Mesh/gen System Utils

INCLUDEPATH += ../..

CONFIG( debug, debug|release ){
        TARGET    = OpenMeshCored
} else {
	TARGET    = OpenMeshCore
}

win32 {
 DEFINES += _USE_MATH_DEFINES NOMINMAX
 CONFIG  += static
}

macx {
      # Set library binary header to the correct path
      QMAKE_LFLAGS_SONAME = -install_name$${LITERAL_WHITESPACE}$${DESTDIR}/
      export(QMAKE_LFLAGS_SONAME)
}

# Input
HEADERS += $$files($$DIRECTORIES/*.hh,true)
SOURCES += $$files($$DIRECTORIES/*.cc, true)
FORMS   += $$files($$DIRECTORIES/*.ui, true)

################################################################################
