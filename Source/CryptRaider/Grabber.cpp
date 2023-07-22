// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"

#include "DrawDebugHelpers.h"
#include "Engine/World.h"

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();

	if (PhysicsHandle == nullptr)
	{
		return;
	}

	if (PhysicsHandle->GetGrabbedComponent() != nullptr)
	{
		const FVector TargetLocation = GetComponentLocation() + GetForwardVector() * GrabDistance;
		PhysicsHandle->SetTargetLocationAndRotation(TargetLocation, GetComponentRotation());
	}
}

void UGrabber::Grab()
{
	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();

	if (FHitResult HitResult;
		PhysicsHandle && HasGrabbedInReach(HitResult))
	{
		UPrimitiveComponent* HitComponent = HitResult.GetComponent();
		HitComponent->SetSimulatePhysics(true);
		HitComponent->WakeAllRigidBodies();
		
		AActor* HitActor = HitResult.GetActor();
		if (HitActor != nullptr)
		{
			HitActor->Tags.Add(GrabbedTag);
			HitActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		}

		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10, 10, FColor::Green, false, 5);

		PhysicsHandle->GrabComponentAtLocationWithRotation(
			HitComponent,
			NAME_None,
			HitResult.ImpactPoint,
			GetComponentRotation());


	}
}

void UGrabber::Release()
{
	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();

	if (UPrimitiveComponent* GrabbedComponent = PhysicsHandle->GetGrabbedComponent();
		PhysicsHandle && GrabbedComponent)
	{
		GrabbedComponent->WakeAllRigidBodies();
		GrabbedComponent->GetOwner()->Tags.Remove(GrabbedTag);
		PhysicsHandle->ReleaseComponent();
	}
}

UPhysicsHandleComponent* UGrabber::GetPhysicsHandle() const
{
	UPhysicsHandleComponent* Result = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (Result == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("No Physics Handle Component found on %s"), *GetOwner()->GetName());
	}

	return Result;
}

bool UGrabber::HasGrabbedInReach(FHitResult& OutHitResult) const
{
	const FVector Start = GetComponentLocation();
	const FVector End = Start + GetForwardVector() * Reach;
	DrawDebugLine(GetWorld(), Start, End, FColor::Red);
	DrawDebugSphere(GetWorld(), End, 10, 10, FColor::Red, false, 5);

	const FCollisionShape CollisionShape = FCollisionShape::MakeSphere(GrabRadius);

	// Do Geometry Sweep with Sphere
	// See Sweep Line Algorithm
	const bool HasHit = GetWorld()->SweepSingleByChannel(
		OutHitResult,
		Start,
		End,
		FQuat::Identity,
		ECC_GameTraceChannel2,
		CollisionShape);

	if (HasHit)
	{
		DrawDebugSphere(GetWorld(), OutHitResult.ImpactPoint, 10, 10, FColor::Green, false, 5);
	}

	return HasHit;
}
