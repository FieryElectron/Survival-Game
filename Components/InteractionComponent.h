#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "InteractionComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteract, class ASurvivalCharacter*, Character);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SURVIVALGAME_API UInteractionComponent : public UWidgetComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly)
	float InteractionTime;

	UPROPERTY(EditDefaultsOnly)
	float InteractionDistance;

	UPROPERTY(EditDefaultsOnly)
	FText InteractableNameText;

	UPROPERTY(EditDefaultsOnly)
	FText InteractableActionText;

	UPROPERTY()
	TArray<class ASurvivalCharacter*> Interactors;

	bool CanInteract(class ASurvivalCharacter* Character) const;

	UPROPERTY(Replicated)
	bool bIsCustomActive;

	void ShowOutLine(bool flag);

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
public:
	UInteractionComponent();

	//-- DELEGATES
	UPROPERTY(EditDefaultsOnly, BlueprintAssignable)
	FOnInteract OnInteract;

	//-- Gets --
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	FORCEINLINE float GetInteractionTime() const { return InteractionTime; }

	FORCEINLINE float GetInteractionDistance() const { return InteractionDistance; }

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	FORCEINLINE FText GetInteractableNameText() const { return InteractableNameText; }

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	FORCEINLINE FText GetInteractableActionText() const { return InteractableActionText; }

	FORCEINLINE bool IsCustomActive() const { return bIsCustomActive; }

	//-- Sets --
	FORCEINLINE void SetInteractionTime(float NewInteractionTime) { InteractionTime = NewInteractionTime; }
	FORCEINLINE void SetInteractionDistance(float NewInteractionDistance) { InteractionDistance = NewInteractionDistance; }
	FORCEINLINE void SetInteractableNameText(FText NewInteractableNameText) { InteractableNameText = NewInteractableNameText; }
	FORCEINLINE void SetInteractableActionText(FText NewInteractableActionText) { InteractableActionText = NewInteractableActionText; }

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void SetIsCustomActive(bool NewActive);
	

	void BeginFocus(class ASurvivalCharacter* Character);
	void EndFocus(class ASurvivalCharacter* Character);

	void BeginInteract(class ASurvivalCharacter* Character);
	void EndInteract(class ASurvivalCharacter* Character);

	void Interact(class ASurvivalCharacter* Character);

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void UpdateUserWidget();

	UFUNCTION(BlueprintPure, Category = "Interaction")
	float GetInteractPercentage();
};
