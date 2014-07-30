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

#ifndef __itktubeNormalizeSpatialObjectFilter_hxx
#define __itktubeNormalizeSpatialObjectFilter_hxx

#include "itktubeNormalizeSpatialObjectFilter.h"

#include "itkTubeSpatialObject.h"

namespace itk
{

namespace tube
{

//----------------------------------------------------------------------------
template< class TSpatialObject >
NormalizeSpatialObjectFilter< TSpatialObject >
::NormalizeSpatialObjectFilter( void )
{
}

//----------------------------------------------------------------------------
template< class TSpatialObject >
void NormalizeSpatialObjectFilter< TSpatialObject >::GenerateData( void )
{
  typedef typename TubeSpatialObjectType::PointListType   PointListType;
  typedef typename TubeSpatialObjectType::TubePointType   TubePointType;
  typedef typename TubeSpatialObjectType::PointType       PointType;
  typedef typename PointType::Iterator                    PointIterarorType;
  typedef typename PointType::VectorType                  VectorType;
  typedef typename VectorType::Iterator                   VectorIterarorType;

  // Get input/output and make sure it's valid
  TubeSpatialObjectType* output = this->GetOutput();
  const TubeSpatialObjectType* input = this->GetInput();

  if (!input || !output)
    {
    std::cerr<<"Error in Normalize spatial object filter. Input and output "
      "MUST be an itk::TubeSpatialObject"<<std::endl;
    return;
    }

  const PointListType& points = input->GetPoints();
  if (points.size() < 2)
    {
    std::cerr<<"Cannot normalize input with less than 2 points"<<std::endl;
    return;
    }

  PointListType normalizedPoints;
  normalizedPoints.push_back(points.front()); // To prevent vessel shortening

  typename VectorType currentPosition = points[0].GetPosition().GetVectorFromOrigin();
  for (size_t i = 0; i < points.size() - 1; ++i)
    {
    typename VectorType referencePosition = points[i].GetPosition().GetVectorFromOrigin();
    typename VectorType nextReferencePosition = points[i + 1].GetPosition().GetVectorFromOrigin();
    typename VectorType referenceDirection = nextReferencePosition - referencePosition;
    referenceDirection.Normalize();

    float radius = points[i].GetRadius();
    typename VectorType direction = referenceDirection;


    while (referenceDirection * direction > 0.0)
      {
      typename TubePointType newPoint = points[i];

      typename VectorType positionSum(0.0f);
      float radiusSum = 0.0f;
      size_t numberOfPoints = 0;

      for (size_t j = 0; j < points.size(); ++j)
        {
        typename TubePointType point = points[j];
        typename VectorType position = point.GetPosition().GetVectorFromOrigin();

        if ((position - currentPosition).GetNorm() < radius)
          {
          radiusSum += point.GetRadius();
          positionSum += position;
          numberOfPoints += 1;
          }
        }

      if (numberOfPoints > 0)
        {
        float weight = 1.0f / numberOfPoints;
        radius = radiusSum * weight;
        currentPosition = positionSum * weight;
        }
      newPoint.SetPosition(currentPosition);
      newPoint.SetRadius(radius);
      normalizedPoints.push_back(newPoint);

      // Find the direction along which to increment the currentPosition
      typename VectorType localDirection = nextReferencePosition - currentPosition;
      if (direction.GetNorm() < 1e-6)
        {
        break;
        }
      direction.Normalize();

      // The new currentPosition is at radius / 2 along the vessel path.
      VectorIterarorType it = currentPosition.Begin();
      VectorIterarorType localDirectionIt = localDirection.Begin();
      for (; it != currentPosition.End(); ++it, ++localDirectionIt)
        {
        *it = (*it) + (*localDirectionIt)*(radius * 0.5);
        }

      // Re-actualize the direction to make sure we didn't pass the next
      // reference point.
      direction = nextReferencePosition - currentPosition;
      if (direction.GetNorm() < 1e-6)
        {
        break;
        }
      direction.Normalize();
      }
    }
  normalizedPoints.push_back(points.back()); // To prevent vessel shortening

  output->SetPoints(normalizedPoints);
}

} // End namespace tube

} // End namespace itk

#endif // End !defined(__itktubeNormalizeSpatialObjectFilter_hxx)
