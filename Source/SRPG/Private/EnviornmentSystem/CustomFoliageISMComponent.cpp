// Fill out your copyright notice in the Description page of Project Settings.


#include "EnviornmentSystem/CustomFoliageISMComponent.h"
#include "Engine/StaticMesh.h"
#include "Math/Vector.h"


void UCustomFoliageISMComponent::GetInstancesInRange(FVector TargetLocaiton, float Range, TArray<int32>& OutInstanceIndexies, TArray<FTransform>& OutInstanceTransforms, UStaticMesh*& OutMeshObject)
{

	int32 NumInstances;
	NumInstances = GetInstanceCount();

	TArray<FTransform> TransformsInRange;
	TArray<int32> InstancesInRange;

	for (int32 Index = 0; Index != NumInstances; ++Index)
	{
		FTransform TransformToAdd;
		if (GetInstanceTransform(Index, TransformToAdd, true))
		{
			FVector TransformLocaiton;
			float Distance;
			TransformLocaiton = TransformToAdd.GetLocation();

			Distance = FVector::Dist(TargetLocaiton, TransformLocaiton);

			if (Range >= Distance)
			{
				TransformsInRange.Add(TransformToAdd);
				InstancesInRange.Add(Index);
			}

		}
	}

	OutMeshObject = GetStaticMesh();
	OutInstanceIndexies = InstancesInRange;
	OutInstanceTransforms = TransformsInRange;

}


