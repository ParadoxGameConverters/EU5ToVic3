cmake_minimum_required(VERSION 3.25)
set(VERBOSE true)

project(EU5ToVic3)
if (RUN_CLANG_TIDY)
	set(BUILD_TYPE "Clang-tidy")
else (RUN_CLANG_TIDY)
	set(BUILD_TYPE ${CMAKE_BUILD_TYPE})
endif (RUN_CLANG_TIDY)

set(CONVERTER_SOURCE_DIR ${CMAKE_SOURCE_DIR}/EU5ToVic3/Source)
set(CONVERTER_DIR ${CMAKE_SOURCE_DIR}/EU5ToVic3)
set(FRONTER_DIR ${CMAKE_SOURCE_DIR}/Fronter/)
set(FRONTER_SOURCE_DIR ${CMAKE_SOURCE_DIR}/Fronter/Fronter/Source)
set(FRONTER_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/${BUILD_TYPE}-${PLATFORM})
set(CONVERTER_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/${BUILD_TYPE}-${PLATFORM}/${PROJECT_NAME})
set(TEST_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/test/${BUILD_TYPE}-${PLATFORM})
set(CMAKE_CXX_STANDARD 23)
set(UNICODE_DEFAULT OFF)

if (PLATFORM STREQUAL "Windows")
	ADD_DEFINITIONS(-DUNICODE -D_UNICODE -DWINDOWS)
elseif (PLATFORM STREQUAL "Linux")
	if (COVERAGE STREQUAL true)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage")
		set(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE ON)
	else (COVERAGE STREQUAL true)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -Wall")
	endif (COVERAGE STREQUAL true)
endif (PLATFORM STREQUAL "Windows")

#### Fronter ####

# Ideally, this would all be set up in the Fronter's cmake file and we'd just need to `add_subdirectory(${FRONTER_DIR})`

if (PLATFORM STREQUAL "Windows")
	link_directories("${FRONTER_DIR}/external/wxWidgets/3.2.1/lib/vc14x_x64_dll")
	link_directories("${FRONTER_DIR}/external/curl/lib")
	find_library(LIBCURL_LIB libcurl HINT "${FRONTER_DIR}/external/curl/lib" NO_DEFAULT_PATH)
	find_library(LIBCURL_DLL_LIB libcurl.dll HINT "${FRONTER_DIR}/external/curl/lib" NO_DEFAULT_PATH)
endif (PLATFORM STREQUAL "Windows")

include_directories("${FRONTER_DIR}/external/SQLiteCpp/include")
include_directories("${FRONTER_DIR}/external/SQLiteCpp/sqlite3")
include_directories("${FRONTER_DIR}/external/commonItems/external/json/single_include")
if (PLATFORM STREQUAL "Windows")
	include_directories(${FRONTER_DIR}/external/curl/include)
	include_directories(${FRONTER_DIR}/external/wxWidgets/3.2.1/include)
	include_directories(${FRONTER_DIR}/external/wxWidgets/3.2.1/include/msvc)
	ADD_DEFINITIONS(-D_WINDOWS)
	ADD_DEFINITIONS(-D__WXMSW__)
	ADD_DEFINITIONS(-DwxMSVC_VERSION_AUTO)
	ADD_DEFINITIONS(-DWXUSINGDLL)
	ADD_DEFINITIONS(-DwxMSVC_VERSION_ABI_COMPAT)
elseif (PLATFORM STREQUAL "Linux")
	find_package(CURL REQUIRED)
	find_package(wxWidgets REQUIRED COMPONENTS net core base)
	include_directories("/usr/include/wx-3.2-unofficial")
	include_directories("/usr/lib/x86_64-linux-gnu/wx/include/gtk3-unicode-3.2-unofficial")
	include(${wxWidgets_USE_FILE})
	add_compile_options(-D_FILE_OFFSET_BITS=64 -DWXUSINGDLL -D__WXGTK__ -pthread)
endif (PLATFORM STREQUAL "Windows")

