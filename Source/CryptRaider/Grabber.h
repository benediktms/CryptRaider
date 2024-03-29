// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/SceneComponent.h"
#include "CoreMinimal.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Grabber.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CRYPTRAIDER_API UGrabber : public USceneComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UGrabber();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(EditAnywhere)
	float Reach = 400;

	UPROPERTY(EditAnywhere)
	float GrabRadius = 100;

	UFUNCTION(BlueprintCallable, Category = "Grabber")
	void Grab();

	UFUNCTION(BlueprintCallable, Category = "Grabber")
	void Release();

	UPROPERTY(EditAnywhere)
	float GrabDistance = 200;

	UPhysicsHandleComponent* GetPhysicsHandle() const;

	bool HasGrabbedInReach(FHitResult& OutHitResult) const;

	FName GrabbedTag = "Grabbed";
};
