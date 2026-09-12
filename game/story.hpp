#include "lib/gameState.hpp"
#include "lib/dialogue.hpp"

#include "bounding.hpp"
#include "graphics.hpp"
#include "raylib.h"

#define CIRNO_DIALOG "Cirno",

inline void ProcessClick(GameState &gs) {
    if (gs.GameOver) return;
    if (gs.DialogueOnlyInput) return;
    
    if (CheckCollisionPointRec(gs.currentMousePos, Forehead)) {
        if (gs.flickCount == 0) {
            SayDialog(gs, std::vector<Dialogues>{
                {CIRNO_DIALOG HURT, "Owww"},
                {CIRNO_DIALOG PISSED, "It hurts."},
                {CIRNO_DIALOG PISSED, "Why did you do that?"}
            });
            gs.flickCount++;
        } else if (gs.flickCount == 1) {
            SayDialog(gs, std::vector<Dialogues> {
                {CIRNO_DIALOG HURT, "You did it again!"},
                {CIRNO_DIALOG PISSED, "Stop it!"},
            });
            gs.flickCount++;
        } else if (gs.flickCount == 2) {
            SayDialog(gs, std::vector<Dialogues> {
                {CIRNO_DIALOG PISSED, "No, I won't let you\ndo it again"},
                {CIRNO_DIALOG PISSED, "You meanie!!"},
            });
            gs.explored.insert(1);
        }
    }
    else if (CheckCollisionPointRec(gs.currentMousePos, UI_arrowBtn)) {
        if (gs.curPos == INSIDE) {
            changeSceneTo(gs, OUTSIDE);
            gs.explored.insert(2);
        } else if (gs.curPos == OUTSIDE) {
            changeSceneTo(gs, FOREST);
            gs.explored.insert(3);
        } else if (gs.curPos == FOREST) {
            changeSceneTo(gs, INSIDE);
        }
    } else if (CheckCollisionPointRec(gs.currentMousePos, UI_talkBtn)) {
        if (gs.TalkButtonUseCount == 0) {
            SayDialog(gs, std::vector<Dialogues> {
                {CIRNO_DIALOG RELAXED, "Good Morning"},
            });
            gs.TalkButtonUseCount++;
        } else if (gs.TalkButtonUseCount == 1) {
            SayDialog(gs, std::vector<Dialogues> {
                {CIRNO_DIALOG JOY, "Having lots of fun is\nwhat matters."},
                {CIRNO_DIALOG RELAXED, "Imagine having no fun\nand being grumpy\nall the time."},
            });
            gs.TalkButtonUseCount++;
        } else if (gs.TalkButtonUseCount == 2) {
            SayDialog(gs, std::vector<Dialogues> {
                {CIRNO_DIALOG JOY, "Next time, when you come,\nwe can play together with\nDaiyousei and the other"},
                {CIRNO_DIALOG JOY, "fairies."},
            });
            gs.TalkButtonUseCount++;
        } else if (gs.TalkButtonUseCount == 3) {
            SayDialog(gs, std::vector<Dialogues> {
                {CIRNO_DIALOG RELAXED, "It would be nice if\nwinter comes soon."},
                {CIRNO_DIALOG JOY, "I want to skate on the\nmisty lake with Letty."},
            });
            gs.TalkButtonUseCount++;
        } else if (gs.TalkButtonUseCount == 4) {
            SayDialog(gs, std::vector<Dialogues> {
                {CIRNO_DIALOG WOW, "I am the smartest in\nGensokyo. I can add\nand multiply fractions!"},
                {CIRNO_DIALOG LAUGH, "Only a few could rival\nan intellect such\nas mine."}
            });
            gs.TalkButtonUseCount = 1;
            gs.explored.insert(4);
        }
    }
    else if (gs.curPos == INSIDE) {
        if (CheckCollisionPointRec(gs.currentMousePos, Calendar)) {
            SayDialog(gs, std::vector<Dialogues> {
                {CIRNO_DIALOG WOW, "Its Cirno Day, todeh."},
                {CIRNO_DIALOG JOY, "Billions must celebrate\nthis occasion."}
            });
            gs.explored.insert(5);
        }
        else if (CheckCollisionPointRec(gs.currentMousePos, Chest)) {
            if (gs.chestCount == 0) {
                SayDialog(gs, std::vector<Dialogues> {
                    {CIRNO_DIALOG WOW, "Hey!! Don't snoop around\nthe chest."},
                    {CIRNO_DIALOG TIERED, "It's where I store all my\ntreasures."}
                });
                gs.chestCount++;
            } else if (gs.chestCount == 1) {
                SayDialog(gs, std::vector<Dialogues> {
                    {CIRNO_DIALOG PISSED, "No, I won't let you\nsee what's inside!!!"},
                });
                gs.explored.insert(6);
            }
        }
        else if (CheckCollisionPointRec(gs.currentMousePos, Futton)) {
            if (gs.explored.size() ==  12) {
                SayDialog(gs, std::vector<Dialogues> {
                    {CIRNO_DIALOG WOW, "I am feeling sleepy\nI had lots of fun with you."},
                    {CIRNO_DIALOG TIERED, " I am gonna take a nap\nnow. Let's see tommorow!!"}
                });
                gs.GameOver = true;
            } else {
                SayDialog(gs, std::vector<Dialogues> {
                    {CIRNO_DIALOG WOW, "I just woke up before\nyou came."},
                    {CIRNO_DIALOG TIERED, "So, I am not sleepy now."}
                });
            }
        }
    } else if (gs.curPos == OUTSIDE){
        if (CheckCollisionPointRec(gs.currentMousePos, YokaiMountain)) {
            if (gs.MountainClickCount == 0) {
                SayDialog(gs, std::vector<Dialogues> {
                    {CIRNO_DIALOG WOW, "That's Youkai Mountain\nThat's where Aya\nlives"},
                    {CIRNO_DIALOG JOY, "She is nosy and snitches\nme to people whenever\nI am up to no good"},
                    {CIRNO_DIALOG PISSED, "Bah!! She is annoying!"},
                });
            } else if (gs.MountainClickCount == 1) {
                SayDialog(gs, std::vector<Dialogues> {
                    {CIRNO_DIALOG JOY, "The Moriya shrine is at\nthe very top of the\nYoukai Mountain"},
                    {CIRNO_DIALOG WOW, "The deities there are\nvery strong."},
                    {CIRNO_DIALOG LAUGH, "Not as strong as me,\nof course!"},
                });
                gs.explored.insert(7);
            }
            gs.MountainClickCount = (gs.MountainClickCount + 1) % 2;
        } else if (CheckCollisionPointRec(gs.currentMousePos, MistyLake)) {
            SayDialog(gs, std::vector<Dialogues> {
                {CIRNO_DIALOG WOW, "Do you know? There is a\ngiant frog that lives at\nthe bottom of the lake."},
                {CIRNO_DIALOG TIERED, "It's very very big."},
                {CIRNO_DIALOG PISSED, "It ate me one time!!"}
            });
            gs.explored.insert(8);
        } else if (CheckCollisionPointRec(gs.currentMousePos, Igloo)) {
            SayDialog(gs, std::vector<Dialogues> {
                {CIRNO_DIALOG WOW, "My house looks very cool,\nright?"},
                {CIRNO_DIALOG JOY, "It was a lot of work,\nbullying the other fairies\ninto building my house."},
                {CIRNO_DIALOG TIERED, "Though, they now barge\ninto my home to play,\nwhenever they want,"},
                {CIRNO_DIALOG TIERED, "without my permission."}
            });
            gs.explored.insert(9);
        } else if (CheckCollisionPointRec(gs.currentMousePos, Flower) && !gs.flowerOnHead) {
            if (gs.FlowerClickCount == 0) {
                SayDialog(gs, std::vector<Dialogues> {
                    {CIRNO_DIALOG WOW, "A flower!"},
                });
                gs.FlowerClickCount++;
            } else if (gs.FlowerClickCount == 1) {
                gs.flowerOnHead = true;
                SayDialog(gs, std::vector<Dialogues> {
                    {CIRNO_DIALOG JOY, "Do I look cute with\nthis flower on me?"},
                    {CIRNO_DIALOG RELAXED, "I like it a lot"}
                });
                gs.explored.insert(10);
            }
        }
    } else if (gs.curPos == FOREST) {
        if (CheckCollisionPointRec(gs.currentMousePos, Frog)) {
            if (!gs.frogDialogDone) {
                SayDialog(gs, std::vector<Dialogues> {
                    {CIRNO_DIALOG WOW, "Suwako somehow always\ncatches me whenever I\nbully frogs here"},
                    {CIRNO_DIALOG JOY, "Let's leave it alone for\nnow"},
                    {CIRNO_DIALOG TIERED, "She will beat me\nup if I get caught red\nhanded."}
                });
                gs.frogDialogDone = true;
            } else {
                SayDialog(gs, std::vector<Dialogues> {
                    {CIRNO_DIALOG LAUGH, "The frogs make funny\nnoises. Ribbit Ribbit."},
                });
                gs.explored.insert(11);
            }
        } else if (CheckCollisionPointRec(gs.currentMousePos, RiceCake)) {
            if (gs.ShrineClickCount == 0) {
                SayDialog(gs, std::vector<Dialogues> {
                    {CIRNO_DIALOG RELAXED, "It's a small shrine for the\nMoriya gods."},
                    {CIRNO_DIALOG WOW, "That Shrine Maiden Sanae\ncomes here from time to\ntime and takes care of it. "},
                });
                gs.ShrineClickCount++;
            } else if (gs.ShrineClickCount == 1) {
                SayDialog(gs, std::vector<Dialogues> {
                    {CIRNO_DIALOG JOY, "Look! There is a rice cake\nhere left as an offering."}
                });
                gs.ShrineClickCount++;
            } else if (gs.ShrineClickCount == 2) {
                gs.riceCakeEaten = true;
                SayDialog(gs, std::vector<Dialogues> {
                    {CIRNO_DIALOG JOY, "The rice cake magically \ndisappeared!"},
                    {CIRNO_DIALOG RELAXED, ". . .", 0.4f},
                    {CIRNO_DIALOG JOY, "*Burp*"},
                });
                gs.explored.insert(12);
                gs.ShrineClickCount++;
            } else if (gs.ShrineClickCount == 3) {
                SayDialog(gs, std::vector<Dialogues> {
                    {CIRNO_DIALOG RELAXED, "It's a small shrine for the\nMoriya gods."},
                    {CIRNO_DIALOG WOW, "That Shrine Maiden Sanae\ncomes here from time to\ntime and takes care of it. "},
                });
            }
        }
    }
}