file(GLOB F_MAIN_SOURCES "${FRONTER_SOURCE_DIR}/*.cpp")
file(GLOB F_CONFIGURATION_SOURCES "${FRONTER_SOURCE_DIR}/Configuration/*.cpp")
file(GLOB F_CONFIGURATION_OPTIONS_SOURCES "${FRONTER_SOURCE_DIR}/Configuration/Options/*.cpp")
file(GLOB F_FRAMES_SOURCES "${FRONTER_SOURCE_DIR}/Frames/*.cpp")
file(GLOB F_FRAMES_TABS_SOURCES "${FRONTER_SOURCE_DIR}/Frames/Tabs/*.cpp")
file(GLOB F_UPDATECHECKER_SOURCES "${FRONTER_SOURCE_DIR}/UpdateChecker/*.cpp")
file(GLOB F_UTILS_SOURCES "${FRONTER_SOURCE_DIR}/Utils/*.cpp")
file(GLOB F_UTILS_LOCALIZATION_SOURCES "${FRONTER_SOURCE_DIR}/Utils/Localization/*.cpp")
file(GLOB F_CONVERTERLAUNCHER_SOURCES "${FRONTER_SOURCE_DIR}/WorkerThreads/ConverterLauncher/*.cpp")
if (PLATFORM STREQUAL "Windows")
	list(REMOVE_ITEM F_CONVERTERLAUNCHER_SOURCES "${FRONTER_SOURCE_DIR}/WorkerThreads/ConverterLauncher/LinuxConverterLauncher.cpp")
elseif (PLATFORM STREQUAL "Linux")
	list(REMOVE_ITEM F_CONVERTERLAUNCHER_SOURCES "${FRONTER_SOURCE_DIR}/WorkerThreads/ConverterLauncher/WinConverterLauncher.cpp")
endif (PLATFORM STREQUAL "Windows")
file(GLOB F_LOGWATCHER_SOURCES "${FRONTER_SOURCE_DIR}/WorkerThreads/LogWatcher/*.cpp")
file(GLOB F_MODCOPIER_SOURCES "${FRONTER_SOURCE_DIR}/WorkerThreads/ModCopier/*.cpp")
file(GLOB F_SQLITE_SOURCES "${FRONTER_SOURCE_DIR}/../../external/SQLiteCpp/src/*.cpp")
file(GLOB F_SQLITE3_SOURCES "${FRONTER_SOURCE_DIR}/../../external/SQLiteCpp/sqlite3/*.c")

if (PLATFORM STREQUAL "Windows")
	add_executable(ConverterFrontend WIN32
		${F_MAIN_SOURCES}
		${F_CONFIGURATION_SOURCES}
		${F_CONFIGURATION_OPTIONS_SOURCES}
		${F_FRAMES_SOURCES}
		${F_FRAMES_TABS_SOURCES}
		${F_UTILS_SOURCES}
		${F_UTILS_LOCALIZATION_SOURCES}
		${F_CONVERTERLAUNCHER_SOURCES}
		${F_LOGWATCHER_SOURCES}
		${F_MODCOPIER_SOURCES}
		${F_UPDATECHECKER_SOURCES}
		${F_SQLITE_SOURCES}
		${F_SQLITE3_SOURCES}
	 )
	 set_target_properties(ConverterFrontend
		PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY ${FRONTER_OUTPUT_DIRECTORY}
	 )
elseif (PLATFORM STREQUAL "Linux")
	add_executable(ConverterFrontend
		${F_MAIN_SOURCES}
		${F_CONFIGURATION_SOURCES}
		${F_CONFIGURATION_OPTIONS_SOURCES}
		${F_FRAMES_SOURCES}
		${F_FRAMES_TABS_SOURCES}
		${F_UTILS_SOURCES}
		${F_UTILS_LOCALIZATION_SOURCES}
		${F_CONVERTERLAUNCHER_SOURCES}
		${F_LOGWATCHER_SOURCES}
		${F_MODCOPIER_SOURCES}
		${F_UPDATECHECKER_SOURCES}
		${F_SQLITE_SOURCES}
		${F_SQLITE3_SOURCES}
	)
	set_target_properties(ConverterFrontend
		PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY ${FRONTER_OUTPUT_DIRECTORY}
	)
endif (PLATFORM STREQUAL "Windows")

if (PLATFORM STREQUAL "Windows")
	target_link_libraries(ConverterFrontend LINK_PUBLIC ${LIBCURL_LIB} ${LIBCURL_DLL_LIB} commonLib) 
