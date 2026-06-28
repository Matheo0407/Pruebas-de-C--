cmake_minimum_required(VERSION 3.16)
project(Qt5CubeOpenGL)

# Forzar el estándar de C++17 para compatibilidad
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# IMPORTANTE: MOC automático de Qt para procesar eventos de la interfaz
set(CMAKE_AUTOMOC ON)

# 1. RUTA DE TU INSTALACIÓN DE C:/Qt 
# (Revisa tu carpeta y pon 'msvc2019_64' o 'mingw81_64' según lo que instalaste)
set(CMAKE_PREFIX_PATH "C:/Qt/5.15.2/msvc2019_64")

# 2. TOOLCHAIN DE VCPKG (Por si instalas otras librerías en el futuro)
# (Ajusta la ruta de abajo según dónde tengas clonado vcpkg en tu disco)
set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "vcpkg toolchain")

# Buscar los paquetes de Qt5 y OpenGL en el sistema
find_package(Qt5 REQUIRED COMPONENTS Core Widgets Gui)
find_package(OpenGL REQUIRED)

# Definir el ejecutable
add_executable(Qt5CubeOpenGL main.cpp)

# Enlazar las librerías al ejecutable final
target_link_libraries(Qt5CubeOpenGL PRIVATE Qt5::Widgets Qt5::Gui OpenGL::GL)