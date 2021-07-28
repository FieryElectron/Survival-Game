#include "InteractionComponent.h"
#include "Player/SurvivalCharacter.h"
#include "Widgets/InteractionWidget.h"
#include "TimerManager.h"
#include "Net/UnrealNetwork.h"

UInteractionComponent::UInteractionComponent() {
	SetIsReplicated(true);
	SetComponentTickEnabled(false);

	InteractionTime = 0.5f;
	InteractionDistance = 200.0f;
	InteractableNameText = FText::FromString("Interactable Object");
	InteractableActionText = FText::FromString("Interact");

	Space = EWidgetSpace::Screen;
	DrawSize = FIntPoint(600, 100);
	bDrawAtDesiredSize = true;

	SetActive(true);
	SetIsCustomActive(true);
	SetHiddenInGame(true);
}

void UInteractionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UInteractionComponent, bIsCustomActive);
}

void UInteractionComponent::ShowOutLine(bool flag) {
	for (auto& VisualComp : GetOwner()->GetComponentsByClass(UPrimitiveComponent::StaticClass())) {
		if (UPrimitiveComponent* Prim = Cast<UPrimitiveComponent>(VisualComp)) {
			Prim->SetRenderCustomDepth(flag);
		}
	}
}

void UInteractionComponent::BeginFocus(class ASurvivalCharacter* Character){
	if (!IsActive() || !IsCustomActive() || !GetOwner() || !Character){
		return;
	}

	if (Character->IsLocallyControlled()){
		SetHiddenInGame(false);
		ShowOutLine(true);
		UpdateUserWidget();
	}
}

void UInteractionComponent::EndFocus(class ASurvivalCharacter* Character){
	if (Character->IsLocallyControlled()){
		SetHiddenInGame(true);
		ShowOutLine(false);
	}
}

bool UInteractionComponent::CanInteract(class ASurvivalCharacter* Character) const{
	return IsActive() && IsCustomActive() && GetOwner() != nullptr && Character != nullptr;
}

void UInteractionComponent::BeginInteract(class ASurvivalCharacter* Character){
	if (CanInteract(Character)){
		Interactors.AddUnique(Character);
	}
}

void UInteractionComponent::EndInteract(class ASurvivalCharacter* Character){
	Interactors.RemoveSingle(Character);
}

void UInteractionComponent::SetIsCustomActive(bool NewActive) {
	bIsCustomActive = NewActive;
}

void UInteractionComponent::Interact(class ASurvivalCharacter* Character){
	if (CanInteract(Character)){
		//SetIsCustomActive(false);
		if (GetNetMode() < NM_Client) {
			OnInteract.Broadcast(Character);
		}
	}
}

float UInteractionComponent::GetInteractPercentage(){
	if (Interactors.IsValidIndex(0)){
		if (ASurvivalCharacter* Interactor = Interactors[0]){
			if (Interactor && Interactor->IsInteracting()){
				return 1.f - FMath::Abs(Interactor->GetRemainingInteractTime() / InteractionTime);
			}
		}
	}
	return 0.0f;
}

void UInteractionComponent::UpdateUserWidget() {
	if (!bHiddenInGame && GetOwner()->GetNetMode() != NM_DedicatedServer) {
		if (UInteractionWidget* InteractionWidget = Cast<UInteractionWidget>(GetUserWidgetObject())) {
			InteractionWidget->UpdateInteractionWidget(this);
		}
	}
}



