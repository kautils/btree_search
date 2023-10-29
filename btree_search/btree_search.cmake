set(${PROJECT_NAME}_m_evacu ${m})
set(m ${PROJECT_NAME})
list(APPEND ${m}_unsetter )

set(${m}_kautil_cmake_heeder CMakeKautilHeader.v0.0.cmake)
if(DEFINED KAUTIL_THIRD_PARTY_DIR AND EXISTS "${KAUTIL_THIRD_PARTY_DIR}/${${m}_kautil_cmake_heeder}")
    include("${KAUTIL_THIRD_PARTY_DIR}/${${m}_kautil_cmake_heeder}")
else()
    if(NOT EXISTS ${CMAKE_BINARY_DIR}/${${m}_kautil_cmake_heeder})
        file(DOWNLOAD https://raw.githubusercontent.com/kautils/CMakeKautilHeader/v0.0/CMakeKautilHeader.cmake ${CMAKE_BINARY_DIR}/${${m}_kautil_cmake_heeder})
    endif()
    include(${CMAKE_BINARY_DIR}/${${m}_kautil_cmake_heeder})
endif()
git_clone(https://raw.githubusercontent.com/kautils/CMakeLibrarytemplate/v0.0.1/CMakeLibrarytemplate.cmake)


set(module_name btree_search)
unset(srcs)
set(${module_name}_common_pref
    MODULE_PREFIX kautil algorithm
    MODULE_NAME ${module_name}
    INCLUDES $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include> $<INSTALL_INTERFACE:include>  
    EXPORT_NAME_PREFIX ${PROJECT_NAME}
    EXPORT_VERSION ${PROJECT_VERSION}
    EXPORT_VERSION_COMPATIBILITY AnyNewerVersion
        
    DESTINATION_INCLUDE_DIR include/kautil/algorithm
    DESTINATION_CMAKE_DIR cmake
    DESTINATION_LIB_DIR lib
)

CMakeLibraryTemplate(${module_name} EXPORT_LIB_TYPE interface ${${module_name}_common_pref} )

list(APPEND ${m}_unsetter ${m}_hpps)
file(GLOB ${m}_hpps ${CMAKE_CURRENT_LIST_DIR}/*.hpp)
install(FILES ${${m}_hpps} DESTINATION include/kautil/algorithm/btree_search)


set(__t ${${module_name}_interface_tmain})
add_executable(${__t})
target_sources(${__t} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/unit_test.cc)
target_link_libraries(${__t} PRIVATE ${${module_name}_interface})
target_compile_definitions(${__t} PRIVATE ${${module_name}_interface_tmain_ppcs})




set(${m}_include_symlink ${CMAKE_CURRENT_LIST_DIR}/include/kautil/algorithm)
if(NOT EXISTS "${${m}_include_symlink}/btree_search")
    file(MAKE_DIRECTORY ${${m}_include_symlink})
    file(CREATE_LINK ${CMAKE_CURRENT_LIST_DIR} "${${m}_include_symlink}/btree_search" SYMBOLIC)
endif()


foreach(__v ${${m}_unsetter})
    unset(${__v})
endforeach()
unset(${m}_unsetter)
set(m ${${PROJECT_NAME}_m_evacu})
