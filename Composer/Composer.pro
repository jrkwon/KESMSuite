#-------------------------------------------------
#
# Project created by QtCreator 2012-08-24T13:35:57
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = Composer
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    Composer.cpp

HEADERS += \
    Composer.h

INCLUDEPATH += $$(ITK_INCLUDE_DIR) \
               $$(ITK_INCLUDE_DIR)/Common \
               $$(ITK_INCLUDE_DIR)/Utilities \
               $$(ITK_INCLUDE_DIR)/Utilities/vxl/vcl \
               $$(ITK_INCLUDE_DIR)/Utilities/vxl/vcl/iso \
               $$(ITK_INCLUDE_DIR)/Utilities/vxl/core \
               $$(ITK_INCLUDE_DIR)/Utilities/vxl/core/vnl \
               $$(ITK_INCLUDE_DIR)/Algorithms \
               $$(ITK_INCLUDE_DIR)/BasicFilters \
               $$(ITK_INCLUDE_DIR)/gdcm \
               $$(ITK_INCLUDE_DIR)/IO \
               $$(ITK_INCLUDE_DIR)/Numerics \
               $$(ITK_INCLUDE_DIR)/SpatialObject \
               $$(ITK_INCLUDE_DIR)

# itk libs
LIBS += $$(ITK_LIB_DIR)/ITKAlgorithms.lib \
        $$(ITK_LIB_DIR)/ITKBasicFilters.lib \
        $$(ITK_LIB_DIR)/ITKCommon.lib \
        $$(ITK_LIB_DIR)/ITKDICOMParser.lib \
        $$(ITK_LIB_DIR)/ITKEXPAT.lib \
        $$(ITK_LIB_DIR)/ITKFEM.lib \
        $$(ITK_LIB_DIR)/itkgdcm.lib \
        $$(ITK_LIB_DIR)/ITKIO.lib \
        $$(ITK_LIB_DIR)/itkjpeg12.lib \
        $$(ITK_LIB_DIR)/itkjpeg16.lib \
        $$(ITK_LIB_DIR)/itkjpeg8.lib \
        $$(ITK_LIB_DIR)/ITKMetaIO.lib \
        $$(ITK_LIB_DIR)/itkNetlibSlatec.lib \
        $$(ITK_LIB_DIR)/ITKniftiio.lib \
        $$(ITK_LIB_DIR)/ITKNrrdIO.lib \
        $$(ITK_LIB_DIR)/ITKNumerics.lib \
        $$(ITK_LIB_DIR)/itkopenjpeg.lib \
        $$(ITK_LIB_DIR)/itkpng.lib \
        $$(ITK_LIB_DIR)/ITKSpatialObject.lib \
        $$(ITK_LIB_DIR)/ITKStatistics.lib \
        $$(ITK_LIB_DIR)/itksys.lib \
        $$(ITK_LIB_DIR)/itktiff.lib \
        $$(ITK_LIB_DIR)/itkv3p_lsqr.lib \
        $$(ITK_LIB_DIR)/itkv3p_netlib.lib \
        $$(ITK_LIB_DIR)/itkvcl.lib \
        $$(ITK_LIB_DIR)/itkvnl.lib \
        $$(ITK_LIB_DIR)/itkvnl_algo.lib \
        $$(ITK_LIB_DIR)/itkvnl_inst.lib \
        $$(ITK_LIB_DIR)/itkzlib.lib \
        $$(ITK_LIB_DIR)/ITKznz.lib

# WIN32 libs
LIBS += $$(WIN_LIB_DIR)/AdvAPI32.Lib \
        $$(WIN_LIB_DIR)/RpcRT4.Lib \
        $$(WIN_LIB_DIR)/WSock32.Lib \
        $$(WIN_LIB_DIR)/Gdi32.Lib \
        $$(WIN_LIB_DIR)/SnmpAPI.Lib
