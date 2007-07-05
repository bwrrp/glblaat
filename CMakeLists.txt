# --------------------------------------------------
# (Sub)Project name
SET( ProjectName GLBlaat )

# --------------------------------------------------
# Modules

SET( ${ProjectName}_Modules
  Texture
  Framebuffer
  Shader
  Misc
)

# Friendly names for the modules
SET( ${ProjectName}_ModuleNames
  "Texturing"
  "Framebuffer Objects"
  "GLSL Shaders"
  "Helpful Things"
)

# --------------------------------------------------
# Classes

# Texturing classes
SET( ${ProjectName}_Texture_Classes
  GLTexture
  GLTextureRectangle
  GLTextureManager
)

# FBO framework
SET( ${ProjectName}_Framebuffer_Classes
  GLFramebuffer
  GLRendertarget
  GLRenderbuffer
  GLRenderTexture2D
  GLRenderTexture2DRectangle
)

# Shader framework
SET( ${ProjectName}_Shader_Classes
  GLProgram
  GLShader
  GLFragmentShader
  GLVertexShader
)

# Misc support classes
SET( ${ProjectName}_Misc_Classes
  GLUtility
)

# --------------------------------------------------
# Macros

MACRO( collect_class_source_files module class headers sources )
  # Find header file
  SET( headerfile "${CMAKE_CURRENT_SOURCE_DIR}/${module}/${class}.h" )
  IF( EXISTS ${headerfile} )
    SET( ${headers} ${${headers}} ${headerfile} )
  ELSE( EXISTS ${headerfile} )
    SET( headerfile "${CMAKE_CURRENT_SOURCE_DIR}/${class}.h" )
    IF( EXISTS ${headerfile} )
      SET( ${headers} ${${headers}} ${headerfile} )
    ELSE( EXISTS ${headerfile} )
      MESSAGE( SEND_ERROR "Class header not found for '${class}'" )
    ENDIF( EXISTS ${headerfile} )
  ENDIF( EXISTS ${headerfile} )
  
  # Find source files
  SET( classfile "${CMAKE_CURRENT_SOURCE_DIR}/${module}/${class}.cpp" )
  IF( EXISTS ${classfile} )
    SET( ${sources} ${${sources}} "${module}/${class}.cpp" )
  ELSE( EXISTS ${classfile} )
    SET( classfile "${CMAKE_CURRENT_SOURCE_DIR}/${module}/${class}.cxx" )
    IF( EXISTS ${classfile} )
      SET( ${sources} ${${sources}} "${module}/${class}.cxx" )
    ELSE( EXISTS ${classfile} )
      SET( classfile "${CMAKE_CURRENT_SOURCE_DIR}/${class}.cpp" )
      IF( EXISTS ${classfile} )
        SET( ${sources} ${${sources}} "${class}.cpp" )
      ELSE( EXISTS ${classfile} )
        SET( classfile "${CMAKE_CURRENT_SOURCE_DIR}/${class}.cxx" )
        IF( EXISTS ${classfile} )
          SET( ${sources} ${${sources}} "${class}.cxx" )
        ELSE( EXISTS ${classfile} )
          MESSAGE( SEND_ERROR "Class implementation not found for '${class}'" )
        ENDIF( EXISTS ${classfile} )
      ENDIF( EXISTS ${classfile} )
    ENDIF( EXISTS ${classfile} )
  ENDIF( EXISTS ${classfile} )
ENDMACRO( collect_class_source_files )

# --------------------------------------------------
# Collect source files and create source groups

SET( ${ProjectName}_Headers )
SET( ${ProjectName}_Source )
SET( ${ProjectName}_GeneratedSource )

# Iterate over the modules
LIST( LENGTH ${ProjectName}_Modules nmodules )
MATH( EXPR nmodules "${nmodules} - 1" )
FOREACH( i RANGE ${nmodules} )
  
  # Fetch name and label of the module
  LIST( GET ${ProjectName}_Modules ${i} module )
  LIST( GET ${ProjectName}_ModuleNames ${i} modulename )
  
  # See if there is a separate CMake include for this module
  SET( cmakeincludefile "${CMAKE_CURRENT_SOURCE_DIR}/${module}/sources.cmake" )
  IF( EXISTS ${cmakeincludefile} )
    
    # Include the file
    INCLUDE( ${cmakeincludefile} )
    
  ELSE( EXISTS ${cmakeincludefile} )
    
    # Iterate over classes
    SET( ${ProjectName}_${module}_Headers )
    SET( ${ProjectName}_${module}_Source )
    SET( ${ProjectName}_${module}_GeneratedSource )
    FOREACH( class ${${ProjectName}_${module}_Classes} )
      
      collect_class_source_files( 
        ${module} 
        ${class} 
        ${ProjectName}_${module}_Headers 
        ${ProjectName}_${module}_Source 
      )
      
    ENDFOREACH( class )
    
  ENDIF( EXISTS ${cmakeincludefile} )
  
  # Create source group for this module
  # Generated sources don't need to show up in this group, headers do
  SOURCE_GROUP( ${modulename} 
    FILES ${${ProjectName}_${module}_Source} ${${ProjectName}_${module}_Headers} 
  )
  # Append files to complete sets for the project
  SET( ${ProjectName}_Headers 
    ${${ProjectName}_Headers} 
    ${${ProjectName}_${module}_Headers} 
  )
  SET( ${ProjectName}_Source
    ${${ProjectName}_Source} 
    ${${ProjectName}_${module}_Source} 
    ${${ProjectName}_${module}_GeneratedSource}
  )
  SET( ${ProjectName}_GeneratedSource
    ${${ProjectName}_GeneratedSource}
    ${${ProjectName}_${module}_GeneratedSource}
  )
  
ENDFOREACH( i )

# Source group for generated files
SOURCE_GROUP( "Generated files"
  FILES ${${ProjectName}_GeneratedSource}
)

# --------------------------------------------------
# Libraries

# OpenGL and GLU
INCLUDE( ${CMAKE_ROOT}/Modules/FindOpenGL.cmake )
INCLUDE_DIRECTORIES( ${OPENGL_INCLUDE_DIR} )

# GLEW
IF( WIN32 )
  SET( GLEW_LIB glew32.lib )
ELSE( WIN32 )
  SET( GLEW_LIB libGLEW.so )
ENDIF( WIN32 )

# The complete set of libraries required by the project
SET( ${ProjectName}_LIBS
  ${OPENGL_LIBRARIES}
  ${GLEW_LIB}
)

# --------------------------------------------------------------------------
# Set up libraries and wrap classes for other languages

# Create the ${ProjectName} C++ library
ADD_LIBRARY( ${ProjectName} STATIC ${${ProjectName}_Source} )
TARGET_LINK_LIBRARIES( ${ProjectName} ${${ProjectName}_LIBS} )
