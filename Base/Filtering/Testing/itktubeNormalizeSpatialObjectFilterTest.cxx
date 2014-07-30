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

#include "itktubeNormalizeSpatialObjectFilter.h"

#include <itkSpatialObjectReader.h>
#include <itkSpatialObjectWriter.h>

int itktubeNormalizeSpatialObjectFilterTest( int argc, char* argv[] )
{
  if( argc < 3 )
    {
    std::cerr << "Usage: "
      << "inputTubeTree "
      << "tubeTreeToCompare "
      << std::endl;
    return EXIT_FAILURE;
    }
  const char * inputTubeTree = argv[1];
  const char * tubeTreeToCompare = argv[2];

  static const unsigned int Dimension = 3;
  typedef itk::VesselTubeSpatialObject< Dimension > TubeSpatialObjectType;
  typedef itk::GroupSpatialObject< Dimension >      GroupSpatialObjectType;

  // Read input tube tree.
  typedef itk::SpatialObjectReader< Dimension >  ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( inputTubeTree );
  try
    {
    reader->Update();
    }
  catch( itk::ExceptionObject & error )
    {
    std::cerr << "Error: " << error << std::endl;
    return EXIT_FAILURE;
    }
  GroupSpatialObjectType::Pointer groupSpatialObject = reader->GetGroup();

  typedef GroupSpatialObjectType::ChildrenListType GroupChildrenListType;

  char childName[] = "Tube";
  GroupChildrenListType* tubeList = groupSpatialObject->GetChildren(
      groupSpatialObject->GetMaximumDepth(), childName);

  if (tubeList->size() < 1)
    {
    std::cerr << "There should be at least one tube !" << std::endl;
    return EXIT_FAILURE;
    }

  typedef itk::VesselTubeSpatialObject<3> VesselTubeType;
  VesselTubeType* tube =
    dynamic_cast<VesselTubeType*>((tubeList->front()).GetPointer());
  if (!tube)
    {
    std::cerr << "The tube found MUST be a VesselTubeSpatialObject !" << std::endl;
    return EXIT_FAILURE;
    }

  typedef itk::tube::NormalizeSpatialObjectFilter< VesselTubeType >
      NormalizeSpatialObjectFilterType;
  NormalizeSpatialObjectFilterType::Pointer normalizeFilter =
    NormalizeSpatialObjectFilterType::New();
  normalizeFilter->SetInput(tube);
  try
    {
    normalizeFilter->Update();
    }
  catch( itk::ExceptionObject & error )
    {
    std::cerr << "Error: " << error << std::endl;
    return EXIT_FAILURE;
    }

  VesselTubeType* result = normalizeFilter->GetOutput();
  if (!result || result->GetPoints().size() < 2)
    {
    std::cerr<<"Error, output is null or empty !"<<std::endl;
    return EXIT_FAILURE;
    }

  ReaderType::Pointer toCompareReader = ReaderType::New();
  toCompareReader->SetFileName( tubeTreeToCompare );
  try
    {
    toCompareReader->Update();
    }
  catch( itk::ExceptionObject & error )
    {
    std::cerr << "Error: " << error << std::endl;
    return EXIT_FAILURE;
    }
  GroupSpatialObjectType::Pointer toCompareGroup = toCompareReader->GetGroup();

  GroupChildrenListType* toCompareList = toCompareGroup->GetChildren(
      toCompareGroup->GetMaximumDepth(), childName);
  assert(toCompareList->size() == 1);

  VesselTubeType* tubeToCompare =
    dynamic_cast<VesselTubeType*>((toCompareList->front()).GetPointer());
  assert(tubeToCompare);

  VesselTubeType::PointListType resultPoints = result->GetPoints();
  VesselTubeType::PointListType toComparePoints = tubeToCompare->GetPoints();
  assert(resultPoints.size() == toComparePoints.size());
  for (size_t i = 0; i < toComparePoints.size(); ++i)
    {
    VesselTubeType::PointType toComparePosition = toComparePoints[i].GetPosition();
    VesselTubeType::PointType resultPosition = resultPoints[i].GetPosition();
    if (toComparePosition.SquaredEuclideanDistanceTo(resultPosition) > 1e-6)
      {
      std::cerr<<"Error point are different for index "<<i<<std::endl
          <<toComparePosition<<" != "<<resultPosition<<std::endl;
      return EXIT_FAILURE;
      }

    float resultRadius = resultPoints[i].GetRadius();
    float toCompareRadius = toComparePoints[i].GetRadius();
    if (fabs(resultRadius - toCompareRadius) > 1e-6)
      {
      std::cerr<<"Error radiuses are different for index "<<i<<std::endl
          <<toCompareRadius<<" != "<<resultRadius<<std::endl;
      return EXIT_FAILURE;
      }
    }

  return EXIT_SUCCESS;
}
