// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyProjectCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "MyProject.h"
#include "MyProjectCharacter.h"
#include "MyProjectGameMode.h"

AMyProjectCharacter::AMyProjectCharacter()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	//////////////////////////////////////////////////////
	OnTakeAnyDamage.AddDynamic(this, &AMyProjectCharacter::TakeAnyDamage);
	//////////////////////////////////////////////////////
}

void AMyProjectCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}

//////////////////////////////////////////////////////
void AMyProjectCharacter::TakeAnyDamage(AActor* DamageActor, float Damage, const UDamageType* TypeDamage, AController* InstigatedBy, AActor* DamageCauser)
{
	Health -= Damage;
	UE_LOG(LogMyProject, Log, TEXT("Health %f"), Health);
	if (Health <= 0.0f)
	{
		Death();
	}
}

void AMyProjectCharacter::Death()
{
	//UE_LOG(LogMyProject, Log, TEXT("Lose"));

	UWorld* World = GetWorld();

	if (IsValid(World) == false)
	{
		return;
	}
	
	AMyProjectGameMode* GameMode = Cast<AMyProjectGameMode>(World -> GetAuthGameMode());
	if (IsValid(GameMode))
	{
		GameMode->EndGame(false);
	}
	
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	FVector Location = GetActorLocation();
	FRotator Rotation = GetActorRotation();
	AActor* AfterDeathActor = World->SpawnActor(AfterDeathSpawnActorClass, &Location, &Rotation, SpawnParameters);

	if (IsValid(AfterDeathActor) == false)
	{
		UE_LOG(LogMyProject, Log, TEXT("IsValid(AfterDeathActor) == false"));
		return;
	}
	Destroy();
}
//////////////////////////////////////////////////////