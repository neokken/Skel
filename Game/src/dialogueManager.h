#pragma once

class DialogueTrigger;

class DialogueManager
{
public:
	DialogueManager() = default;


	const CodecDialogue* GetDialogueFrequency(int frequency) const;


	const CodecDialogue* GetActiveDialogue() const;
	void UseDialogue();
	void SetActiveDialogue(const CodecDialogue* dialogue, DialogueTrigger* trigger);
	void ClearActiveDialogue();
	

private:
	const CodecDialogue* activeDialogue{ nullptr };
	DialogueTrigger* activeTrigger{ nullptr };
};