elseif (PLATFORM STREQUAL "Linux")
	target_link_libraries(ConverterFrontend LINK_PUBLIC ${wxWidgets_LIBRARIES} ${CURL_LIBRARIES} commonLib)
endif (PLATFORM STREQUAL "Windows")

if (PLATFORM STREQUAL "Windows" AND NOT RUN_CLANG_TIDY)
	configure_file("external/Fronter/Fronter/Resources/converter.ico" "${FRONTER_OUTPUT_DIRECTORY}/converter.ico" COPYONLY)
	configure_file("external/Fronter/Fronter/Resources/GeneralFAQ-READ.ME.FIRST.txt" "${FRONTER_OUTPUT_DIRECTORY}/GeneralFAQ-READ.ME.FIRST.txt" COPYONLY)
	configure_file("external/Fronter/Fronter/Resources/libcrypto-1_1-x64.dll" "${FRONTER_OUTPUT_DIRECTORY}/libcrypto-1_1-x64.dll" COPYONLY)
	configure_file("external/Fronter/Fronter/Resources/libssl-1_1-x64.dll" "${FRONTER_OUTPUT_DIRECTORY}/libssl-1_1-x64.dll" COPYONLY)
	configure_file("external/Fronter/external/curl/bin/libcurl-x64.dll" "${FRONTER_OUTPUT_DIRECTORY}/libcurl-x64.dll" COPYONLY)
	if (CMAKE_BUILD_TYPE STREQUAL "Release")
		configure_file("external/Fronter/external/wxWidgets/3.2.1/lib/vc14x_x64_dll/wxbase32u_vc14x_x64.dll" "${FRONTER_OUTPUT_DIRECTORY}" COPYONLY)
		configure_file("external/Fronter/external/wxWidgets/3.2.1/lib/vc14x_x64_dll/wxmsw32u_core_vc14x_x64.dll" "${FRONTER_OUTPUT_DIRECTORY}" COPYONLY)
	elseif (CMAKE_BUILD_TYPE STREQUAL "Debug")
		configure_file("external/Fronter/external/wxWidgets/3.2.1/lib/vc14x_x64_dll/wxbase32ud_vc14x_x64.dll" "${FRONTER_OUTPUT_DIRECTORY}" COPYONLY)
		configure_file("external/Fronter/external/wxWidgets/3.2.1/lib/vc14x_x64_dll/wxmsw32ud_core_vc14x_x64.dll" "${FRONTER_OUTPUT_DIRECTORY}" COPYONLY)
	endif (CMAKE_BUILD_TYPE STREQUAL "Release")
	file(GLOB FRONTER_CONFIG "data/fronter/*")
	file(COPY ${FRONTER_CONFIG} DESTINATION "${FRONTER_OUTPUT_DIRECTORY}/Configuration/")
	file(GLOB LOCALIZATIONS "external/Fronter/Fronter/Resources/*.yml")
	file(COPY ${LOCALIZATIONS} DESTINATION "${FRONTER_OUTPUT_DIRECTORY}/Configuration/")
elseif (PLATFORM STREQUAL "Linux" AND NOT RUN_CLANG_TIDY)
	add_custom_command(TARGET ConverterFrontend
		COMMAND mkdir -p "${FRONTER_OUTPUT_DIRECTORY}/Configuration/" &&
		cp -r "data/fronter/*" "${FRONTER_OUTPUT_DIRECTORY}/Configuration/" &&
		cp -r "external/Fronter/Fronter/Resources/*yml" "${FRONTER_OUTPUT_DIRECTORY}/Configuration/" &&
		cp external/Fronter/Fronter/Resources/converter.ico ${FRONTER_OUTPUT_DIRECTORY} &&
		cp external/Fronter/Fronter/Resources/GeneralFAQ-READ.ME.FIRST.txt ${FRONTER_OUTPUT_DIRECTORY}
		WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
	)
endif (PLATFORM STREQUAL "Windows" AND NOT RUN_CLANG_TIDY)


### Converter ####

