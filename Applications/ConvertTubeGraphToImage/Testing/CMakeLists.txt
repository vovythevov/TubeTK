##############################################################################
#
# Library:   TubeTK
#
# Copyright 2010 Kitware Inc. 28 Corporate Drive,
# Clifton Park, NY, 12065, USA.
#
# All rights reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#       http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
##############################################################################

include_regular_expression( "^.*$" )

include( Midas3FunctionAddTest )
set( MIDAS_REST_URL http://midas3.kitware.com/midas/api/rest )
set( MIDAS_KEY_DIR ${TubeTK_SOURCE_DIR}/MIDAS_Keys )

set( TEMP ${TubeTK_BINARY_DIR}/Temporary )

set( PROJ_EXE
 ${TubeTK_LAUNCHER} $<TARGET_FILE:${MODULE_NAME}> )

set( IMAGECOMPARE_EXE
 ${TubeTK_LAUNCHER} $<TARGET_FILE:ImageCompareCommand> )

# Test1
Midas3FunctionAddTest( NAME ${MODULE_NAME}-Test1
  COMMAND ${PROJ_EXE}
    MIDAS{${MODULE_NAME}-CVT50-Branch-truth.cvt.mha.md5}
    MIDAS{${MODULE_NAME}-CVT50-Branch-truth.grp.md5}
    MIDAS_FETCH_ONLY{${MODULE_NAME}-CVT50-Branch-truth.grp.md5}
    MIDAS_FETCH_ONLY{${MODULE_NAME}-CVT50-Branch-truth.grp.mat.md5}
    MIDAS_FETCH_ONLY{${MODULE_NAME}-CVT50-Branch-truth.grp.brc.md5}
    MIDAS_FETCH_ONLY{${MODULE_NAME}-CVT50-Branch-truth.grp.rot.md5}
    ${TEMP}/${MODULE_NAME}-CVT50-Branch-truth.grp.Image )

# Test1-Compare
Midas3FunctionAddTest( NAME ${MODULE_NAME}-Test1-Compare-00
  COMMAND ${IMAGECOMPARE_EXE}
    -t ${TEMP}/${MODULE_NAME}-CVT50-Branch-truth.grp.Image.mat.mha
    -b MIDAS{${MODULE_NAME}-CVT50-Branch-truth.grp.Image.mat.mha.md5} )

set_property( TEST ${MODULE_NAME}-Test1-Compare-00
  APPEND PROPERTY DEPENDS ${MODULE_NAME}-Test1 )
