// Copyright Epic Games, Inc. All Rights Reserved.

#include "AI_testCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include <Perception/AIPerceptionStimuliSourceComponent.h>
#include <Perception/AISense_Sight.h>
#include <Perception/AISense_Hearing.h>
#include <Perception/AISense_Damage.h>
#include "AI_test/Private/UInteractable.h"
#include "AI_test/Private/AI_controller.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DamageEvents.h"

//////////////////////////////////////////////////////////////////////////
// AAI_testCharacter

AAI_testCharacter::AAI_testCharacter()
{
	static ConstructorHelpers::FObjectFinder<USoundWave> shootRes(TEXT("/Script/Engine.SoundWave'/Game/LevelPrototyping/Sounds/Rifle02.Rifle02'"));

	if (shootRes.Succeeded())
		shootCue = shootRes.Object;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	//CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetupAttachment(this->GetMesh(), TEXT("headSocket"));

	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = true; 

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	_stimulus = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("Stimulus"));
	_stimulus->RegisterForSense(TSubclassOf<UAISense_Sight>());
	_stimulus->RegisterForSense(TSubclassOf<UAISense_Hearing>());
	_stimulus->RegisterForSense(TSubclassOf<UAISense_Damage>());
	_stimulus->RegisterWithPerceptionSystem();
}

void AAI_testCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void AAI_testCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAI_testCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AAI_testCharacter::Look);

		//Shooting
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Triggered, this, &AAI_testCharacter::Shoot);

		EnhancedInputComponent->BindAction(UseAction, ETriggerEvent::Triggered, this, &AAI_testCharacter::Interract);
	}
}

void AAI_testCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		//UAISense_Hearing::ReportNoiseEvent(GetWorld(), this->GetActorLocation(), 1, this, 5000.f);
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AAI_testCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AAI_testCharacter::Shoot(const FInputActionValue& Value)
{
	UAISense_Hearing::ReportNoiseEvent(GetWorld(), GetActorLocation(), 1, this, 5000.f);
	if (shootCue)
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), shootCue, GetActorLocation(), 0.15f);

	FHitResult Hit{};
	FCollisionQueryParams cpara{};
	cpara.AddIgnoredActor(this);
	FCollisionObjectQueryParams qpara{};
	FVector head = CameraBoom->GetComponentLocation();

	DrawDebugLine(GetWorld(), head, head + FVector(10000.f) * FRotationMatrix(Controller->GetControlRotation()).GetUnitAxis(EAxis::X), FColor(255, 0, 0), false, 5.f);
	
	if (GetWorld()->LineTraceSingleByObjectType(Hit, head,
		head + FVector(10000.f) * FRotationMatrix(Controller->GetControlRotation()).GetUnitAxis(EAxis::X), qpara, cpara))
	{
		if (AAI_testCharacter* ch = Cast<AAI_testCharacter>(Hit.GetActor()))
		{
			FPointDamageEvent de{};
			de.Damage = 20.f;
			ch->TakeDamage(20.f, de, Controller, this);
		}
	}
}

void AAI_testCharacter::Interract(const FInputActionValue& Value)
{
	FHitResult Hit{};
	FCollisionQueryParams cpara{};
	cpara.AddIgnoredActor(this);
	FCollisionObjectQueryParams qpara{};
	FVector head = CameraBoom->GetComponentLocation();

	DrawDebugLine(GetWorld(), head, head + FVector(1000.f) * FRotationMatrix(Controller->GetControlRotation()).GetUnitAxis(EAxis::X), FColor(0, 255, 0), false, 5.f);
	if (GetWorld()->LineTraceSingleByObjectType(Hit, head,
		head + FVector(1000.f) * FRotationMatrix(Controller->GetControlRotation()).GetUnitAxis(EAxis::X), qpara, cpara))
	{
		const auto& obj = Hit.GetActor()->GetInterfaceAddress(IUInteractable::UClassType::StaticClass());

		if (obj)
			Cast<IUInteractable>(Hit.GetActor())->Interract();
	}
}