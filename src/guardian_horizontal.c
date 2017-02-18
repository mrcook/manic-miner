// Manic Miner C Port Copyright 2017 Michael R. Cook

#include "headers.h"

#include "guardian_horizontal.h"

bool GuardianH_is_blank(GuardianHorizontal *guardian) {
    if (guardian->speedColour == 0 &&
        guardian->attributeAddress == 0 &&
        guardian->addressMSB == 0 &&
        guardian->frame == 0 &&
        guardian->addressLeftLSB == 0 &&
        guardian->addressRightLSB == 0) {
        return true;
    }

    return false;
}
