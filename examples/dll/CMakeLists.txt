velo_add_library(greet greet.c EXPORTS Add Greeting)
velo_add_executable(greeter greeter.c)
add_dependencies(greeter greet)
