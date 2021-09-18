cmake_minimum_required(VERSION 3.0.0)
project(TestProject VERSION 0.1.0)

include(CTest)
enable_testing()


# 收集头文件和源文件
file(GLOB_RECURSE P_HEAD_FILES Code/*.h)
file(GLOB_RECURSE P_SOURCE_FILES Code/*.c*)
set(ALL_FILES ${P_HEAD_FILES} ${P_SOURCE_FILES})
macro(GROUP_FILES src_files)
        set(cur_dir ${CMAKE_CURRENT_SOURCE_DIR})
        foreach(group_files ${${src_files}})
            string(REGEX REPLACE ${cur_dir}/\(.*\) \\1 sgbd_fpath ${group_files})
            string(REGEX REPLACE "\(.*\)/.*" \\1 sgbd_group_name ${sgbd_fpath})
            string(COMPARE EQUAL ${sgbd_fpath} ${sgbd_group_name} sgbd_nogroup)
            string(REPLACE "/" "\\" sgbd_group_name ${sgbd_group_name})
            if(sgbd_nogroup)
                set(sgbd_group_name "common")
            endif(sgbd_nogroup)
                  
            source_group(${sgbd_group_name} FILES ${group_files})
            
        endforeach(group_files)
endmacro(GROUP_FILES)
GROUP_FILES(ALL_FILES)

add_executable(TestProject ${ALL_FILES})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
