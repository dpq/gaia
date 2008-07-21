#############################################################################
# Makefile for building: gaia
# Generated by qmake (2.01a) (Qt 4.4.0) on: Mon Jul 21 20:03:25 2008
# Project:  gaia.pro
# Template: app
# Command: /usr/bin/qmake -unix -o Makefile gaia.pro
#############################################################################

####### Compiler, tools and options

CC            = gcc
CXX           = g++
DEFINES       = -DQT_NO_DEBUG -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -DQT_SHARED
CFLAGS        = -pipe -O2 -D_REENTRANT -Wall -W $(DEFINES)
CXXFLAGS      = -pipe -O2 -D_REENTRANT -Wall -W $(DEFINES)
INCPATH       = -I/usr/share/qt4/mkspecs/linux-g++ -I. -I/usr/include/qt4/QtUiTools -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtGui -I/usr/include/qt4/QtGui -I/usr/include/qt4/QtXml -I/usr/include/qt4/QtXml -I/usr/include/qt4 -I. -I. -I.
LINK          = g++
LFLAGS        = -Wl,--no-undefined
LIBS          = $(SUBLIBS)  -L/usr/lib -lQtXml -lQtGui -lQtCore -lQtUiTools -lpthread
AR            = ar cqs
RANLIB        = 
QMAKE         = /usr/bin/qmake
TAR           = tar -cf
COMPRESS      = gzip -9f
COPY          = cp -f
SED           = sed
COPY_FILE     = $(COPY)
COPY_DIR      = $(COPY) -r
INSTALL_FILE  = install -m 644 -p
INSTALL_DIR   = $(COPY_DIR)
INSTALL_PROGRAM = install -m 755 -p
DEL_FILE      = rm -f
SYMLINK       = ln -sf
DEL_DIR       = rmdir
MOVE          = mv -f
CHK_DIR_EXISTS= test -d
MKDIR         = mkdir -p

####### Output directory

OBJECTS_DIR   = ./

####### Files

SOURCES       = config.cpp \
		core.cpp \
		main.cpp \
		stack.cpp \
		uiloader.cpp moc_core.cpp \
		moc_stack.cpp \
		qrc_redbook.cpp
OBJECTS       = config.o \
		core.o \
		main.o \
		stack.o \
		uiloader.o \
		moc_core.o \
		moc_stack.o \
		qrc_redbook.o
DIST          = /usr/share/qt4/mkspecs/common/g++.conf \
		/usr/share/qt4/mkspecs/common/unix.conf \
		/usr/share/qt4/mkspecs/common/linux.conf \
		/usr/share/qt4/mkspecs/qconfig.pri \
		/usr/share/qt4/mkspecs/features/qt_functions.prf \
		/usr/share/qt4/mkspecs/features/qt_config.prf \
		/usr/share/qt4/mkspecs/features/exclusive_builds.prf \
		/usr/share/qt4/mkspecs/features/default_pre.prf \
		/usr/share/qt4/mkspecs/features/release.prf \
		/usr/share/qt4/mkspecs/features/default_post.prf \
		/usr/share/qt4/mkspecs/features/qt.prf \
		/usr/share/qt4/mkspecs/features/uitools.prf \
		/usr/share/qt4/mkspecs/features/unix/thread.prf \
		/usr/share/qt4/mkspecs/features/moc.prf \
		/usr/share/qt4/mkspecs/features/warn_on.prf \
		/usr/share/qt4/mkspecs/features/resources.prf \
		/usr/share/qt4/mkspecs/features/uic.prf \
		/usr/share/qt4/mkspecs/features/yacc.prf \
		/usr/share/qt4/mkspecs/features/lex.prf \
		gaia.pro
QMAKE_TARGET  = gaia
DESTDIR       = 
TARGET        = gaia

first: all
####### Implicit rules

.SUFFIXES: .o .c .cpp .cc .cxx .C

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.C.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -o "$@" "$<"

####### Build rules

all: Makefile $(TARGET)

$(TARGET): ui_redbook.h $(OBJECTS)  
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(OBJCOMP) $(LIBS)

Makefile: gaia.pro  /usr/share/qt4/mkspecs/linux-g++/qmake.conf /usr/share/qt4/mkspecs/common/g++.conf \
		/usr/share/qt4/mkspecs/common/unix.conf \
		/usr/share/qt4/mkspecs/common/linux.conf \
		/usr/share/qt4/mkspecs/qconfig.pri \
		/usr/share/qt4/mkspecs/features/qt_functions.prf \
		/usr/share/qt4/mkspecs/features/qt_config.prf \
		/usr/share/qt4/mkspecs/features/exclusive_builds.prf \
		/usr/share/qt4/mkspecs/features/default_pre.prf \
		/usr/share/qt4/mkspecs/features/release.prf \
		/usr/share/qt4/mkspecs/features/default_post.prf \
		/usr/share/qt4/mkspecs/features/qt.prf \
		/usr/share/qt4/mkspecs/features/uitools.prf \
		/usr/share/qt4/mkspecs/features/unix/thread.prf \
		/usr/share/qt4/mkspecs/features/moc.prf \
		/usr/share/qt4/mkspecs/features/warn_on.prf \
		/usr/share/qt4/mkspecs/features/resources.prf \
		/usr/share/qt4/mkspecs/features/uic.prf \
		/usr/share/qt4/mkspecs/features/yacc.prf \
		/usr/share/qt4/mkspecs/features/lex.prf \
		/usr/lib/libQtXml.prl \
		/usr/lib/libQtGui.prl \
		/usr/lib/libQtCore.prl \
		/usr/lib/libQtUiTools.prl
	$(QMAKE) -unix -o Makefile gaia.pro
