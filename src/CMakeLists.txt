cmake_minimum_required( VERSION 3.10 )
project( homework10 VERSION 0.1.1 )

file( COPY ../data/test1.sh DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )
file( COPY ../data/test2.sh DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )
file( COPY ../data/test3.sh DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )
file( COPY ../data/test4.sh DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )
file( COPY ../data/test_all.sh DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )

file( COPY ../data/inp1.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )
file( COPY ../data/inp2.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )
file( COPY ../data/inp3.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )
file( COPY ../data/inp4.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )
file( COPY ../data/inp1_inline.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )

add_executable(bulk_server bulk_server.cpp server_connection.cpp service.cpp 
               executor.cpp console_executor.cpp file_executor.cpp )

add_library( async async.cpp async.h exe_dispatcher.cpp exe_dispatcher.h 
             executor.h executor.cpp console_executor.cpp console_executor.h 
             file_executor.cpp file_executor.h )

target_link_libraries( bulk_server PRIVATE async )

add_executable( bulk_client bulk_client.cpp client_connection.cpp )


# package part which enables
# run: cmake --build . --target package
# install(TARGETS bulk_server RUNTIME DESTINATION bin)
# set(CPACK_GENERATOR DEB)
# set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
# set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
# set(CPACK_PACKAGE_VERSION_PATCH "${PATCH_VERSION}")
# set(CPACK_PACKAGE_CONTACT berrendej@gmail.com)
# include(CPack)
