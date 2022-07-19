



add_executable(posttest posttest.cpp)
target_link_libraries(posttest workflow wfrest)



install(DIRECTORY html DESTINATION ${PROJECT_SOURCE_DIR}/build/webserverBin )




