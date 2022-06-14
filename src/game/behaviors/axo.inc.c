extern u8 totwc_dl_canvas_i4[];
extern u8 totwc_dl_reference_i4[];
extern u8 totwc_dl_safezone_i4[];
extern u8 totwc_dl_undostate_i4[];

void bhv_axo_controller_loop(void) {
    s16 i, j;

    u8 *canvas = segmented_to_virtual(totwc_dl_canvas_i4);
    u8 *undostate = segmented_to_virtual(totwc_dl_undostate_i4);

    gMarioState->vel[2] = 0;
    gMarioState->pos[2] = 0;

    if (gPlayer1Controller->buttonPressed & R_TRIG) {
        for (i = 0; i < 2752; i++) {
            undostate[i] = canvas[i];
        }
    }

    if (
        gMarioState->pos[0] >= -400.0f &&
        gMarioState->pos[0] <= 400.0f &&
        gMarioState->pos[1] >= 1000.0f &&
        gMarioState->pos[1] <= 1600.0f &&
        gPlayer1Controller->buttonDown & R_TRIG
    ) {
        s16 marioImageX = (s16)((gMarioState->pos[0] + 400) * 0.1075f);
        s16 marioImageY = (s16)((gMarioState->pos[1] - 1000) * 0.106667f);
        for (i = 0; i < 5; i++) {
            s16 pixelX = (marioImageX - 2) + i;
            if (pixelX >= 0 && pixelX < 86) {
                for (j = 0; j < 5; j++) {
                    s16 pixelY = marioImageY + j;
                    if (pixelY >= 0 && pixelY < 64) {
                        if (
                            !(
                                (i == 0 && j == 0) || 
                                (i == 0 && j == 4) || 
                                (i == 4 && j == 0) || 
                                (i == 4 && j == 4)
                            )
                        ) {
                            u8 pixelNibble = pixelY % 2 == 0 ? 0x0F : 0xF0;
                            canvas[32 * pixelX + (pixelY / 2)] &= pixelNibble;
                        }
                    }
                }
            }
        }
    }

    if (gPlayer1Controller->buttonPressed & L_TRIG) {
        for (i = 0; i < 2752; i++) {
            u8 redoByte = canvas[i];
            canvas[i] = undostate[i];
            undostate[i] = redoByte;
        }
    }

    if (gPlayer1Controller->buttonDown & L_TRIG) {
        if (o->oF4 >= 0) { // The controller object's o->oF4 refers to the timer for how long L has been held
            o->oF4++;
            if (o->oF4 >= 90) {
                for (i = 0; i < 2752; i++) {
                    canvas[i] = 0xFF;
                    o->oF4 = -1;
                }
            }
        }
    }
    else {
        o->oF4 = 0;
    }
}

f32 calculate_flipnote_accuracy(void) {
    s16 i, j;
    
    u8 *canvas = segmented_to_virtual(totwc_dl_canvas_i4);
    u8 *reference = segmented_to_virtual(totwc_dl_reference_i4);
    u8 *safezone = segmented_to_virtual(totwc_dl_safezone_i4);

    s16 numWrong = 0;
    s16 numRight = 0;

    for (i = 0; i < 2752; i++) {
        for (j = 0; j < 2; j++) {
            u8 canvasPixel = canvas[i] & (0xF << (4 * j));
            u8 referencePixel = reference[i] & (0xF << (4 * j));
            if (canvasPixel != referencePixel) {
                if (referencePixel == 0xF) {
                    u8 safezonePixel = safezone[i] & (0xF << (4 * j));
                    if (safezonePixel == 0xF) {
                        numWrong++;
                    }
                }
                else {
                    numWrong++;
                }
            }
            else if (referencePixel == 0x0) {
                numRight++;
            }
        }
    }

    f32 baseAccuracy = 1.0f - (numWrong / 1190.0f);
    f32 scaledAccuracy = baseAccuracy > 0 ? baseAccuracy + ((baseAccuracy - (baseAccuracy * baseAccuracy)) * (numRight / 1190.0f)) : baseAccuracy;

    return scaledAccuracy;
}

