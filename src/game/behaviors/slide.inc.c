#include <ultra64.h>

#include "sm64.h"

// slide.inc.c

#define ANALOG_AMOUNT 162144 / 315
#define RATEX (ANALOG_AMOUNT * ((gPlayer1Controller->stickX / 64.0f) * (gPlayer1Controller->stickX / 64.0f)) * 1.0f)
#define RATEY (ANALOG_AMOUNT * ((gPlayer1Controller->stickY / 64.0f) * (gPlayer1Controller->stickY / 64.0f)) * 1.0f)

void bhv_tilt_platform(void) {

    // Tilts the platform based on the left joystick
    if (!(gMarioState->action == ACT_STAR_DANCE_WATER)) {
        if (gPlayer1Controller->rawStickX > 2) {
            o->oFaceAngleRoll += RATEX;
        } else if (gPlayer1Controller->rawStickX < -2) {
            o->oFaceAngleRoll += RATEX * -1.0;
        }
    }

    // Makes it so the platform doesn't rotate past 45 degrees
    if (o->oFaceAngleRoll > 0x2000) {
        o->oFaceAngleRoll = 0x2000;
    }

    if (o->oFaceAngleRoll < -0x2000) {
        o->oFaceAngleRoll = -0x2000;
    }
    
}

void bhv_tilt_box(void) {

    // Tilts the platform based on the left joystick
    if (!(gMarioState->action == ACT_STAR_DANCE_WATER)) {
        if (gPlayer1Controller->rawStickX > 2) {
            o->oFaceAngleRoll += RATEX * -1.0;
        } else if (gPlayer1Controller->rawStickX < -2) {
            o->oFaceAngleRoll += RATEX;
        }
    }
    
}

void bhv_slide_platform_hori(void) {
    o->oVelX = 0;

    // Left joystick makes the platform move left/right
    if (!(gMarioState->action == ACT_STAR_DANCE_WATER)) {
        if (gPlayer1Controller->rawStickX > 2) {
            o->oVelX = RATEX / 13;
            //o->oPosX += RATEX / 13;
        } else if (gPlayer1Controller->rawStickX < -2) {
             o->oVelX = RATEX / 13 * -1.0;
            //o->oPosX += RATEX / 13 * -1.0;
        }
    }

    o->oPosX += o->oVelX;

    f32 limitRight = GET_BPARAM1(o->oBehParams);
    f32 limitLeft = GET_BPARAM3(o->oBehParams);

    // Flips the limit if the next BPARAM is not 0
    if (GET_BPARAM2(o->oBehParams) > 0) {
        limitRight = (GET_BPARAM1(o->oBehParams) * -1.0);
    }

    if (GET_BPARAM4(o->oBehParams) > 0) {
        limitLeft = (GET_BPARAM3(o->oBehParams) * -1.0);
    }

    // Limits the platform to be in between BPARAM2 and BPARAM 3 divided by 10
    if (o->oPosX > limitRight * 10.0) {
        o->oPosX = limitRight * 10.0;
    }

    if (o->oPosX < limitLeft * -10.0) {
        o->oPosX = limitLeft * -10.0;
    }

    // Spawns Markers
    if (o->oTimer == 0) {
        spawn_object_abs_with_rot(o, 0, MODEL_BOWLING_BALL, bhvMarker,
                                    ((limitLeft * -10.0) - 250.0), o->oPosY, o->oPosZ, 0, 0, 0);

        spawn_object_abs_with_rot(o, 0, MODEL_BOWLING_BALL, bhvMarker,
                                    ((limitRight * 10.0) + 250.0), o->oPosY, o->oPosZ, 0, 0, 0);
    }

}

void bhv_slide_platform_vert(void) {

    // Left joystick makes the platform move up/down
    if (!(gMarioState->action == ACT_STAR_DANCE_WATER)) {
        if (gPlayer1Controller->rawStickY > 2) {
            o->oPosY += RATEX / 10;
        } else if (gPlayer1Controller->rawStickY < -2) {
            o->oPosY += RATEX / 10 * -1.0;
        }
    }

    // Limits the platform to be in between BPARAM2 and BPARAM 3 divided by 10
    if (o->oPosX > GET_BPARAM2(o->oBehParams) * 10.0) {
        o->oPosX = GET_BPARAM2(o->oBehParams) * 10.0;
    }

    if (o->oPosX < (GET_BPARAM3(o->oBehParams) * -10.0)) {
        o->oPosX = (GET_BPARAM3(o->oBehParams) * -10.0);
    }

}

