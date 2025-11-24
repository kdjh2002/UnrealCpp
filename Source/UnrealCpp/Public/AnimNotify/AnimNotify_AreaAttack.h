// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_AreaAttack.generated.h"

//애님노티파이어임으로 신호한번만 보내면됨
/**
 * 
 */
UCLASS()
class UNREALCPP_API UAnimNotify_AreaAttack : public UAnimNotify
{
	GENERATED_BODY()

	//범위공격은 캐릭터가 결정

public:
	virtual void Notify(
		USkeletalMeshComponent* MeshComp,
		UAnimSequenceBase* Animation,
		const FAnimNotifyEventReference& EventReference);

private:
	TWeakObjectPtr<class AActionCharacter> OwnerCharacter = nullptr;
};
