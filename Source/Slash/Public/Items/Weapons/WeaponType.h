#pragma once

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	EWT_OneHand UMETA(DisplayName = "One hand weapon"),
	EWT_TwoHand UMETA(DisplayName = "Two hand weapon")
};