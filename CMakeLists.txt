cmake_minimum_required(VERSION 3.2)
set(EXE_NAME idcard-ocr)
project(${EXE_NAME})
set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS} -g")

aux_source_directory(./src DIR_ROOT_SRCS)

include_directories(/usr/local/include/opencv)
include_directories(/usr/local/include)
link_directories(/usr/local/lib)
list(APPEND DEPS_LIB opencv_stitching)
list(APPEND DEPS_LIB opencv_objdetect)
list(APPEND DEPS_LIB opencv_dnn)
list(APPEND DEPS_LIB opencv_videostab)
list(APPEND DEPS_LIB opencv_calib3d)
list(APPEND DEPS_LIB opencv_photo)
list(APPEND DEPS_LIB opencv_superres)
list(APPEND DEPS_LIB opencv_shape)
list(APPEND DEPS_LIB opencv_features2d)
list(APPEND DEPS_LIB opencv_highgui)
list(APPEND DEPS_LIB opencv_videoio)
list(APPEND DEPS_LIB opencv_flann)
list(APPEND DEPS_LIB opencv_ml)
list(APPEND DEPS_LIB opencv_imgcodecs)
list(APPEND DEPS_LIB opencv_video)
list(APPEND DEPS_LIB opencv_imgproc)
list(APPEND DEPS_LIB opencv_core)


add_executable(${EXE_NAME} ${DIR_ROOT_SRCS})
target_link_libraries(${EXE_NAME} ${DEPS_LIB})


