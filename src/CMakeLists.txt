#link_directories(${PROJECT_SOURCE_DIR}/src/libs)
#link_libraries(${PROJECT_SOURCE_DIR}/src/libs)
#include_directories(include)
find_package (glog 0.6.0 REQUIRED)
#
#
#
add_executable(go_wzxy go_wzxy.cpp )
target_link_libraries(go_wzxy cpr alibabacloud-sdk-core glog::glog)







#install(DIRECTORY cookie_json DESTINATION ${PROJECT_SOURCE_DIR}/build/bin )
install(PROGRAMS wzxy.sh DESTINATION ${PROJECT_SOURCE_DIR}/build/bin  )
install(FILES config.json DESTINATION ${PROJECT_SOURCE_DIR}/build/bin PERMISSIONS OWNER_READ )




