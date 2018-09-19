// Fill out your copyright notice in the Description page of Project Settings.

#include "LoneSurvivorBullet.h"
#include "LoneSurvivorZombie.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Engine.h"
#include "GameFramework/ProjectileMovementComponent.h"

ALoneSurvivorBullet::ALoneSurvivorBullet()
{
	//Set the Bullet Speed
	GetProjectileComponent()->InitialSpeed = 90000.f;
	GetProjectileComponent()->MaxSpeed = 90000.f;

	//Set the damage done by the bullet
	OnHitDamage = 250.0f;

	GetCollisionComponent()->OnComponentHit.AddDynamic(this, &ALoneSurvivorBullet::OnHit);
}

float ALoneSurvivorBullet::GetBulletDamage() const
{
	return OnHitDamage;
}

void ALoneSurvivorBullet::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if ((OtherActor != NULL) && (OtherActor != NULL) && (OtherComp != NULL))
	{
		ALoneSurvivorZombie* HitActor = Cast<ALoneSurvivorZombie>(OtherActor);
		if (HitActor)
		{
			if (OtherComp == HitActor->GetBodyTriggerComponent())
			{
				HitActor->UpdateHealth(-OnHitDamage);
			}
			else if (OtherComp == HitActor->GetHeadTriggerComponent())
			{
				HitActor->SetNewDeadState(true);
			}

			FString message = TEXT("Zombie Hit");
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, message);
		}
	}
}

void ALoneSurvivorBullet::FireInDirection(const FVector& ShootDirection)
{
	GetProjectileComponent()->Velocity = ShootDirection * GetProjectileComponent()->InitialSpeed;
}