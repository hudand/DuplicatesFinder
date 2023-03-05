project(DupFinderExecutable LANGUAGES CXX)

set(TARGET dupfinder)

set(SOURCE_FILES
    FileComparer.cpp
    FileComparer.h
    FileInfo.h
    Finder.cpp
    Finder.h
    main.cpp)

add_executable(${TARGET} ${SOURCE_FILES})

target_include_directories(
    ${TARGET}
    PRIVATE
        $<TARGET_PROPERTY:${TARGET},SOURCE_DIR>)

install(TARGETS ${TARGET}
    RUNTIME
        DESTINATION ${CMAKE_INSTALL_BINDIR})