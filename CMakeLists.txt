cmake_minimum_required(VERSION 3.0.0)
project(AlphaZeroDemos VERSION 0.1.0)
set(CXX_STANDARD 20)
set(CMAKE_BUILD_TYPE Debug)
#set(CMAKE_CXX_COMPILER "/usr/bin/gcc")

include(CTest)
enable_testing()
message(STATUS ${CMAKE_MODULE_PATH})

file(GLOB SharedFiles
  "include/*"
  "scr/*"
)
file(GLOB UnsupervisedFiles
  "unsupervised/src/*"
  "unsupervised/include/*" 
)
file(GLOB SupervisedFiles
  "supervised/src/*"
  "supervised/include/*"
)
add_executable(VectorQuantization main.cpp ${SharedFiles} ${UnsupervisedFiles})
add_executable(Supervised supervised.cpp ${SharedFiles} ${SupervisedFiles})

# Settting model device macro
find_package(CUDA 11.3)
message(STATUS "Using cuda: " ${CUDA_VERSION_STRING})


if (CUDA_VERSION_STRING EQUAL "")
  target_compile_definitions(Supervised PUBLIC MODEL_DEVICE="cpu")
else()
  target_compile_definitions(Supervised PRIVATE MODEL_DEVICE="cuda:0")
endif()

# Download libtorch from offical website
if (EXISTS "${PROJECT_SOURCE_DIR}/dependencies/libtorch.zip" OR EXISTS "${PROJECT_SOURCE_DIR}/dependencies/libtorch")
  message(STATUS "libtorch already downloaded")
else ()
  if (CUDA_VERSION_STRING EQUAL "")
	  message(STATUS "did not find cuda11.3 +")
    message(STATUS "downloading libtorch cpu")
    file(
      DOWNLOAD
      "https://download.pytorch.org/libtorch/cpu/libtorch-cxx11-abi-shared-with-deps-1.11.0%2Bcpu.zip"
      "${PROJECT_SOURCE_DIR}/dependencies/libtorch.zip"
    )
    set(libtorch_hash "5b5ea3067f878dea051f6fcd8fb00338f55517cb6baecdc810983a814e030845")
  else ()
    message(STATUS "found cuda version " ${CUDA_VERSION_STRING})
    message(STATUS "downloading libtorch cuda " ${CUDA_VERSION_STRING})
    file(
      DOWNLOAD
      "https://download.pytorch.org/libtorch/cu113/libtorch-cxx11-abi-shared-with-deps-1.11.0%2Bcu113.zip"
      "${PROJECT_SOURCE_DIR}/dependencies/libtorch.zip"
    )
    set(libtorch_hash "8d9e829ce9478db4f35bdb7943308cf02e8a2f58cf9bb10f742462c1d57bf287")
  endif()
  file(SHA256 "${PROJECT_SOURCE_DIR}/dependencies/libtorch.zip" libtorch_checksum)
  message(STATUS "check sum" ${libtorch_checksum})
  if (libtorch_checksum MATCHES "${libtorch_hash}")
    message(STATUS "libtorch checksum is valid")
  else()
    message(FATAL_ERROR "libtorch checksum is not valid")
  endif()
endif()

 #unzip libtorch
if (EXISTS "${PROJECT_SOURCE_DIR}/dependencies/libtorch")
  message(STATUS "libtorch already installed")
else()
  message(STATUS "installing libtorch")
  file(ARCHIVE_EXTRACT INPUT "${PROJECT_SOURCE_DIR}/dependencies/libtorch.zip" DESTINATION "${PROJECT_SOURCE_DIR}/dependencies")
  file(REMOVE "${PROJECT_SOURCE_DIR}/dependencies/libtorch.zip")
  message(STATUS "libtorch installed")
endif()
message(STATUS "libtorch Path:\t" "${PROJECT_SOURCE_DIR}/dependencies/libtorch")
set (CMAKE_PREFIX_PATH "${PROJECT_SOURCE_DIR}/dependencies/libtorch/")


find_package(Torch REQUIRED Torch_DIR)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS}")

find_package(colormap REQUIRED INTERFACE)

find_package(wxWidgets REQUIRED gl core base OPTIONAL_COMPONENTS net)
include(${wxWidgets_USE_FILE})

target_include_directories(VectorQuantization PUBLIC "include/" "unsupervised/include")
target_link_libraries(VectorQuantization PRIVATE ${wxWidgets_LIBRARIES})

target_include_directories(Supervised PUBLIC "include/" "supervised/include")
target_link_libraries(Supervised PRIVATE ${wxWidgets_LIBRARIES} ${TORCH_LIBRARIES})
target_link_libraries(Supervised INTERFACE ${colormap_DIR})
message(STATUS "colormap at: " ${colormap_DIR})
set_property(TARGET Supervised PROPERTY CXX_STANDARD 23)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
