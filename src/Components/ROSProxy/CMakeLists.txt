# Include the directory itself as a path to include directories
SET(CMAKE_INCLUDE_CURRENT_DIR ON)

# Create a variable containing all .cpp files:
FILE(GLOB files *.cpp)

# Find required packages
FIND_PACKAGE( OpenCV REQUIRED )
FIND_PACKAGE(roscpp)
FIND_PACKAGE(std_msgs)
FIND_PACKAGE(serwo)
FIND_PACKAGE(rosconsole)

# Create an executable file from sources:
ADD_LIBRARY(ROSProxy SHARED ${files})

# Link external libraries
TARGET_LINK_LIBRARIES(ROSProxy ${DisCODe_LIBRARIES} ${OpenCV_LIBS} ${roscpp_LIBRARIES} ${std_msgs_LIBRARIES})

INSTALL_COMPONENT(ROSProxy)