void bhv_fan_init(void) {
    o->oOnOff = FALSE;
    o->oActivated = TRUE;
    o->oRotVel = 0;

    if (GET_BPARAM1(o->oBehParams) != 0) {
        o->oOnOff = TRUE;
        spawn_object_relative(ORANGE_NUMBER_A, 0, 0, 50, o, MODEL_NONE, bhvButtonIndicator);
    }

    if (GET_BPARAM1(o->oBehParams) >= 2) {
        o->oActivated = FALSE;
    }

}

void bhv_fan(void) {
    f32 areaTop;
    f32 areaBottom;
    f32 areaLeft;
    f32 areaRight;
    f32 direction;
    f32 speed = 32.0f;

    // 0: North
    // 1: East
    // 2: South
    // 3: West
    if ((o->oMoveAnglePitch > 0) && (o->oMoveAngleYaw == 0)){
        direction = 0;
    } else if ((o->oMoveAnglePitch == 0) && (o->oMoveAngleYaw > 0)){
        direction = 1;
    } else if ((o->oMoveAnglePitch < 0) && (o->oMoveAngleYaw == 0)) {
        direction = 2;
    } else {
        direction = 3;
    }

    if (o->oMoveAnglePitch == 0) {
        areaTop = o->oPosY + 300.0f;
        areaBottom = o->oPosY - 300.0f;
    } else if (o->oMoveAnglePitch > 0) {
        areaTop = (o->oPosY + (o->oBehParams2ndByte * 10.0));
        areaBottom = o->oPosY;
    } else {
        areaTop = o->oPosY;
        areaBottom = (o->oPosY - (o->oBehParams2ndByte * 10.0));
    }

    if (o->oMoveAngleYaw == 0) {
        areaLeft = o->oPosX - 300.0f;
        areaRight = o->oPosX + 300.0f;
    } else if (o->oMoveAngleYaw > 0) {
        areaLeft = o->oPosX;
        areaRight = (o->oPosX + (o->oBehParams2ndByte * 10.0) + 100.0);
    } else {
        areaLeft = (o->oPosX - (o->oBehParams2ndByte * 10.0));
        areaRight = o->oPosX;
    }

    if (o->oActivated) {
        spawn_wind_fan_particles(o->oFaceAnglePitch);
        if (!(gMarioState->action == ACT_STAR_DANCE_WATER)) {
            if ((gMarioState->pos[0] > areaLeft) && (gMarioState->pos[0] < areaRight) && (gMarioState->pos[1] > areaBottom) && (gMarioState->pos[1] < areaTop)) {
                if (direction == 0) {
                    gMarioState->vel[1] = speed;
                } else if (direction == 1) {
                    gMarioState->pos[0] += speed;
                } else if (direction == 2) {
                    gMarioState->vel[1] = (speed * -1.0);
                } else {
                    gMarioState->pos[0] -= speed;
                }
            }
        }

        if (o->oRotVel < 3000) {
            o->oRotVel += 500;
        } else {
            o->oRotVel = 3000;
        }

    } else {
        if (o->oRotVel > 0) {
            o->oRotVel -= 500;
        } else {
            o->oRotVel = 0;
        }
    }

    if ((o->oOnOff) && (gPlayer1Controller->buttonPressed & (A_BUTTON))) {
        o->oActivated = !(o->oActivated);
    }

    o->oFaceAngleRoll -= o->oRotVel;

    /*
    // Debug Markers for top right and bottom left of push area
    if (o->oTimer == 0) {
        spawn_object_abs_with_rot(o, 0, MODEL_BOWLING_BALL, bhvMarker,
                                    areaRight, areaTop, o->oPosZ, 0, 0, 0);

        spawn_object_abs_with_rot(o, 0, MODEL_BOWLING_BALL, bhvMarker,
                                    areaLeft, areaBottom, o->oPosZ, 0, 0, 0);
    }
    */
    
}

void spawn_wind_fan_particles(s32 pitch) {
    s32 i;
    for (i = 0; i < 3; i++) {
        struct Object *wind = spawn_object(o, MODEL_MIST, bhvWindFan);
        wind->oMoveAnglePitch = pitch;
    }
}

