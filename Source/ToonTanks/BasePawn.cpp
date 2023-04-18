// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePawn.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Projectile.h"
#include "Particles/ParticleSystemComponent.h"
#include "Camera/CameraShakeBase.h"


// Sets default values
ABasePawn::ABasePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule collider"));
	RootComponent = CapsuleComp;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh component"));
	BaseMesh->SetupAttachment(CapsuleComp);

	TurretMesh=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Mesh component"));
	TurretMesh->SetupAttachment(BaseMesh); 

	SpawnPoint=CreateDefaultSubobject<USceneComponent>(TEXT("Spawn Point"));
	SpawnPoint->SetupAttachment(TurretMesh);

	// DeathParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Death Particle"));
	// DeathParticle->SetupAttachment(CapsuleComp);
}

void ABasePawn::HandleDestruction()
{
	
	if(DeathParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation
		(
			this,
			DeathParticle,
			GetActorLocation(),
			GetActorRotation()
		);
	}

	if(DeathSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation(), 0.2f);
	}

	if(DeathCameraShakeClass)
	{
		GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(DeathCameraShakeClass);
	}
}

void ABasePawn::RotateTurret(FVector LookAtTarget)
{
	FVector ToTarget = LookAtTarget - TurretMesh->GetComponentLocation();
	FRotator LookAtRotation = FRotator(0.f, ToTarget.Rotation().Yaw, 0.f);
	TurretMesh->SetWorldRotation (
		FMath::RInterpTo
		(TurretMesh->GetComponentRotation(), 
		LookAtRotation, 
		UGameplayStatics::GetWorldDeltaSeconds(this),
		5.f
		)
		);
}

void ABasePawn::Fire()
{

	FVector ProjectileLocation = SpawnPoint->GetComponentLocation();
	FRotator ProjectileRotation = SpawnPoint->GetComponentRotation();

	auto Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, ProjectileLocation, ProjectileRotation);
	Projectile->SetOwner(this);
}



