#pragma once

#include "CharInfoDefinitions.generated.h"

UENUM(BlueprintType)
enum class ECardinalDirections : uint8
{
	Forward,
	Backward,
	Right,
	Left
};

UENUM(BlueprintType)
enum class EMovementGait : uint8
{
	Walking,
	Jogging
};

UENUM(BlueprintType)
enum class ELocomotionMode : uint8
{
	Standing,
	Crouching
};

UENUM(BlueprintType)
enum class ELocomotionTypes : uint8
{
	Grounded,
	Swimming
};