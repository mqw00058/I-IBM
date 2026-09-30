# Linux (Qt5 + GCC) build of I-IBM2, generated from I-IBM.vcxproj.
# Kinect v2 capture is Windows-only: its code is excluded with #ifdef _WIN32 and the
# global grabber object uses src/Kinect/KinectGrabberStub.h.
TEMPLATE = app
TARGET = I-IBM
DESTDIR = $$PWD/../bin
QT += core gui widgets opengl
CONFIG += c++17 release warn_off object_parallel_to_source  # plyfile.c vs PlyFile.cpp clash on case-insensitive FS
DEFINES += _USE_MATH_DEFINES OM_STATIC_BUILD
QMAKE_CXXFLAGS += -fpermissive -w -include $$PWD/linux_compat.h
QMAKE_CFLAGS += -w

OPENMESH = $$PWD/../3rdparty/OpenMesh-3.1
INCLUDEPATH += $$PWD $$PWD/src $$OPENMESH /usr/include/opencv4 /usr/include/suitesparse \
    /usr/include/pcl-1.14 /usr/include/eigen3
LIBS += -L$$OPENMESH/lib -lOpenMesh \
    -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_imgcodecs -lopencv_flann \
    -lpcl_features -lpcl_search -lpcl_kdtree -lpcl_common \
    -lcholmod -lsuitesparseconfig -lboost_filesystem -lglut -lGLU -lGL -lpthread

MOC_DIR = build/moc
OBJECTS_DIR = build/obj
UI_DIR = build/ui
RCC_DIR = build/rcc

FORMS += DeformationWidget.ui
RESOURCES += IIBM.qrc
HEADERS += \
    DeformationWidget.h \
    src/GLWidget/GLOptionWidget.h \
    mainwindow.h \
    src/EmbeddedDeform/genGraphDialog.h \
    src/Logview/logviewdockwidget.h \
    src/GLWidget/GLWidget.h \

SOURCES += \
    CorrespondenceOpenCV.cpp \
    DeformationWidget.cpp \
    globals.cpp \
    MeshRecon.cpp \
    src/EmbeddedDeform/DeformableMesh3d.cpp \
    src/EmbeddedDeform/DeformationGraph.cpp \
    src/EmbeddedDeform/GaussNewtonSolver.cpp \
    src/EmbeddedDeform/genGraphDialog.cpp \
    src/EmbeddedDeform/uniform_triangulation.cpp \
    src/GLWidget/GLOptionWidget.cpp \
    src/Geometry3D/Geo3DMesh.cpp \
    src/GLWidget/GLWidget.cpp \
    src/Logview/logviewdockwidget.cpp \
    main.cpp \
    mainwindow.cpp \
    src/MemoryLeakDetector/DebugNew.cpp \
    src/MemoryLeakDetector/MemoryLeak.cpp \
    src/NR/EigenSolver.cpp \
    src/NR/MatrixEq.cpp \
    src/NR/NR.cpp \
    src/NR/NR_EigenJacobi.cpp \
    src/NR/NR_Matrix.cpp \
    src/PBM/ImpicitSurfaceReconstruction/ImplicitSurfaceReconstruction.cpp \
    src/PBM/ImpicitSurfaceReconstruction/MarchingCubes/Cube.cpp \
    src/PBM/ImpicitSurfaceReconstruction/MarchingCubes/define.cpp \
    src/PBM/ImpicitSurfaceReconstruction/MarchingCubes/Edge.cpp \
    src/PBM/ImpicitSurfaceReconstruction/MarchingCubes/eigsrt.cpp \
    src/PBM/ImpicitSurfaceReconstruction/MarchingCubes/Graph.cpp \
    src/PBM/ImpicitSurfaceReconstruction/MarchingCubes/jacobi.cpp \
    src/PBM/ImpicitSurfaceReconstruction/MarchingCubes/Kruskal.cpp \
    src/PBM/ImpicitSurfaceReconstruction/MarchingCubes/MarchingCube.cpp \
    src/PBM/ImpicitSurfaceReconstruction/MarchingCubes/Node.cpp \
    src/PBM/ImpicitSurfaceReconstruction/MarchingCubes/plyfile.c \
    src/PBM/ImpicitSurfaceReconstruction/MarchingCubes/Point.cpp \
    src/PBM/ImpicitSurfaceReconstruction/MarchingCubes/Vector3f.cpp \
    src/Point/Geo3DPoint.cpp \
    src/PoissonRecon/CmdLineParser.cpp \
    src/PoissonRecon/Factor.cpp \
    src/PoissonRecon/Geometry.cpp \
    src/PoissonRecon/MarchingCubes.cpp \
    src/PoissonRecon/PlyFile.cpp \
    src/PoissonRecon/Time.cpp \
    src/VisionWidget/VisionWidget.cpp \

