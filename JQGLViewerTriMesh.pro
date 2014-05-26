#-------------------------------------------------
#
# Project created by QtCreator 2014-05-17T19:32:45
#
#-------------------------------------------------

QT       += core gui\
        opengl\
        xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = JQGLViewerTriMesh
TEMPLATE = app


SOURCES += main.cpp\
        jmainwindow.cpp \
    JQGLViewerTriMesh/jqglvewertrimesh.cpp \
    JQGLViewerTriMesh/manipulatedFrameSetConstraint.cpp \
 #   JQGLViewerTriMesh/object.cpp


HEADERS  += jmainwindow.h \
    JQGLViewerTriMesh/jqglvewertrimesh.h \
    JQGLViewerTriMesh/manipulatedFrameSetConstraint.h \
#    JQGLViewerTriMesh/object.h

FORMS    += jmainwindow.ui


# change the location of the libraries for your computer
INCLUDEPATH += C:/develop/QGLViewer2.5.2/QGLViewer \
    trimesh2/include \



## C h a n g e   t h e   l o c a t i o n   o f  the libraries  f o r    y o u r   c o m p u t e r
LIBS *= -LC:/develop/QGLViewer2.5.2/libs/debug -lQGLViewerd2


# add the trimesh source cpp files
SOURCES += trimesh2/libsrc/conn_comps.cc  \
        trimesh2/libsrc/diffuse.cc  \
        trimesh2/libsrc/edgeflip.cc  \
        trimesh2/libsrc/faceflip.cc  \
        trimesh2/libsrc/filter.cc  \
        trimesh2/libsrc/GLCamera.cc  \
        trimesh2/libsrc/ICP.cc  \
        trimesh2/libsrc/KDtree.cc  \
        trimesh2/libsrc/lmsmooth.cc  \
        trimesh2/libsrc/overlap.cc  \
        trimesh2/libsrc/remove.cc  \
        trimesh2/libsrc/reorder_verts.cc  \
        trimesh2/libsrc/shared.cc  \
        trimesh2/libsrc/subdiv.cc  \
        trimesh2/libsrc/TriMesh_bounding.cc  \
        trimesh2/libsrc/TriMesh_connectivity.cc  \
        trimesh2/libsrc/TriMesh_curvature.cc  \
        trimesh2/libsrc/TriMesh_grid.cc  \
        trimesh2/libsrc/TriMesh_io.cc  \
        trimesh2/libsrc/TriMesh_normals.cc  \
        trimesh2/libsrc/TriMesh_pointareas.cc  \
        trimesh2/libsrc/TriMesh_stats.cc  \
        trimesh2/libsrc/TriMesh_tstrips.cc  \
