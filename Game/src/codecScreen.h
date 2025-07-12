#pragma once
#include "UIBase.h" // HJAHAH

namespace skel
{
	class InputManager;
}

class ResourceManager;
class InputManager;
class TileSheet;
class Font;
class DialogueManager;

class CodecScreen : public UIBase
{
public:
	CodecScreen() = default;

	void Init(const ResourceManager& resourceManager, DialogueManager* dialogueManager);

	void Update(float deltaTime) override;

	void Draw(Surface& target, int2 offst = { 0, 0 }) const override;

	void OpenCodec();

	void HandleDialogue(const CodecDialogue* dialogue);

private:
	void NextLine();
	void SetText(); // set the text based of current dialogie count;

	skel::InputManager* inputMananger{ nullptr };
	DialogueManager* dialogueManager{ nullptr };


	Codec codec;
	PortraitFrame leftCharacter;
	PortraitFrame rightCharacter;
	
	CodecTextBox textBox;

	// 

	const CodecDialogue* currentDialogue{ nullptr };
	uint currentDialogueIndex{ 0 };

	float playerFrequency = 0.f;
	float playerFrequencySpeed = 25.f;
};

