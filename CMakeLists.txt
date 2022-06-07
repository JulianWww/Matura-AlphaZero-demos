cmake_minimum_required(VERSION 3.0.0)
project(AlphaZeroDemos VERSION 0.1.0)
set(CXX_STANDARD 14)

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
find_package(CUDA 10.2)

if (NOT CUDA_VERSION_STRING EQUAL "")
  target_compile_definitions(Supervised PUBLIC MODEL_DEVICE="cpu")
else()
target_compile_definitions(Supervised PRIVATE DEVICE="cuda:0")
endif()

# Download libtorch from offical website
if (EXISTS "${PROJECT_SOURCE_DIR}/dependencies/libtorch.zip" OR EXISTS "${PROJECT_SOURCE_DIR}/dependencies/libtorch")
  message(STATUS "libtorch already downloaded")
else ()
  if (NOT CUDA_VERSION_STRING EQUAL "")
	  message(STATUS "did not find cuda10 +")
    message(STATUS "downloading libtorch cpu")
    file(
      DOWNLOAD
      "https://download.pytorch.org/libtorch/cpu/libtorch-cxx11-abi-shared-with-deps-1.11.0%2Bcpu.zip"
      "${PROJECT_SOURCE_DIR}/dependencies/libtorch.zip"
    )
    set(libtorch_hash "5b5ea3067f878dea051f6fcd8fb00338f55517cb6baecdc810983a814e030845")
  else ()
    message(STATUS "found cuda version " ${CUDA_VERSION_STRING})
    COMPARE_VERSION_STRINGS(CUDA_VERSION_STRING "11.3"  result)
    if (result GREATER)
      message(STATUS "did found cuda 10")
      message(STATUS "downloading libtorch cuda 10")
      file(
        DOWNLOAD
        "https://download.pytorch.org/libtorch/cu102/libtorch-cxx11-abi-shared-with-deps-1.11.0%2Bcu102.zip"
        "${PROJECT_SOURCE_DIR}/dependencies/libtorch.zip"
      )
      set(libtorch_hash "e89a08fdbcd5f6c474ee6af082beeb27d828552ad9c2e4fa06ee52e947dcf6e3")
    else()
    message(STATUS "did found cuda 10")
      message(STATUS "downloading libtorch cuda 10")
      file(
        DOWNLOAD
        "https://download.pytorch.org/libtorch/cu113/libtorch-cxx11-abi-shared-with-deps-1.11.0%2Bcu113.zip"
        "${PROJECT_SOURCE_DIR}/dependencies/libtorch.zip"
      )
      set(libtorch_hash "8d9e829ce9478db4f35bdb7943308cf02e8a2f58cf9bb10f742462c1d57bf287")
    endif()
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
set (Torch_DIR "${PROJECT_SOURCE_DIR}/dependencies/libtorch/share/cmake/Torch")


find_package(Torch REQUIRED Torch_DIR)
set(DCMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS}")

find_package(colormap REQUIRED INTERFACE)

find_package(wxWidgets REQUIRED gl core base OPTIONAL_COMPONENTS net)
include(${wxWidgets_USE_FILE})

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
target_include_directories(VectorQuantization PUBLIC "include/" "unsupervised/include")
target_link_libraries(VectorQuantization PRIVATE ${wxWidgets_LIBRARIES})

add_executable(Supervised supervised.cpp ${SharedFiles} ${SupervisedFiles})
target_include_directories(Supervised PUBLIC "include/" "supervised/include")
target_link_libraries(Supervised PRIVATE ${wxWidgets_LIBRARIES} ${TORCH_LIBRARIES})
target_link_libraries(Supervised INTERFACE ${colormap_DIR})
message(STATUS "colormap at: " ${colormap_DIR})
#set_property(TARGET Supervised PROPERTY CXX_STANDARD 23)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