void bhv_flipnote_frog_init(void) {
    o->oInteractionSubtype = INT_SUBTYPE_NPC;
}

enum oActionsFlipnoteFrog {
    FLIPNOTE_FROG_ACT_IDLE,
    FLIPNOTE_FROG_ACT_TALK
};

void flipnote_frog_act_idle(void) {

    // Frame animation
    if (o->oTimer < 30) {          // 1000ms, 30 frames, 30 total
        o->oAnimState = 0;
    }
    else if (o->oTimer < 34) {     // 130ms, 3.9 frames, 33.9 total
        o->oAnimState = 1;
    }
    else if (o->oTimer < 94) {     // 2000ms, 60 frames, 93.9 total
        o->oAnimState = 0;
    }
    else if (o->oTimer < 97) {     // 100ms, 3 frames, 96.9 total
        o->oAnimState = 2;
    }
    else if (o->oTimer < 100) {    // 100ms, 3 frames, 99.9 total
        o->oAnimState = 3;
    }
    else if (o->oTimer < 103) {    // 100ms, 3 frames, 102.9 total
        o->oAnimState = 2;
    }
    else if (o->oTimer < 163) {    // 2000ms, 60 frames, 162.9 total
        o->oAnimState = 0;
    }
    else {
        o->oTimer = 0;
        o->oAnimState = 0;
    }

    if (o->oInteractStatus == INT_STATUS_INTERACTED) {
        o->oAction = FLIPNOTE_FROG_ACT_TALK;
        cur_obj_play_sound_2(SOUND_ACTION_READ_SIGN);
    }
}

void flipnote_frog_act_talk(void) {
    if (set_mario_npc_dialog(MARIO_DIALOG_LOOK_FRONT) == MARIO_DIALOG_STATUS_SPEAK) {
        o->activeFlags |= ACTIVE_FLAG_INITIATED_TIME_STOP;
        u32 text;
        u8 spawnStar = FALSE;
        if (o->oF4 == 0) { // The frog object's o->oF4 refers to whether or not the star has already been spawned
            s16 accuracy = (s16)(calculate_flipnote_accuracy() * 100);
            if (accuracy <= -50) {
                text = DIALOG_151;
            }
            else {
                if (accuracy < 50) {
                    text = DIALOG_150 << 16;
                }
                else if (accuracy < 75) {
                    text = DIALOG_152 << 16;
                }
                else {
                    spawnStar = TRUE;
                    if (accuracy < 85) {
                        text = DIALOG_153 << 16;
                    }
                    else if (accuracy < 95) {
                        text = DIALOG_154 << 16;
                    }
                    else {
                        text = DIALOG_155 << 16;
                    }
                }
                text += accuracy;
            }
        }
        else {
            text = DIALOG_156;
        }
        if (cutscene_object_with_dialog(CUTSCENE_DIALOG, o, text)) {
            set_mario_npc_dialog(MARIO_DIALOG_STOP);
            o->activeFlags &= ~ACTIVE_FLAG_INITIATED_TIME_STOP;
            o->oInteractStatus = INT_STATUS_NONE;
            o->oAction = FLIPNOTE_FROG_ACT_IDLE;
            if (spawnStar) {
                cur_obj_spawn_star_at_y_offset(0.0f, 1300.0f, 0.0f, 50.0f);
                o->oF4 = 1;
            }
        }
    }
}

void bhv_flipnote_frog_loop(void) {
    switch (o->oAction) {
        case FLIPNOTE_FROG_ACT_IDLE:
            flipnote_frog_act_idle();
            break;

        case FLIPNOTE_FROG_ACT_TALK:
            flipnote_frog_act_talk();
            break;
    }
    if (o->oInteractStatus & INT_STATUS_INTERACTED) {
        cur_obj_play_sound_2(SOUND_ACTION_READ_SIGN);
        if (set_mario_npc_dialog(MARIO_DIALOG_LOOK_FRONT) == MARIO_DIALOG_STATUS_SPEAK) {
            o->activeFlags |= ACTIVE_FLAG_INITIATED_TIME_STOP;
        }
    }
    o->oInteractStatus = INT_STATUS_NONE;
}