include_directories("${CONVERTER_DIR}")
include_directories("${CONVERTER_SOURCE_DIR}")
include_directories("${CONVERTER_SOURCE_DIR}/Mappers")
include_directories("${CONVERTER_SOURCE_DIR}/EU5World")
include_directories("${CONVERTER_SOURCE_DIR}/EU5World/Loaders")
include_directories("${CONVERTER_SOURCE_DIR}/V3World")
include_directories("${CONVERTER_SOURCE_DIR}/Helpers")
include_directories("${CONVERTER_SOURCE_DIR}/Output")
include_directories("${CONVERTER_SOURCE_DIR}/Configuration")

link_directories(${CONVERTER_DIR}/Resources)


set(PCH_SOURCES "commonItems/BulkParser.h")
set(PCH_SOURCES ${PCH_SOURCES} "commonItems/Color.h")
set(PCH_SOURCES ${PCH_SOURCES} "commonItems/CommonFunctions.h")
set(PCH_SOURCES ${PCH_SOURCES} "commonItems/CommonRegexes.h")
set(PCH_SOURCES ${PCH_SOURCES} "commonItems/ConvenientParser.h")
set(PCH_SOURCES ${PCH_SOURCES} "commonItems/ConverterVersion.h")
set(PCH_SOURCES ${PCH_SOURCES} "commonItems/Date.h")
set(PCH_SOURCES ${PCH_SOURCES} "commonItems/GameVersion.h")
set(PCH_SOURCES ${PCH_SOURCES} "commonItems/Log.h")
set(PCH_SOURCES ${PCH_SOURCES} "commonItems/OSCompatibilityLayer.h")
set(PCH_SOURCES ${PCH_SOURCES} "commonItems/Parser.h")
set(PCH_SOURCES ${PCH_SOURCES} "commonItems/ParserHelpers.h")
set(PCH_SOURCES ${PCH_SOURCES} "commonItems/StringUtils.h")
set(PCH_SOURCES ${PCH_SOURCES} "commonItems/targa.h")
set(PCH_SOURCES ${PCH_SOURCES} "commonItems/ModLoader/ModFilesystem.h")
set(PCH_SOURCES ${PCH_SOURCES} "commonItems/ModLoader/Mod.h")
set(PCH_SOURCES ${PCH_SOURCES} "commonItems/ModLoader/ModLoader.h")
set(PCH_SOURCES ${PCH_SOURCES} "commonItems/ModLoader/ModParser.h")
set(PCH_SOURCES ${PCH_SOURCES} "commonItems/Localization/LocalizationBlock.h")
set(PCH_SOURCES ${PCH_SOURCES} "commonItems/Localization/LocalizationDatabase.h")
set(PCH_SOURCES ${PCH_SOURCES} "commonItems/Localization/LocalizationLambdas.h")

set(COMMON_SOURCES "commonItems/CommonFunctions.cpp")
set(COMMON_SOURCES ${COMMON_SOURCES} "commonItems/Color.cpp")
set(COMMON_SOURCES ${COMMON_SOURCES} "commonItems/ConvenientParser.cpp")
set(COMMON_SOURCES ${COMMON_SOURCES} "commonItems/ConverterVersion.cpp")
set(COMMON_SOURCES ${COMMON_SOURCES} "commonItems/Date.cpp")
set(COMMON_SOURCES ${COMMON_SOURCES} "commonItems/GameVersion.cpp")
set(COMMON_SOURCES ${COMMON_SOURCES} "commonItems/LinuxUtils.cpp")
set(COMMON_SOURCES ${COMMON_SOURCES} "commonItems/Log.cpp")
set(COMMON_SOURCES ${COMMON_SOURCES} "commonItems/ModLoader/ModLoader.cpp")
set(COMMON_SOURCES ${COMMON_SOURCES} "commonItems/ModLoader/ModParser.cpp")
set(COMMON_SOURCES ${COMMON_SOURCES} "commonItems/Parser.cpp")
set(COMMON_SOURCES ${COMMON_SOURCES} "commonItems/ParserHelpers.cpp")
set(COMMON_SOURCES ${COMMON_SOURCES} "commonItems/OSCommonLayer.cpp")
set(COMMON_SOURCES ${COMMON_SOURCES} "commonItems/StringUtils.cpp")
set(COMMON_SOURCES ${COMMON_SOURCES} "commonItems/targa.cpp")
set(COMMON_SOURCES ${COMMON_SOURCES} "commonItems/ModLoader/ModFilesystem.cpp")
set(COMMON_SOURCES ${COMMON_SOURCES} "commonItems/ModLoader/ModLoader.cpp")
set(COMMON_SOURCES ${COMMON_SOURCES} "commonItems/ModLoader/ModParser.cpp")
set(COMMON_SOURCES ${COMMON_SOURCES} "commonItems/external/zip/src/zip.c")

