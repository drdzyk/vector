include(ExternalProject)

externalproject_add(benchmark_lib
    GIT_REPOSITORY    https://github.com/google/benchmark.git
    GIT_TAG           master
    GIT_PROGRESS      True                                          # show progress bar during download
    PREFIX            ${CMAKE_CURRENT_SOURCE_DIR}/external_sources  # sources and build artifacts will be in this dir
    CMAKE_ARGS        -DBENCHMARK_ENABLE_TESTING=OFF                # turn off downloading gtest and building tests
                      -DCMAKE_BUILD_TYPE=Release                    # build library in release mode
    INSTALL_COMMAND   ""                                            # just skip install step
    UPDATE_COMMAND    ""                                            # same for update step
)

externalproject_get_property(benchmark_lib SOURCE_DIR)
externalproject_get_property(benchmark_lib BINARY_DIR)

set(
    BENCHMARK_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/benchmark.cpp
)

add_executable(benchmark ${BENCHMARK_SOURCES})
target_include_directories(benchmark PRIVATE ${CMAKE_SOURCE_DIR} ${SOURCE_DIR}/include)
target_link_libraries(benchmark PRIVATE
    -Wl,-Bstatic ${BINARY_DIR}/src/libbenchmark.a -fsanitize=address
    -Wl,-Bdynamic
)

add_dependencies(benchmark benchmark_lib)

# And few crutches at the last:
# 1) avoid compilation issue in benchmark header
set_source_files_properties(${BENCHMARK_SOURCES} PROPERTIES COMPILE_FLAGS -Wno-redundant-decls)
# 2) make directory with artifacts removable with 'git clean -fdx'
add_custom_command(
    TARGET benchmark_lib POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${SOURCE_DIR}/.git
)
