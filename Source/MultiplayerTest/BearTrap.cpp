// Fill out your copyright notice in the Description page of Project Settings.


#include "BearTrap.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/DamageType.h"
#include "MultiplayerTestCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ABearTrap::ABearTrap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	//Definition for the SphereComponent that will serve as the Root component for the projectile and its collision.
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
	SphereComponent->InitSphereRadius(40.0f);
	SphereComponent->SetCollisionProfileName(TEXT("Trigger"));
	RootComponent = SphereComponent;

	//Registering the Projectile Impact function on a Hit event.
	if (GetLocalRole() == ROLE_Authority)
	{
		SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ABearTrap::OnOverlapBegin); 
	}

	//Definition for the Mesh that will serve as our visual representation.

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> DefaultMesh(TEXT("/Game/BearTrap/BearTrapMesh.BearTrapMesh"));
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	SkeletalMeshComponent->SetupAttachment(RootComponent);
	if (DefaultMesh.Succeeded())
	{

		SkeletalMeshComponent->SetSkeletalMesh(DefaultMesh.Object);
		SkeletalMeshComponent->SetAnimationMode(EAnimationMode::AnimationSingleNode);

		static ConstructorHelpers::FObjectFinder<UAnimationAsset> DefaultAnimationSequence(TEXT("/Game/BearTrap/BearTrap_Anim.BearTrap_Anim"));
		if (DefaultAnimationSequence.Succeeded())
		{
			SkeletalMeshComponent->SetAnimation(DefaultAnimationSequence.Object);
		}
	}
	
	bTrapTriggered = false;
}

// Called when the game starts or when spawned
void ABearTrap::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABearTrap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABearTrap::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && !bTrapTriggered) {
		ACharacter* character = dynamic_cast<ACharacter*>(OtherActor);

		if (character == NULL) 
			return;

		UCharacterMovementComponent* movement = character->GetCharacterMovement();

		// TODO Add delay before locking the player. Also check that they are still in the trigger when the lock actually happens.
		movement->MaxWalkSpeed = 0.0f;
		movement->MaxWalkSpeedCrouched = 0.0f;
		movement->SetJumpAllowed(false);
		//UGameplayStatics::ApplyDamage(OtherActor, 5.0f, GetInstigator()->Controller, this, UDamageType::StaticClass());
		bTrapTriggered = true;
		SkeletalMeshComponent->Play(false);
	}
}



