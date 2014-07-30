/*=========================================================================

Library:   TubeTK

Copyright 2010 Kitware Inc. 28 Corporate Drive,
Clifton Park, NY, 12065, USA.

All rights reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

=========================================================================*/

#ifndef __itktubeNormalizeSpatialObjectFilter_h
#define __itktubeNormalizeSpatialObjectFilter_h

#include "itktubeSpatialObjectToSpatialObjectFilter.h"

namespace itk
{

namespace tube
{
/** \class SpatialObjectToSpatialObjectFilter
 *
 * \brief Base class for filters that take a SpatialObject as input and produce
 * a SpatialObject as output.
 */
template< class TSpatialObject >
class NormalizeSpatialObjectFilter
  : public SpatialObjectToSpatialObjectFilter< TSpatialObject, TSpatialObject >
{
public:
  /** Standard class typedefs */
  typedef NormalizeSpatialObjectFilter                          Self;
  typedef SpatialObjectToSpatialObjectFilter< TSpatialObject, TSpatialObject > Superclass;
  typedef SmartPointer< Self >                                  Pointer;
  typedef SmartPointer< const Self >                            ConstPointer;

  typedef TSpatialObject                                        TubeSpatialObjectType;

  /** Run-time type information (and related methods). */
  itkTypeMacro( NormalizeSpatialObjectFilter, SpatialObjectToSpatialObjectFilter );

  /** Method for creation through the object factory. */
  itkNewMacro( Self );

protected:
  NormalizeSpatialObjectFilter( void );
  virtual ~NormalizeSpatialObjectFilter( void ) {}

  virtual void GenerateData( void );

private:
  NormalizeSpatialObjectFilter( const Self & ); // purposely not implemented
  void operator=( const Self & );                     // purposely not implemented
}; // End class NormalizeSpatialObjectFilter

} // End namespace tube

} // End namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itktubeNormalizeSpatialObjectFilter.hxx"
#endif

#endif // End !defined(__itktubeNormalizeSpatialObjectFilter_h)
