// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/TP_Character.h"

//Custom Invludes
#include "Character/TP_CharacterMovement.h"
#include "LogFiles.h"

//UE4 Includes
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"


// Sets default values
ATP_Character::ATP_Character(const FObjectInitializer& ObjectInitializer)


	: Super(ObjectInitializer.SetDefaultSubobjectClass<UTP_CharacterMovement>(ACharacter::CharacterMovementComponentName))

{

	// ==== Default Values ===== =====
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	// ==== Components ===== =====

	//Add Spring Arm for Camera
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->bUsePawnControlRotation = false;
	SpringArmComp->bEnableCameraLag = true;
	SpringArmComp->bEnableCameraRotationLag = true;
	SpringArmComp->bInheritPitch = false;

	//Spring Arm default values
	SpringArmComp->SetRelativeLocation(FVector(20.f, 0.f, 85.f));
	SpringArmComp->SetRelativeRotation(FRotator(-15.f, 0.f, 0.f));
	SpringArmComp->TargetArmLength = 150.f;

	//Add Camera Component
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComp->SetupAttachment(SpringArmComp);


	//Default Movement Values
	GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;
	GetCharacterMovement()->MaxWalkSpeed = DefaultWalkSpeed;
	GetCharacterMovement()->MaxWalkSpeedCrouched = DefaultCrouchSpeed;


}


// Called when the game starts or when spawned
void ATP_Character::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ATP_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// ==== Replicated Variables ===== =====

void ATP_Character::GetLifetimeReplicatedProps(TArray<FLifetimeProperty >& OutLifetimeProps) const
{

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATP_Character, bWantsToSprint);
	DOREPLIFETIME(ATP_Character, bIsSprinting);
	DOREPLIFETIME(ATP_Character, SprintSpeedMuliplyer)


}




// ==== Movement ===== =====


// Called to bind functionality to input
void ATP_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ATP_Character::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ATP_Character::MoveRight);
	PlayerInputComponent->BindAxis("TurnRight", this, &ATP_Character::TurnRight);
	PlayerInputComponent->BindAxis("LookUp", this, &ATP_Character::LookUp);


	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ATP_Character::Jump);
	PlayerInputComponent->BindAction("ToggleCrouch", IE_Pressed, this, &ATP_Character::ToggleCrouch);
	PlayerInputComponent->BindAction("ToggleSprint", IE_Pressed, this, &ATP_Character::SetWantsToSprint);

}

bool ATP_Character::GetWantsToSprint()
{
	// resets sprint if not moving
	if (GetVelocity().Size() == 0 && bWantsToSprint == true)
	{
		SetWantsToSprint();
	}

	return bWantsToSprint;
}

float ATP_Character::GetSprintSpeedModifier() { return SprintSpeedMuliplyer; }

void ATP_Character::SetSprintSpeedModifer(float NewSprintModifer)
{
	if (HasAuthority())
	{
		SprintSpeedMuliplyer = NewSprintModifer;
	}

}

float ATP_Character::GetDefaultWalkSpeed() { return DefaultWalkSpeed; }


//Forward and Back
void ATP_Character::MoveForward(float Value)
{

	AddMovementInput(GetActorForwardVector() * Value);

}


//Right and Left
void ATP_Character::MoveRight(float Value)
{

	AddMovementInput(GetActorRightVector() * Value);

}

void ATP_Character::TurnRight(float Value)
{

	AddControllerYawInput(Value);

}


void ATP_Character::LookUp(float Value)
{

	if (CameraComp == nullptr) { return; }

	AddControllerPitchInput(Value);


}

void ATP_Character::Jump()
{
	Super::Jump();
}

void ATP_Character::ToggleCrouch()
{
	if (GetCharacterMovement()->IsCrouching())
	{
		UnCrouch();
	}
	else
	{
		Crouch();
		if (bWantsToSprint == true) { SetWantsToSprint(); }
	}
}



void ATP_Character::SetWantsToSprint()
{

	if (GetLocalRole() < ROLE_Authority)
	{
		// Is NOT Authority
		Server_SetWantsToSprint();
	}
	else
	{
		//Is Authority
		if (bWantsToSprint == false)
		{
			bWantsToSprint = true;
			WantsToSprint_OnSprintStart.Broadcast();
			UE_LOG(LogTPCharacter, Log, TEXT("Character wants to sprint"))

		}
		else
		{
			bWantsToSprint = false;
			WantsToNotSprint_OnSptrintStop.Broadcast();
			UE_LOG(LogTPCharacter, Log, TEXT("Character wants to stop sprint"))
		}
	}
}


bool ATP_Character::Server_SetWantsToSprint_Validate()
{
	return true;
}

void ATP_Character::Server_SetWantsToSprint_Implementation()
{
	SetWantsToSprint();

}

float ATP_Character::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	Damage_DamageTaken.Broadcast(DamageAmount);

	return DamageAmount;
}

