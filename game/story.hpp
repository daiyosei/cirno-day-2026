#include "lib/gameState.hpp"
#include "lib/dialogue.hpp"

#include "bounding.hpp"
#include "graphics.hpp"
#include "raylib.h"

#define CIRNO_DIALOG "Cirno",

inline void ProcessClick(GameState &gs) {
    if (CheckCollisionPointRec(gs.currentMousePos, Forehead)) {
        if (gs.flickCount == 0) {
            SayDialog(gs, std::vector<Dialogues>{
                {CIRNO_DIALOG HURT, "owww"},
                {CIRNO_DIALOG PISSED, "it hurts. You are mean!"},
                {CIRNO_DIALOG PISSED, "Why did you do that?"}
            });
        } else if (gs.flickCount == 1) {
            SayDialog(gs, std::vector<Dialogues> {
                {CIRNO_DIALOG HURT, "You did it again!"},
                {CIRNO_DIALOG PISSED, "Owww, it hurts.\nStop it!"},
            });
        } else if (gs.flickCount == 2) {
            SayDialog(gs, std::vector<Dialogues> {
                {CIRNO_DIALOG PISSED, "No, I won't let you\ndo it again"},
                {CIRNO_DIALOG PISSED, "You meanie!!"},
            });
            gs.explored.insert(1);
        }
        gs.flickCount++;
    }
    else if (CheckCollisionPointRec(gs.currentMousePos, UI_arrowBtn)) {
        if (gs.curPos == INSIDE) {
            changeSceneTo(gs, OUTSIDE);
            SayDialog(gs, std::vector<Dialogues> {
                {CIRNO_DIALOG WOW, "This is where I live"},
            });
            gs.explored.insert(2);
        } else if (gs.curPos == OUTSIDE) {
            changeSceneTo(gs, FOREST);
            SayDialog(gs, std::vector<Dialogues> {
                {CIRNO_DIALOG LAUGH, "Let's freeze some\nfrogs!"},
            });
            gs.explored.insert(3);
        } else if (gs.curPos == FOREST) {
            changeSceneTo(gs, INSIDE);
            SayDialog(gs, std::vector<Dialogues> {
                {CIRNO_DIALOG RELAXED, "Back in sweet cozy\nhome!"}
            });
            gs.explored.insert(4);
        }
    } else if (CheckCollisionPointRec(gs.currentMousePos, UI_talkBtn)) {
        if (!gs.usedTalkButton) {
            SayDialog(gs, std::vector<Dialogues> {
                {CIRNO_DIALOG RELAXED, "Good Morning"},
            });
            gs.usedTalkButton = true;
        } else {
            SayDialog(gs, std::vector<Dialogues> {
                {CIRNO_DIALOG JOY, "Having lots of fun is\nwhat matters."},
                {CIRNO_DIALOG RELAXED, "Imagine having no fun\nand being grumpy\nall the time."},
                {CIRNO_DIALOG JOY, "Next time, when you come,\nwe can play together."},
                {CIRNO_DIALOG JOY, "With Daiyousei and\nthe other fairies."},
                {CIRNO_DIALOG RELAXED, "It would be nice if\nwinter comes soon."},
                {CIRNO_DIALOG JOY, "I want to skate on the\nmisty lake with Letty."},
                {CIRNO_DIALOG WOW, "I am the smartest\nin Gensokyo."},
                {CIRNO_DIALOG WOW, "I can add and multiply\nfractions!"},
                {CIRNO_DIALOG LAUGH, "Only a few could rival\nan intellect such\nas mine."}
            });
            gs.explored.insert(7);
        }
    }
    else if (gs.curPos == INSIDE) {
        if (CheckCollisionPointRec(gs.currentMousePos, Calendar)) {
            SayDialog(gs, std::vector<Dialogues> {
                {CIRNO_DIALOG WOW, "its cirno day, todeh"},
                {CIRNO_DIALOG JOY, "billions must celebrate"}
            });
            gs.explored.insert(5);
        }
        else if (CheckCollisionPointRec(gs.currentMousePos, Chest)) {
            if (gs.chestCount == 0) {
                SayDialog(gs, std::vector<Dialogues> {
                    {CIRNO_DIALOG WOW, "Hey!! Don't snoop around\nthe chest"},
                    {CIRNO_DIALOG TIERED, "It's where I store all my\ntreasures"}
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
            if (gs.explored.size() == 13) {
                SayDialog(gs, std::vector<Dialogues> {
                    {CIRNO_DIALOG WOW, "I am feeling sleepy\nI had lots of fun with you"},
                    {CIRNO_DIALOG TIERED, " I am gonna take a nap\nLet's see tommorow!!"}
                });
            } else {
                SayDialog(gs, std::vector<Dialogues> {
                    {CIRNO_DIALOG WOW, "I just woke up before\nyou came"},
                    {CIRNO_DIALOG TIERED, "So, I am not sleepy now\nI want to play"}
                });
            }
        }
    } else if (gs.curPos == OUTSIDE){
        if (CheckCollisionPointRec(gs.currentMousePos, YokaiMountain)) {
            SayDialog(gs, std::vector<Dialogues> {
                {CIRNO_DIALOG WOW, "That's the Youkai Mountain\nThat's where Aya lives"},
                {CIRNO_DIALOG JOY, "She is nosy and snitches\nme to people whenever\nI am up to no good"},
                {CIRNO_DIALOG PISSED, "Bah!! She is annoying!"},
                {CIRNO_DIALOG JOY, "The Moriya shrine is at\nthe very top of the\nYoukai Mountain"},
                {CIRNO_DIALOG WOW, "The deities there,\nare very strong."},
                {CIRNO_DIALOG LAUGH, "Not as strong as me,\nof course!"},
                {CIRNO_DIALOG WOW, "But I like the Hakurei\nshrine more"},
                {CIRNO_DIALOG RELAXED, "Reimu sometimes gives\nme some snacks\nwhen I go there"},
            });
            gs.explored.insert(8);
        } else if (CheckCollisionPointRec(gs.currentMousePos, MistyLake)) {
            SayDialog(gs, std::vector<Dialogues> {
                {CIRNO_DIALOG WOW, "Do you know? There is a\ngiant frog that lives"},
                {CIRNO_DIALOG TIERED, "at the bottom of the lake.\nIt's very very big."},
                {CIRNO_DIALOG PISSED, "It ate me one time!!"}
            });
            gs.explored.insert(9);
        } else if (CheckCollisionPointRec(gs.currentMousePos, Igloo)) {
            SayDialog(gs, std::vector<Dialogues> {
                {CIRNO_DIALOG WOW, "My house looks very cool,\nright?"},
                {CIRNO_DIALOG JOY, "It was a lot of work,\nbullying the other fairies\ninto building my house"},
                {CIRNO_DIALOG TIERED, "Though, sometimes they\nbarge into my home to\nplay"}
            });
            gs.explored.insert(10);
        } else if (CheckCollisionPointRec(gs.currentMousePos, Flower) && !gs.flowerOnHead) {
            SayDialog(gs, std::vector<Dialogues> {
                {CIRNO_DIALOG JOY, "A flower!"},
            });
            gs.flowerOnHead = true;
            SayDialog(gs, std::vector<Dialogues> {
                {CIRNO_DIALOG WOW, "Do I look cute with\nthis flower on me?"},
                {CIRNO_DIALOG LAUGH, "I like it a lot"}
            });
            gs.explored.insert(11);
        }
    } else if (gs.curPos == FOREST) {
        if (CheckCollisionPointRec(gs.currentMousePos, Frog)) {
            if (!gs.frogDialogDone) {
                SayDialog(gs, std::vector<Dialogues> {
                    {CIRNO_DIALOG WOW, "Suwako somehow always\ncatches me henever I bully\nfrogs here"},
                    {CIRNO_DIALOG JOY, "Let's leave it alone for now\nShe will beat me up"},
                    {CIRNO_DIALOG TIERED, "if I get caught red handed"}
                });
                gs.frogDialogDone = true;
                gs.explored.insert(12);
            } else {
                SayDialog(gs, std::vector<Dialogues> {
                    {CIRNO_DIALOG LAUGH, "Ribbit ribbit\nThe frogs make\nfunny noises."},
                });
            }
        } else if (CheckCollisionPointRec(gs.currentMousePos, RiceCake) && !gs.riceCakeEaten) {
            SayDialog(gs, std::vector<Dialogues> {
                {CIRNO_DIALOG RELAXED, "It's a small shrine for the\nMoriya gods built by the Kappas"},
                {CIRNO_DIALOG WOW, "The Miko called Sanae comes\nhere from time to time\nand takes care of it. "},
                {CIRNO_DIALOG JOY, "Look! There is a rice cake here\nleft as an offering."}
            });
            gs.explored.insert(13);
            gs.riceCakeEaten = true;
            SayDialog(gs, std::vector<Dialogues> {
                {CIRNO_DIALOG JOY, "The rice cake magically disappeared!"},
                {CIRNO_DIALOG RELAXED, ". . .", 0.4f},
                {CIRNO_DIALOG JOY, "*Burp*"},
            });
        }
    }
}