add_library(CommonItems ${COMMON_SOURCES})

file(GLOB MAIN_SOURCES "${CONVERTER_SOURCE_DIR}/*.cpp")
file(GLOB CONFIGURATION_SOURCES "${CONVERTER_SOURCE_DIR}/Configuration/*.cpp")
file(GLOB EU5_WORLD_SOURCES "${CONVERTER_SOURCE_DIR}/EU5World/*.cpp")
file(GLOB EU5_SUBDIR_SOURCES "${CONVERTER_SOURCE_DIR}/EU5World/*/*.cpp")
file(GLOB EU5_SUBSUBDIR_SOURCES "${CONVERTER_SOURCE_DIR}/EU5World/*/*/*.cpp")
file(GLOB V3_WORLD_SOURCES "${CONVERTER_SOURCE_DIR}/V3World/*.cpp")
file(GLOB V3_SUBDIR_SOURCES "${CONVERTER_SOURCE_DIR}/V3World/*/*.cpp")
file(GLOB V3_SUBSUBDIR_SOURCES "${CONVERTER_SOURCE_DIR}/V3World/*/*/*.cpp")
file(GLOB V3_SUBSUBSUBDIR_SOURCES "${CONVERTER_SOURCE_DIR}/V3World/*/*/*/*.cpp")
file(GLOB OUT_WORLD_SOURCES "${CONVERTER_SOURCE_DIR}/Output/*.cpp")
file(GLOB OUT_SUBDIR_SOURCES "${CONVERTER_SOURCE_DIR}/Output/*/*.cpp")
file(GLOB OUT_SUBSUBDIR_SOURCES "${CONVERTER_SOURCE_DIR}/Output/*/*/*.cpp")
file(GLOB MAPPER_SOURCES "${CONVERTER_SOURCE_DIR}/Mappers/*/*.cpp")
file(GLOB MAPPER_SUBDIR_SOURCES "${CONVERTER_SOURCE_DIR}/Mappers/*/*/*.cpp")

link_directories("${CONVERTER_DIR}/resources") # You may need to place the rakaly files in a subfolder named rakaly for this to work properly

add_subdirectory(${CMAKE_SOURCE_DIR}/commonItems EXCLUDE_FROM_ALL)

if (RUN_CLANG_TIDY)
	set(CMAKE_CXX_CLANG_TIDY clang-tidy --header-filter=.*src.* --config-file=${CMAKE_SOURCE_DIR}/.clang-tidy --extra-arg=/EHsc)
endif(RUN_CLANG_TIDY)

add_library(EU5ToVic3lib
	${CONFIGURATION_SOURCES}
	${EU5_WORLD_SOURCES}
	${EU5_SUBDIR_SOURCES}
	${EU5_SUBSUBDIR_SOURCES}
	${V3_WORLD_SOURCES}
	${V3_SUBDIR_SOURCES}
	${V3_SUBSUBDIR_SOURCES}
	${V3_SUBSUBSUBDIR_SOURCES}
	${OUT_WORLD_SOURCES}
	${OUT_SUBDIR_SOURCES}
	${OUT_SUBSUBDIR_SOURCES}
	${MAPPER_SOURCES}
	${MAPPER_SUBDIR_SOURCES}
)

target_precompile_headers(EU5ToVic3lib PUBLIC ${PCH_SOURCES})
if (PLATFORM STREQUAL "Windows")
	target_compile_options(EU5ToVic3lib PRIVATE /permissive /W4 /w14242 /w14254 /w14263 /w14265 /w14287 /we4289 /w14296 
        /w14311 /w14545 /w14546 /w14547 /w14549 /w14555 /w14619 /w14640 /w14826 /w14905 /w14906 /w14928 
        /external:anglebrackets /external:W0 /WX)
	target_link_libraries(EU5ToVic3lib LINK_PUBLIC commonLib rakaly.dll.lib)
