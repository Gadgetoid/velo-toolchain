include_guard(GLOBAL)

get_filename_component(VELO_TOOLCHAIN_ROOT "${CMAKE_CURRENT_LIST_DIR}/.." ABSOLUTE)
find_package(Python3 REQUIRED COMPONENTS Interpreter)
enable_language(ASM)
enable_language(CXX)

if(NOT VELO_ARCH)
    set(VELO_ARCH mips)
endif()
string(REPLACE "." "" VELO_CE_NAME "ce${VELO_CE_VERSION}")
if(VELO_CE_VERSION STREQUAL "1.01")
    set(VELO_WIN32_WCE 101)
else()
    set(VELO_WIN32_WCE ${VELO_CE_VERSION}00)
endif()
if(VELO_ARCH STREQUAL "mips")
    set(VELO_EXPORTS_NAME "${VELO_CE_NAME}")
else()
    set(VELO_EXPORTS_NAME "${VELO_CE_NAME}-${VELO_ARCH}")
endif()
set(VELO_EXPORTS_DIR "${VELO_TOOLCHAIN_ROOT}/exports/${VELO_EXPORTS_NAME}")
if(NOT IS_DIRECTORY "${VELO_EXPORTS_DIR}")
    message(FATAL_ERROR "No export lists for VELO_CE_VERSION=${VELO_CE_VERSION} VELO_ARCH=${VELO_ARCH}")
endif()

if(NOT DEFINED VELO_OS_SYMBOLS)
    set(VELO_OS_SYMBOLS "$ENV{VELO_OS_SYMBOLS}")
endif()
if(VELO_OS_SYMBOLS)
    get_filename_component(VELO_OS_SYMBOLS "${VELO_OS_SYMBOLS}" ABSOLUTE)
    if(NOT EXISTS "${VELO_OS_SYMBOLS}/nk.elf" OR NOT EXISTS "${VELO_OS_SYMBOLS}/rom.elf")
        message(WARNING "VELO_OS_SYMBOLS is ${VELO_OS_SYMBOLS}, which lacks nk.elf or rom.elf")
    endif()
endif()

add_library(velo_headers INTERFACE)
target_include_directories(velo_headers SYSTEM INTERFACE "${VELO_TOOLCHAIN_ROOT}/include" "${VELO_TOOLCHAIN_ROOT}/include/w32api")
add_library(velo::headers ALIAS velo_headers)

file(GLOB VELO_RUNTIME_SOURCES "${VELO_TOOLCHAIN_ROOT}/runtime/*.c" "${VELO_TOOLCHAIN_ROOT}/runtime/compiler-rt/*.c")
add_library(velo_runtime STATIC EXCLUDE_FROM_ALL ${VELO_RUNTIME_SOURCES})
target_compile_options(velo_runtime PRIVATE -fno-builtin -w)
add_library(velo::runtime ALIAS velo_runtime)

file(GLOB VELO_CXX_RUNTIME_SOURCES "${VELO_TOOLCHAIN_ROOT}/runtime/cxx/*.cpp")
add_library(velo_cxx_runtime STATIC EXCLUDE_FROM_ALL ${VELO_CXX_RUNTIME_SOURCES})
target_link_libraries(velo_cxx_runtime PRIVATE velo_headers)
add_library(velo::cxx_runtime ALIAS velo_cxx_runtime)

file(GLOB VELO_EXPORT_LISTS "${VELO_EXPORTS_DIR}/*.txt")
file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/velo/${VELO_EXPORTS_NAME}")
foreach(exports IN LISTS VELO_EXPORT_LISTS)
    get_filename_component(dll "${exports}" NAME_WE)
    string(TOUPPER "${dll}" dll_upper)
    set(stubs "${CMAKE_BINARY_DIR}/velo/${VELO_EXPORTS_NAME}/${dll}.S")
    add_custom_command(
        OUTPUT "${stubs}"
        COMMAND "${Python3_EXECUTABLE}" "${VELO_TOOLCHAIN_ROOT}/tools/mkimplib.py" "${exports}" "${dll_upper}.dll" "${stubs}" --arch ${VELO_ARCH}
        DEPENDS "${exports}" "${VELO_TOOLCHAIN_ROOT}/tools/mkimplib.py"
        VERBATIM)
    add_library(velo_${dll} STATIC EXCLUDE_FROM_ALL "${stubs}")
    add_library(velo::${dll} ALIAS velo_${dll})
endforeach()

function(_velo_link target script cxx_entry)
    set_target_properties(${target} PROPERTIES SUFFIX ".elf" LINK_DEPENDS "${script}")
    target_link_options(${target} PRIVATE -T "${script}" "$<$<LINK_LANGUAGE:CXX>:--entry=${cxx_entry}>")
    target_link_libraries(${target} PRIVATE velo::headers velo::runtime velo::coredll "$<$<LINK_LANGUAGE:CXX>:velo::cxx_runtime>")