/usr/share/qt4/mkspecs/common/g++.conf:
/usr/share/qt4/mkspecs/common/unix.conf:
/usr/share/qt4/mkspecs/common/linux.conf:
/usr/share/qt4/mkspecs/qconfig.pri:
/usr/share/qt4/mkspecs/features/qt_functions.prf:
/usr/share/qt4/mkspecs/features/qt_config.prf:
/usr/share/qt4/mkspecs/features/exclusive_builds.prf:
/usr/share/qt4/mkspecs/features/default_pre.prf:
/usr/share/qt4/mkspecs/features/release.prf:
/usr/share/qt4/mkspecs/features/default_post.prf:
/usr/share/qt4/mkspecs/features/qt.prf:
/usr/share/qt4/mkspecs/features/uitools.prf:
/usr/share/qt4/mkspecs/features/unix/thread.prf:
/usr/share/qt4/mkspecs/features/moc.prf:
/usr/share/qt4/mkspecs/features/warn_on.prf:
/usr/share/qt4/mkspecs/features/resources.prf:
/usr/share/qt4/mkspecs/features/uic.prf:
/usr/share/qt4/mkspecs/features/yacc.prf:
/usr/share/qt4/mkspecs/features/lex.prf:
/usr/lib/libQtXml.prl:
/usr/lib/libQtGui.prl:
/usr/lib/libQtCore.prl:
/usr/lib/libQtUiTools.prl:
qmake:  FORCE
	@$(QMAKE) -unix -o Makefile gaia.pro

dist: 
	@$(CHK_DIR_EXISTS) .tmp/gaia1.0.0 || $(MKDIR) .tmp/gaia1.0.0 
	$(COPY_FILE) --parents $(SOURCES) $(DIST) .tmp/gaia1.0.0/ && $(COPY_FILE) --parents config.h core.h stack.h uiloader.h .tmp/gaia1.0.0/ && $(COPY_FILE) --parents redbook.qrc .tmp/gaia1.0.0/ && $(COPY_FILE) --parents config.cpp core.cpp main.cpp stack.cpp uiloader.cpp .tmp/gaia1.0.0/ && $(COPY_FILE) --parents redbook.ui .tmp/gaia1.0.0/ && (cd `dirname .tmp/gaia1.0.0` && $(TAR) gaia1.0.0.tar gaia1.0.0 && $(COMPRESS) gaia1.0.0.tar) && $(MOVE) `dirname .tmp/gaia1.0.0`/gaia1.0.0.tar.gz . && $(DEL_FILE) -r .tmp/gaia1.0.0


clean:compiler_clean 
	-$(DEL_FILE) $(OBJECTS)
	-$(DEL_FILE) *~ core *.core


####### Sub-libraries

distclean: clean
	-$(DEL_FILE) $(TARGET) 
	-$(DEL_FILE) Makefile


mocclean: compiler_moc_header_clean compiler_moc_source_clean

mocables: compiler_moc_header_make_all compiler_moc_source_make_all

compiler_moc_header_make_all: moc_core.cpp moc_stack.cpp
compiler_moc_header_clean:
	-$(DEL_FILE) moc_core.cpp moc_stack.cpp
moc_core.cpp: core.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) core.h -o moc_core.cpp

moc_stack.cpp: stack.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) stack.h -o moc_stack.cpp

compiler_rcc_make_all: qrc_redbook.cpp
compiler_rcc_clean:
	-$(DEL_FILE) qrc_redbook.cpp
qrc_redbook.cpp: redbook.qrc \
		logo.png \
		redbook.conf \
		icon.png \
		redbook.ui
	/usr/bin/rcc -name redbook redbook.qrc -o qrc_redbook.cpp

compiler_image_collection_make_all: qmake_image_collection.cpp
compiler_image_collection_clean:
	-$(DEL_FILE) qmake_image_collection.cpp
compiler_moc_source_make_all:
compiler_moc_source_clean:
compiler_uic_make_all: ui_redbook.h
compiler_uic_clean:
	-$(DEL_FILE) ui_redbook.h
ui_redbook.h: redbook.ui \
		stack.h
	/usr/bin/uic-qt4 redbook.ui -o ui_redbook.h

compiler_yacc_decl_make_all:
compiler_yacc_decl_clean:
compiler_yacc_impl_make_all:
compiler_yacc_impl_clean:
compiler_lex_make_all:
compiler_lex_clean:
compiler_clean: compiler_moc_header_clean compiler_rcc_clean compiler_uic_clean 

####### Compile

config.o: config.cpp config.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o config.o config.cpp

core.o: core.cpp core.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o core.o core.cpp

main.o: main.cpp core.h \
		config.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o main.o main.cpp

stack.o: stack.cpp stack.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o stack.o stack.cpp

uiloader.o: uiloader.cpp uiloader.h \
		stack.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o uiloader.o uiloader.cpp

moc_core.o: moc_core.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_core.o moc_core.cpp

moc_stack.o: moc_stack.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_stack.o moc_stack.cpp

qrc_redbook.o: qrc_redbook.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o qrc_redbook.o qrc_redbook.cpp

####### Install

install:   FORCE

uninstall:   FORCE

FORCE:

