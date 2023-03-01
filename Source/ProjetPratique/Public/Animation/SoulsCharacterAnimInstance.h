// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Animation/CharacterAnimInstanceBase.h"
#include "SoulsCharacterAnimInstance.generated.h"

class ASoulsCharacter;

UCLASS()
class PROJETPRATIQUE_API USoulsCharacterAnimInstance : public UCharacterAnimInstanceBase
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	UPROPERTY(BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float MovementOffsetYaw;
};
