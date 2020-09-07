// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BearTrap.generated.h"

UCLASS()
class MULTIPLAYERTEST_API ABearTrap : public AActor
{
	GENERATED_BODY()

protected:
	FTimerHandle DamageTickTimer;
	//AMultiplayerTestCharacter* TrappedCharacter;
	
public:	
	// Sets default values for this actor's properties
	ABearTrap();

	UPROPERTY()
	bool bTrapTriggered;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage")
	float InitialDamage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage")
	float TickDamage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage")
	float TickRate;

	//The damage type and damage that will be done by this projectile
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage")
	TSubclassOf<class UDamageType> DamageType;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(Server, Reliable)
	void DealTickDamage();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Sphere component used to test collision.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USphereComponent* SphereComponent;

	// Static Mesh used to provide a visual representation of the object.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USkeletalMeshComponent* SkeletalMeshComponent;
};
