// Copyright 2017-2019, Institute for Artificial Intelligence - University of Bremen
// Author: Andrei Haidu (http://haidu.eu)

#pragma once

#include "PIDController3D.h"
#include "MC6DControlType.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"

/**
 * 6D controller update callbacks
 */
struct FMC6DController
{
public:
	// Default constructor
	FMC6DController();

	// Destructor
	~FMC6DController();

	// Init as skeletal mesh
	void Init(USceneComponent* InTarget,
		USkeletalMeshComponent* InSelfAsSkeletalMesh,
		bool bApplyToAllBodies,
		EMC6DControlType ControlType,
		float PLoc, float ILoc, float DLoc, float MaxLoc,
		float PRot, float IRot, float DRot, float MaxRot);

	// Init as skeletal mesh with an offset
	void Init(USceneComponent* InTarget,
		USkeletalMeshComponent* InSelfAsSkeletalMesh,
		bool bApplyToAllBodies,
		EMC6DControlType ControlType,
		float PLoc, float ILoc, float DLoc, float MaxLoc,
		float PRot, float IRot, float DRot, float MaxRot,
		FTransform InOffset);

	// Init as static mesh
	void Init(USceneComponent* InTarget,
		UStaticMeshComponent* InSelfAsStaticMesh,
		EMC6DControlType ControlType,
		float PLoc, float ILoc, float DLoc, float MaxLoc,
		float PRot, float IRot, float DRot, float MaxRot);

	// Init as static mesh with an offset
	void Init(USceneComponent* InTarget,
		UStaticMeshComponent* InSelfAsStaticMesh,
		EMC6DControlType ControlType,
		float PLoc, float ILoc, float DLoc, float MaxLoc,
		float PRot, float IRot, float DRot, float MaxRot,
		FTransform InOffset);

	// Reset the location pid controller
	void ResetLoc(float P, float I, float D, float Max);

	// Reset the rotation pid controller
	void ResetRot(float P, float I, float D, float Max);

	// Call the update function pointer
	void Update(float DeltaTime);

private:
	// Get the location delta (error)
	FORCEINLINE FVector GetRotationDelta(const FQuat& From, const FQuat& To)
	{
		// TODO test internal versions as well using FQuat/FRotator SLerp / Lerp
		// Get the delta between the quaternions
		FQuat DeltaQuat = To * From.Inverse();

		// Avoid taking the long path around the sphere
		// // See void FQuat::EnforceShortestArcWith(const FQuat& OtherQuat)
		//	const float CosTheta = ToQuat | FromQuat;
		//	if (CosTheta < 0)
		if (DeltaQuat.W < 0.f)
		{
			DeltaQuat *= -1.f;
		}
		// The W part of the vector is always ~1.f, not relevant for applying the rotation
		return FVector(DeltaQuat.X, DeltaQuat.Y, DeltaQuat.Z);
	}

private:
	// Target (goal) component (to which transform to move to)
	USceneComponent* TargetSceneComp;

	// Relative offset to target (goal)
	FTransform LocalTargetOffset;

	// Self as skeletal mesh (from which transform to move away)
	USkeletalMeshComponent* SelfAsSkeletalMeshComp;

	// Flag to apply the controller on every body of the skeletal mesh
	bool bApplyToAllChildBodies;

	// Self as static mesh (from which transform to move away)
	UStaticMeshComponent* SelfAsStaticMeshComp;

	// Location pid controller
	FPIDController3D PIDLoc;

	// Rotation pid controller
	FPIDController3D PIDRot;

	/* Update function bindings */
	// Function pointer type for calling the correct update function
	typedef void(FMC6DController::*UpdateFunctionPointerType)(float);

	// Function pointer for update
	UpdateFunctionPointerType UpdateFunctionPointer;

	// Default update function
	void Update_NONE(float DeltaTime);

	// Skeletal updates
	void Update_Skel_Position(float DeltaTime);
	void Update_Skel_Velocity(float DeltaTime);
	void Update_Skel_Acceleration(float DeltaTime);
	void Update_Skel_Force(float DeltaTime);
	void Update_Skel_Impulse(float DeltaTime);

	// Skeletal updates with offset
	void Update_Skel_Position_Offset(float DeltaTime);
	void Update_Skel_Velocity_Offset(float DeltaTime);
	void Update_Skel_Acceleration_Offset(float DeltaTime);
	void Update_Skel_Force_Offset(float DeltaTime);
	void Update_Skel_Impulse_Offset(float DeltaTime);

	// Static mesh updates
	void Update_Static_Position(float DeltaTime);
	void Update_Static_Velocity(float DeltaTime);
	void Update_Static_Acceleration (float DeltaTime);
	void Update_Static_Force(float DeltaTime);
	void Update_Static_Impulse(float DeltaTime);

	// Static mesh updates with offset
	void Update_Static_Position_Offset(float DeltaTime);
	void Update_Static_Velocity_Offset(float DeltaTime);
	void Update_Static_Acceleration_Offset(float DeltaTime);
	void Update_Static_Force_Offset(float DeltaTime);
	void Update_Static_Impulse_Offset(float DeltaTime);
};
