// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingSystem/BuildingPiece.h"
#include "LogFiles.h"

//UE4 Includes
#include "Net/UnrealNetwork.h"

// Sets default values
ABuildingPiece::ABuildingPiece()
{
	bReplicates = true;
	BuildStatus = EBuildingPieceStatus::BPSE_Spawning;
	BuildingType = EBuildingTypes::BTE_None;

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
	DOREPLIFETIME(ABuildingPiece, ConnectionLevel);
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

void ABuildingPiece::BP_SetConnectionLevel(EBuildingConnectionLevel NewConnectionLevel)
{
	SetConnectionLevel(NewConnectionLevel);
}

void ABuildingPiece::BP_IncreaseConnectionLevel(EBuildingConnectionLevel InConnectionLevel, EBuildingConnectionLevel& OutConnecitonLevel)
{

	int8 InConnectionLevelByte = int8(InConnectionLevel);
	int8 OutConnectionLevelByte = InConnectionLevelByte + int8(1);

	OutConnecitonLevel = EBuildingConnectionLevel(OutConnectionLevelByte);

}



void ABuildingPiece::SetBuildStatusFromLoad(EBuildingPieceStatus LoadedBuildStatus)
{
	BuildStatus = LoadedBuildStatus;

	switch (BuildStatus)
	{
	case EBuildingPieceStatus::BPSE_Spawning:
		break;
	case EBuildingPieceStatus::BPSE_Set:
		BP_OnBuildStatusChangeToSet();
		break;
	case EBuildingPieceStatus::BPSE_FullyBuilt:
		BP_OnBuildStatusChangeToBuilt();
		break;
	default:
		break;
	}

}

void ABuildingPiece::UpdateBuildStatus(EBuildingPieceStatus NewBuildStatus)
{
		
	UE_LOG(LogBuildingSystem, Log, TEXT("Build status change request received"))

	if (NewBuildStatus == BuildStatus)
	{
		//do nothing it is the same..
		UE_LOG(LogBuildingSystem, Warning, TEXT("Build status change request is the same"))
	}
	else if (NewBuildStatus < BuildStatus)
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

		BP_OnBuildStatusChangeToBuilt();
	}
}

void ABuildingPiece::SetConnectionLevel(EBuildingConnectionLevel NewConnectionLevel)
{

	const TEnumAsByte<EBuildingConnectionLevel> ByteConnectionLevel = NewConnectionLevel;
	FString EnumAsString = UEnum::GetValueAsString(ByteConnectionLevel.GetValue());

	ConnectionLevel = NewConnectionLevel;
	UE_LOG(LogBuildingSystem, Log, TEXT("Connection Level Changed to %s"),*EnumAsString)

}
