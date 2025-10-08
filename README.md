# Browsio

A small browser made in C++ by one person. I made this, because I'm curious on how the different tools we use day to day are made.

## Clone

To clone the whole project and submodules, run <br>
```git clone --recurse-submodules -j8 git://github.com/joaom/Browsio.git``` <br>
Or if you're using GithubCLI, run <br>
```gh repo clone https://github.com/joaom/Browsio.git --recursive```

## Dependencies

1. CMake - required to build the project
2. Ninja - the build system that CMake will create
2. OpenSSL - required by libcurl

If your on windows, you also have to download vcpkg to manage some other dependencies. 
