// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_WeaponTrail.generated.h"

//캐릭터한테 신호주는 정도로 만들기 
//Attack_Eable꺼 긁어오기 // 타이밍 같으면 같아도되긴함 근데 그냥함 
/**
 * 
 */
UCLASS()
class UNREALCPP_API UAnimNotifyState_WeaponTrail : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:
	virtual void NotifyBegin(
		USkeletalMeshComponent* MeshComp,
		UAnimSequenceBase* Animation,
		float TotalDuration,
		const FAnimNotifyEventReference& EventReference) override;

	virtual void NotifyEnd(
		USkeletalMeshComponent* MeshComp,
		UAnimSequenceBase* Animation,
		const FAnimNotifyEventReference& EventReference) override;

private:
	TWeakObjectPtr<class AActionCharacter> OwnerCharacter = nullptr;
};