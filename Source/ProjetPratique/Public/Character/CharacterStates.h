#pragma once

UENUM(BlueprintType)
enum class EEquipementState : uint8
{
	ECS_Unequipped UMETA(DisplayName = "Unequipped"),
	ECS_Equipped_Dual UMETA(DisplayName = "Equipped Dual"),
	ECS_Equipped_Greatsword UMETA(DisplayName = "Equipped Greatsword")
};

UENUM(BlueprintType)
enum class EActionState : uint8
{
	EAS_Unocupied UMETA(DisplayName = "Unocupied"),
	EAS_Attacking UMETA(DisplayName = "Attacking"),
	EAS_Dodge UMETA(DisplayName = "Dodge")
};

UENUM(BlueprintType)
enum class ETargetState : uint8
{
	
	ETS_Targeted UMETA(DisplayName = "Targeted"),
	ETS_NonTargeted UMETA(DisplayName = "NonTargeted")
};

UENUM()
enum class EAIState : uint8
{
	EAS_Patrol,
	EAS_Combat,
	EAS_Dead
};
