cmake_minimum_required(VERSION 3.20)
set(__version 1.0.1)
project(KautilBtreeSearch.${__version}
        LANGUAGES C CXX
        VERSION ${__version}
        )
unset(__version)
include(${CMAKE_CURRENT_LIST_DIR}/btree_search/btree_search.cmake)