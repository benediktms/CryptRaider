// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerComponent.h"

UTriggerComponent::UTriggerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UTriggerComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UTriggerComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                      FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	const AActor* ActorForUnlock = GetAcceptableActor();

	if (ActorForUnlock == nullptr)
	{
		Mover->SetShouldMove(false);
	}
	else
	{
		UPrimitiveComponent* Component = Cast<UPrimitiveComponent>(ActorForUnlock->GetRootComponent());
		if (Component != nullptr)
		{
			Component->SetSimulatePhysics(false);
			Component->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);
		}

		Mover->SetShouldMove(true);
	}
}

void UTriggerComponent::SetMover(UMover* NewMover)
{
	Mover = NewMover;
}

AActor* UTriggerComponent::GetAcceptableActor() const
{
	TArray<AActor*> Actors;
	GetOverlappingActors(Actors);

	for (AActor* Actor : Actors)
	{
		if (Actor->ActorHasTag(AcceptableForUnlockTag) && !Actor->ActorHasTag("Grabbed"))
		{
			UE_LOG(LogTemp, Log, TEXT("Unlocking"));
			return Actor;
		}
	}

	return nullptr;
}