endfunction()

function(_velo_pe target output)
    set_property(TARGET ${target} APPEND PROPERTY LINK_DEPENDS "${VELO_TOOLCHAIN_ROOT}/tools/mkpe.py")
    add_custom_command(TARGET ${target} POST_BUILD
        COMMAND "${Python3_EXECUTABLE}" "${VELO_TOOLCHAIN_ROOT}/tools/mkpe.py" "$<TARGET_FILE:${target}>" "$<TARGET_FILE_DIR:${target}>/${output}" "--ce-version=${VELO_CE_VERSION}" ${ARGN}
        VERBATIM)
endfunction()

function(_velo_resources target out_options)
    set(options "")
    set(dependencies "")
    set(scripts "")
    foreach(resource IN LISTS ARGN)
        get_filename_component(resource "${resource}" ABSOLUTE)
        if(resource MATCHES "\\.rc$")
            list(APPEND scripts "${resource}")
        else()
            list(APPEND dependencies "${resource}")
        endif()
    endforeach()
    if(scripts)
        find_program(VELO_RC llvm-rc HINTS "${VELO_LLVM_ROOT}/bin" REQUIRED)
    endif()
    set(VELO_RC_ARCH_DEFINES "")
    foreach(define IN LISTS VELO_ARCH_DEFINES)
        list(APPEND VELO_RC_ARCH_DEFINES -D "${define}")
    endforeach()
    foreach(script IN LISTS scripts)
        get_filename_component(name "${script}" NAME_WE)
        get_filename_component(directory "${script}" DIRECTORY)
        set(compiled "${CMAKE_CURRENT_BINARY_DIR}/${target}_${name}.res")
        add_custom_command(
            OUTPUT "${compiled}"
            COMMAND "${VELO_RC}" -D "_WIN32_WCE=${VELO_WIN32_WCE}" ${VELO_RC_ARCH_DEFINES} -I "${VELO_TOOLCHAIN_ROOT}/include" -I "${VELO_TOOLCHAIN_ROOT}/include/w32api" -I "${directory}" -FO "${compiled}" -- "${script}"
            DEPENDS "${script}" ${dependencies}
            VERBATIM)
        target_sources(${target} PRIVATE "${compiled}")
        set_property(TARGET ${target} APPEND PROPERTY LINK_DEPENDS "${compiled}")
        list(APPEND options "--resources=${compiled}")
    endforeach()
    set(${out_options} ${options} PARENT_SCOPE)
endfunction()

