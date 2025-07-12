#include "precomp.h"
#include "resourceManager.h"
#include "dialogueManager.h"

#include "tilesheet.h"
#include "font.h"

#include "textBox.h"
#include "animatedTextBox.h"
#include "codecTextBox.h"

#include "codec.h"
#include "portraitFrame.h"


#include "codecScreen.h"

#include "Core/Engine.h"


void CodecScreen::Init(const ResourceManager& resourceManager, DialogueManager* _dialogueManager)
{
	inputMananger = &Engine::GetInstance().GetInput();
	dialogueManager = _dialogueManager;

	codec.Init(resourceManager);
	leftCharacter.Init(resourceManager);
	rightCharacter.Init(resourceManager);

	textBox.Init(resourceManager);

	leftCharacter.SetCharacter(Characters::Snake);
	textBox.Close(true);
}

void CodecScreen::Update(float deltaTime)
{
	codec.Update(deltaTime);
	leftCharacter.Update(deltaTime);
	rightCharacter.Update(deltaTime);
	textBox.Update(deltaTime);

	if ((currentDialogue != nullptr && inputMananger->IsKeyJustDown(GLFW_KEY_ENTER) || inputMananger->IsKeyJustDown(GLFW_KEY_N)) && textBox.GetCurrentState() == CodecTextBoxState::Open)
	{
		if (textBox.FinishedText())
		{
			NextLine();
		}
		else
		{
			textBox.SkipDialogue();
		}

	}


	if (currentDialogue == nullptr)
	{
		if (inputMananger->IsKeyDown(GLFW_KEY_RIGHT))
		{
			playerFrequency += deltaTime * playerFrequencySpeed;
		}

		if (inputMananger->IsKeyDown(GLFW_KEY_LEFT))
		{
			playerFrequency -= deltaTime * playerFrequencySpeed;
		}

		playerFrequency = std::clamp(playerFrequency, 0.f, 99.f);
		codec.SetFrequency(static_cast<int>(playerFrequency));

		if (inputMananger->IsKeyJustDown(GLFW_KEY_DOWN))
		{
			HandleDialogue(dialogueManager->GetDialogueFrequency(static_cast<int>(playerFrequency)));
		}

	}

}

void CodecScreen::Draw(Surface& target, int2 offset) const
{
	

	target.Bar(offset.x, offset.y, offset.x+ROOM_WIDTH * TERRAIN_TILE_SIZE, offset.y+ ROOM_HEIGHT * TERRAIN_TILE_SIZE, 0xff060606);
	
	int leftCenter = (ROOM_WIDTH * TERRAIN_TILE_SIZE - 60) / 2;
	codec.Draw(target, offset + int2(leftCenter, 0));

	leftCharacter.Draw(target, offset + int2((leftCenter - 36) / 2, 16));
	rightCharacter.Draw(target, offset + int2(leftCenter+60+(leftCenter - 36) / 2, 16));

	textBox.Draw(target, offset+ int2(0, 147));

}

void CodecScreen::OpenCodec()
{
	if (dialogueManager->GetActiveDialogue() != nullptr)
	{
		HandleDialogue(dialogueManager->GetActiveDialogue());
		codec.SetFrequency(dialogueManager->GetActiveDialogue()->frequency);
		dialogueManager->UseDialogue();
	}
	
}

void CodecScreen::HandleDialogue(const CodecDialogue* dialogue)
{
	if (dialogue == nullptr)
		return;

	currentDialogue = dialogue;
	currentDialogueIndex = 0;


	codec.SetDisconnected();

	leftCharacter.SetCharacter(currentDialogue->leftCharacter);
	leftCharacter.SetGlitching(currentDialogue->leftGlitching);
	rightCharacter.SetCharacter(currentDialogue->rightCharacter);
	rightCharacter.SetGlitching(currentDialogue->rightGlitching);

	SetText();
	textBox.Close(true);
	textBox.Open();
}

void CodecScreen::NextLine()
{
	
	if (currentDialogue && (currentDialogueIndex+1) >= currentDialogue->dialogueLineCount)
	{
		codec.Disconnect();
		rightCharacter.currentCharacter = Characters::None;
		textBox.Close();
		currentDialogue = nullptr;
	}
	else
	{
		currentDialogueIndex++;
		SetText();
	}

}

void CodecScreen::SetText()
{
	Characters talking = currentDialogue->dialogueLines[currentDialogueIndex].isSending ? currentDialogue->leftCharacter : currentDialogue->rightCharacter;

	textBox.SetText(talking, currentDialogue->dialogueLines[currentDialogueIndex].dialogueText);

	if (currentDialogue->dialogueLines[0].isSending)
	{
		codec.SetSending();
	}
	else
	{
		codec.SetReceiving();
		codec.Connect(); // apperantly only goes on when you receive
	}
}