void bhv_wind_fan_loop(void) {

    if (o->oTimer == 0) {
        o->parentObj = cur_obj_nearest_object_with_behavior(bhvFan);

        o->oOpacity = 100;
        if (o->oMoveAnglePitch == 0) {
            obj_translate_xz_random(o, 900.0f);

            o->oPosX += sins(o->oMoveAngleYaw + 0x8000) * -300; // NOP as Pitch is 0
            o->oPosY += random_f32_around_zero(600.0f);
            o->oPosZ += coss(o->oMoveAngleYaw + 0x8000) * 500; // -coss(a) * sp2E
            //o->oMoveAngleYaw += random_f32_around_zero(4000.0f);
            o->oForwardVel = random_float() * 70.0f + 50.0f;
        } else {
            obj_translate_xz_random(o, 600.0f);
            //o->oPosY -= 300;
            if (o->oMoveAnglePitch > 0) {
                o->oVelY = random_float() * 30.0f + 50.0f;
            } else {
                o->oVelY = random_float() * 30.0f + 50.0f * -1.0;
            }
            //o->oMoveAngleYaw = random_u16();
            o->oForwardVel = 10.0f;
        }
        obj_set_billboard(o);
    }


    // Delete Object if travelled too far
    if (o->oMoveAnglePitch == 0) {
        if (o->oPosX > (o->oHomeX + (o->parentObj->oBehParams2ndByte * 10.0))) {
            obj_mark_for_deletion(o);
        }
        if (o->oPosX < (o->oHomeX - (o->parentObj->oBehParams2ndByte * 10.0))) {
            obj_mark_for_deletion(o);
        }
    } else {
        if (o->oPosY > (o->oHomeY + (o->parentObj->oBehParams2ndByte * 10.0))) {
            obj_mark_for_deletion(o);
        }
        if (o->oPosY < (o->oHomeY - (o->parentObj->oBehParams2ndByte * 10.0))) {
            obj_mark_for_deletion(o);
        }
    }


    if (o->oTimer > 50) {
        obj_mark_for_deletion(o);
    }

    o->oFaceAnglePitch += 4000.0f + 2000.0f * random_float();
    //o->oFaceAngleYaw += 4000.0f + 2000.0f * random_float();
    cur_obj_move_using_fvel_and_gravity();

    /*
    f32 speed;
    s16 pushAngle;

    pushAngle = floor->force << 8;

    m->slideVelX += 1.2f * sins(pushAngle);
    m->slideVelZ += 1.2f * coss(pushAngle);

    speed = (sqr(m->slideVelX) + sqr(m->slideVelZ));

    if (speed > sqr(48.0f)) {
        speed = sqrtf(speed);
        m->slideVelX = m->slideVelX * 48.0f / speed;
        m->slideVelZ = m->slideVelZ * 48.0f / speed;
        speed = 48.0f;
    } else if (speed > 32.0f) {
        speed = 32.0f;
    }

    m->vel[0] = m->slideVelX;
    m->vel[2] = m->slideVelZ;
    m->slideYaw = atan2s(m->slideVelZ, m->slideVelX);
    m->forwardVel = speed * coss(m->faceAngle[1] - m->slideYaw);
    */
}

void bhv_level_button_init(void) {
    s16 numStars = save_file_get_total_star_count(gCurrSaveFileNum - 1, COURSE_MIN - 1, COURSE_MAX - 1);
    s16 requiredNumStars = ((GET_BPARAM2(o->oBehParams)) - 1);
    if (GET_BPARAM2(o->oBehParams) >= 0x10) {
        requiredNumStars = ((GET_BPARAM2(o->oBehParams)) - 7);
    }

    if (numStars > requiredNumStars) {
        cur_obj_set_model(MODEL_LEVEL_BUTTON_STAR);
    }

    // Deletes the object if not enough stars
    if (numStars < requiredNumStars) {
        obj_mark_for_deletion(o);
    } else {
        if (GET_BPARAM2(o->oBehParams) >= 0x10) {
            spawn_object_relative(((GET_BPARAM2(o->oBehParams) - 0x06) / 10), 30, 0, 0, o, MODEL_NUMBER, bhvLevelNumber);
            spawn_object_relative((GET_BPARAM2(o->oBehParams) - 0x10), -30, 0, 0, o, MODEL_NUMBER, bhvLevelNumber);
        } else {
            spawn_object_relative(GET_BPARAM2(o->oBehParams), 0, 0, 0, o, MODEL_NUMBER, bhvLevelNumber);
        }
        o->oButtonScale = 1.0f;
    }
}