elseif (PLATFORM STREQUAL "Linux")
	target_link_libraries(EU5ToVic3lib LINK_PUBLIC commonLib rakaly)
endif (PLATFORM  STREQUAL "Windows")

add_executable(EU5ToVic3Converter
	${MAIN_SOURCES}
)
set_target_properties(EU5ToVic3Converter
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CONVERTER_OUTPUT_DIRECTORY}
)
target_precompile_headers(EU5ToVic3Converter REUSE_FROM EU5ToVic3lib)
target_link_libraries(EU5ToVic3Converter LINK_PUBLIC EU5ToVic3lib)
if (PLATFORM STREQUAL "Windows")
	target_compile_options(EU5ToVic3Converter PRIVATE /permissive /W4 /w14242 /w14254 /w14263 /w14265 /w14287 /we4289 /w14296 
        /w14311 /w14545 /w14546 /w14547 /w14549 /w14555 /w14619 /w14640 /w14826 /w14905 /w14906 /w14928 
        /external:anglebrackets /external:W0 /WX)
	target_link_libraries(EU5ToVic3Converter LINK_PUBLIC commonLib rakaly.dll.lib)
elseif (PLATFORM STREQUAL "Linux")
	target_link_libraries(EU5ToVic3Converter LINK_PUBLIC commonLib rakaly)
endif (PLATFORM  STREQUAL "Windows")

if (NOT RUN_CLANG_TIDY)
	configure_file("${CONVERTER_DIR}/Data_Files/configurables/version.txt" "${FRONTER_OUTPUT_DIRECTORY}/version.txt" COPYONLY)
	configure_file("${CONVERTER_DIR}/Data_Files/ReadMe.txt" "${FRONTER_OUTPUT_DIRECTORY}/ReadMe.txt" COPYONLY)
	file(COPY "${CONVERTER_DIR}/Data_Files/blankMod" DESTINATION "${CONVERTER_OUTPUT_DIRECTORY}")
	file(COPY "${CONVERTER_DIR}/Data_Files/configurables" DESTINATION "${CONVERTER_OUTPUT_DIRECTORY}")
	if (PLATFORM STREQUAL "Windows")
		configure_file("${CONVERTER_DIR}/resources/rakaly.dll" "${CONVERTER_OUTPUT_DIRECTORY}/rakaly.dll" COPYONLY) # if you update the rakaly location also update it here
	endif (PLATFORM STREQUAL "Windows")
	add_custom_command(TARGET EU5ToVic3Converter POST_BUILD WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} COMMAND git rev-parse HEAD > ${CONVERTER_OUTPUT_DIRECTORY}/../commit_id.txt)
endif (NOT RUN_CLANG_TIDY)

### TESTS ###

file(GLOB TEST_SOURCES "EU5ToVic3Tests/*.cpp")
file(GLOB TEST_SUB_SOURCES "EU5ToVic3Tests/*/*.cpp")
file(GLOB TEST_SUB_SUB_SOURCES "EU5ToVic3Tests/*/*/*.cpp")
set(GTEST_SOURCES ${GTEST_SOURCES} "commonItems/external/googletest/googletest/src/gtest_main.cc")
set(GTEST_SOURCES ${GTEST_SOURCES} "commonItems/external/googletest/googletest/src/gtest-all.cc")
set(GTEST_SOURCES ${GTEST_SOURCES} "commonItems/external/googletest/googlemock/src/gmock-all.cc")

add_executable(
	EU5ToVic3Tests
	${TEST_SOURCES}
	${TEST_SUB_SOURCES}
	${TEST_SUB_SUB_SOURCES}
	${GTEST_SOURCES}
)

set_target_properties(EU5ToVic3Tests
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIRECTORY}
)

target_precompile_headers(EU5ToVic3Tests REUSE_FROM EU5ToVic3lib)
target_link_libraries(EU5ToVic3Tests LINK_PUBLIC EU5ToVic3lib)

file(COPY "EU5ToVic3Tests/TestFiles" DESTINATION "${TEST_OUTPUT_DIRECTORY}")
