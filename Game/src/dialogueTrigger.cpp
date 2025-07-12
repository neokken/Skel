#include "precomp.h"
#include "aabb.h"
#include "dialogueManager.h"
#include "dialogueTrigger.h"


DialogueTrigger::DialogueTrigger(const AABB& aabb, Level& _level, Player& _player, bool _crawlActivatable, DialogueManager* dialogueManager, const CodecDialogue* dialogue)
	: LevelTrigger(aabb, _level, _player, _crawlActivatable)
	, dialogueManager{dialogueManager}
	, dialogue{dialogue}
{
}

void DialogueTrigger::ActivateTrigger()
{
	if (!used)
		dialogueManager->SetActiveDialogue(dialogue,this);
}

void DialogueTrigger::SetUsed()
{
	used = true;
}

