// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseTrigger.generated.h"

UCLASS()
class MYPROJECT_API ABaseTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseTrigger();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	////////////////////////////////////////////
	virtual void Action(AActor* InTarget);
	virtual void ActionStart(AActor* InTarget);
	virtual void ActionEnd(AActor* InTarget);
	void CustomTick();
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY();
	AActor* Target;
	class USphereComponent* SphereComponent;
	
	UPROPERTY(EditAnywhere);
	float Range;
	UPROPERTY();
	float CustomTickRate = 1.0f;
	////////////////////////////////////////////

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};