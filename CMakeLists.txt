cmake_minimum_required(VERSION 3.24)
project(WallmartApp LANGUAGES CXX)

# ──────────────────────────────────────────────────────────────────────
#  Global settings
# ──────────────────────────────────────────────────────────────────────
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Enable Qt’s automatic MOC/UIC/RCC processing
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# ──────────────────────────────────────────────────────────────────────
#  Dependencies
# ──────────────────────────────────────────────────────────────────────
# Qt 6 (Widgets + SQL for SQLite)
find_package(Qt6 REQUIRED COMPONENTS Widgets Sql)

# POSIX threads (needed by Crypto++)
find_package(Threads REQUIRED)

# Pull in Crypto++ that you vendored under third_party/
add_library(crypto++ STATIC IMPORTED)
set_target_properties(crypto++ PROPERTIES
    IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/third_party/cryptopp/libcryptopp.a"
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/third_party/cryptopp"
)

# ──────────────────────────────────────────────────────────────────────
#  Source layout variables
# ──────────────────────────────────────────────────────────────────────
set(BACKEND_SRC
    backend/gen_token.cpp
    backend/store_token.cpp
    backend/redeem.cpp
)

set(GUI_SRC
    src/main.cpp                       # Qt entry‑point
    src/mainwindow.h  src/mainwindow.cpp  src/mainwindow.ui
    src/storewindow.h src/storewindow.cpp src/storewindow.ui
    src/wallet_backend.h               # header‑only façade
)

# ──────────────────────────────────────────────────────────────────────
#  GUI executable
# ──────────────────────────────────────────────────────────────────────
add_executable(WallmartGUI
    ${GUI_SRC}
    ${BACKEND_SRC}
)

target_include_directories(WallmartGUI
    PRIVATE backend third_party/crypto++
)

target_link_libraries(WallmartGUI
    Qt6::Widgets
    Qt6::Sql
    crypto++
    Threads::Threads
)

# Embed icons / images with Qt’s resource system if you have a .qrc file
if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/resources.qrc")
    qt_add_resources(WallmartGUI RESOURCES src/resources.qrc)
endif()

# ──────────────────────────────────────────────────────────────────────
#  Optional CLI executable for backend testing
#      Enable with:  cmake -B build -DBUILD_CLI=ON
# ──────────────────────────────────────────────────────────────────────
option(BUILD_CLI "Build command‑line backend tester" OFF)

if (BUILD_CLI)
    # Rename your old main.cpp to cli_driver.cpp (or adjust path below)
    add_executable(WallmartCLI backend/cli_driver.cpp ${BACKEND_SRC})

    target_include_directories(WallmartCLI
        PRIVATE backend third_party/crypto++
    )

    target_link_libraries(WallmartCLI
        crypto++
        Threads::Threads
    )
endif()

# ──────────────────────────────────────────────────────────────────────
#  Installation (optional)
# ──────────────────────────────────────────────────────────────────────
install(TARGETS WallmartGUI
        RUNTIME DESTINATION bin)

if (BUILD_CLI)
    install(TARGETS WallmartCLI
            RUNTIME DESTINATION bin)
endif()

# ──────────────────────────────────────────────────────────────────────
#  Developer info
# ──────────────────────────────────────────────────────────────────────
message(STATUS "Build type:                ${CMAKE_BUILD_TYPE}")
message(STATUS "Qt6 prefix path:           ${Qt6_DIR}")
message(STATUS "Crypto++ library location: ${CMAKE_BINARY_DIR}/third_party/crypto++")
message(STATUS "CLI tester build:          ${BUILD_CLI}")
