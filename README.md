MCHECKSUM
---------

These are the release notes for MChecksum.

WHAT IS MCHECKSUM?

   MChecksum is a generic checksum library.

   Please see the accompanying COPYING file for license details.

ARCHITECTURES SUPPORTED:

DOCUMENTATION:

SOFTWARE REQUIREMENTS:

BUILDING:

 - If you install the full sources, put the tarball in a directory where you
   have permissions (e.g., your home directory) and unpack it:

     gzip -cd mchecksum-X.tar.gz | tar xvf -

   or

     bzip2 -dc mchecksum-X.tar.bz2 | tar xvf -

   Replace "X" with the version number of the package.

 - MChecksum makes use of the CMake build-system and requires that you do an
   out-of-source build. In order to do that, you must create a new build
   directory and run the 'ccmake' command from it:

     cd mchecksum-X
     mkdir build
     cd build
     ccmake .. (where ".." is the relative path to the mchecksum-X directory)

 - Type 'c' multiple times and choose suitable options. Recommended options
   are:

     BUILD_SHARED_LIBS                OFF (or ON if the library you link
                                      against does not require static libraries)
     BUILD_TESTING                    ON
     CMAKE_INSTALL_PREFIX             /path/to/install/directory

   Setting include directory and library paths may require you to toggle to
   the advanced mode by typing 't'. Once you are done and do not see any
   errors, type 'g' to generate makefiles. Once you exit the CMake
   configuration screen and are ready to build the targets, do:

      make

 - (Optional) Verbose compile/build output:

   This is done by inserting "VERBOSE=1" in the "make" command. E.g.:

     make VERBOSE=1

INSTALLING:

 - Assuming that the CMAKE_INSTALL_PREFIX has been set (see previous step)
   and that you have write permissions to the destination directory, do
   from the build directory:
   
     make install
     
TESTING:

 - CTest is used to run the tests, simply run from the build directory:

     ctest .

 - (Optional) Verbose testing:

   This is done by inserting "-V" in the "ctest" command.  E.g.:

     ctest -V .

   Extra verbose information can be displayed by inserting "-VV". E.g.:

     ctest -VV .

