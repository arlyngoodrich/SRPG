// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingSystem/BuildingPiece.h"
#include "LogFiles.h"

//UE4 Includes
#include "Net/UnrealNetwork.h"

// Sets default values
ABuildingPiece::ABuildingPiece()
{


}

// Called when the game starts or when spawned
void ABuildingPiece::BeginPlay()
{
	Super::BeginPlay();
	
}




void ABuildingPiece::GetLifetimeReplicatedProps(TArray<FLifetimeProperty >& OutLifetimeProps) const
{

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABuildingPiece, BuildStatus);
}

void ABuildingPiece::BP_UpdateBuildStatus(EBuildingPieceStatus NewBuildStatus)
{

	if (GetLocalRole() >= ROLE_Authority)
	{
		UpdateBuildStatus(NewBuildStatus);
	}
	else
	{
		UE_LOG(LogBuildingSystem,Warning,TEXT("Attempted to set build status from client"))
	}

}

void ABuildingPiece::UpdateBuildStatus(EBuildingPieceStatus NewBuildStatus)
{

	if (NewBuildStatus == BuildStatus)
	{
		//do nothing it is the same..
		UE_LOG(LogBuildingSystem, Warning, TEXT("Build status change request is the same"))
	}
	else if (NewBuildStatus > BuildStatus)
	{
		//log warning cannot go backwards
		UE_LOG(LogBuildingSystem, Warning, TEXT("Attempting to set build piece status to lower state"))

	}
	else if (EBuildingPieceStatus::BPSE_Spawning == BuildStatus && EBuildingPieceStatus::BPSE_Set == NewBuildStatus)
	{
		BuildStatus = NewBuildStatus;
		UE_LOG(LogBuildingSystem, Log, TEXT("Build status changed to SET"))

		BP_OnBuildStatusChangeToSet();
	}
	else if (EBuildingPieceStatus::BPSE_Set == BuildStatus && EBuildingPieceStatus::BPSE_FullyBuilt == NewBuildStatus)
	{
		BuildStatus = NewBuildStatus;
		UE_LOG(LogBuildingSystem, Log, TEXT("Build status changed to FULLY BUILT"))

		BP_OnBuildStatusChangeToSet();
	}
}
