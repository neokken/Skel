#include "precomp.h"
#include "aabb.h"
#include "dialogueTrigger.h"
#include "dialogueManager.h"


const CodecDialogue* DialogueManager::GetDialogueFrequency(int frequency) const
{
	if (frequency == 85)
	{
		if (RandomFloat() > .5f)
		{
			return &CAMPB0_DIALOGUE;
		}
		else
		{
			return &CAMPB1_DIALOGUE;
		}
	}



	// default case
	return &EMPTY_FREQUENCY_DIALOGUE;
}

const CodecDialogue* DialogueManager::GetActiveDialogue() const
{
	return activeDialogue;
}

void DialogueManager::UseDialogue()
{

	activeDialogue = nullptr;
	if (activeTrigger)
	{
		activeTrigger->SetUsed();
		activeTrigger = nullptr;
	}
}

void DialogueManager::SetActiveDialogue(const CodecDialogue* dialogue, DialogueTrigger* trigger)
{
	activeDialogue = dialogue;
	activeTrigger = trigger;
}

void DialogueManager::ClearActiveDialogue()
{
	activeDialogue = nullptr;
}
