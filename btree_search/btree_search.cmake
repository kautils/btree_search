set(${PROJECT_NAME}_m_evacu ${m})
set(m CMakePullLocalRepositoryAsSymLink)
list(APPEND ${m}_unsetter )


if(NOT EXISTS ${CMAKE_BINARY_DIR}/CMakeKautilHeader.cmake)
    file(DOWNLOAD https://raw.githubusercontent.com/kautils/CMakeKautilHeader/v0.0.1/CMakeKautilHeader.cmake ${CMAKE_BINARY_DIR}/CMakeKautilHeader.cmake)
endif()
include(${CMAKE_BINARY_DIR}/CMakeKautilHeader.cmake)
git_clone(https://raw.githubusercontent.com/kautils/CMakeLibrarytemplate/v0.0.1/CMakeLibrarytemplate.cmake)

set(module_name btree_search)
unset(srcs)
set(${module_name}_common_pref
    MODULE_PREFIX kautil algorithm
    MODULE_NAME ${module_name}
    INCLUDES $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}> $<INSTALL_INTERFACE:include>  
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
install(FILES ${${m}_hpps} DESTINATION include/kautil/algorithm)


set(__t ${${module_name}_interface_tmain})
add_executable(${__t})
target_sources(${__t} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/unit_test.cc)
target_link_libraries(${__t} PRIVATE ${${module_name}_interface})
target_compile_definitions(${__t} PRIVATE ${${module_name}_interface_tmain_ppcs})




if(NOT EXISTS "${CMAKE_CURRENT_LIST_DIR}/include/kautil/btree_search")
    file(MAKE_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/include/kautil)
    file(CREATE_LINK ${CMAKE_CURRENT_LIST_DIR} "${CMAKE_CURRENT_LIST_DIR}/include/kautil/btree_search" SYMBOLIC)
endif()


foreach(__v ${${m}_unsetter})
    unset(${__v})
endforeach()
unset(${m}_unsetter)
set(m ${${PROJECT_NAME}_m_evacu})
