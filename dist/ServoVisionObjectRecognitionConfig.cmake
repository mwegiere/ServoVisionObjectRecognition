# This file is used when other components needs to use something provided by this DCL. 
# Provide any include paths and lib directories. Use /home/mwegiere/DCL/ServoVisionObjectRecognition/dist
# to point to 'dist' directory of current DCL, it'll be substituted during installation. 

# directory containing header files
SET(ServoVisionObjectRecognition_INCLUDE_DIR /home/mwegiere/DCL/ServoVisionObjectRecognition/dist/include)
INCLUDE_DIRECTORIES(${ServoVisionObjectRecognition_INCLUDE_DIR})

# directory containing libraries
SET(ServoVisionObjectRecognition_LIB_DIR /home/mwegiere/DCL/ServoVisionObjectRecognition/dist/lib)
LINK_DIRECTORIES(${ServoVisionObjectRecognition_LIB_DIR})

# list of libraries to link against when using features of ServoVisionObjectRecognition
# add all additional libraries built by this dcl (NOT components)
# SET(ServoVisionObjectRecognition_LIBS lib_1 lib_2)
# SET(ADDITIONAL_LIB_DIRS /home/mwegiere/DCL/ServoVisionObjectRecognition/dist/lib ${ADDITIONAL_LIB_DIRS})
