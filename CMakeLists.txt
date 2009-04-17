# --------------------------------------------------
# Project name
SET( ProjectName GLBlaat )
# --------------------------------------------------
# Dependencies

cmake_minimum_required( VERSION 2.4.0 )
# OpenGL and GLU
FIND_PACKAGE( OpenGL REQUIRED )
INCLUDE_DIRECTORIES( ${OPENGL_INCLUDE_DIR} )
SET( OpenGL_lib
    ${OPENGL_LIBRARIES}
)

# GLEW
IF( WIN32 )
    IF( CYGWIN )
        FIND_LIBRARY( GLEW_lib 
            NAMES libGLEW GLEW glew32
            PATHS /usr/lib /usr/lib/w32api
        )
    ELSE( CYGWIN )
        # FIND_LIBRARY does not seem to work here (or maybe we need the paths...)
        SET( GLEW_lib
            glew32.lib
        )
    ENDIF( CYGWIN )
ELSE( WIN32 )
    FIND_LIBRARY( GLEW_lib
        NAMES libGLEW GLEW glew32
        PATHS /usr/lib /usr/local/lib
    )
ENDIF( WIN32 )

# The complete set of libraries required by the project
SET( ${ProjectName}_Libraries
    ${OpenGL_lib}
    ${GLEW_lib}
)

# --------------------------------------------------
# Modules

SET( ${ProjectName}_Modules
    Texture
    Framebuffer
    Shader
    Buffer
    Query
    Misc
)

# Friendly names for the modules
SET( ${ProjectName}_ModuleNames
    "Texturing"
    "Framebuffer Objects"
    "GLSL Shaders"
    "Buffer Objects"
    "Asynchronous Queries"
    "Helpful Things"
)

# --------------------------------------------------
# Classes for each of the modules

# Texturing classes
SET( ${ProjectName}_Texture_Classes
    GLTexture
    GLTextureRectangle
    GLTextureManager
    GLTexture3D
)

# FBO framework
SET( ${ProjectName}_Framebuffer_Classes
    GLFramebuffer
    GLRendertarget
    GLRenderbuffer
    GLRenderTexture2D
    GLRenderTexture2DRectangle
    GLRenderTexture3DLayer
)

# Shader framework
SET( ${ProjectName}_Shader_Classes
    GLProgram
    GLShader
    GLFragmentShader
    GLVertexShader
)

# Buffer Object framework (VBO, PBO)
SET( ${ProjectName}_Buffer_Classes
    GLBuffer
    GLPixelBuffer
)

# Asynchronous Query framework (occlusion / timer queries)
SET( ${ProjectName}_Query_Classes
    GLQuery
    GLOcclusionQuery
    GLTimerQuery
)

# Misc support classes
SET( ${ProjectName}_Misc_Classes
    GLUtility
)

# --------------------------------------------------
# Extra source files (e.g. program main)
SET( ${ProjectName}_ExtraSource
)

# ==================================================
# YOU DO NOT NEED TO MODIFY ANYTHING BELOW THIS LINE
# ==================================================

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
                    # Allow header-only implementations
                    # MESSAGE( SEND_ERROR "Class implementation not found for '${class}'" )
                ENDIF( EXISTS ${classfile} )
            ENDIF( EXISTS ${classfile} )
        ENDIF( EXISTS ${classfile} )
    ENDIF( EXISTS ${classfile} )
ENDMACRO( collect_class_source_files )

# --------------------------------------------------
# Collect source files and create source groups

SET( ${ProjectName}_Headers )
SET( ${ProjectName}_Source ${${ProjectName}_ExtraSource} )
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
        ${${ProjectName}_${module}_Headers} # Add headers as well
    )
    SET( ${ProjectName}_GeneratedSource
        ${${ProjectName}_GeneratedSource}
        ${${ProjectName}_${module}_GeneratedSource}
    )
ENDFOREACH( i )

# Add generated sources and give them a source group
SET( ${ProjectName}_Source
    ${${ProjectName}_Source}
    ${${ProjectName}_GeneratedSource}
)
SOURCE_GROUP( "Generated files"
    FILES ${${ProjectName}_GeneratedSource}
)

#INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_SOURCE_DIR} )

# --------------------------------------------------
# Library target

ADD_LIBRARY( 
    ${ProjectName} STATIC
    ${${ProjectName}_Source}
)

TARGET_LINK_LIBRARIES(
    ${ProjectName} 
    ${${ProjectName}_Libraries}
)