void bhv_level_button(void) {
    f32 areaTop;
    f32 areaBottom;
    f32 areaLeft;
    f32 areaRight;

    areaTop = o->oPosY + o->oButtonSizeVert;
    areaBottom = o->oPosY - o->oButtonSizeVert;
    areaLeft = o->oPosX - o->oButtonSizeHori;
    areaRight = o->oPosX + o->oButtonSizeHori;
    
    o->parentObj = cur_obj_nearest_object_with_behavior(bhvSelector);

    switch (o->oAction) {
        case 0:
            // Button Selected
            if ((o->parentObj->oPosX > areaLeft) && (o->parentObj->oPosX < areaRight) && (o->parentObj->oPosY > areaBottom) && (o->parentObj->oPosY < areaTop) 
            && (gPlayer1Controller->buttonPressed & (A_BUTTON))) {
                play_sound(SOUND_MENU_CLICK_FILE_SELECT, gGlobalSoundSource);
                spawn_mist_particles();
                o->oAction = 1;
            }
            break;
        case 1:
            if (o->oButtonScale > 1.1) {
                o->oAction = 2;
            } else {
                o->oButtonScale += 0.05f;
            }
            break;
        case 2:
            if (o->oButtonScale > 1.0) {
                o->oButtonScale -= 0.05f;
            } else {
                play_sound(SOUND_MENU_ENTER_HOLE, gGlobalSoundSource);
                gMarioState->usedObj = o;
                level_trigger_warp(gMarioState, WARP_OP_WARP_OBJECT);
                o->oAction = 3;
            }
            break;
    }

    // Debug Markers for top right and bottom left of push area
    /*
    if (o->oTimer == 0) {
        spawn_object_abs_with_rot(o, 0, MODEL_AMP, bhvMarker,
                                    areaRight, areaTop, o->oPosZ, 0, 0, 0);

        spawn_object_abs_with_rot(o, 0, MODEL_AMP, bhvMarker,
                                    areaLeft, areaBottom, o->oPosZ, 0, 0, 0);
    }
    */

    cur_obj_scale(o->oButtonScale);

}

void bhv_level_number(void) {
    o->oAnimState = o->oBehParams2ndByte;
}

void bhv_selector(void) {

    s16 rawStickX = gPlayer1Controller->rawStickX;
    s16 rawStickY = gPlayer1Controller->rawStickY;
    

    // Handle deadzone
    if (rawStickY > -2 && rawStickY < 2) {
        rawStickY = 0;
    }
    if (rawStickX > -2 && rawStickX < 2) {
        rawStickX = 0;
    }

    // Move cursor
    o->oPosX += rawStickX / 2;
    o->oPosY += rawStickY / 2;

    // Stop cursor from going offscreen
    if (o->oPosX > 1320.0f) {
        o->oPosX = 1320.0f;
    }
    if (o->oPosX < -1320.0f) {
        o->oPosX = -1320.0f;
    }

    if (o->oPosY > 900.0f) {
        o->oPosY = 900.0f;
    }
    if (o->oPosY < -900.0f) {
        o->oPosY = -900.0f;
    }
}

void bhv_caged_star_init(void) {
    // Sets the BPARAM of the star
    SET_BPARAM1(o->oBehParams, GET_BPARAM2(o->oBehParams));
}

void bhv_caged_star(void) {
    o->parentObj = cur_obj_nearest_object_with_behavior(bhvBreakableBoxSmall);
    
    if (o->parentObj == NULL) {
        obj_mark_for_deletion(o);
    } else {
        o->oPosY = (o->parentObj->oPosY + 50);
        o->oPosX = o->parentObj->oPosX;
    }
}

void bhv_pinball_init(void) {
    struct Object *orangeNumber;
    orangeNumber = spawn_object_relative(ORANGE_NUMBER_A, 50, 0, 0, o, MODEL_NONE, bhvButtonIndicator);
    orangeNumber->oPosZ += 250;
}

void bhv_pinball(void) {

    if (gPlayer1Controller->buttonDown & (A_BUTTON)) {
        if (o->oFaceAngleRoll > -12000) {
            o->oFaceAngleRoll -= 3000;
        } else {
            o->oFaceAngleRoll = -12000;
        }
    } else {
        if (o->oFaceAngleRoll < 0) {
            o->oFaceAngleRoll += 3000;
        } else {
            o->oFaceAngleRoll = 0;
        }
    }

}

void bhv_button_indicator(void) {
    struct Object *orangeNumber;
    orangeNumber = spawn_object_relative(GET_BPARAM2(o->oBehParams), 0, 0, 0, o, MODEL_NUMBER, bhvOrangeNumber);
    orangeNumber->oHomeX = orangeNumber->oPosX;
    orangeNumber->oHomeZ = orangeNumber->oPosZ;
}