function(_velo_gdb_scripts)
    get_property(executables GLOBAL PROPERTY VELO_EXECUTABLES)
    get_property(libraries GLOBAL PROPERTY VELO_LIBRARIES)
    set(library_folders "")
    set(library_uploads "")
    foreach(library IN LISTS libraries)
        get_target_property(output ${library} VELO_OUTPUT)
        string(APPEND library_folders ":$<TARGET_FILE_DIR:${library}>")
        string(APPEND library_uploads "  remote put \"$<TARGET_FILE_DIR:${library}>/${output}\" /Windows/${output}\n")
    endforeach()
    set(os_symbols_setup "")
    set(os_symbols_folder "")
    set(os_symbols_load "")
    if(VELO_OS_SYMBOLS)
        set(os_symbols_setup "add-symbol-file \"${VELO_OS_SYMBOLS}/nk.elf\"
python
import os
def velo_os_symbols(event):
    folder = os.path.realpath(\"${VELO_OS_SYMBOLS}\")
    loaded = [os.path.realpath(objfile.filename) for objfile in gdb.objfiles() if objfile.filename]
    if os.path.join(folder, \"rom.elf\") in loaded:
        return
    files = {name.lower(): name for name in os.listdir(folder)}
    for line in gdb.execute(\"info sharedlibrary\", to_string=True).splitlines():
        fields = line.split()
        if len(fields) < 2 or fields[-2] != \"No\":
            continue
        name = files.get(os.path.basename(fields[-1]).lower())
        if name and os.path.join(folder, name) not in loaded:
            gdb.execute(\"add-symbol-file \\\"%s\\\"\" % os.path.join(folder, name))
            loaded.append(os.path.join(folder, name))
    if [path for path in loaded if os.path.dirname(path) == folder and os.path.basename(path) != \"nk.elf\"]:
        return
    gdb.execute(\"remove-symbol-file \\\"%s\\\"\" % os.path.join(folder, \"nk.elf\"))
    gdb.execute(\"add-symbol-file \\\"%s\\\"\" % os.path.join(folder, \"rom.elf\"))
gdb.events.stop.connect(velo_os_symbols)
end
")
        set(os_symbols_folder ":${VELO_OS_SYMBOLS}")
        set(os_symbols_load "  sharedlibrary\n")
    endif()
    set(architecture "")
    if(VELO_ARCH STREQUAL "sh3")
        set(architecture "set architecture sh3\n")
    endif()
    foreach(target IN LISTS executables)
        get_target_property(output ${target} VELO_OUTPUT)
        file(GENERATE OUTPUT "$<TARGET_FILE:${target}>.gdb" CONTENT
"${architecture}set confirm off
set exec-file-mismatch off
maint set target-non-stop on
file \"$<TARGET_FILE:${target}>\"
${os_symbols_setup}set breakpoint pending on
set solib-search-path $<TARGET_FILE_DIR:${target}>${library_folders}${os_symbols_folder}
set remote exec-file /Windows/${output}
define velo-load
  remote put \"$<TARGET_FILE_DIR:${target}>/${output}\" /Windows/${output}
${library_uploads}${os_symbols_load}end
")
    endforeach()
endfunction()

function(_velo_add_gdb_script target output kind)
    set_target_properties(${target} PROPERTIES VELO_OUTPUT "${output}")
    set_property(GLOBAL APPEND PROPERTY ${kind} ${target})
    get_property(deferred GLOBAL PROPERTY VELO_GDB_SCRIPTS_DEFERRED)
    if(NOT deferred)
        set_property(GLOBAL PROPERTY VELO_GDB_SCRIPTS_DEFERRED TRUE)
        cmake_language(DEFER DIRECTORY "${CMAKE_SOURCE_DIR}" CALL _velo_gdb_scripts)
    endif()
endfunction()

function(_velo_require_exports target)
    foreach(export IN LISTS ARGN)
        string(REGEX REPLACE "^[^=]*=" "" symbol "${export}")
        target_link_options(${target} PRIVATE "--undefined=${symbol}")
    endforeach()
endfunction()

function(velo_add_executable target)
    cmake_parse_arguments(PARSE_ARGV 1 VELO "EXCLUDE_FROM_ALL" "OUTPUT;ICON" "RESOURCES")
    if(NOT VELO_OUTPUT)
        set(VELO_OUTPUT "${target}.exe")
    endif()
    set(exclude "")
    if(VELO_EXCLUDE_FROM_ALL)
        set(exclude EXCLUDE_FROM_ALL)
    endif()
    add_executable(${target} ${exclude} ${VELO_UNPARSED_ARGUMENTS})
    _velo_link(${target} "${VELO_TOOLCHAIN_ROOT}/ld/exe.ld" __velo_exe_start)
    set(options "")
    if(VELO_ICON)
        get_filename_component(icon "${VELO_ICON}" ABSOLUTE)
        set(options "--icon=${icon}")
    endif()
    _velo_resources(${target} resource_options ${VELO_RESOURCES})
    _velo_pe(${target} "${VELO_OUTPUT}" ${options} ${resource_options})
    _velo_add_gdb_script(${target} "${VELO_OUTPUT}" VELO_EXECUTABLES)
endfunction()

function(velo_add_library target)
    cmake_parse_arguments(PARSE_ARGV 1 VELO "EXCLUDE_FROM_ALL" "OUTPUT;EXPORTS_FILE" "EXPORTS;RESOURCES")
    if(NOT VELO_OUTPUT)
        set(VELO_OUTPUT "${target}.dll")
    endif()
    if(NOT VELO_EXPORTS AND NOT VELO_EXPORTS_FILE)
        message(FATAL_ERROR "velo_add_library(${target}) needs EXPORTS or EXPORTS_FILE")
    endif()
    set(exclude "")
    if(VELO_EXCLUDE_FROM_ALL)
        set(exclude EXCLUDE_FROM_ALL)
    endif()
    add_executable(${target} ${exclude} ${VELO_UNPARSED_ARGUMENTS})
    _velo_link(${target} "${VELO_TOOLCHAIN_ROOT}/ld/dll.ld" __velo_dll_start)
    set(options "")
    _velo_require_exports(${target} ${VELO_EXPORTS})
    if(VELO_EXPORTS)
        string(JOIN "," exports ${VELO_EXPORTS})
        list(APPEND options "--exports=${exports}")
    endif()
    if(VELO_EXPORTS_FILE)
        get_filename_component(exports_file "${VELO_EXPORTS_FILE}" ABSOLUTE)
        file(STRINGS "${exports_file}" file_exports)
        _velo_require_exports(${target} ${file_exports})
        list(APPEND options "--exports-file=${exports_file}")
        set_property(TARGET ${target} APPEND PROPERTY LINK_DEPENDS "${exports_file}")
    endif()
    _velo_resources(${target} resource_options ${VELO_RESOURCES})
    _velo_pe(${target} "${VELO_OUTPUT}" ${options} ${resource_options})
    if(NOT VELO_EXCLUDE_FROM_ALL)
        _velo_add_gdb_script(${target} "${VELO_OUTPUT}" VELO_LIBRARIES)
    endif()
endfunction()
